### Communication C++ Package

## Setting
```bash
sudo usermod -aG dialout $USER
```

## Requirements

### requiered os
`ubuntu 22.04` `ubuntu 21.04`

### required packages
`cmake` | `wget` | `nlohmann-json` | `libspdlog`

### install requirements 
```bash
  sudo apt install build-essential cmake wget nlohmann-json3-dev libspdlog-dev
```

## Install
```bash
git clone https://github.com/ladianchad/LachaCom.git
cd LachaCom
mkdir build && cd build
sudo cmake --build --install ..
```

## CONFIGURATION
[Configuration](./CONFIGURATION.md)

## Use
### C style
```c
#include <communication/interface.hpp>
#include <communication/interfaces/uart.hpp>
#include <stdio.h>

void onSysPoll(const char in){
  printf("read : %c", in);
}

int main(int argc, char const *argv[])
{
  using namespace communication;

  Logger::set_level(Logger::level::debug);
  InterfaceInitParam param({
    {UART::USE_SYS_POLLING, true},
    {UART::PORT, "/dev/ttyUSB0"},
    {UART::BAUDRATE, 115200}
  });

  UART uart;
  uart.init(param);
  uart.setSysPollingCallback(onSysPoll);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}
```
### C++ Style
```c++
#include <communication/interface.hpp>
#include <communication/interfaces/uart.hpp>
#include <iostream>

int main(int argc, char const *argv[])
{
  using namespace communication;

  Logger::set_level(Logger::level::debug);
  InterfaceInitParam param({
    {UART::USE_SYS_POLLING, true},
    {UART::PORT, "/dev/ttyUSB0"},
    {UART::BAUDRATE, 115200}
  });

  UART uart;
  uart.init(param);
  uart.setSysPollingCallback([&](const char c){
    std::cout<<"read : "<<c<<std::endl;
  });
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}
```

### CMakeLists
```cmake
cmake_minimum_required(VERSION 3.0.2)
project(your project)

findpackage(LaChaCom REQUIRED)

add_executable(${your_executable})
target_link_library(${your_executable} LaChaCom::LaChaCom)

```

Now Exist
- Serial

TODO
- Socket
- I2C
