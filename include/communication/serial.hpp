#ifndef Serial_COMUNICATION_LIBS___
#define Serial_COMUNICATION_LIBS___
#include "communication.hpp"
#include <thread>
#include <memory>
#include <mutex>
#include <functional>
#include <future>

namespace communication
{
  class Serial : public Communication
  {
  public:
    using ReadInterruptCallBack = std::function<void()>;
    Serial(std::string port, int baud_rate, int max_buff = 1024);
    ~Serial();
    virtual void init() override;

    int write(const char *data) override;

    int read(char *buf, int size = 1) override;

    void setReadInterrupt(ReadInterruptCallBack intterupt_callback);

    void close();
  
  protected:
    void readThread();
    std::string port_;
    int baud_rate_;
    int serial_port_;
    ReadInterruptCallBack intterupt_callback_;
    std::shared_ptr<std::mutex> mutex_;
    std::shared_ptr<std::thread> read_thread_;
    std::promise<void> thread_end_promise_;
  };

} // namespace communication
#endif