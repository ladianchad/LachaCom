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

    void addDefaultValue(const std::string & key, const char * default_value = "");

    template<typename ValueT>
    void addDefaultValue(const std::string & key, const ValueT default_value = ValueT()){
      if(!this->param_.contains(key)){
        this->param_[key] = default_value;
      } 
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
    static const char * AUTO_START;
    static const char * AUTO_RECONNECT;

    using SysPollingCallbackT = std::function<void(const char)>;
    using mutex_t = std::recursive_mutex;
    using thread_t = std::thread;

    explicit Interface(const Type & type);

    ~Interface();

    Type getType() const;

    virtual std::string getFDName() const = 0;

    bool ok() const;

    bool init(const InterfaceInitParam & param);

    virtual bool connect() = 0;

    ssize_t write(const char *data, size_t size = 1);

    ssize_t read(char *buf, size_t size = 1);

    void setSysPollingCallback(const SysPollingCallbackT polling_cb);
  
   protected:

    void failHandle();

    void backgroundThread();

    virtual bool onInit() = 0;

    virtual void prepareParams() = 0;

    const Type type_;
    int fd_;
    std::atomic_bool ok_, stop_thread_;
    mutex_t mutex_;
    std::unique_ptr<thread_t> background_thread_;
    bool auto_reconnect_;
    SysPollingCallbackT polling_cb_;
    std::shared_ptr<Logger::logger> logger_;
    InterfaceInitParam param_;
  };

} // namespace communication

#endif