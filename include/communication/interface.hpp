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

#include "logger.hpp"

namespace communication
{

  using ParamContainerT = nlohmann::json;

  class InterfaceInitParam
  {
  public:

    InterfaceInitParam();

    explicit InterfaceInitParam(const ParamContainerT & param);

    std::string toString() const;

    InterfaceInitParam & operator=(const ParamContainerT & param);

    friend std::ostream & operator<<(std::ostream & io, InterfaceInitParam & param);

    std::string getKeyValue(const std::string & key, const char * default_value = "") const;

    template<typename ValueT>
    ValueT getKeyValue(const std::string & key, const ValueT default_value = ValueT()) const
    {
      if(this->param_.contains(key)){
        return this->param_[key];
      }
      return default_value;
    }
    
  protected:
    ParamContainerT param_;
  };

  std::ostream & operator<<(std::ostream & io, InterfaceInitParam & param);

  class Interface
  {
  public:

    enum Type{
      UART
    };

    static const char * USE_SYS_POLLING;

    using SysPollingCallbackT = std::function<void(const char)>;
    using mutex_t = std::recursive_mutex;
    using thread_t = std::thread;

    explicit Interface(int max_buff, const Type & type);

    ~Interface();

    Type getType() const;

    virtual std::string getFDName() const = 0;

    bool ok() const;

    bool init(const InterfaceInitParam & param);

    size_t write(const char *data, size_t size = 1);

    size_t read(char *buf, size_t size = 1);

    void setSysPollingCallback(const SysPollingCallbackT polling_cb);
  
   protected:

    void backgroundThread();

    virtual bool onInit(const InterfaceInitParam &) {
      return true;
    };

    const Type type_;
    int max_buff_;
    int fd_;
    std::atomic_bool ok_, stop_thread_;
    mutex_t mutex_;
    std::unique_ptr<thread_t> background_thread_;
    SysPollingCallbackT polling_cb_;
    std::shared_ptr<Logger::logger> logger_;
    InterfaceInitParam param_;
  };

} // namespace communication

#endif