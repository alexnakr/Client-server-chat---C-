#ifndef MAIN_CPP_
#define MAIN_CPP_

#include <iostream>
#include <string.h>
#include "MessengerClient.h"

using namespace std;

void printInstructions(){
	cout<<"c <server ip> - connect to <server> "<<endl;
	cout<<"lu - print user list from server"<<endl;
	cout<<"lcu -print the connected user list"<<endl;
	cout<<"lr - print all rooms"<<endl;
	cout<<"cr - create chat room"<<endl;
	cout<<"cc - close chat room"<<endl;
	cout<<"o <peername>/<room> - open session with peer"<<endl;
	cout<<"s <message> - send a message"<<endl;
	cout<<"cs - close opened session"<<endl;
	cout<<"d - disconnect from server"<<endl;
	cout<<"x - close the app"<<endl;
}

int main(){
	cout<<"Welcome to TCP messenger"<<endl;
	printInstructions();
	cout<<"\nPlease select:\n1.Login\n2.Signup\n";
	string userSelect;
	getline(std::cin,userSelect);
	MessengerClient* messenger = new MessengerClient();
	string msg,pass,user;
	if(userSelect=="1")
    {
	  cout<<"Authentication required\nusername:";
	  getline(std::cin,user);
	  cout<<"password:";
	  getline(std::cin,pass);
	  messenger->MessengerSetCred(user,pass);}
	else if(userSelect=="2")
	{
		cout<<"Please choose new username and password\nusername:";
		getline(std::cin,user);
		cout<<"password:";
		getline(std::cin,pass);
		messenger->MessengerSetCred(user,pass);
		messenger->SetSignup();
	}
	string command;
	while(true){
		cin >> command;
		if(command == "c"){
			string ip;
			cin >> ip;
			messenger->connect(ip);
		}
		else if(command == "o")
		{
			string dest;
			cin >> dest;
			messenger->open(dest);
		}
		else if(command == "cr")
		{
			messenger->OpenChatRoom();
		}
		else if(command == "s")
		{
		getline(std::cin,msg);
		if(msg.size()>0 && msg[0] == ' ') msg.erase(0,1);
		    if (!messenger->isActiveClientSession())
		    {
		    cout<<"Fail: No client session is active"<<endl;
		    printInstructions();
		    }
		    else
		    {
			messenger->Send(msg);
		    }
		}
		else if(command == "lr")
		{
		    messenger->DisplayRooms();
		}
		else if(command == "lru")
		{
			getline(std::cin,msg);
			if(msg.size()>0 && msg[0] == ' ') msg.erase(0,1);
			messenger->GetMembersLIst(msg);
		}
		else if(command == "lu")
		{
			messenger->GetUserLIst();
		}
		else if(command == "lcu")
		{
			messenger->DisplayConnectedList();
		}
		else if(command == "cc")
		{
			messenger->CloseChatRoom();
		}
		else if(command == "cs")
		{
		   messenger->closeActiveSession();
		}
		else if(command == "d")
		{
			messenger->disconnect();
		}
		else if(command == "x")
		{
			break;
		}
	}
	messenger->disconnect();
	delete messenger;
	cout<<"messenger was closed"<<endl;
	return 0;
}


#endif /* MAIN_CPP_ */
