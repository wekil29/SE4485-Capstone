#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fstream>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

class ServerConfig
{
   private:
      int port_num;
      String ip_addr;
   
      std::fstream readFile(String file_name) { return std::fstream(file_name, std::fstream::in); }
   
   public:
      ServerConfig ServerConfig(String file_name) 
      {
          std::fstream fs;
          String port;
          fs = readFile(file_name);
          std::getline(fs, port);
          std::getline(fs, ip_addr);
          fs.close();
          port_num = std::stoi (port, std::string::size_type);
      }
      
      void setPortNum(int num) { port_num = num; }
      void setIPAddr(String address) { ip_addr = address; }
      int getPortNum() { return port_num; }
      String getIPAddr() { return ip_addr; }
};
