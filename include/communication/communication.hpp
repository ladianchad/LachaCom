#ifndef COMUNICATION_LIBS___
#define COMUNICATION_LIBS___
#include <string>
namespace communication
{
  class Communication
  {
  public:
    Communication(int max_buff) : max_buff_(max_buff) {}

    std::string getType()
    {
      return type_;
    };

    bool isGood()
    {
      return good_;
    }

    virtual void init(){};

    virtual int write(const char *data) = 0;

    virtual int read(char *buf, int size = 1) = 0;
  
   protected:
    std::string type_;
    int max_buff_;
    bool good_;
  };

} // namespace communication

#endif