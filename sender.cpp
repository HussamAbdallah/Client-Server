#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int main()
{
    /* INITIALZING THE DLL FILE  */

    WSADATA ws;
    if (WSAStartup(MAKEWORD(2,2), &ws) < 0 )
    {
        cout << "dll was not initialized\n";
    }

    else
    {
        cout << "Dll is initialized successfully\n";
    }


    /* OPENING A SOCKET */
    int new_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (new_socket <0)
    {
        cout << "the socket is not opened\n";
    }

    else
    {
        cout << "Socket is opened\n";
    }


    /* Connecting to a server socket */


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(55555);     // port 555 55 is initialized in the server side
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");    // very important!!!


    if ( connect(new_socket,( struct sockaddr*) &addr , sizeof(addr)) < 0 )
    {
        cout << "failed to connect\n";
    }

    else
    {
        cout << "Connected!!\n";
    }


    char buffer[100] = {0};

    // enter the data to be sent to the server
    printf("Send your message:  \n");



    gets(buffer);
    send(new_socket,buffer,sizeof(buffer),0);
  recv(new_socket,buffer,sizeof(buffer),0);
    printf("Received from the server: %s\n",buffer);





}
