#include <communication/interfaces/uart.hpp>
#include <sys/file.h>

namespace communication
{

const char * UART::BAUDRATE = "BAUDRATE";
const char * UART::PORT = "PORT";

UART::UART()
: Interface(Type::UART)
{}

std::string
UART::getFDName() const
{
  return "UART Port " + this->port_name_;
}

bool
UART::onInit()
{
  this->port_name_ = this->param_.getKeyValue(PORT, "");
  if(this->port_name_ == ""){
    this->logger_->error("port name not setted.");
    return false;
  }
  this->baudrate_ = this->param_.getKeyValue(BAUDRATE, B115200);
  return true;
}

void
UART::prepareParams()
{
  this->param_.addDefaultValue(UART::BAUDRATE, 115200);
  this->param_.addDefaultValue(UART::PORT, "/dev/ttyUSB0");
}

bool
UART::connect()
{
  this->fd_ = ::open(this->port_name_.c_str(), O_RDWR);
  if(this->fd_ < 0){
    this->logger_->critical("failed open port : {} | error : {}", this->port_name_, strerror(errno));
    return false;
  }
  if(flock(this->fd_, LOCK_EX | LOCK_NB) == -1) {
    this->logger_->critical("port {} already use by other process. | error : {}", this->port_name_, strerror(errno));
    return false;
  }
  struct termios tty;
  if(tcgetattr(this->fd_, &tty) != 0)
  {
    this->logger_->error("failed get {} info. | error : {}", this->port_name_, strerror(errno));
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
  cfsetispeed(&tty, this->baudrate_);
  cfsetospeed(&tty, this->baudrate_);
  if (tcsetattr(this->fd_, TCSANOW, &tty) != 0)
  {
    this->logger_->error("failed set {} attr. | error : {}", strerror(errno));
    return false;
  }
  return true;
}

} // namespace communication
