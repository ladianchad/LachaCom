#include <communication/communication.hpp>
#include <communication/serial.hpp>
#include <iostream>

class SampleClass
{
public:
  SampleClass(){
    serial_ = std::make_shared<communication::Serial>("/dev/ttyACM0", 115200);
    serial_->init();
    if(serial_->isGood()){
      serial_->setReadInterrupt(std::bind(&SampleClass::onSerialRead, this));
    }
  }

  ~SampleClass(){
    serial_->close();
  }
  
  void run(){
    char c = 'a';
    while(c != 'q'){
      if(std::isalpha(c)){
        std::cin>>c;
        serial_->write(&c);
      }
    }
  }

private:
  
  void onSerialRead(){
    serial_->read(buff_,2);
    std::cout<<"I READ : "<<buff_ << std::endl;
  }

  char buff_[100] {};
  communication::Serial::SharedPtr serial_;
};


int main(int argc, char const *argv[])
{
  SampleClass cl;
  std::cout<<"test..."<<std::endl;
  cl.run();
  return 0;
}
