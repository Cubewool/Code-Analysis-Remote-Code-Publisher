#pragma once
///////////////////////////////////////////////////////////////////
// MsgServer.h - Create function to connection with Client and   //  
//                send message                                   // 
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
* and define function to connect to channel and client
* It uses C++11 constructs
*
* The elements defined in the companion package They are:
*
*   readMessage    -make message to server
*   readFile       -receive fiel
*   ServerListen   -server start listen
*   makeMessage    -create message
*   Connection     -start connection
*   sendMessage    -send message to client
*   sendFile       -send file to client
*   DoAnaly        -do analysiser
*   operator       -overide operator
*   returnCategories       -return categories names
*   htmNames               -return htm files names
*   filesincategory        - return file names in category
*   NoParent               -return no parents file name
*   returnFiles            -return files download
*   deletefile             -delete files
*
* Dependency :
*--------------
* MsgServer.h
* HttpMessage.h
* FilySystem.h
* Logger.h
* Utilities.h
* Sockets.j
*
* Maintenance History:
* --------------------
* ver 1.0 : 1 MAy 2017
*/
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <string>

using Show = StaticLogger<1>;
using namespace Utilities;

class ClientHandler
{
public:
	using EndPoint = std::string;
	ClientHandler() {
		path = "../RemoteRepository";
		ss = new SocketSystem();
		msgQ = new BlockingQueue<HttpMessage>();
	}

	HttpMessage readMessage(Socket& socket); //read message from client
	void readFile(const std::string& filename, size_t fileSize, std::string category, Socket& socket); //read files from client
	void ServerListen(); //server start listening
	HttpMessage makeMessage(std::string str, const std::string& body, const EndPoint& ep); //server make message and send to client
	void Connection(std::string port); //start connection channel
	void sendMessage(HttpMessage& msg); //send message to client
	bool sendFile(const std::string& filename); //send files back
	void DoAnaly(std::string port); //call Project3
	void operator()(Socket socket); // overide ()operator

	void returnCategories(std::string port); //return categories names
	void htmNames(std::string port); //return html file names
	void filesincategory(std::string category, std::string port); //return files in category
	void NoParent(std::string category, std::string port); //return no parent files names
	void returnFiles(const std::string& msgBody, std::string port); // do download files and send back .js and .css
	void deletefile(std::string path);//delete file and folder
private:
	unsigned ifempty(std::vector<std::string>v, std::string str);
	bool connectionclose;
	void split2(const std::string &s, char delim, std::vector<std::string> &elems);
	void split(std::string &s, char delim, std::string &elems);
	EndPoint endtarget;
	std::string path;
	SocketSystem *ss;
	SocketConnecter *si;
	SocketListener * sl;
	BlockingQueue<HttpMessage>* msgQ;
};