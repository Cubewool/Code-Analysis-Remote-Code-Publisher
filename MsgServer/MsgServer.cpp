/////////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Demonstrates two-way HTTP style messaging           //
//                 and file transfer                                   //
//                                                                     //
// Jingyi Yang, CSE687 - Object Oriented Design, Spring 2017           //
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

#include"../MsgServer/MsgServer.h"
#include <iostream>
#include <cstdlib>

void ClientHandler::sendMessage(HttpMessage& msg) {
	std::string msgString = msg.toString();
	si->send(msgString.size(), (Socket::byte*)msgString.c_str());

}
void ClientHandler::Connection(std::string port) {

	Show::title("Server do connection: " + port);
	try
	{
		while (!si->connect("localhost", std::stoi(port)))
		{
			Show::write("\n Server waiting to connect");
			::Sleep(100);
		}
		std::cout << "\n\n Connection Success\n" << std::endl;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Caught Exeception : ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

void ClientHandler::ServerListen() {
	std::cout << "Server start listening\n\n";
	try {
		sl = new SocketListener(8080, Socket::IP6);
		si = new SocketConnecter();
		sl->start(*this);
		while (true) {
			HttpMessage msg = msgQ->deQ();
			Show::write("\n\n Server: " +msg.headerString()+ msg.bodyString());
		}
	}
	catch (std::exception& exc) {
		Show::write("\n Caught Exception: ");
		std::string exit = "\n" + std::string(exc.what()) + "\n\n";
		Show::write(exit);
	}
}

HttpMessage ClientHandler::makeMessage(std::string str, const std::string& body, const EndPoint& ep) {
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8080"; 													// given to its constructor.
	msg.clear();
	msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

	msg.addBody(body);

	if (str == "_returncate" || "_returnhtm" || "_returnfileCate" || "_returnnoparent"
		|| "_sendfile") {
		msg.addAttribute(HttpMessage::attribute("mode", str));
		if (str != "_sendfile")
			msg.addAttribute(HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size())));
		msg.addBody(body);
	}
	else
	{
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}

void ClientHandler::split(std::string &s, char delim, std::string &elem) {
	std::string temp;
	std::vector<std::string>elems;
	for (size_t i = 0; i < s.size(); i++) {
		if (s.at(i) != delim) {
			temp.append(s, i, 1);
			if (i == s.size() - 1) {
				elems.push_back(temp);
				elem = elems.at(1);
			}
		}
		else if (temp != "") {
			elems.push_back(temp);
			temp = "";
			//elem = elems.at(1);
		}
	}
}

bool ClientHandler::sendFile(const std::string& filename) {
	std::string fqname = "..\\Web\\" + filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage("_sendfile", "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
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

//----< this defines processing to frame messages >------------------

HttpMessage ClientHandler::readMessage(Socket& socket){
	HttpMessage msg;
	connectionclose = false;
	while (true){
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1){
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);}
		else break;}
	if (msg.attributes().size() == 0) {
		connectionclose = true;
		return msg;}
	std::string from = msg.findValue("fromAddr");
	std::string port;
	split(from, ':', port);
	endtarget = port;
	std::string  mode= msg.findValue("mode");
	size_t pos = msg.findAttribute("content-length");
	size_t numBytes = 0;
	if (mode == "sendfile"){
		std::string category = msg.findValue("category");
		std::string filename = msg.findValue("file");
		std::string sizeString = msg.findValue("content-length");
		size_t contentSize;
		if (sizeString != "") contentSize = Converter<size_t>::toValue(sizeString);
		else return msg;
		readFile(filename, contentSize, category, socket);}	 
	else{	
		numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
		Socket::byte* buffer = new Socket::byte[numBytes + 1];
		socket.recv(numBytes, buffer);
		buffer[numBytes] = '\0';
		std::string msgBody(buffer);
		msg.addBody(msgBody);
		delete[] buffer;}
	std::cout << "Client Port:" << port;
	Connection(port);
	if (mode == "sendfile"){ std::cout << "Recevied File:" + msg.findValue("file");
		returnFiles(msg.bodyString(), port);}
	else if (mode == "showhtml") htmNames(port);
	else if (mode == "showcategory") returnCategories(port);
	else if (mode == "typeanalysis") DoAnaly(port);
	else if (mode == "showallfiles") filesincategory(msg.findValue("category"), port);
	else if (mode == "shownoparents") NoParent(msg.findValue("category"), port);
	else if (mode == "download") returnFiles(msg.findValue("file"),port);
	else if (mode == "delete") deletefile(msg.bodyString());
	if (msg.attributes().size() == 0) {
		return msg;}
	return msg;
}

void ClientHandler::returnCategories(std::string port) {
	HttpMessage msg;
	std::vector<std::string> directories = FileSystem::Directory::getDirectories(path);
	std::string msgbody = "";
	for (std::string folder : directories) // for each( string i: directro)
	{
		if (folder == "." || folder == "..") continue;
		msgbody += folder + ",";
	}
	msg = makeMessage("_returncate", msgbody, port);
	sendMessage(msg);
}

void ClientHandler::htmNames(std::string port)
{
	std::string msgbody = "";
	for (std::string file : FileSystem::Directory::getFiles("../web","*.htm"))
	{
		msgbody += FileSystem::Path::getName(file) + ","; // use : to split the files
	}

	HttpMessage tempmsg = makeMessage("_returnhtm", msgbody, port);
	sendMessage(tempmsg);
}

void ClientHandler::filesincategory(std::string category, std::string port) {
	HttpMessage msg;
	std::string msgbody = "";
	std::vector<std::string>files= FileSystem::Directory::getFiles(path + "/" + category);
	for (std::string file : files)
	{
		msgbody += FileSystem::Path::getName(file) + ",";
	}
	msg = makeMessage("_returnfileCate", msgbody, port);
	msg.addAttribute(HttpMessage::Attribute("category", category));
	sendMessage(msg);
}
void ClientHandler::DoAnaly(std::string port) {
	int i = system("start ../Debug/CodeAnalyzer.exe ../Repository *.h *.cpp /m /r /f");
}
unsigned ClientHandler::ifempty(std::vector<std::string> v, std::string d)
{
	if (v.size() == 0)
		return NULL;
	unsigned i;
	for (i = 0; i < v.size(); i++)
	{
		if (v[i] == d) {
			i++;
			break;
		}
	}
	if (v[i - 1] != d)
	{
		return NULL;
	}
	return i;
}
void ClientHandler::NoParent(std::string category, std::string port) {
	std::vector<std::string> files = FileSystem::Directory::getFiles("../RemoteRepository/" + category);
	std::vector<std::string> p;
	std::string body;
	HttpMessage msg;
	if (FileSystem::Directory::exists("../Web/Noparent")) {
		std::ifstream noParent("../Web/Noparent/noparent.txt");
		std::string nop;
		while (std::getline(noParent, nop)) {
			p.push_back(nop);
		}
	}
	for (auto file : files) {
		if (ifempty(p, file) != 0) {
			body += file + ",";
		}
	}
	msg = makeMessage("_returnnoparent", body, endtarget);
	msg.addAttribute(HttpMessage::Attribute("category", category));//add the category attribute to different from other message
	sendMessage(msg);
}
void ClientHandler::returnFiles(const std::string& body, std::string port) {
	std::vector<std::string>files = StringHelper::split(body);
	files.push_back("Hide.js");
	files.push_back("style.css");
	for (auto file : files) {
		sendFile(file);
	}
}
void ClientHandler::deletefile(std::string path) {
	std::vector<std::string>files;
	std::string fqname;
    split2(path,',', files);
	if (files[0] == "none") {
		fqname = "../RemoteRepository/" + files[1];
		std::vector<std::string> files = FileSystem::Directory::getFiles(fqname);
		for (auto file : files) {
			std::string fpath = fqname + '/' + file;
			remove(fpath.c_str());
		}

		std::string dic = FileSystem::Path::getFullFileSpec(fqname);
		RemoveDirectory(dic.c_str());
	}
	else {
		if (files[1] == "none") {
			fqname = "../Web/" + files[0];
			std::string dic = FileSystem::Path::getFullFileSpec(fqname);
			remove(dic.c_str());
		}
		else {
			fqname = "../RemoteRepository/" + files[1] + "/" + files[0];
			std::string dic = FileSystem::Path::getFullFileSpec(fqname);
			remove(dic.c_str());
		}
	}
}
void ClientHandler::split2(const std::string &s, char delim, std::vector<std::string> &elems) {
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

void ClientHandler::readFile(const std::string& filename, size_t fileSize, std::string cate, Socket& socket)
{
	if (!FileSystem::Directory::exists("../RemoteRepository/" + cate))
	{
		FileSystem::Directory::create("../RemoteRepository/" + cate);
	}
	std::string fqname = "../RemoteRepository/" + cate + "/" + FileSystem::Path::getName(filename);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood()) {
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
void ClientHandler::operator()(Socket socket) {
	while (true) {
		HttpMessage msg = readMessage(socket);
		if (connectionclose || msg.bodyString() == "quit") {
			Show::write("\n\n  ClientHandler thread is terminating\n");
			break;
		}
		msgQ->enQ(msg);
	}
}

//----< test stub >--------------------------------------------------

int main()
{
	ClientHandler si;
	si.ServerListen();
}
