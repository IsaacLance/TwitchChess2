/*main file for driver.
Isaac Lance
SOURCES: (some are now almost unused as it gets streamlined)
https://www.freebsd.org/cgi/man.cgi?query=getaddrinfo&apropos=0&sektion=0&manpath=FreeBSD+7.1-RELEASE&format=html
http://www.the-tech-tutorial.com/simple-c-irc-bot-template/
http://kiki.ee.ntu.edu.tw/~course/Intro_NetS10/classnotes/Socket%20programming-com.pdf
http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
*/

#include "chessdriver.hpp"
#include "consumer.hpp"
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
#include <queue>
#include <vector>
#include <Python/Python.h>
#include <mutex>
#include <map>

using namespace std; //FIXME?: using std:"function" may be better
using namespace std::chrono;
#define PORT "6667" //Required by TWITCH.TV
#define MAXDATASIZE 100 //Max we can recieve
#define URL "irc.chat.twitch.tv" //Connect to this server
#define DEFAULT_CHANNEL "#yung_jrobes" //Connect to this channel
//Public Methods
ChessDriver::ChessDriver(string username,string oauth, queue<string> output){
	user = username;
	pass = oauth;
	shared_queue = output;
}

/*ChessDriver::~ChessDriver(){//FIXME: Add destructor of consumer side	
	close(my_socket);
}*/

void ChessDriver::run(){
	SocketSetup();
	Login();
	this_thread::sleep_for(10ms);
	Join(DEFAULT_CHANNEL);
	this_thread::sleep_for(10ms);
	Join(DEFAULT_CHANNEL);
	
	threads.push_back(std::thread(&ChessDriver::producer, this));
	threads.push_back(std::thread(&ChessDriver::consumer, this));
	//FIXME: Might have to join these two.
	for (auto& t: threads) t.join();
}

//
//Private Methods
///	
void ChessDriver::producer(){
	cout << "Producer thread running\n" << flush;
	//Setup();
	//Login();
	this_thread::sleep_for(10ms);
	//Join(DEFAULT_CHANNEL);
	
	//RUN main loop
	int bytes_recieved;
	//queue <char*> msgs;
	string middleman;
	//OLD WAY: char buff[MAXDATASIZE];
	while (true){
		char buff[MAXDATASIZE];
		//msgs.push( buff );
		bytes_recieved = recv(my_socket, buff, MAXDATASIZE -1, 0);
		//Begin parsing in this scope or we get problems.
		buff[bytes_recieved] = '\0';//Null terminate so we can parse
		middleman.append(buff); //Strings can append char*'s which is nice
		
		size_t end_index = middleman.find("\n"); //Need middleman to pre-process the data
		if (end_index !=string::npos){
			string msg = middleman.substr(0, end_index+1);//get one message
			middleman = middleman.substr(end_index+1, string::npos);//Chop middleman
			string parsed = Parse(msg); //Parse the message. . .
			if (parsed[0] != '^'){//If it's a valid command . . .
				//cout << "Valid input--" <<parsed <<"\n"<< flush;
				cout << "Middleman: " << middleman << flush;
				queue_mutex.lock();
				shared_queue.push(parsed);//Push it to the shared queue
				queue_mutex.unlock();
			}
		}
		
		//msgs.pop();
		this_thread::sleep_for(10ms);
		//this_thread::sleep_for(chrono::milliseconds(300));
		//cout << buff;
		//cout << msgs.front() << flush;
		//cout <<buff;//<< flush;
	}	
}

