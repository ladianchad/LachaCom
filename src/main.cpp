#include <communication/interface.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
  communication::InterfaceInitParam param({
    {communication::Interface::USE_INTERRUPT_K, true}
  });
  return 0;
}
