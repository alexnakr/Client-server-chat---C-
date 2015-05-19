#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define TCP_PORT 3346
#define UDP_PORT 3347

#define CLOSE_SESSION_WITH_PEER 	1
#define OPEN_SESSION_WITH_PEER 		2
#define EXIT						3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6
#define AUTHENTICATION  			7
#define SIGNUP						8
#define USER_LIST					9
#define CREATE_CHAT					10
#define CLOSE_CHAT					11
#define NEW_CHAT_USER				12
#define LOGGED_NEW_USER				13
#define ROOM_LIST					14
#define MEMBERS_LIST				15

#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"



#endif /* TCPMESSENGERPROTOCOL_H_ */
