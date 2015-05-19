#include <iostream>
#include "MessengerServer.h"
using namespace std;

void printInstructions()
{
	cout<<"lu - list all users"<<endl;
	cout<<"lcu - list all connected users"<<endl;
	cout<<"ls - list two clients communicating"<<endl;
	cout<<"lr - list all rooms"<<endl;
	cout<<"x - exit"<<endl;
}

int main()
{
	cout<<"Welcome to messenger Server"<<endl;
	printInstructions();
	MessengerServer msngrServer;
	bool loop = true;
	while(loop){
		string msg;
		string command;
		cin >> command;
		if(command == "lcu")
		{
			msngrServer.ListPeers();
		}
		else if(command == "lu")
		{
			msngrServer.ListUserNames();
		}
		else if(command == "ls")
		{
			msngrServer.ListPairs();
		}
		else if(command == "lr")
		{
			msngrServer.ListRooms();
		}

	    else if(command == "x"){
			loop = false;
		}else{
			cout<<"wrong input"<<endl;
			printInstructions();
		}
	}
	msngrServer.close();
	cout<<"messenger was closed"<<endl;
	return 0;
}
