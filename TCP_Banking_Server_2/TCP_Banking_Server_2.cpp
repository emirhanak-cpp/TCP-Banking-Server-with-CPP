#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <winsock2.h> 
#include <Ws2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

//#pragma warning(disable : 4996)

using namespace std;

int main()
{
	// Initialize winsock
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0)
	{
		cout << "winsock dll NOT found!" << endl;
		return 0;
	}
	else
	{
		cout << "winsock dll found!" << endl;
		cout << "STATUS: " << wsaData.szSystemStatus << endl;
	}

	// Create a socket
	SOCKET serverSocket;
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (serverSocket == INVALID_SOCKET)
	{
		cout << "error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "socket is OK." << endl;
	}

	// Bind the socket
	sockaddr_in service;
	service.sin_family = AF_INET;
	//service.sin_addr.S_un.S_addr = INADDR_ANY;
	service.sin_addr.s_addr = INADDR_ANY;
	//InetPton(AF_INET, __TEXT("127.0.0.1"), &service.sin_addr.s_addr);
	//service.sin_addr.S_un.S_addr = inet_addr("10.50.12.133");
	service.sin_port = htons(54000);

	if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR)
	{
		cout << "bind() failed: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "bind() is OK." << endl;
	}

	// Listen for connections
	if (listen(serverSocket, 1) == SOCKET_ERROR)
	{
		cout << "listen(): Error listening socket: " << WSAGetLastError() << endl;
	}
	else
	{
		cout << "listen() is OK. Waiting for new connections..." << endl;
	}

	// Accept connections
	SOCKET acceptSocket;
	acceptSocket = accept(serverSocket, nullptr, nullptr);

	if (acceptSocket == INVALID_SOCKET)
	{
		cout << "accept() failed: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "accept() is OK." << endl;
	}

	int i = 0;
	char bankInfoArray[3][4096];

	ifstream infile("/Users/Administrator/Desktop/PITON Project/server/data/bankInformation.txt"); // Change the bankInformation.txt path to your case.
	while (!infile.eof())
	{
		infile >> bankInfoArray[i];
		i++;
	}
	infile.close();

	/*for (int i = 0; i < 3; i++)
	{
		cout << bankInfoArray[i] << endl;
	}*/

	string splitBankInfoArray[3][2];

	// Loop through each entry
	for (int i = 0; i < 3; ++i) {
		istringstream ss(bankInfoArray[i]);
		string token;
		int j = 0;

		// Split by delimiter '/' and store in the array
		while (getline(ss, token, '/') && j < 5) {
			splitBankInfoArray[i][j] = token;
			j++;
		}
	}

	// Display the separated entries
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 2; ++j) {
			std::cout << splitBankInfoArray[i][j] << " ";
		}
		std::cout << std::endl;
	}

	// Receive and send data
	/*char receiveBuffer[4096];
	while (receiveBuffer != "terminate")
	{
		int rbyteCount = recv(acceptSocket, receiveBuffer, 4096, 0);
		if (rbyteCount < 0)
		{
			cout << "server recv() error: " << WSAGetLastError() << endl;
			return 0;
		}
		else
		{
			cout << "received data: " << receiveBuffer << endl;
		}
	}*/

	const int receivedDataSize = 1024;
	char receivedData[receivedDataSize] = {};
	int j = 0;
	int jCopy = 0;
	for (int i = 0; i < 2; i++)
	{
		char receiveBuffer[receivedDataSize] = {};
		int rbyteCount = recv(acceptSocket, receiveBuffer, receivedDataSize - 1, 0);
		if (rbyteCount < 0)
		{
			cout << "server recv() error: " << WSAGetLastError() << endl;
			return 0;
		}
		else
		{
			cout << "received data: " << receiveBuffer << endl;
		}
		
		while(receiveBuffer[j] != '\0')
		{
			receivedData[j + jCopy] = receiveBuffer[j];
			j++;
		}
		receivedData[j + 1] = ' ';
		j++;
		jCopy = j;
		j = 0;

	}
	receivedData[jCopy] = '\0';
	/*for (int i = 0; i < receivedDataSize; i++)
	{
		cout << receivedData[i];
	}*/

	string receivedDataStr = receivedData;
	//cout << receivedDataStr;

	bool flag = false;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (strcmp(splitBankInfoArray[i][j].c_str(), receivedDataStr.c_str()) == 0)
			{
				flag = true;
			}
			//cout << flag;
		}
	}

	//cout << if(splitBankInfoArray[0][0] == receivedDataStr);

	/*receiveBuffer[rbyteCount] = '\0';
	memcpy(receivedData, receiveBuffer, receivedDataSize);
	receivedData[receivedDataSize] = '\0';*/
	
	if (flag == true)
	{
		string tempMessage = "true";
		char* sendBuffer = tempMessage.data();
		const int dataSize = tempMessage.size();
		int sbyteCount = send(acceptSocket, sendBuffer, dataSize, 0);
		if (sbyteCount == SOCKET_ERROR)
		{
			cout << "server send() error: " << WSAGetLastError() << endl;
			return -1;
		}
		else
		{
			cout << "server: sent " << sbyteCount << " bytes" << endl;
		}
		cout << sendBuffer;
	}
	else
	{
		string tempMessage = "false";
		char* sendBuffer = tempMessage.data();
		const int dataSize = tempMessage.size();
		int sbyteCount = send(acceptSocket, sendBuffer, dataSize, 0);
		if (sbyteCount == SOCKET_ERROR)
		{
			cout << "server send() error: " << WSAGetLastError() << endl;
			return -1;
		}
		else
		{
			cout << "server: sent " << sbyteCount << " bytes" << endl;
		}
		//cout << sendBuffer;
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));


	return 0;
}