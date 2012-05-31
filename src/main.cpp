#include <iostream>

#include "io.h"

using namespace std;

void AcceptConnHandler(int fd, void *userdata, int mask)
{
    int connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen);
    if (connfd < 0){
        perror("connfd < 0");
        exit(1);
    }
    // setnonblocking(connfd);

    char *str = inet_ntoa(clientaddr.sin_addr);
    cout << "accapt a connection from " << str << endl;

    IO::IOService *service = reinterpret_cast<IO::IOService*>(userdata);
    service->AddEvent(connfd, NULL, IO_READABLE);
}



int main()
{

    struct sockaddr_in clientaddr;

    int listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char *local_addr="127.0.0.1";
    inet_aton(local_addr, &(serveraddr.sin_addr));
    serveraddr.sin_port = htons(8000);
    bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, 5);

    IOService io_service;
    io_service.AddEvent(listenfd, IO_READABLE, &io_service)

    io_service.Run();

    cout << "Hello world!" << endl;
    return 0;
}
