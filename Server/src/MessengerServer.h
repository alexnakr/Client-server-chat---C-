#include <string.h>
#include <map>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "MSProtocol.h"
#include "Dispatcher.h"
#include "User.h"

using namespace std;

class MessengerServer: MThread
{
	vector<User> usersDB;          //Registered users DB
	TCPSocket* servSock;           //main server listener
	map<User*,TCPSocket*> peers;   //map of ACTIVE user + proper TCPSocket channel

public:
	MessengerServer();
	void run();
	//check if source_ip::port is already loggedin
	bool CheckAuthenticated(string peer);

	//Receive control msg
	string ReceiveMsg(TCPSocket* sock);

	//check user's credentials
	bool checkAuthCred(string user,string pass,TCPSocket* sock);

	//check if user already exists in users DB
	bool CheckUser(string user);

	//send session denied
	void sendDenyMsg(TCPSocket* peer);

	void sendCreateMsg(TCPSocket* peer);

	map<User*,TCPSocket*>::iterator find(string destIpAndPort);

	//list all connected peers;
	void ListPeers();

	//list users
	void ListUserNames();

	//list pairs
	void ListPairs();

	//list rooms
	void ListRooms();

	void close();

	~MessengerServer();
};

