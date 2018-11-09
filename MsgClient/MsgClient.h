#pragma once
///////////////////////////////////////////////////////////////////
// MsgClient.h - Create Receiver and Lisener for connection      //
//                                                               //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to Create Receiver and Lisener
* and define function to connect to channel and server
* It uses C++11 constructs
*
* The elements defined in the companion package They are:
*
*   makeMessage   -make message to server
*   sendMessage   -send message to serever
*   sendFile      -upload files to server
*   ClientListen  -start listen from server
*   Connection   -start connection
*   readMessage  -read message from client 
*   acceptFile   -download file
*   AnalQuery    -request to do type anylsisr and create htm
*   Display()    -return pointer )_Display
*   operator()   -overide the () operator
*   DemoClient   -Demo the requriement
*
* Dependency :
*--------------
* MsgClient.h
* HttpMessage.h
* FilySystem.h
* Logger.h
* Utilities.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 1 MAy 2017
*/

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include <thread>

using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;
using namespace Utilities;

class MsgClient {
public:
	using EndPoint = std::string;
	MsgClient() { //constructor
		si = new SocketConnecter();
		ss = new SocketSystem();
		Connection();
		std::thread t1([&]() {
			ClientListen();
		});
		t1.detach();
	}
	HttpMessage makeMessage(std::string n, const std::string& msgBody, const EndPoint& ep);// create message
	void sendMessage(HttpMessage& msg); //send message to server
	bool sendFile(const std::string& fqname, std::string category); //send files to server
	void ClientListen(); //start listen message from server
	void Connection(); //start connection
	HttpMessage readMessage(Socket& socket); //read server message 
	void acceptFile(const std::string& fqname, size_t fileSize, Socket& socket); //accept returned files
	void AnalQuery(); //do analysisr 
	BlockingQueue<HttpMessage>* Display() { return _Display; } //call _Display
	void operator()(Socket socket); //overide() operator
	void DemoClient(); //do requriement demonstration

private:
	void split(const std::string &s, char delim, std::vector<std::string> &elems);
	void DemoRequirment58();
	void DemoRequirment346();
	void DemoRequirment7();
	void DemoRequirment9();



	bool connectionclose;
	std::string localhost;
	std::string clientRepository;
	SocketSystem *ss;
	SocketConnecter *si;
	SocketListener *sl;
	BlockingQueue<HttpMessage>* msgQ;
	BlockingQueue<HttpMessage>* _Display = new BlockingQueue<HttpMessage>();

};




