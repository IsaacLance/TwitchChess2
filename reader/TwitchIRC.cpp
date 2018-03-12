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
//#define URL "irc.us.ircnet.net"
//FIXME: Edit max size? It could actually be way smaller but its risky?
//Fixme: Change error handling
TwitchIRC::TwitchIRC(std::string username,std::string oauth){//,char* outbuff){char* name, 
	//nick = name;
	user = username;
	pass = oauth;
	//buff = outbuff;
}
TwitchIRC::~TwitchIRC(){//FIXME: This might need to be virtual? What happens if we close no s?
	close(my_socket);
}
//Public Methods
void TwitchIRC::start(){
	cout << "BOT START\n" << flush;
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
	//hints.ai_flags = AI_PASSIVE; //I'm 90% sure this is unneeded
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
	//so basically free(servinfo) but, like, the way that works
	int bytes_recieved;
	char buff[MAXDATASIZE];
	
	int count = 0; //fixme: why is this here
	//TEMP: connect to a channel and print its chat in console
	
	//login
	cout << "send data:\n" << flush;
	sendData(pass);
	sendData(user);
	cout << "begin loop\n" << flush;
	while (true){//fixme: main loop should be in a different method
		count++;
		bytes_recieved = recv(my_socket, buff, MAXDATASIZE -1, 0);
		mutex.lock(buffer);
		buff[bytes_recieved] = '\0';
		mutex.unlock(buffer);
		cout <<buff<< "\n" << flush;
		this_thread::sleep_for(1s);
		//this_thread::sleep_for(chrono::milliseconds(300));
		//cout << buff;
		
	}	
}
	
	bool TwitchIRC::charPresent( string block, string target){
		bool found = false; //Fixing the absolutely STELLAR "example" code, now this function returns true or false instead of true or 0, yeah I know.
		//Loop through block,
		for (int i = 0; i < block.length(); i++){
			if (block[i] == target[0]){ //if we match the first letter,
				found = true;
				//keep trying to match until we get a full match.
				for (int n = 1; n < target.length(); n++){
					if (block[i+n] =! target[n]){ // Starting check at the 1 index character, we checked 0
						found = false;
					}
				}
			}
		}
		return found;
	}

	
//Private Methods
	bool TwitchIRC::isConnected(char* buf){
		//returns true if '/MOTD' is found in the input string. This is a very trivial function but I'm leaving it in for now.
		//If we find /MOTD then its ok to join a channel #FIXME: LEARN WHY THIS IS
		return (charPresent(buf, "/MOTD"));
	}
	
	bool TwitchIRC::sendData(std::string msg){ //the bool return makes this method a sort of "try" method.
		int bytes_sent = send(my_socket, msg.c_str(), msg.length(),0);
		cout << "bytes sent:" <<bytes_sent << "\n msg: " << msg << "\n";
		if (bytes_sent == 0){return false;}
		
		return true;
	}

	void TwitchIRC::sleep(double seconds){
		clock_t start;
		double duration = 0;
		start = clock();
		while (duration < 3){
			duration = (clock() - start) / (double) CLOCKS_PER_SEC;
		}
		cout << "Awake" << flush;
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
	TwitchIRC bot = TwitchIRC("NICK crowdsourcechess\r\n", "PASS oauth:knx6ly88bw0bqwmvckrbp2r288ffc6\r\n");
	bot.start();
	
	return 0;
}








