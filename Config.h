#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fstream>
#define SA struct sockaddr

class Config
{
   private:
      int port_num;
      std::string ip_addr;
   
      std::fstream readFile(std::string file_name) { return std::fstream(file_name, std::fstream::in); }
   
   public:
      Config(std::string file_name)
      {
          std::fstream fs;
          std::string port;
          fs = readFile(file_name);
          std::getline(fs, port);
          fs.close();
          port_num = std::atoi (port.c_str());
      }
      
      void setPortNum(int num) { port_num = num; }
      int getPortNum() { return port_num; }
};
