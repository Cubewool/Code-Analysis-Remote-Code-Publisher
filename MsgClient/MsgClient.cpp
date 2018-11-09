/////////////////////////////////////////////////////////////////////////
// MsgClient.cpp - Demonstrates simple two-way HTTP messaging          //
//                                                                     //
// Jingyi Yang-CSE687 - Object Oriented Design, Spring 2017            //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 13, Windows 10            //
/////////////////////////////////////////////////////////////////////////

/*
* Required Files:
*   MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11-BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp
*/

#include"../MsgClient/MsgClient.h"
#include<iostream>

HttpMessage MsgClient::makeMessage(std::string str, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:"+localhost;//+ localhost;  // ToDo: make this a member of the sender
										   // given to its constructor.
	msg.clear();
	msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
	msg.addBody(body);

	if (str == "sendfile" || "showhtml" || "showcategory" || "typeanalysis"
		|| "showallfiles" || "shownoparents" || "download" ||"delete") {
		msg.addAttribute(HttpMessage::attribute("mode", str));
		if (str != "sendfile") {
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		msg.addBody(body);
	}
	else{
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}	
	return msg;
}
//----< send message using socket >----------------------------------

void MsgClient::sendMessage(HttpMessage& msg)
{
	std::string msgString = msg.toString();
	si->send(msgString.size(), (Socket::byte*)msgString.c_str());
}

bool MsgClient::sendFile(const std::string& filename, std::string category)
{
	std::cout << "\n  Upload File: " << filename;
	std::string fqname = filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage("sendfile", "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", fi.name()));
	msg.addAttribute(HttpMessage::Attribute("category", category));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		si->send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}
void MsgClient::ClientListen() {
	std::cout << "Client start listening\n\n";
	try {
		localhost = Utilities::Converter<std::thread::id>::toString(std::this_thread::get_id());
		localhost = "8081";
		sl = new SocketListener(std::stoi(localhost), Socket::IP6);
		std::cout << "From Client id: " + localhost << "\n";
		msgQ = new BlockingQueue<HttpMessage>();
		sl->start(*this);
		while (true) {
			HttpMessage msg = msgQ->deQ();
			//Show::write(msg.bodyString());
		}
	}
	catch (std::exception& exc) {
		Show::write("\n Caught Exception: ");
		std::string exit = "\n" + std::string(exc.what()) + "\n\n";
		Show::write(exit);
	}
}

