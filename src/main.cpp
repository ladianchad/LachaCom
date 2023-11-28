#include <communication/interface.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
  communication::Logger::set_level(communication::Logger::level::debug);
  communication::InterfaceInitParam param({
    {communication::Interface::USE_INTERRUPT_K, true}
  });

  communication::Interface interface(1, communication::Type::UART);
  interface.init(param);
  std::this_thread::sleep_for(std::chrono::milliseconds(4000));
  return 0;
}
