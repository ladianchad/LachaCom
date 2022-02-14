#include <communication/communication.hpp>
#include <communication/serial.hpp>

int main(int argc, char **argv)
{
  communication::Communication *communicator;
  communicator = new communication::Serial("/dev/ttyUSB0", 9600);
  communicator->init();
  printf("%s", communicator->isGood() ? "good" : "flase");
  return 0;
}