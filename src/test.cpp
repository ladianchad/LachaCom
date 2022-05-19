#include <communication/communication.hpp>
#include <communication/serial.hpp>
#include <iostream>
#include <locale>
communication::Serial *serial;
char buff[100];
void interrupt(){
  serial->read(buff,2);
  std::cout<<"I READ : "<<buff << std::endl;
}

int main(int argc, char **argv)
{
  serial = new communication::Serial("/dev/ttyACM0", 115200);
  std::cout<<serial<<std::endl;
  serial->init();
  std::cout<<"check is good : "<<serial->isGood()<<std::endl;
  serial->setReadInterrupt(&interrupt);
  char c = 'a';
  while(c != 'q'){
    if(std::isalpha(c)){
      std::cin>>c;
      serial->write(&c);
    }
  }
  serial->close();
  return 0;
}