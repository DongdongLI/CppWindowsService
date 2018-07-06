#include "ClientThread.h"
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <thread>
using namespace std;

class ListeningThread {
public:

	static void setServerSockAddr(sockaddr_in *sockAddr, int portNumber){
		sockAddr->sin_family = AF_INET;
		sockAddr->sin_port = htons(portNumber);
		sockAddr->sin_addr.S_un.S_addr = INADDR_ANY;
	}

	void operator()(int portNumber){

		while (true){

			SOCKET hSocket = INVALID_SOCKET;
			//SOCKET hClientSocket = INVALID_SOCKET;

			bool success = true;
			sockaddr_in sockAddr = { 0 };

			try{


				cout << "creating socket... ";
				if ((hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
					throw ROTException("Could not create socket");
				cout << "created.\n";

				cout << "Binding socket... ";
				setServerSockAddr(&sockAddr, portNumber);
				if (::bind(hSocket, reinterpret_cast<sockaddr*>(&sockAddr), sizeof(sockAddr)) != 0)
					throw ROTException("could not bind socket.");
				cout << "bound. \n";

				cout << "Putting socket in listening mode... ";
				if (listen(hSocket, SOMAXCONN) != 0)
					throw ROTException("could not put socket in listening mode.");
				cout << "done.\n";

				cout << "Waiting for incoming connection... ";

				sockaddr_in clientSockAddr;
				int	clientSockSize = sizeof(clientSockAddr);


				SOCKET hClientSocket = accept(hSocket, reinterpret_cast<sockaddr*>(&clientSockAddr), &clientSockSize);

				if (hClientSocket == INVALID_SOCKET)
					throw ROTException("accept function failed.");
				cout << "accepted.\n";

				//handleConnection(hClientSocket, clientSockAddr);
				thread newThread(thread_obj(), hClientSocket, clientSockAddr);
				newThread.detach();

			}
			catch (ROTException e){
				cerr << "\nError: " << e.what() << endl;
				success = false;
			}

			if (hSocket != INVALID_SOCKET)
				closesocket(hSocket);
		}
	}
};