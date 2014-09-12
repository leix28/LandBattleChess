#include "ezsocket.h"
#include <cassert>

void EzServer::run()
{
    //QThread::run();

    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, (socklen_t*)&clientAddrLen);
    emit connected();
    if (clientSocket == -1) return;

    while (true) {
        char *buf = new char[bufSize];
        memset(buf, 0, bufSize);
        int ret = recv(clientSocket, buf, bufSize, 0);
        if (ret == 0 || ret == -1) {
            delete buf;
            break;
        }
        emit received((void*)buf, ret);
    }
}

EzServer::EzServer(int size, QObject *parent) :
    QThread(parent)
{
    bufSize = size;
}

EzServer::~EzServer()
{
    if (clientSocket) {
        close(clientSocket);
        clientSocket = 0;
    }
    if (serverSocket) {
        close(serverSocket);
        serverSocket = 0;
    }
}

bool EzServer::initServer(char *ip, int port)
{
    if (clientSocket) {
        close(clientSocket);
        clientSocket = 0;
    }
    if (serverSocket) {
        close(serverSocket);
        serverSocket = 0;
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) return 0;

    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_addr.s_addr  = inet_addr(ip);
    serverAddr.sin_port         = htons(port);
    memset(serverAddr.sin_zero, 0x00, 8);

    int ret = bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret == -1) return 0;

    ret = listen(serverSocket, 10);
    if ( ret == -1 ) return 0;

    return 1;
}

bool EzServer::startServer()
{
    if (clientSocket) {
        close(clientSocket);
        clientSocket = 0;
    }
    start();
    return 1;
}

bool EzServer::sendData(void *buf, int len)
{
    if (!clientSocket) return 0;
    if (len > bufSize) return 0;
    int ret = send(clientSocket, buf, len, 0);
    return ret != -1;
}

bool EzServer::closeServer()
{
    if (clientSocket) {
        close(clientSocket);
        clientSocket = 0;
    }
    if (serverSocket) {
        close(serverSocket);
        serverSocket = 0;
    }
    return 1;
}


void EzClient::run()
{
    //QThread::run();

    while (true) {
        char *buf = new char[bufSize];
        memset(buf, 0, bufSize);
        int ret = recv(localSocket, buf, bufSize, 0);
        if (ret == 0 || ret == -1) {
            delete buf;
            break;
        }
        emit received((void*)buf, ret);
    }
}

EzClient::EzClient(int size, QObject *parent) :
    QThread(parent)
{
    bufSize = size;
}

bool EzClient::connectServer(char *ip, int port)
{
    if (localSocket != 0) {
        close(localSocket);
        localSocket = 0;
    }
    localSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (localSocket == -1) return 0;

    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_addr.s_addr  = inet_addr(ip);
    serverAddr.sin_port         = htons(port);
    memset(serverAddr.sin_zero, 0x00, 8);
    int ret = ::connect(localSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret == -1) return 0;
    start();
    return 1;
}

bool EzClient::sendData(void *buf, int len)
{
    if (!localSocket) return 0;
    if (len > bufSize) return 0;
    int ret = send(localSocket, buf, len, 0);
    return ret != -1;
}

bool EzClient::closeClient()
{
    if (localSocket) {
        close(localSocket);
        localSocket = 0;
    }
    return 1;
}

EzClient::~EzClient()
{
    if (localSocket) {
        close(localSocket);
        localSocket = 0;
    }
}
