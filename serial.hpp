#ifndef Serial_COMUNICATION_LIBS___
#define Serial_COMUNICATION_LIBS___
#include "communication.hpp"
namespace communication
{
  class Serial : Communication
  {
  protected:
    std::string port_;
    int baud_rate_;
    int serial_port_;

  public:
    Serial(std::string port, int baud_rate, int max_buff = 1024);
    ~Serial();
    virtual void init() override;

    virtual int write(const char *data) override;

    virtual int read(char *buf, int size = 1) override;
  };

} // namespace communication
#endif