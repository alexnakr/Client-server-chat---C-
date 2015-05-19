#include "MessengerServer.h"
using namespace std;

MessengerServer::MessengerServer()
{
	servSock = new TCPSocket(TCP_PORT);
	start();
	usersDB.push_back(User("admin","123"));
	usersDB.push_back(User("user","123"));
}

//Handles new connected user (Login/Sign_up) and transfers him
//to Dispatcher for further communication
void MessengerServer::run()
{
	Dispatcher dispatch(&peers);
	while(1)
	{
        TCPSocket* peer = servSock->listenAndAccept();
        if(peer!=NULL)
        {
            cout<<"Starting authentication"<<endl;
            int authMsg = ntohl(AUTHENTICATION);
            peer->send((char*)&authMsg,4);
            int cmd;
            peer->recv((char*)&cmd,4);
            cmd = ntohl(cmd);
            switch(cmd)              //according to protocol user must confirm auth
            {
            case AUTHENTICATION:
                 {string cred(ReceiveMsg(peer));
                  size_t userPos = cred.find("user:");
                  size_t passPos = cred.find("pass:");
                  if(userPos != string::npos && passPos != string::npos) //if proper auth msg
                  {
                   string user = cred.substr(strlen("user:")+userPos,passPos-strlen("user:")-userPos);
                   string pass = cred.substr(strlen("user:")+passPos);
                        if(checkAuthCred(user,pass,peer))
                        dispatch.AddToOpenPeers(peer);
                        else sendDenyMsg(peer);
                  }
                  else
                  sendDenyMsg(peer);
            	  }break;

            case SIGNUP:
                    {string cred(ReceiveMsg(peer));
            		size_t userPos = cred.find("user:");
            		size_t passPos = cred.find("pass:");
            		if(userPos != string::npos && passPos != string::npos) //if proper auth msg
            		{
            		 string user = cred.substr(strlen("user:")+userPos,passPos-strlen("user:")-userPos);
            		 string pass = cred.substr(strlen("user:")+passPos);
            		 if(!CheckUser(user))
            			 {
            			  User* usr = new User(user,pass);
            			  usersDB.push_back(*usr);
            		      cout<<"New user "<<user<<" created"<<endl;
            		      sendCreateMsg(peer);}
            		 else
            			 sendDenyMsg(peer);
            		}}break;
            }
        }
	}
}


void MessengerServer::sendDenyMsg(TCPSocket* peer)
{
	int denyMsg = ntohl(SESSION_REFUSED); //bad login - deny
	peer->send((char*)&denyMsg,4);
}

void MessengerServer::sendCreateMsg(TCPSocket* peer)
{
	int created = ntohl(LOGGED_NEW_USER);
	peer->send((char*)&created,4);
}

//go through USERS Database and check if credentials are valid
bool MessengerServer::checkAuthCred(string user,string pass,TCPSocket* peer)
{
	for(vector<User>::iterator it = usersDB.begin(); it != usersDB.end() ; it++)
	{
	  if((*it).username == user)
	    if((*it).password == pass)
	    {
	         cout<<"Authenticated successfuly"<<endl;
	         int sesEst = ntohl(SESSION_ESTABLISHED);
	         peer->send((char*)&sesEst,4);
             peers.insert(pair<User*,TCPSocket*>(&(*it),peer));
	         return true;
	     }
	}
 return false;
}

string MessengerServer::ReceiveMsg(TCPSocket* sock)
{
	int msgLen;
	sock->recv((char*)&msgLen,4);
	msgLen = ntohl(msgLen);
	char buffer[100];
	memset((void*)buffer,0,100);
	sock->recv(buffer, msgLen);
	return (string)buffer;
}

void MessengerServer::ListPeers()
{
	for(map<User*,TCPSocket*>::iterator it = peers.begin(); it != peers.end() ; it++)
	{
        //mutex
	     cout<<"peer "<<(*it).first->username<<": "<<(*it).second->destIpAndPort()<<endl;
	    //mutex
	}
}

//
bool MessengerServer::CheckAuthenticated(string peer)
{
	map<User*,TCPSocket*>::iterator it = find(peer);
    if(it!=peers.end()) return true;
    else return false;
}

//check if username is already exists in USERS Database
bool MessengerServer::CheckUser(string user)
{
	//mutex
	for(vector<User>::iterator it = usersDB.begin(); it != usersDB.end() ; it++)
          if((*it).username==user)
			  return true;
	//mutex
	return false;
}

map<User*,TCPSocket*>::iterator MessengerServer::find(string destIpAndPort)
{
	for(map<User*,TCPSocket*>::iterator it = peers.begin(); it != peers.end() ; it++)
		  if((*it).second->destIpAndPort()==destIpAndPort)
			  return it;
	return peers.end();
}

//list users
void MessengerServer::ListUserNames()
{
    string lst;
    //mutex
    for(vector<User>::iterator it=usersDB.begin();it!=usersDB.end();it++)
   	 lst = lst + it->username+'\n';
    //mutex
    if(lst.length()>0) cout<<lst<<endl;
}

//list pairs
void MessengerServer::ListPairs()
{
    string lst;
    //mutex
    for(vector<User>::iterator it=usersDB.begin();it!=usersDB.end();it++)
   	 if(it->status==PAIR) lst = lst + it->username+'\n';
    //mutex
    if(lst.length()>0) cout<<lst<<endl;
}

//list rooms
void MessengerServer::ListRooms()
{
    string lst;
    //mutex
    for(vector<User>::iterator it=usersDB.begin();it!=usersDB.end();it++)
   	 if(it->status==CHAT_OWNER) lst = lst + it->username+'\n';
    //mutex
    if(lst.length()>0) cout<<lst<<endl;
}

void MessengerServer::close()
{
	this->~MessengerServer();
}

MessengerServer::~MessengerServer()
{
	this->usersDB.clear();
	this->peers.clear();
	servSock->cclose();
	servSock = NULL;
    this->~MThread();
}


