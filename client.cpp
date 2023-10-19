#include <iostream>
#include "socketClient.h"


int main(int argc, char ** argv){

    SocketClient client("example.com", 80);

    std::vector<std::string> requests = {"GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n", 
                                         "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n" }; //Connection: keep-alive\r\n\r\n
    client.setRequests(requests);

    if(!client.get()){
        std::cerr << " Client failed to get " << std::endl;
        return EXIT_FAILURE;
    }

    getchar();
    //sleep(60);

    client.disconnect();

    return 0;
}
