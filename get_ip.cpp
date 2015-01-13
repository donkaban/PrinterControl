#include <iostream>
#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <vector>

std::vector<std::string> get_ip()
{
    static const char *fmt = "%8s: %s";
    char buff[256];
    std::vector<std::string> res;
    struct ifaddrs * if_addr = NULL;
    struct ifaddrs * ifa     = NULL;
    void * addr_ptr          = NULL;
    getifaddrs(&if_addr);
    for (ifa = if_addr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (!ifa->ifa_addr) continue;
        if (ifa->ifa_addr->sa_family == AF_INET) 
        { 
            addr_ptr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, addr_ptr, addressBuffer, INET_ADDRSTRLEN);
            sprintf(buff,fmt, ifa->ifa_name, addressBuffer);
            res.push_back(std::string(buff)); 
        } 
    }
    if (if_addr!=NULL) freeifaddrs(if_addr);
    return res;
}


