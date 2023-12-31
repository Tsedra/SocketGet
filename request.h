#ifndef DEF_REQUEST
#define DEF_REQUEST

#include <string>
#include <iostream>
#include <vector>


class Request{

private:
    const char *http = {"https://"};
    const char *host = { "localhost"};
    const char *parm = {"/api/method.php"};
    std::string mGETdata;
    std::string getProtocol( std::string &url );
    std::string getHost( std::string &url );
    std::string getAction( std::string &url );
    std::string getParams( std::vector< std::pair< std::string, std::string> > requestData );


public:
    std::string getReqGET();
    std::string GET( std::string url);
    Request(std::string url);
    ~Request();
};



#endif
