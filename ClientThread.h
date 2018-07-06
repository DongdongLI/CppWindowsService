#include "ROTException.h"
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
using namespace std;

const int TEMP_BUFFER_SIZE = 128;

class thread_obj {
public:

	static string getHostDescription(const sockaddr_in &sockAddr) {
		ostringstream stream;
		stream << inet_ntoa(sockAddr.sin_addr) << ":" << ntohs(sockAddr.sin_port);
		return stream.str();
	}

	static void browse(string url){
		string command = "rundll32 url.dll,FileProtocolHandler " + url;
		system(command.c_str());
		// todo. call the command more directly
	}

	int operator()(SOCKET clientSocket, const sockaddr_in &sockAddr){

		cout << "Connected with" << getHostDescription(sockAddr) << ".\n";
		char tempBuffer[TEMP_BUFFER_SIZE];
		string url;

		while (true){
			int res;
			res = recv(clientSocket, tempBuffer, sizeof(tempBuffer), 0);
			if (res == 0){
				break;
			}
			else if (res == SOCKET_ERROR){
				throw ROTException("socket error while receiving. ");
			}
			else{
				if (tempBuffer[0] == '\r'){
					cout << "found a enter" << endl;
					cout << "url: " << url << endl;
					browse(url);
					url = "";
				}

				url += tempBuffer[0];
				if (send(clientSocket, tempBuffer, res, 0) == SOCKET_ERROR)
					throw ROTException("socket error while sending");
			}
		}

		cout << "Connection closed.\n";
	}
};