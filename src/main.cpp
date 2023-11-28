#include <communication/interface.hpp>
#include <communication/interfaces/uart.hpp>
#include <iostream>

void Callback(const char data) {
  std::cout<<data<<std::endl;
}

int main(int argc, char const *argv[])
{
  using namespace communication;

  Logger::set_level(Logger::level::debug);
  InterfaceInitParam param({
    {UART::USE_SYS_POLLING, true},
    {UART::PORT, "/dev/ttys8"},
    {UART::BAUDRATE, 115200}
  });

  UART uart(1);
  uart.init(param);
  uart.setSysPollingCallback(Callback);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}
