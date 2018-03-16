#ifndef chessdriver_HPP_
#define chessdriver_HPP_
#include <queue>
#include <string>
#include <vector>
#include <thread>

class ChessDriver{
	public:
		//*structors
		ChessDriver(std::string username,std::string password, std::queue<std::string> output);
		//~ChessDriver();
		//Public Methods
		void run();
		//Public vars
		bool setup_bool;

	private:
		//Main two private methods, inf loops
		void producer(/**/);
		void consumer(/**/);
		//Threads vector
		std::vector<std::thread> threads;
		//Private vars for producer
		std::string port;
		std::string url;
		std::string user;
		std::string pass;
		int my_socket; //socket descriptor
		std::queue<std::string> shared_queue;
		
		//Private methods for producer:
		bool SocketSetup();
		bool sendData(std::string msg);
		bool Join(std::string channel);
		bool Login();
		std::string Parse(std::string msg);
		
		//void sendPong(char* buf);
		//void msgHandle(char* buf);
};
#endif