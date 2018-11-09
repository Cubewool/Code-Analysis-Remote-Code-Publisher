#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - Demo for CSE687 Project #4, Spring 2015                 //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ                      //
//                                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015               //
/////////////////////////////////////////////////////////////////////////////
/*
*
* The elements defined in the companion package They are:
*
*  startClient     - start client connection
*  sendfile        - sendfiles connetion
*  todoAnalysiser  - call server analysiser
*  showCategories  - call show category function 
*  displayHTML     - call display html names funcition
*  showfiles       - call show files in category file function
*  NoParent        - call no parent files function
*  downLoad        - call download function
*  deletefile      - deletefiles
*  setSender       - set sender
*  startdemo       - call demo function
*
* Maintenance History:
* --------------------
* ver 1.0 : 1 MAy 2017
*/
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message = std::string;

struct ISendr
{
	virtual void postMessage(const Message& msg) = 0;
};

struct IRecvr
{
	virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};


struct IClient
{
public:
	virtual void startClient() = 0;// start listener
	virtual void sendfile(std::string filename, std::string category) = 0; //do send file call
	virtual void todoAnalysiser() = 0; // do project3 call
	virtual void showCategories() = 0; //do show category call
	virtual void displayHTML() = 0; //do html show call
	virtual void showfiles(std::string category) = 0; // show file names call
	virtual void NoParent(std::string category) = 0;// call no parent names
	virtual void downLoad(std::string filename) = 0;// call download files
	virtual void deletefile(std::string filename,std::string category) = 0; //cal  delete files
	virtual void setSender(ISendr*) = 0;// start set sender
	virtual void startdemo() = 0;//call demo 

};

extern "C" {
	struct ObjectFactory
	{
		DLL_DECL ISendr* createSendr();
		DLL_DECL IRecvr* createRecvr();
		DLL_DECL IClient* createClient();
		DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr, IClient* pIClient);
	};
}

#endif


