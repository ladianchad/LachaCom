#ifndef COMUNICATION_LIBS___
#define COMUNICATION_LIBS___
#include <atomic>
#include <string>
#include <map>
#include <functional>
#include <mutex>
#include <thread>
#include <memory>
#include <ostream>

#include <nlohmann/json.hpp>

// #include "logger.hpp"

namespace communication
{

  using ParamContainerT = nlohmann::json;

  class InterfaceInitParam
  {
  public:

    friend class Interface;

    InterfaceInitParam();

    explicit InterfaceInitParam(const ParamContainerT & param);

    InterfaceInitParam & operator=(const ParamContainerT & param);

    friend std::ostream & operator<<(std::ostream & io, InterfaceInitParam & param);
  private:
    
    template<typename ValueT>
    ValueT getKeyValue(const std::string & key, const ValueT default_value = ValueT()) const;

    ParamContainerT param_;
  };

  std::ostream & operator<<(std::ostream & io, InterfaceInitParam & param);

  enum Type{
    UART
  };


  class Interface
  {
  public:
    static const char * USE_INTERRUPT_K;

    using InterruptCallbackT = std::function<void(const char)>;
    using mutex_t = std::recursive_mutex;
    using thread_t = std::thread;

    explicit Interface(int max_buff, const Type & type);

    Type getType() const;

    bool ok() const;

    bool init(const InterfaceInitParam & param);

    virtual int write(const char *data, int size = 1) = 0;

    virtual int read(char *buf, int size = 1) = 0;

    void setInterruptCallback(const InterruptCallbackT interrupt_cb);
  
   protected:

    void backgroundThread();

    virtual bool onInit(const InterfaceInitParam & param) = 0;

    const Type type_;
    int max_buff_;
    std::atomic_bool ok_, stop_thread_;
    mutex_t mutex_;
    std::unique_ptr<thread_t> background_thread_;
    InterruptCallbackT interrupt_cb_;
  };

} // namespace communication

#endif