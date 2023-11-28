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

std::string
InterfaceInitParam::getKeyValue(const std::string & key, const char * default_value) const
{
  return this->getKeyValue<std::string>(key, default_value);
}

void
InterfaceInitParam::addDefaultValue(const std::string & key, const char * default_value)
{
  this->addDefaultValue<std::string>(key, default_value);
}


const char * Interface::USE_SYS_POLLING = "USE_SYS_POLLING";
const char * Interface::AUTO_START = "AUTO_START";
const char * Interface::AUTO_RECONNECT = "AUTO_RECONNECT";

Interface::Interface(const Type & type) : type_(type)
{
  this->stop_thread_.store(true);
  this->ok_.store(false);
  std::string type_name = magic_enum::enum_name(this->type_).cbegin();
  this->logger_ = Logger::stdout_color_mt(type_name);
}

Interface::~Interface()
{
  this->logger_->info("Start clean up.");
  if(!this->stop_thread_.load()){
    this->stop_thread_.store(true);
    if(this->background_thread_){
      this->logger_->info("Stop polling thread.");
      this->background_thread_->join();
      this->logger_->info("Stopped polling thread.");
    }
  }
  if(this->fd_){
    this->logger_->info("close file descriptor.");
    ::close(this->fd_);
  }
}

Interface::Type
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
  this->param_ = param;
  this->param_.addDefaultValue(Interface::AUTO_START, true);
  this->param_.addDefaultValue(Interface::AUTO_RECONNECT, true);
  this->param_.addDefaultValue(Interface::USE_SYS_POLLING, false);
  prepareParams();
  this->logger_->info("Start initialize with below parameter.\n{}", this->param_.toString());
  
  bool init_success = onInit();
  bool auto_start = this->param_.getKeyValue(Interface::AUTO_START, true);
  if(auto_start){
    init_success &= connect();
  }
  this->ok_.store(init_success);
  if(!init_success){
    this->failHandle();
    return false;
  }
  bool use_polling = this->param_.getKeyValue(Interface::USE_SYS_POLLING, false);
  if(use_polling){
    this->logger_->info("Create polling thread.");
    this->stop_thread_.store(false);
    this->background_thread_ = std::make_unique<thread_t>(std::bind(&Interface::backgroundThread, this));
  }
  this->auto_reconnect_ = this->param_.getKeyValue(Interface::AUTO_RECONNECT, true);
  return init_success;
}

ssize_t
Interface::write(const char *data, size_t size)
{
  std::lock_guard<mutex_t> lock(this->mutex_);
  if(this->fd_ < 0){
    this->logger_->error("Destination {} not opened. can't write data.", getFDName());
    this->failHandle();
    return -1;
  }
  ssize_t write_byte = ::write(this->fd_, data, size);
  if(write_byte < 0){
    this->logger_->error("Write to {} failed.", getFDName());
    this->failHandle();
  }
  return write_byte;
}

ssize_t
Interface::read(char *buf, size_t size)
{
  std::lock_guard<mutex_t> lock(this->mutex_);
  if(this->fd_ < 0){
    this->logger_->error("Destination {} not opened. can't read data.", getFDName());
    this->failHandle();
    return -1;
  }
  ssize_t read_byte = ::read(this->fd_, buf, size);
  if(read_byte < 0){
    this->logger_->error("Read from {} failed.", getFDName());
    this->failHandle();
  }
  return read_byte;
}

void
Interface::failHandle()
{
  this->ok_.store(false);
  ::close(this->fd_);
  this->fd_ = -1;
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
  char buff;
  while(this->ok() && !this->stop_thread_.load())
  {
    if(this->polling_cb_ && this->fd_){
      poll_fd.fd = this->fd_;
      if(poll(&poll_fd, POLLIN, 100)){
        if(this->read(&buff, sizeof(buff))){
          this->polling_cb_(buff);
        }
      }
    } else {
      this->logger_->debug("USE_SYS_POLLING setted. but no interrupt cb of file descriptor.");
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    if(!this->ok()){
      this->ok_.store(this->connect());
    }
  }
  if(!this->ok()){
    this->logger_->error("polling thread stopped because interface not ok");
  }
}

} // namespace communication
