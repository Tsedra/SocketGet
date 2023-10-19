#ifndef DEF_SOCKETCLIENT
#define DEF_SOCKETCLIENT

#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <map>

#include "request.h" // не используется

class SocketClient {
    private:

        std::vector<const char*> mDataResponse; //  Для всех полученных ответов и синхронизации между потоками 
        std::vector<std::string> mRequests; // Для всех запросов, которые будут отправляться.
        pthread_mutex_t mMutex;
        addrinfo mHints, *mSerInfo, *mAddrPtr; // Информация хостов как для ip4, так  ip6 для получения нужного адреса.

        std::string mAddress;//хост

        int mPort; //
        int mSocket; //
        char mIP[INET6_ADDRSTRLEN];// ntop, представление адреса

        bool mConnected; //
        bool mThreadRun;//флаги в циклах
                        //

        int mPacketSize;// размер буфера
        char *mBuffer;

        pthread_t mThread_1, mThread_2;

        std::vector<std::string> mResponse;// тоже самое, что mDataResponse, только доступна одному потоку

        static void* innerConnect(void* _client){ // статическая функия для первого потока для вызова метода superConnect
            SocketClient *client = static_cast<SocketClient*> (_client);
            client->superConnect();
            return nullptr;
        }
        static void* _printClear(void* _client){// для второго потока
            SocketClient *client = static_cast<SocketClient*> (_client);
            client->printClear();
            return nullptr;
        }


    protected:
        bool superConnect(); // создает сокет, коннект, отправляет запросы и читает из сокета данные, котоыре передает их второму потоку
        void printClear();// получает 

    public:
        SocketClient();
        SocketClient(std::string address, int port);// вся информация по искомому адресу.
        SocketClient(int socket);
        ~SocketClient();

        int getSocket();
        void* get_in_addr(sockaddr*);// для преобразования в sockaddr из ip4 или ip6 семейства
        bool get();// создает два потока для передачи функций superConnect и printClear
        void disconnect();//закрывает сокет
        bool setRequests(std::vector<std::string> &requests); // устанавливает отправлямые запросы.
        char* getIP();
        
};

#endif
