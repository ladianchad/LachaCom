#include <unistd.h>
#include <poll.h>

#include <iostream>

#include <communication/interface.hpp>
#include <magic_enum.hpp>


namespace communication
{
InterfaceInitParam::InterfaceInitParam()
{}

InterfaceInitParam::InterfaceInitParam(const ParamContainerT & param)
{
  this->param_ = param;
}

std::string
InterfaceInitParam::toString() const
{
  return this->param_.dump(2);
}

InterfaceInitParam &
InterfaceInitParam::operator=(const ParamContainerT & param)
{
  this->param_ = param;
  return *this;
}

std::ostream &
operator<<(std::ostream & io, InterfaceInitParam & param)
{
  io<<"[param]\n"<<param.param_.dump(2)<<std::endl;
  return io;
}

    
template<typename ValueT>
ValueT
InterfaceInitParam::getKeyValue(const std::string & key, const ValueT default_value) const
{
  if(this->param_.contains(key)){
    return this->param_[key];
  }
  return default_value;
}

const char * Interface::USE_SYS_POLLING = "USE_SYS_POLLING";

Interface::Interface(int max_buff, const Type & type) : type_(type), max_buff_(max_buff)
{
  this->stop_thread_.store(true);
  this->ok_.store(false);
  std::string type_name = magic_enum::enum_name(this->type_).cbegin();
  this->logger_ = Logger::stdout_color_mt(type_name);
}

Interface::~Interface()
{
  this->logger_->info("Start clean up.");
  if(this->background_thread_ && !this->stop_thread_.load()){
    this->logger_->info("Stop polling thread.");
    this->stop_thread_.store(true);
    this->background_thread_->join();
    this->logger_->info("Stopped polling thread.");
  }
}

Type
Interface::getType() const
{
  return type_;
};

bool
Interface::ok() const
{
  return ok_.load();
}

bool
Interface::init(const InterfaceInitParam & param)
{
  this->logger_->info("Start initialize with below parameter.\n{}", param.toString());
  bool init_success = onInit(param);
  this->ok_.store(init_success);
  bool use_interrupt = param.getKeyValue(Interface::USE_SYS_POLLING, false);
  if(use_interrupt){
    this->logger_->info("Create polling thread.");
    this->stop_thread_.store(false);
    this->background_thread_ = std::make_unique<thread_t>(std::bind(&Interface::backgroundThread, this));
  }
  return init_success;
}

void
Interface::setSysPollingCallback(const SysPollingCallbackT polling_cb)
{
  std::lock_guard<mutex_t> lock(this->mutex_);
  this->polling_cb_ = polling_cb;
}

void
Interface::backgroundThread()
{
  struct pollfd poll_fd;
  poll_fd.events = POLLIN;
  this->logger_->debug("Start polling thread.");
  while(this->ok() && !this->stop_thread_.load())
  {
    if(this->polling_cb_ && this->fd_){
      poll_fd.fd = this->fd_;
      if(poll(&poll_fd, POLLIN, 100)){
        char b;
        this->read(&b, 1);
        this->polling_cb_(b);
      }
    } else {
      this->logger_->debug("USE_SYS_POLLING setted. but no interrupt cb of file descriptor.");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  }
}

} // namespace communication
