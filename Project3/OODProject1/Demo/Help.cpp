///////////////////////////////////////////////////////////////////
// Help.cpp - include test code to implement the toXML and fromXML//
// funciton                                                      //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////

#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../HelpSessionDemo/CppProperties/CppProperties.h"
#include "../HelpSessionDemo/Convert/Convert.h"


#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include "Help.h"

#ifdef HELP
int main()
{
	using StrData = std::string;
	using intData = int;
	NoSqlDb<StrData> db;

	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test";
	elem2.data = "elem2's StrData";
	elem2.description = "the second one";
	db.save(elem2.name, elem2);

	Element<StrData> elem3;
	elem3.name = "elem3";
	elem3.category = "test";
	elem3.data = "elem3's StrData";
	elem3.description = "the third one";
	elem3.children = { "elem2.name" };//store the key in children
	db.save(elem3.name, elem3);

	TransXml<StrData> trans;
	trans.toXml(db);
	NoSqlDb<StrData> dbstr= trans.fromXml("../Database.xml");
	Keys keys = dbstr.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key;
		std::cout << dbstr.value(key).show();
	}

	NoSqlDb<intData> idb;

	Element<intData> ielem1;
	ielem1.name = "elem1";
	ielem1.category = "test";
	ielem1.data = 1;
	ielem1.description = "111111";
	ielem1.children = { "elem1.name"};
	idb.save(ielem1.name, ielem1);
	
	TransXml<intData> trans1;
	trans1.toXml(idb);
	NoSqlDb<intData> dbstr1 = trans1.fromXml("../Database.xml");
	Keys ikeys = dbstr1.keys();
	for (Key key : ikeys)	{
		std::cout << "\n  " << key;
		std::cout << dbstr1.value(key).show();
	}
	return 0;
}
#endif // HELP