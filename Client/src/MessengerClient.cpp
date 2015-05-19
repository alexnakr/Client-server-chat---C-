#include "MessengerClient.h"

MessengerClient::MessengerClient()
{
tcpeer=NULL;
signup = false;
}

void MessengerClient::run()
{
	while(1)
    {
	    memset((void*)buffer,0,100);
	    int cmd;
	    tcpeer->recv((char*)&cmd,4);
		cmd = ntohl(cmd);
		switch(cmd)
		{
			case OPEN_SESSION_WITH_PEER:
		         {
		         int len = ReceiveBuffer(tcpeer,buffer);
		         cout<<buffer<<" joined"<<endl;
		         string msg(buffer,len);
		         size_t pos= msg.find('-');
		         string username = msg.substr(0,pos);
		         string ip = msg.substr(pos+1);
		         chat->AddNewUser(username,ip);
		         }
		         break;

			case SEND_MSG_TO_PEER:
				 ReceiveBuffer(tcpeer,buffer);
  			     cout<<buffer<<endl;break;

			case AUTHENTICATION:
				 if(!signup)SendAuth();
				 else {newUserSignup();signup=false;}
				 break;

			case SESSION_ESTABLISHED:
                 cout<<"Connected"<<endl;
                 break;

			case SESSION_REFUSED:
				 cout<<"Bad credentials / user already exists";
				 break;

			case LOGGED_NEW_USER:
				 cout<<"New user created. please connect again"<<endl;
				 break;

			case USER_LIST:
				 ReceiveList(USERLIST);
				 break;

			case ROOM_LIST:
				ReceiveList(ROOMLIST);
                break;

			case MEMBERS_LIST:
				 ReceiveList(MEMBERSLIST);
				 break;

			case CLOSE_SESSION_WITH_PEER:
				 cout<<"disconnected"<<endl;
				 break;

			default:
				 break;
		}
    }
}

void MessengerClient::DisplayRooms()
{
		int send = ntohl(ROOM_LIST);
		tcpeer->send((char*)&send,4);
}

//connect to the given server ip (the port is defined in the protocol header file
bool MessengerClient::connect(string ip){
	tcpeer = new TCPSocket(ip,TCP_PORT);
	chat = new ChatClient(tcpeer);
	if(tcpeer->getSocketFid()>0)
	{
		this->start();
		return true;
	}
	return false;
}

//send to the server new user credentials
void MessengerClient::newUserSignup()
{
	int sign = ntohl(SIGNUP);
	tcpeer->send((char*)&sign,4);
	string msg = "user:"+username+"pass:"+password;
	this->SendTCPData(tcpeer,msg);
}

 //return true if connected to the server
bool MessengerClient::isConnected(){
	if(tcpeer->destIpAndPort()=="")
		return false;
	return true;
}

 //disconnect from messenger server
bool MessengerClient::disconnect(){
	chat->close();
	int close = ntohl(CLOSE_SESSION_WITH_PEER);
			tcpeer->send((char*)&close,4);
	tcpeer = NULL;
	return true;
}

 //set destination ip according to username (go through received list)
void MessengerClient::open(string dest)
{
	if(user_list.size()>0)
	{
	string list = user_list+room_list;
	size_t pos =list.find(dest+"-");
	size_t endpos = list.find(" ",pos);
	if(pos != string::npos)
	{	destIP = list.substr(pos+strlen(dest.c_str())+1,pos+strlen(dest.c_str())+1-endpos);
	    int con = ntohl(OPEN_SESSION_WITH_PEER);
	    tcpeer->send((char*)&con,4);
	    this->SendTCPData(tcpeer,dest);
	}
	else
	{
	cout<<"Please update user/chat list"<<endl;
	}
	}
}

 //return true if a session is active
bool MessengerClient::isActiveClientSession()
{
	if(tcpeer!=NULL)
		return true;
	return false;
}

void MessengerClient::DisplayConnectedList()
{
chat->DisplayConnected();
}

//close active session
bool MessengerClient::closeActiveSession()
{
	tcpeer->cclose();
	this->~MThread();
	return true;
}

//display user / chat list
void MessengerClient::DisplayList()
{
   cout<<user_list<<endl;
}

//Upon connect to server set this option to perfom signup
void MessengerClient::SetSignup()
{
	signup = true;
}

 //send UDP message to connected peer
void MessengerClient::Send(string message)
{
   string msg = '<'+username+'>' + message;
	chat->sendTo(msg,destIP);
}

//receive tcp message from server
int MessengerClient::ReceiveBuffer(TCPSocket* sock,char* arr)
{
	int msgLen;
	sock->recv((char*)&msgLen,4);
	msgLen = ntohl(msgLen);
	int rc = sock->recv(buffer, msgLen);
	if(rc != msgLen)
	  sock=NULL;
	return rc;
}

//receive list from server
void MessengerClient::ReceiveList(eList type)
{
	int msgLen;
	tcpeer->recv((char*)&msgLen,4);
	msgLen = ntohl(msgLen);
	int rc = tcpeer->recv(buffer, msgLen);
	cout<<"List - "<<buffer<<endl;
	string list(buffer,rc);
	if(type==USERLIST) 	{user_list.clear();user_list = list;}
	else if(type==ROOMLIST) {room_list.clear(); room_list = list;}
	if(rc!=msgLen)
	    tcpeer=NULL;
}

//create new chat room on udp port
void MessengerClient::OpenChatRoom()
{
   chat->CreateChat();
}

void MessengerClient::CloseChatRoom()
{
	chat->CloseChat();
}

//send tcp message (to server)
void MessengerClient::SendTCPData(TCPSocket* sock,string msg)
{
	int msgLen = msg.length();
	msgLen = htonl(msgLen);
	sock->send((char*)&msgLen,4);
	sock->send(msg.data(),msg.length());
}

//send server USER_LIST request and wait for respond
void MessengerClient::GetUserLIst()
{
	int send = ntohl(USER_LIST);
	tcpeer->send((char*)&send,4);
}

void MessengerClient::GetMembersLIst(string room)
{
	int send = ntohl(MEMBERS_LIST);
	tcpeer->send((char*)&send,4);
	this->SendTCPData(tcpeer,room);
}

//send server AUTH response and wait for accept / deny
void MessengerClient::SendAuth()
{
int auth = ntohl(AUTHENTICATION);
tcpeer->send((char*)&auth,4);
string msg = "user:"+username+"pass:"+password;
this->SendTCPData(tcpeer,msg);
}


