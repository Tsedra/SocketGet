#include "socketClient.h"

class PThreadLockGuard {
public:
    PThreadLockGuard(pthread_mutex_t* mutex) : mutex_(mutex) {
        pthread_mutex_lock(mutex_);
    }

    ~PThreadLockGuard() {
        pthread_mutex_unlock(mutex_);
    }

private:
    pthread_mutex_t* mutex_;
};

SocketClient::SocketClient(){}

SocketClient::SocketClient(std::string address, int port){

    mAddress = address;
    mPort = port;
    mSocket = -1;
    mHints = {0};

    mHints.ai_family = AF_UNSPEC;
    mHints.ai_socktype = SOCK_STREAM;

    int answer;
    if(answer = getaddrinfo(mAddress.c_str(), std::to_string(mPort).c_str(), &mHints, &mSerInfo)){
        std::cerr <<" getaddrinfo: " << gai_strerror(answer) << std:: endl;
        exit(EXIT_FAILURE);
    }

    mConnected = false;
    mThreadRun = false;
    mMutex = PTHREAD_MUTEX_INITIALIZER;
    mPacketSize = 4096;
    //mBuffer = new char[mPacketSize]();
   

}

bool SocketClient::get(){

    if(pthread_create(&mThread_1, nullptr, &innerConnect, this) != 0){
        return false;
    }
    pthread_detach(mThread_1); 
    
    if(pthread_create(&mThread_2, nullptr, &_printClear, this) != 0){
        return false;
    }
    pthread_detach(mThread_2); 
    
    return true;
}


SocketClient::SocketClient(int socket){
    mSocket = socket;
    mConnected = true;
    mThreadRun = false;
    mPacketSize = 4096;
    mBuffer = new char[mPacketSize];
    

}

SocketClient::~SocketClient(){

}

int SocketClient::getSocket(){
    return mSocket;
}
bool SocketClient::superConnect(){
    mThreadRun = true;
    if(mSocket < 0){
        for (mAddrPtr = mSerInfo; mAddrPtr != nullptr; mAddrPtr->ai_next){

            if(mSocket = socket(mAddrPtr->ai_family, mAddrPtr->ai_socktype, mAddrPtr->ai_protocol) == -1){

                std::cerr << " client socket";
                continue;
            }
            if(::connect(mSocket, mAddrPtr->ai_addr, mAddrPtr->ai_addrlen) == -1){

                close(mSocket);
                continue;
            } 
            break;
        }
    }
        
    if(!mAddrPtr){
        return false;
    }

    inet_ntop(mAddrPtr->ai_family, get_in_addr(mAddrPtr->ai_addr), mIP, sizeof mIP);

    std::cout << "\n Client connected to server -> " << getIP() << std::endl;
    mConnected = true;

    freeaddrinfo(mSerInfo);

    while (mThreadRun) {
        std::vector<const char*> tempData;
        for (size_t i = 0; i < mRequests.size(); i++){

            size_t bytesSent = ::send(mSocket, mRequests[i].c_str(), mRequests[i].length(), 0);

            if (bytesSent < 0) {
                std::cerr << "\n send failed\n";
                mThreadRun = false;
                break;
            }

            char response[mPacketSize];

            size_t bytesRead;
            bytesRead = recv(mSocket, response, sizeof(response), 0);

            if (bytesRead < 0) {
                std::cerr << "\n read failed\n";
                break;
            }

            //memset(response, '\0', sizeof(response));
            
            tempData.push_back(response);
           
        }
        pthread_mutex_lock(&mMutex);

        mDataResponse.insert(mDataResponse.begin(), tempData.begin(), tempData.end());

        pthread_mutex_unlock(&mMutex);
        //mThreadRun = false;
    }
        
    return false;
    
}
void SocketClient::printClear(){
    while(mThreadRun){
        
        pthread_mutex_lock(&mMutex);
        if(!mDataResponse.empty()){
            mResponse.insert(mResponse.begin(), mDataResponse.begin(), mDataResponse.end());
            mDataResponse.clear();
        }
        pthread_mutex_unlock(&mMutex);
        
        if(!mResponse.empty()){
            for(auto &item : mResponse){
                std::cout << item;
            }
            mResponse.clear();
        }
      
    }

}


bool SocketClient::setRequests(std::vector<std::string> &requests){

    if(!requests.empty()){
        mRequests.insert(mRequests.end(), requests.begin(), requests.end());
        return true;
    }
    return false;

}

void SocketClient::disconnect(){
    close(mSocket);
    mConnected = false;
    mThreadRun = false;
}

char* SocketClient::getIP(){
    return mIP;
}

void* SocketClient::get_in_addr(sockaddr* sa){

    if(sa->sa_family == AF_INET)
        return &(((sockaddr_in*) sa)->sin_addr);

    return &(((sockaddr_in6*) sa)->sin6_addr);
}

