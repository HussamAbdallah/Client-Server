#include <iostream>
#include <winsock.h>
#include <string.h>
#include <queue>
#include<unistd.h>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;


int nsocket;
struct sockaddr_in adr;
int accepted_socket;


char buffer[100] = {0};

queue<int> Q_received;
queue<int> Q_Processed;

void Recv_data();

void Process_data();
void Send_data();

mutex m;

int main()
{             // 0 means success
WSADATA ws;


    /* INITIALZING THE DLL FILE  */

    if (WSAStartup(MAKEWORD(2,2), &ws) < 0 )
    {
        cout << "dll was not initialized\n";
    }

    else
    {
        cout << "Dll is initialized successfully\n";
    }


    /* OPENING A SOCKET */
     nsocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (nsocket <0)
    {
        cout << "the socket is not opened\n";
    }

    else
    {
        cout << "Socket is opened\n";
    }


    /* BINDING ThE CREATED SOCKET to a specific port and IP */


    adr.sin_family = AF_INET;
    adr.sin_port= htons(55555);
    adr.sin_addr.s_addr = INADDR_ANY;

    if ( bind(nsocket, (struct sockaddr*) &adr  , sizeof(adr) ) < 0 )
    {
        cout << "bind failed\n";
    }

    else
    {
        cout << "bind successfully \n" ;
    }



    /* Listening to any client socket */
  if ( (listen(nsocket,10 )) < 0 )
    {
        cout<< "unable to listen ....\n ";
    }

    else
    {
        cout << "listening\n";
    }


    /* accept the request by creating a new socket*/
    accepted_socket = accept(nsocket,NULL,NULL);

    if (accepted_socket<0)
    {
        cout << "accept failed\n";
    }

    else
    {
        cout << "accepted\n";

    }






while(1)
{


thread th1(Recv_data);
thread th2(Process_data);
thread th3(Send_data);

if(th1.joinable())
th1.detach();

if(th2.joinable())
th2.detach();

if(th3.joinable())
th3.detach();

}

}

void Recv_data()
{
    m.lock();
    recv(accepted_socket,buffer,sizeof(buffer),0);
    Q_received.push( atoi(buffer) );
    m.unlock();

}

void Process_data()
{

        m.lock();
        Q_Processed.push(Q_received.front() + 4 );
        Q_received.pop();
        m.unlock();


}

void Send_data()
{


        m.lock();
        itoa(Q_Processed.front(),buffer,10);
        Q_Processed.pop();
        send(accepted_socket,buffer,sizeof(buffer),0);
        m.unlock();

}