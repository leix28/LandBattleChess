#ifndef EZSOCKET_H
#define EZSOCKET_H

#include <QThread>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>


class EzServer : public QThread
{
    Q_OBJECT

    int serverSocket = 0, clientSocket = 0, bufSize = 0, port = 4321;
    sockaddr_in serverAddr;

protected:
    void run();

public:
    explicit EzServer(int size, QObject *parent = 0);
    ~EzServer();
    bool initServer(char *ip, int port);
    bool startServer();
    bool sendData(void *buf, int len);
    bool closeServer();

signals:
    void received(void *buf, int len);
    void connected();
};

class EzClient : public QThread
{
    Q_OBJECT

    int localSocket = 0, bufSize = 0;
    sockaddr_in serverAddr;

protected:
    void run();

public:
    explicit EzClient(int size, QObject *parent = 0);
    bool connectServer(char *ip, int port);
    bool sendData(void *buf, int len);
    bool closeClient();
    ~EzClient();

signals:
    void received(void *buf, int len);
};

#endif // EZSOCKET_H
