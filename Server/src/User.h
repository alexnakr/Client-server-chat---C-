#ifndef USER_H_
#define USER_H_

#include <string.h>
#include "UDPSocket.h"
#include "MSProtocol.h"
#include "MultipleTCPSocketsListener.h"

enum eUser
{
	CHAT_OWNER=1,
	CHAT_MEMBER=2,
	PAIR=3,
	SINGLE=4,
};


class User {
public:
	User(const string &user,const string &pass) : username(user), password(pass)
	{status=SINGLE;}
	string username,password;
    eUser status;

    vector<User*> chatMembers;  //chat members / pair member

    //insert user pointer to chat members
    void addUserToChat(User* user);

    //retrieve list of chat members
    string retrieveMembers(); //
	virtual ~User();
};

#endif /* USER_H_ */
