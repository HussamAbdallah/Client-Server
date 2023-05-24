#include <iostream>
#include <winsock.h>
#include <string.h>
#include <queue>
#include<unistd.h>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;
typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
} connection_t;


int nsocket;
struct sockaddr_in adr;
int accepted_socket;


char buffer[100] = {0};

queue<int> Q_received;
queue<int> Q_Processed;

void Recv_data(int*ptr);

void Process_data();
void Send_data(int*ptr);
void handle_connection(void* ptr);
mutex m;

int main()
{             // 0 means success
WSADATA ws;
thread th1;
thread th2;
thread th3;
int* ptr;
int sock=-1;

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
     sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (sock <0)
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

    if ( bind(sock, (struct sockaddr*) &adr  , sizeof(adr) ) < 0 )
    {
        cout << "bind failed\n";
    }

    else
    {
        cout << "bind successfully \n" ;
    }



    /* Listening to any client socket */
  if ( (listen(sock,10 )) < 0 )
    {
        cout<< "unable to listen ....\n ";
    }

    else
    {
        cout << "listening\n";
    }




while(1)
{
     ptr = new int[1];
     *ptr = accept(sock,NULL,NULL);

    if ( *ptr <=0 )
    {
        delete[] ptr;
    }

    else

    {
        cout<< "accepted, new socket=" << *ptr<<"\n";
     th1=thread( Recv_data  , ptr);
    th2=thread(Process_data);
    th3=thread(Send_data,ptr);

     th1.detach();
     th2.detach();
     th3.detach();
    }

}


}


void Recv_data(int*ptr)
{
    m.lock();
     char buffer[100];
    int length = recv(*ptr,buffer,sizeof(buffer),0);       // THIS FUNCTION WAITS THE CLIENT TO SEND! THUS, THREAD WON'T 
                                                           // QUIT UNTIL SOMETHING IS RECEIVED, PTR ADRESS IS RESERVED 
     if (  length >0 )                                     // FOR THIS SPECIFIC CLIENT
    {

     cout << "RECEIVED: " << buffer << "\n";
     Q_received.push( atoi(buffer) );
    }
    m.unlock();
    cout<<"RECV_RETURNED";
}

void Process_data()
{
    m.lock();
     Q_Processed.push(Q_received.front() + 4 );
     Q_received.pop();
     m.unlock();
}

void Send_data(int*ptr)
{
    m.lock();
    itoa(Q_Processed.front(),buffer,10);
      Q_Processed.pop();
      cout << "sending on socket" << *ptr;
      send(*ptr,buffer,sizeof(buffer),0);
      m.unlock();
}

