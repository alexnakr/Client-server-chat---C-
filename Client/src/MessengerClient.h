#include "string.h"
#include <map>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "ChatClient.h"
#include "MCProtocol.h"

using namespace std;

enum eList
{
USERLIST = 1,
ROOMLIST = 2,
MEMBERSLIST = 3,
};

class MessengerClient : MThread
{
	TCPSocket* tcpeer;
	ChatClient* chat;

	char buffer[100];
	string password;
	string username;
	string user_list;
	string room_list;
	string destIP;
	bool signup;
public:

	MessengerClient();

	void run();

	//connect to the given server ip (the port is defined in the protocol header file)
	bool connect(string ip);

	//return true if connected to the server
	bool isConnected();

	//disconnect from messenger server
	bool disconnect();

	//open session with the given username / chatroom
	void open(string dest);

	int ReceiveBuffer(TCPSocket* sock,char* arr);

	//return true if a session is active
	bool isActiveClientSession();

	void CloseChatRoom();

	void OpenChatRoom();

	//close active session
	bool closeActiveSession();

	//retrieve user list from server
	void GetUserLIst();

	//receive the list
	void ReceiveList(eList room);

	//display current chat or user List
	void DisplayList();

	//create new user at server
	void newUserSignup();

	//set current user to signup
	void SetSignup();

	//send the given message to the connected UDP peer
	void Send(string message);

	//display all rooms as in the server
	void DisplayRooms();

	void DisplayConnectedList();

	//chat room members display
	void GetMembersLIst(string room);

	//create new chat room
	void openChatRoom();

	void SendTCPData(TCPSocket* sock,string data);

	//send authentication credentials
	void SendAuth();

	void MessengerSetCred(string user,string pass) {username = user; password = pass;}

};

