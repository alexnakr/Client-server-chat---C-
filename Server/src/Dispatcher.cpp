#include "Dispatcher.h"

Dispatcher::Dispatcher(map<User*,TCPSocket*>* _peers)
{
	this->peers = _peers;
	multiListener = new MultipleTCPSocketsListener();
	cout<<"Dispatcher started"<<endl;
	start();
}

void Dispatcher::run()
{
  while(1)
  {
	  TCPSocket* speaker = multiListener->listenToSocket(10);
	  if(speaker!=NULL)
	  {
	  int cmd;
	  speaker->recv((char*)&cmd,4);
	  cmd = ntohl(cmd);
	  switch(cmd)
		  {
	      case SESSION_ESTABLISHED:
		      cout<<"connected";
	    	  break;

		  case OPEN_SESSION_WITH_PEER:
			  {string dest = ReceiveMsg(speaker);
			   ConnectRequest(speaker->destIpAndPort(),dest);}
			   break;

		  case USER_LIST:
			   sendUserList(speaker);
			   break;

		  case ROOM_LIST:
			   sendRoomList(speaker);
			   break;

		  case MEMBERS_LIST:
			  {string room = ReceiveMsg(speaker);
			   sendMembersList(speaker,room);}
			   break;

		  case CREATE_CHAT:
			   SetActiveChat(speaker->destIpAndPort());
			   break;

		  case CLOSE_CHAT:
			   CloseUserChat(speaker->destIpAndPort());
			   break;

		  case CLOSE_SESSION_WITH_PEER:
			   closePeer(speaker->destIpAndPort());
			   break;

		  default:
		  break;
		 }
	   }
   }
}

void Dispatcher::AddToOpenPeers(TCPSocket* sock)
{
	multiListener->addSocket(sock);
}

void Dispatcher::closePeer(string speaker)
{
	map<User*,TCPSocket*>::iterator it = find(speaker);
	{ if(it!=peers->end()) peers->erase((it));
	  (*it).second->cclose();
	  (*it).second=NULL;
	}
}

//handle connect request from peer, forward to other peer with proper details
void Dispatcher::ConnectRequest(string speaker,string dest)
{
	string sourceUser,sourceIp,msg;
	map<User*,TCPSocket*>::iterator is = find(speaker);
	if(is!=peers->end() && dest.length()>0)
	{
		sourceUser=(*is).first->username;
	    sourceIp = (*is).second->destIp();
	    msg = sourceUser+'-'+sourceIp;
	    cout<<"Request from "<<msg<<" to join "<<dest<<endl;
	    for(map<User*,TCPSocket*>::iterator id = peers->begin(); id != peers->end() ; id++) //finds dest
	       if(((*id).first->username)==dest)
	       {
	    	  this->Send((*id).second,msg,OPEN_SESSION_WITH_PEER);
	    	  (*id).first->addUserToChat((*is).first);
	    	  (*is).first->status = CHAT_MEMBER;
	    	  return;
	       }
	}
}

void Dispatcher::SetActiveChat(string speaker)
{
	map<User*,TCPSocket*>::iterator it = find(speaker);
    if(it!=peers->end()) (*it).first->status=CHAT_OWNER;
}

map<User*,TCPSocket*>::iterator Dispatcher::find(string destIpAndPort)
{
	for(map<User*,TCPSocket*>::iterator it = peers->begin(); it != peers->end() ; it++)
		  if((*it).second->destIpAndPort()==destIpAndPort)
			  return it;
	return peers->end();
}

string Dispatcher::ReceiveMsg(TCPSocket* sock)
{
	int msgLen;
	sock->recv((char*)&msgLen,4);
	msgLen = ntohl(msgLen);
	int rc = sock->recv(buffer, msgLen);
	if (rc!=msgLen)
		sock = NULL;
	string msg(buffer,rc);
	return msg;
}

void Dispatcher::CloseUserChat(string speaker)
{
	map<User*,TCPSocket*>::iterator it = find(speaker);
    if(it!=peers->end())
    {(*it).first->status=SINGLE;
    (*it).first->chatMembers.clear();
    }
	//send user to close chat
}

void Dispatcher::Send(TCPSocket* sock,string msg,int type)
{
	int msgLen = htonl(msg.length());
	int send = ntohl(type);
	sock->send((char*)&send,4);
	sock->send((char*)&msgLen,4);
	sock->send(msg.data(),msg.length());
}

void Dispatcher::sendUserList(TCPSocket* sock)
{  string lst;
    //mutex
    for(map<User*,TCPSocket*>::iterator it = peers->begin(); it != peers->end() ; it++)
	lst = lst + (*it).first->username+'-'+(*it).second->destIp()+' ';
    //mutex
  this->Send(sock,lst,USER_LIST);
}

void Dispatcher::sendMembersList(TCPSocket* sock,string room)
{
	string lst;
	for(map<User*,TCPSocket*>::iterator it = peers->begin(); it != peers->end() ; it++)
	   if((*it).first->username == room)
		   {lst = (*it).first->retrieveMembers();
		   Send(sock,lst,MEMBERS_LIST);
		   return;
		   }
}

void Dispatcher::sendRoomList(TCPSocket* sock)
{  string lst;
   for(map<User*,TCPSocket*>::iterator it = peers->begin(); it != peers->end() ; it++)
   {
    //mutex
	if((*it).first->status==CHAT_OWNER)
	lst = lst + (*it).first->username+'-'+(*it).second->destIp()+' ';
    //mutex
   }
  this->Send(sock,lst,ROOM_LIST);
}
Dispatcher::~Dispatcher()
{
	this->peers->clear();
	this->~MThread();
}
