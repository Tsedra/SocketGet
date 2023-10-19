#include "request.h"


std::string Request::getProtocol( std::string &url ){
    std::string protocol = "";

    for(int i = 0; i < url.size(); i++){
        if( url[i] != '/' || url[i+1] != '/'  ){
            protocol += url[i];
        }
        else{
            protocol += "//";
            break;
        }
    }

    return protocol;
}

std::string Request::getHost( std::string &url ){
    std::string host = "";
    url.replace(0, getProtocol(url).size(), "");


    for(int i = 0; i < url.size(); i++){

        if( url[i] != '/' ){
            host += url[i];
        }
        else{
            break;
        }
    }

    return host;
}

std::string Request::getAction( std::string &url ){
    std::string parm = "";

    url.replace(0, getProtocol(url).size()+getHost(url).size(), "");

    for(int i = 0; i < url.size(); i++){

        if( url[i] != '?' && url[i] != '#' ){
            parm += url[i];
        }
        else{
            break;
        }
    }

    return parm;
}

std::string Request::getParams( std::vector< std::pair< std::string, std::string> > requestData ){
    std::string parm = "";

    std::vector< std::pair< std::string, std::string> >::iterator itr = requestData.begin();

    for( ; itr != requestData.end(); ++itr ){

        if( parm.size() < 1 ){
            parm += "";
        }
        else{
            parm += "&";
        }
        parm += itr->first + "=" + itr->second;
    }

    return parm;
}

//, std::vector< std::pair< std::string, std::string>> requestData  = {std::make_pair("","")
std::string Request::GET( std::string url)
{
    std::string http = getProtocol(url);
    std::string host = getHost(url);
    std::string script = getAction(url);
    //std::string parm = getParams(requestData );

    std::string mGETdata = "";

    mGETdata += "GET ";
    mGETdata += host + script ;//+ "?";// + parm;
    mGETdata += (std::string)" HTTP/1.1" + "\r\n";
    mGETdata += (std::string)"Host: " + http + host + "/" + "\r\n";
    mGETdata += (std::string)"User-Agent: Mozilla/5.0" + "\r\n";
    //header += (std::string)"Accept: text/html" + "\r\n";
    mGETdata += (std::string)"Accept-Language: ru,en-us;q=0.7,en;q=0.3" + "\r\n";
    mGETdata += (std::string)"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7" + "\r\n";
    mGETdata += (std::string)"Connection: keep-alive " + "\r\n";
    mGETdata += "\r\n";
    
    return mGETdata;;
}

std::string Request::getReqGET(){
    return mGETdata;
}
Request::Request(std::string url){
    mGETdata = GET(url);
}
Request::~Request(){
    
};
