#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <fstream>
#define SA struct sockaddr

class Config
{
   private:
      int port_num;
      std::string ip_addr;

   public:
      Config(std::string file_name)
      {
          std::fstream fs;
          std::string port;
          fs.open(file_name.c_str(), std::fstream::in);
          std::getline(fs, port);
          fs.close();
          port_num = std::stoi (port);
      }

      void setPortNum(int num) { port_num = num; }
      int getPortNum() { return port_num; }
};
