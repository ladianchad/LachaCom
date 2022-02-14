#include "communication/serial.hpp"
#include <stdio.h>
#include <string>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>

namespace communication
{

  Serial::Serial(std::string port, int baud_rate, int max_buff) : Communication(max_buff), port_(port), baud_rate_(baud_rate)
  {
  }

  Serial::~Serial()
  {
    if (serial_port_)
    {
      ::close(serial_port_);
    }
  }

  void Serial::init()
  {
    serial_port_ = open(port_.c_str(), O_RDWR);
    std::cout << "Opening " << port_ << " port..." << std::endl;
    if (serial_port_ < 0)
    {
      good_ = false;
      printf("Error %i from open: %s\n", errno, strerror(errno));
    }
    printf("Succesed open port...\n");
    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;

    // Read in existing settings, and handle any error
    printf("Setting port tty...\n");
    if (tcgetattr(serial_port_, &tty) != 0)
    {
      good_ = false;
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    printf("Succesed set port tty...\n");
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
    if (baud_rate_ == 9600)
    {
      cfsetispeed(&tty, B9600);
      cfsetospeed(&tty, B9600);
    }
    else if (baud_rate_ == 115200)
    {
      cfsetispeed(&tty, B115200);
      cfsetospeed(&tty, B115200);
    }
    printf("Setting tty serial...\n");
    if (tcsetattr(serial_port_, TCSANOW, &tty) != 0)
    {
      good_ = false;
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
    printf("!! ALL setting end !!\n");
    good_ = true;
  }

  int Serial::write(const char *data)
  {
    if (serial_port_ >= 0)
    {
      int write_byte = ::write(serial_port_, data, sizeof(data));
      if (write_byte < 0)
      {
        std::cout << "Write Error!!" << std::endl;
        return -1;
      }
      else
        return write_byte;
    }
    else
    {
      std::cout << "Can't use port!!" << std::endl;
      return false;
    }
  }

  int Serial::read(char *buf, int size)
  {
    if (serial_port_ >= 0)
    {
      int read_byte = ::read(serial_port_, buf, size);
      if (read_byte < 0)
      {
        std::cout << "Read Error!!" << std::endl;
        return -1;
      }
      else
        return read_byte;
    }
    else
    {
      std::cout << "Can't use port!!" << std::endl;
      return -1;
    }
  }
}