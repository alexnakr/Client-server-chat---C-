#include "User.h"


void User::addUserToChat(User* user)
{
    if(status==CHAT_OWNER || status==SINGLE)
        {
    	 this->chatMembers.push_back(user);
         if(status!=CHAT_OWNER) status=PAIR;
        }
}
string User::retrieveMembers()
{
    string lst;
    //mutex
    for(vector<User*>::iterator it=chatMembers.begin();it!=chatMembers.end();it++)
   	 lst = lst + (*it)->username+'\n';
    //mutex
    return lst;
}

User::~User() {}

