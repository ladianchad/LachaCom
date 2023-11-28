#include <communication/interface.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
  using namespace communication;

  Logger::set_level(Logger::level::debug);
  InterfaceInitParam param({
    {Interface::USE_SYS_POLLING, true}
  });

  Interface interface(1, Type::UART);
  interface.init(param);
  std::this_thread::sleep_for(std::chrono::milliseconds(4000));
  return 0;
}
