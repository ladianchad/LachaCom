#include <communication/interface.hpp>
#include <iostream>

namespace communication
{
InterfaceInitParam::InterfaceInitParam()
{}

InterfaceInitParam::InterfaceInitParam(const ParamContainerT & param)
{
  this->param_ = param;
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
  io<<"[interface param]\n"<<param.param_.dump(2)<<std::endl;
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

const char * Interface::USE_INTERRUPT_K = "USE_INTERRUPT";

Interface::Interface(int max_buff, const Type & type) : type_(type), max_buff_(max_buff)
{
  this->stop_thread_.store(true);
  this->ok_.store(false);
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
  bool use_interrupt = param.getKeyValue(Interface::USE_INTERRUPT_K, false);
  if(use_interrupt){
    this->stop_thread_.store(false);
    this->background_thread_ = std::make_unique<thread_t>(std::bind(&Interface::background_thread_, this));
  }

  return true;
}

void
Interface::setInterruptCallback(const InterruptCallbackT interrupt_cb)
{
  std::lock_guard<mutex_t> lock(this->mutex_);
  this->interrupt_cb_ = interrupt_cb;
}

void
Interface::backgroundThread()
{}

} // namespace communication
