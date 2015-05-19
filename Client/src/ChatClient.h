#ifndef ChatClient_H_
#define ChatClient_H_

#include "MThread.h"
#include "MCProtocol.h"
#include <strings.h>
#include "TCPSocket.h"
#include "UDPSocket.h"
#include <map>
using namespace std;

class ChatClient : MThread
{
	TCPSocket* tcpeer;
	UDPSocket* udpsock;
	char buffer[100];

	bool chatRoom;
	string history;
	map<string,string> members;  //username - ip

public:

	ChatClient(TCPSocket* sock);

	//udp thread, receives message and forwards to all members
	void run();

	//send server that i created chat (and owner)
	void CreateChat();

	void CloseChat();

	//adds new user from server and send him message history
	void AddNewUser(string user,string ip);

	//removes existing member from server.
	void RmoveUser(string user);

	//send udp message to specific member
	void sendTo(string msg,string ip);

	void DisplayConnected();

	//send msg to all members
	void SendAll(string msg);

	void close();
};


#endif /* ChatClient_H_ */
