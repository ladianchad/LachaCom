#ifndef COMMUNICATION__INTERFACES__UART_HPP_
#define COMMUNICATION__INTERFACES__UART_HPP_

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <poll.h>

#include "communication/interface.hpp"

namespace communication
{

class UART : public Interface
{
public:

  static const char * BAUDRATE;
  static const char * PORT;

  UART();

  std::string getFDName() const override;

  bool connect() override;

protected:

  bool onInit() override;

  void prepareParams() override;

  std::string port_name_;
  speed_t baudrate_;
};
  
} // namespace communication

#endif