void ChessDriver::consumer(){
	cout << "Consumer thread running\n" << flush;

	start();
	PyObject* module = PyImport_ImportModule("moveMaker");
	PyObject* klass = PyObject_GetAttrString(module, "MoveMaker");
	PyObject* instance = PyInstance_New(klass, NULL, NULL);
	PyObject* result = PyObject_CallMethod(instance, "signin", NULL);
	startGame(instance);
	string board[8][8];
	bool democracy = true;
	steady_clock::time_point time1;
	steady_clock::time_point time2;
	while(true){
		if (democracy){
			//Set times
			time1 = steady_clock::now();
			time2 = steady_clock::now();
			duration<double> time_span;
			//Set up maps, user -> command, and command -> count
			map<string, string> votes;
			map<string, int> tally;
			//Set up vars
			int winner_count = 0; //Update this if we make a count greater then it
			string winner = "NoInput";
			//Get a move democratically
			while((time_span.count()) < 20.0){
				this_thread::sleep_for(10ms); 
				//Lock
				queue_mutex.lock();
				if(shared_queue.empty()){
					//Unlock possible
					queue_mutex.unlock();
					continue;
				}
				string command = shared_queue.front();
				shared_queue.pop();
				queue_mutex.unlock();
				//Unlocked
				cout << "Consumed: "<< command<<"\n"<<flush;
				//Get coords and check
				string coords = command.substr(0,2) + command.substr(3,2);
				if (isValid(board, coords)){
					string name = command.substr(6, string::npos);
					//Set the two map values since it was valid
					votes[name] = coords;
					tally[coords]++;     //If no entry exists, it is initialized to 0! Awesome
					if (tally[coords] > winner_count){
						//Set new winner
						winner_count = tally[coords];
						winner = coords;
					}
				}
			time_span = duration_cast<duration<double>>(time2 - time1)
			time2 = steady_clock::now();
			}
			if (winner_count > 0){
				string chosen = getPiece(board, winner.substr(0,2));
				move(instance, winner);
				this_thread::sleep_for(2s)
				updateBoard(instance, board);
				if (getpiece(board, winner.substr(2,2)) != chosen){
					democracy = false;
				}
			}
		}
		else{ //ANARCHY MODE!!!
			while(true){
				this_thread::sleep_for(10ms); 
				//Lock
				queue_mutex.lock();
				if(shared_queue.empty()){
					//Unlock possible
					queue_mutex.unlock();
					continue;
				}
				string winner = shared_queue.front();
				shared_queue.pop();
				queue_mutex.unlock();
				//Unlocked
				cout << "(Anarchy) Consumed: "<< winner <<"\n"<<flush;
				//Get coords and check
				string coords = winner.substr(0,2) + winner.substr(3,2);
				if (isValid(board, coords)){
					string name = winner.substr(6, string::npos);
					string chosen = getPiece(board, coords.substr(0,2));
					move(instance, coords);
					this_thread::sleep_for(2s);
					updateBoard(instance, board);
					if (getpiece(board, coords.substr(2,2)) == chosen){
						democracy = true;
						break;
					}
				}
			}
		}
	}
}
//
//Private Methods
//
bool ChessDriver::SocketSetup(){
	struct addrinfo hints;// "hints" is an optional pointer to the struct addrinfo-
					  //-this confused me for a long time! haha
	struct addrinfo *servinfo;// points to the results

	//set some values
	setup_bool = true;
	port = PORT;
	url = URL;
	int status;
	//setup hints
	memset(&hints, 0 ,sizeof(hints)); //set the struct to being empty
	hints.ai_family = AF_UNSPEC; //unspecified (IPv4 or v6 accepted)
	hints.ai_socktype = SOCK_STREAM; //other option: sock_dgram, not for us though
	//Setup structs/sockets/ then connect, all with error checking
	if ((status = getaddrinfo(url.c_str(), port.c_str(), &hints, &servinfo)) !=0){
		setup_bool = false;
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

bool ChessDriver::sendData(string msg){ //the bool return makes this method a sort of "try" method.
	int bytes_sent = send(my_socket, msg.c_str(), msg.length(),0);
	cout << "bytes sent:" <<bytes_sent << "\n msg: " << msg << "\n" <<flush;
	if (bytes_sent == 0){
		return false;}
	return true;
}

bool ChessDriver::Join(string channel){
	string msg = "JOIN " + channel + "\r\n";
	return sendData(msg);
}

bool ChessDriver::Login(){
	string P = "PASS oauth:" + pass + "\r\n";
	string N = "NICK " + user + "\r\n";
	return(sendData(P) && sendData(N));
}

string ChessDriver::Parse(string msg){
	//cout <<"Parsed>"<< msg << flush;
	//Non-shared results
	if (msg.find("PING") == 0){
		cout<< "Recieved PING from server\n" << flush;
		sendData("PONG :tmi.twitch.tv");
		return "^PING"; //Don't do anything more
	}
	if (msg[0] != ':'){
		cout << "invalid msg recieved: didn't start with ':'\n" << flush;
		return "^FAIL";
	}
	if(msg.substr(1, 13) == "tmi.twitch.tv"){
		cout << "message from server: "<< msg  <<"\n" << flush;
		return "^SERV";
	}
	if(msg.substr(1, 30) == "crowdsourcechess.tmi.twitch.tv"){
		cout << "message from channel: " <<msg <<"\n" << flush;
		return "^CHAN";
	}
	//Shared results are possible
		//Get name of sender
	size_t name_x = 1;
	size_t name_y = msg.find("!");
	if (name_y != string::npos){
		string channel = DEFAULT_CHANNEL;
		string name = msg.substr(name_x, name_y - name_x);
		//cout<< "recieved message from-- " << name << ":" << flush;
		//Get the actual message, parse_a
		size_t channel_x = msg.find(channel);
		if (channel_x == string::npos){
			cout<<"ERROR no channel name in message"<< flush;
			return "^NOCH";
		}
		string parse_a = msg.substr(channel_x + channel.length()+2, string::npos);
		cout<< parse_a << flush;
		//Get the candidate indexes, the first 5 characters, parse_b
		string astr = "ABCDEFGHabcdefgh";
		string nstr = "12345678";
		string parse_b = parse_a.substr(0, 5);
		//cout << "\n parse_b:" << parse_b <<"\n"<< flush; 
		
		//This next part looks bad but I like it this way, so it's easy to change just-
		//- one index of the input specification. (ex. We want space to be an x now! or whatever) I had it in one single line before.
		if (astr.find(parse_b[0]) == string::npos){ return "^XERR";} //Char
		if (nstr.find(parse_b[1]) == string::npos){ return "^XERR";} //Num
		if           (parse_b[2]!= ' ')           { return "^XERR";} //Space
		if (astr.find(parse_b[3]) == string::npos){ return "^XERR";} //Char
		if (nstr.find(parse_b[4]) == string::npos){ return "^XERR";} //Num
		cout << "passed final check--" << flush;
		//If the format is CharNumSpaceCharNumSpace send this and name to the queue
		string parse_c = parse_b + ":" + name;
		cout << parse_c << "\n" << flush;
		return parse_c;

	}
	return "^XERR";
}

int main(){
	//mutex m;
	queue<string> shared_queue;
	ChessDriver driver = ChessDriver("crowdsourcechess", "knx6ly88bw0bqwmvckrbp2r288ffc6", shared_queue);
	driver.run();
}
