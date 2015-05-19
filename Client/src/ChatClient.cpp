#include "ChatClient.h"
using namespace std;

ChatClient::ChatClient(TCPSocket* sock)
{
this->tcpeer=sock;
chatRoom = false;
udpsock = new UDPSocket(UDP_PORT);
this->start();
}

void ChatClient::run()
{
	int rc;
    while(1)
    {
    	rc = udpsock->recv(buffer,100);
    	if(rc>0)
    	{   string msg(buffer,rc);
   	        cout<<buffer<<endl;
    	    history = history+msg+'\n';
    	    bzero(buffer, sizeof(char*)*rc);
    	}
    }
}

void ChatClient::DisplayConnected()
{
  cout<<"Connected:"<<endl;
  //mutex
  for(map<string,string>::iterator it=members.begin();it!=members.end();it++)
  {
	  cout<<((*it).first)+"-"+((*it).second)<<endl;
  }
  //mutex
}

//send the server msg that i created a chat room
void ChatClient::CreateChat()
{
  chatRoom=true;
  cout<<"Chat room created"<<endl;
  int crc = ntohl(CREATE_CHAT);
  tcpeer->send((char*)&crc,4);
}

void ChatClient::CloseChat()
{
  cout<<"Chat room was closed"<<endl;
  int crc = ntohl(CLOSE_CHAT);
  tcpeer->send((char*)&crc,4);
  this->SendAll("Chatroom was closed\nbye bye");
  this->members.clear();
  this->chatRoom = false;
}

//add new member to members list and send him msg history
void ChatClient::AddNewUser(string user,string ip)
{
if(!chatRoom && members.size() == 2) {members.clear(); history.clear();}
if(members.find(user) == members.end()) //user is not a chat member yet
{
    members.insert(pair<string,string>(user,ip));
    sendTo(history,ip);
}
}
//send received msg to all active members
void ChatClient::SendAll(string msg)
{
   for(map<string,string>::iterator it = members.begin() ; it != members.end(); it++)
	   sendTo(msg,(*it).second);
}

//single send to specific member
void ChatClient::sendTo(string msg,string ip)
{
	int port = UDP_PORT;
	udpsock->sendTo(msg,ip,port);
}

//remove user from members list
void ChatClient::RmoveUser(string user)
{
 map<string,string>::iterator it= members.find(user);
 if(it != members.end())
	 members.erase(it);
}

void ChatClient::close(){
	udpsock->cclose();
    this->~MThread();
}
