#include <communication/interfaces/uart.hpp>

namespace communication
{

const char * UART::BAUDRATE = "BAUDRATE";
const char * UART::PORT = "PORT";

UART::UART(int max_buff)
: Interface(max_buff, Type::UART)
{}

std::string
UART::getFDName() const
{
  std::string port_name = this->param_.getKeyValue(PORT, "");
  return "UART Port " + port_name;
}

bool
UART::onInit(const InterfaceInitParam & param)
{
  std::string port_name = param.getKeyValue(PORT, "");
  if(port_name == ""){
    this->logger_->error("port name not setted.");
    return false;
  }
  speed_t baudrate = param.getKeyValue(BAUDRATE, B115200);
  this->fd_ = ::open(port_name.c_str(), O_RDWR);
  if(this->fd_ < 0){
    this->logger_->critical("failed open port : {} | error : {}", port_name, strerror(errno));
    return false;
  }
  struct termios tty;
  if(tcgetattr(this->fd_, &tty) != 0)
  {
    this->logger_->error("failed get {} info. | error : {}", port_name, strerror(errno));
    return false;
  }
  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;
  tty.c_cc[VTIME] = 10;
  tty.c_cc[VMIN] = 0;
  cfsetispeed(&tty, baudrate);
  cfsetospeed(&tty, baudrate);
  if (tcsetattr(this->fd_, TCSANOW, &tty) != 0)
  {
    this->logger_->error("failed set {} attr. | error : {}", strerror(errno));
    return false;
  }
  return true;
}

} // namespace communication