void MsgClient::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		//std::cout << "Client Recieved: " << msg.headerString()<<msg.bodyString();
		if (connectionclose || msg.bodyString() == "quit") {
			Show::write("\n\n  Client thread is terminating\n");
			break;
		}
		msgQ->enQ(msg);
	}
}
//----< this defines the behavior of the client >--------------------
void MsgClient::Connection() {
	Show::attach(&std::cout);
	Show::start();
	Show::title(
		"Starting HttpMessage client on thread " + localhost
	);
	try
	{
		//si = new SocketConnecter();
		while (!si->connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
		std::cout << "\n Connection success\n\n" << std::endl;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Caught Exeception : ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

HttpMessage MsgClient::readMessage(Socket &socket) {
	HttpMessage msg;
	connectionclose = false;
	while (true){
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1){
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else
		{
			break;
		}
	}
	if (msg.attributes().size() == 0) {
		connectionclose = true;
		return msg;
	}
	std::string mode = msg.findValue("mode");
	std::string tempcate = msg.findValue("category");
	std::string tempname = msg.findValue("file");
	size_t tempsize = stoi(msg.findValue("content-length"));
	std::string body = msg.bodyString();
	if (mode != "_sendfile")
	{
		size_t bite = 0;
		size_t pos = msg.findAttribute("content-length");
		if (pos < msg.attributes().size())
		{
			bite = Converter<size_t>::toValue(msg.attributes()[pos].second);
			Socket::byte* buffer = new Socket::byte[bite + 1];
			socket.recv(bite, buffer);
			buffer[bite] = '\0';
			std::string msgBody(buffer);
			msg.addBody(msgBody);
			delete[] buffer;
		}
	}

	if (mode == "_sendfile"){
		//std::cout << "Client received file: " << msg.findValue("file") << std::endl;
		acceptFile(tempname, tempsize, socket);
}
	else {
		//std::cout << "\n\nReceived message:" << msg.bodyString() << "from Server" << std::endl;
		_Display->enQ(HttpMessage(msg));
	}
	return msg;
}
void MsgClient::acceptFile(const std::string& filename, size_t fileSize, Socket& socket) {
	std::string fqname = "../Repository/" + filename;
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		Show::write("\n\n  can't open file " + fqname);
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;

		socket.recv(bytesToRead, buffer);

		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
}

void MsgClient::AnalQuery() {
	HttpMessage msg = makeMessage("typeanalysis", "", "localhost:8081");
	sendMessage(msg);
}

void MsgClient::DemoClient() {
	std::cout << "\n\nRequirement 1#\n" << "===============================================\n"<<"  Start WPF\n\n";
	std::cout << "\nRequirement 2#\n" << "===============================================\n" << "  Using C++ \n\n";
	std::cout << "\nRequirement 5# & 8#\n" << "===============================================\n" ;
	DemoRequirment58();
	std::cout << "\nRequirement 3# & 4# & 6#\n" << "===============================================\n" ;
	DemoRequirment346();
	std::cout << "\nRequirement 7#\n" << "===============================================\n" ;
	DemoRequirment7();
	std::cout << "\nRequirement 9#\n" << "===============================================\n" ;
    DemoRequirment9();
}

void MsgClient::DemoRequirment58() {
	std::cout << " Upload Files in Repository\n\n";
	for (auto file :FileSystem::Directory::getFiles("../Repository/", "*.h")) { 
		sendFile("../Repository/" + file, "Demo5");
	}
	for (auto file :FileSystem::Directory::getFiles("../Repository/", "*.cpp")) {
		sendFile("../Repository/" + file, "Demo5");
	}
	std::thread t1([&]() { std::cout << "\n\n  Remote Repository Contents: \n----------------------------\n\n";
	std::cout << "Client received file: " << "Hide.js, style.css" << std::endl;
	   sendMessage(makeMessage("showcategory", "", "8080"));
	while (true) {
		std::vector<std::string>temp;
		split(_Display->deQ().bodyString(), ',', temp);
		std::cout << "\n";
		for (auto category: temp) 
			std::cout << category << " ";
		std::cout << "\n";break;
	}});
	t1.join();
	std::thread t2([&]() {std::cout << "\nCategory Contents: \n";
	HttpMessage msg = makeMessage("showallfiles", "", "8080");
	msg.addAttribute(HttpMessage::attribute("category", "Demo5")); 
	sendMessage(msg);
	while (true) {
		std::vector<std::string>temp;
		split(_Display->deQ().bodyString(), ',', temp);
		std::cout << "\n";
		for (auto file : temp) 
			std::cout << file << " ";
		 std::cout << "\n"; break;
	}});
	t2.join();
	std::thread t3([&]() {
		std::cout << "\nNo Parent Files:" ;
	HttpMessage msg = makeMessage("shownoparents", "", "8080");
	msg.addAttribute(HttpMessage::attribute("category", "Demo5")); 
	sendMessage(msg);
	while (true) {
		std::vector<std::string>temp;
		std::cout << "\n";
		split(_Display->deQ().bodyString(), ',', temp);
		for (auto file : temp) {
			std::cout << file << " ";
		} std::cout << "\n"; break;
	}});
	t3.join();
	std::cout << "\nUsing GUI to complete the download and delete function. Download html files from ../web to ../Repository\n\n";
}

void MsgClient::DemoRequirment346() {
	std::thread t4([&]() {
		HttpMessage msg = makeMessage("typeanalysis", "", "8080");
	std::cout << "\n  Do Remote Type Analysiser " << std::endl; 
	sendMessage(msg);
	});
	t4.join();
}

void MsgClient::DemoRequirment7() {

	std::thread t5([&]() {	
	HttpMessage msg;
	msg.addBody("Test Requirement 7#.");
	msg.addAttribute(HttpMessage::attribute("Test Attribute", "This is a test body value")); msg.addAttribute(HttpMessage::attribute("fromAddr", "8081"));
	msg.addAttribute(HttpMessage::attribute("toAddr", "8080"));
	std::cout << " Message: \n  Body: Test Requirement 7#.\n  Test Attribute: This is a test body value\n fromAddr: 8081\n toAddr:8080\n\n";
	});
	t5.join();
}

void MsgClient::DemoRequirment9() {
	std::cout << "\n  Click Fist Tab Botton To Start Auto Test\n\n";
}


void MsgClient::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::string temp;
	for (size_t i = 0; i < s.size(); i++) {
		if (s.at(i) != delim) {
			temp.append(s, i, 1);
			if (i == s.size() - 1) {
				elems.push_back(temp);
			}
		}
		else if (temp != "") {
			elems.push_back(temp);
			temp = "";
		}
	}
}


#ifdef TEST_CLIENT
int main()
{
	MsgClient c;
	c.Connection();

	std::thread t1(
		[&c]() {
		std::vector<std::string> ts = FileSystem::Directory::getFiles("../Repository");
		for (std::string t : ts) {
			c.sendFile(t, "a");
		}
		c.ClientListen();
	});

	t1.join();
}
#endif