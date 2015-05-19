#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <map>
#include "User.h"
#include <string.h>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "MSProtocol.h"
#include "MultipleTCPSocketsListener.h"

//listen to all peers and handle the
//incoming commands from the peers.

class Dispatcher : MThread
{
    MultipleTCPSocketsListener* multiListener;
    map<User*,TCPSocket*>* peers;
    char buffer[100];
public:

	//thread
	void run();  //listen to all

    //handle connect request from peer, forward to other peer with proper details
	Dispatcher(map<User*,TCPSocket*>* _peers);

	//add new socket to multi listener
	void AddToOpenPeers(TCPSocket* sock);

	//send to socket current userlist
	void sendUserList(TCPSocket* sock);

	//send to socket current roomlist
	void sendRoomList(TCPSocket* sock);

	//send to socket room's members
	void sendMembersList(TCPSocket* sock,string room);

	//set specific speaker to be chat owner
	void SetActiveChat(string speaker);

	//set current speaker to be single
	void CloseUserChat(string speaker);

	//handle connect request from peer, forward to other peer with proper details
	void ConnectRequest(string speaker,string dest);

	//finds specific peer using ip and port
	map<User*,TCPSocket*>::iterator find(string destIpAndPort);

	//receive tcp msg from sock
	string ReceiveMsg(TCPSocket* sock);

	//send to socket msg with specific type
	void Send(TCPSocket* sock,string msg,int type);

    //close peer using by finding destipandport
	void closePeer(string speaker);
	virtual ~Dispatcher();
};

#endif /* DISPATCHER_H_ */
