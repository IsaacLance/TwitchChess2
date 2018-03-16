#ifndef chessdriver_HPP_
#define chessdriver_HPP_
#include <queue>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

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
		//Mutex
		std::mutex queue_mutex;
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
		bool Chat(std::string msg, std::string channel);
		std::string Parse(std::string msg);
    
        //Private variables for consumer

    
        //Priavte Methods for consumer

    
};
#endif
