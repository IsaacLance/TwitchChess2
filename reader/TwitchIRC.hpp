/*Header for irc bot without any special Twitch^(tm) functionality.
Isaac Lance
*/

#ifndef TwitchIRC_HPP_
#define TwitchIRC_HPP_

#include <string>

class TwitchIRC{
	public:
		//*structors
		TwitchIRC(std::string username,std::string password);//char* outbuffchar* name, 
		~TwitchIRC();
		//Public Methods
		void start();
		bool charPresent(std::string block,std::string target);
		//Public vars
		bool setup;

	private:
		//Private vars
		std::string port;
		std::string url;
		int my_socket; //socket descriptor
		std::string nick;
		std::string user;
		std::string pass;
		char* buff[100];
		//Private methods
		bool isConnected(char* buf);
		//char* timeNow();
		void sleep(double seconds);
		bool sendData(std::string msg);
		//void sendPong(char* buf);
		//void msgHandle(char* buf);
};
#endif