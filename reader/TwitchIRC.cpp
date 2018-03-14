/*main file for irc bot without any special Twitch^(tm) functionality.
Isaac Lance
SOURCES:
https://www.freebsd.org/cgi/man.cgi?query=getaddrinfo&apropos=0&sektion=0&manpath=FreeBSD+7.1-RELEASE&format=html
http://www.the-tech-tutorial.com/simple-c-irc-bot-template/
http://kiki.ee.ntu.edu.tw/~course/Intro_NetS10/classnotes/Socket%20programming-com.pdf
http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
*/

#include "TwitchIRC.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <csignal>
#include <ctime>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std; //FIXME: using std:"function" may be better
#define PORT "6667" //Required by TWITCH.TV
#define MAXDATASIZE 100
#define URL "irc.chat.twitch.tv"
#define DEFAULT_CHANNEL "#doyouphilsme"
//#define URL "irc.us.ircnet.net"
//FIXME: Edit max size? It could actually be way smaller but its risky?
//Fixme: Change error handling
TwitchIRC::TwitchIRC(string username,string oauth, queue<string> output){
	user = username;
	pass = oauth;
	output = output;
}

TwitchIRC::~TwitchIRC(){//FIXME: This might need to be virtual? What happens if we close no s?
	close(my_socket);
}
//Public Methods
void TwitchIRC::start(){
	cout << "BOT START\n" << flush;
	Setup();
	Login();
	Join(DEFAULT_CHANNEL);
	
	//RUN main loop
	int bytes_recieved;
	char buff[MAXDATASIZE];
	while (true){//fixme: main loop should be in a different method
		bytes_recieved = recv(my_socket, buff, MAXDATASIZE -1, 0);
		buff[bytes_recieved] = '\0';//Null terminate so we can parse
		//cout <<buff;//<< flush;
		Parse(buff);
		this_thread::sleep_for(10ms);
		//this_thread::sleep_for(chrono::milliseconds(300));
		//cout << buff;
		
	}	
}
	
	size_t TwitchIRC::substringPresent(string block, string target){
		size_t found = block.find(target);
		if (found!=string::npos){
			return true;
		}
		return false;
	}

	
//Private Methods
	/*bool TwitchIRC::isConnected(char* buf){ //DELETEME: This is useless now?
		//returns true if '/MOTD' is found in the input string. This is a very trivial function but I'm leaving it in for now.
		//If we find /MOTD then its ok to join a channel #FIXME: LEARN WHY THIS IS
		return (charPresent(buf, "/MOTD"));
	}*/
	bool TwitchIRC::Setup(){
		struct addrinfo hints;// "hints" is an optional pointer to the struct addrinfo-
						  //-this confused me for a long time! haha
		struct addrinfo *servinfo;// points to the results
	
		//set some values
		setup = true;
		port = PORT;
		url = URL;
		int status;
		//setup hints
		memset(&hints, 0 ,sizeof(hints)); //set the struct to being empty
		hints.ai_family = AF_UNSPEC; //unspecified (IPv4 or v6 accepted)
		hints.ai_socktype = SOCK_STREAM; //other option: sock_dgram, not for us though
		//Setup structs/sockets/ then connect, all with error checking
		if ((status = getaddrinfo(url.c_str(), port.c_str(), &hints, &servinfo)) !=0){
			setup = false;
			printf("ERROR 1");
			fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status)); //gai_strerror: return text string describing error value from getaddrinfo()
		}

		if ((my_socket = socket(servinfo->ai_family, servinfo->ai_socktype,servinfo->ai_protocol)) == -1){
			printf("ERROR 2");
			perror("client: socket");
		}

		if (connect(my_socket, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
			printf("ERROR3");
			perror("Client connect");
			close(my_socket);
		}

		//freeaddrinfo is a function that takes a pointer to an addrinfo instance and frees associated memory
		freeaddrinfo(servinfo);
	}
	
	bool TwitchIRC::sendData(string msg){ //the bool return makes this method a sort of "try" method.
		int bytes_sent = send(my_socket, msg.c_str(), msg.length(),0);
		cout << "bytes sent:" <<bytes_sent << "\n msg: " << msg << "\n";
		if (bytes_sent == 0){
			return false;}
		return true;
	}

	bool TwitchIRC::Join(string channel){
		string msg = "JOIN " + channel + "\r\n";
		return sendData(msg);
	}
	
	bool TwitchIRC::Login(){
		string P = "PASS oauth:" + pass + "\r\n";
		string N = "NICK " + user + "\r\n";
		return(sendData(P) && sendData(N));
	}
	
	bool TwitchIRC::Parse(char* line){
		string msg(line);
		cout <<"Parsed:"<< msg;//<< flush;
		/*
		if (msg.find("PING") == 0){
			sendData("PONG :tmi.twitch.tv");
			return false; //Don't do anything more
		}
		if (msg[0] != ':'){
			cout << "invalid msg recieved: didn't start with ':'\n" << flush;
			return false;
		}
		size_t name_x = 1;
		size_t name_y = msg.find("!");
		string name = msg.substr(name_x, name_y - name_x);
		cout<< "recieved message from:  " << name << "\n" << flush;
		return true;*/
	}
	/*void TwitchIRC::sendPong(char* buf){
		
	}*/
	
	
	//void TwitchIRC::msgHandle(char* buf){}





/*These are here so I remember without having to tab	//Public vars
	bool setup;
/
	private:
		//Private vars
		char* port;
		int s; //socket descriptor
		char* nick;
		char* user;
		//Private methods
		bool isConnected(char* buf);
		char* timeNow();
		bool sendData(char* msg);
		void sendPong(char* buf);
		void msgHandle(char* buf);
		
};
*/
int main(){
	cout << "PROGRAM START\n"<< flush;
	queue<string> test;
	TwitchIRC bot = TwitchIRC("crowdsourcechess", "knx6ly88bw0bqwmvckrbp2r288ffc6", test);
	bot.start();
	
	return 0;
}








