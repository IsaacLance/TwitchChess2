/*Header for irc bot without any special Twitch^(tm) functionality.
Isaac Lance
*/

#ifndef TwitchIRC_HPP_
#define TwitchIRC_HPP_
#include <queue>
#include <string>

class TwitchIRC{
	public:
		//*structors
		TwitchIRC(std::string username,std::string password, std::queue<std::string> output);
		~TwitchIRC();
		//Public Methods
		void start();
		size_t substringPresent(std::string block,std::string target);
		//Public vars
		bool setup;

	private:
		//Private vars
		std::string port;
		std::string url;
		
		std::string user;
		std::string pass;
		
		int my_socket; //socket descriptor
		std::queue<std::string> shared_queue;
		//FIXME: DO I NEED THIS? char* buff[100];
		//Private methods
		//bool isConnected(char* buf);
		bool Setup();
		bool sendData(std::string msg);
		bool Join(std::string channel);
		bool Login();
		bool Parse(char* msg);
		
		//void sendPong(char* buf);
		//void msgHandle(char* buf);
};
#endif