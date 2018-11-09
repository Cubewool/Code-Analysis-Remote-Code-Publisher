#pragma once
#ifndef  HELP_H
#define  HELP_H
///////////////////////////////////////////////////////////////////
// Help.h - a function changing Element to XML, and return XML   //
// back to Database                                              //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to implement the function changing Database element to XML and 
* stored it. Then could load the XML back as an NoSql database. It uses C++11 constructs,
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   toXML              - Based on transXML and change Element to XML and stored it
*   fromXML            - load XML file and return it as DataBase
*   transXML           - write each value of the key/value pair to the XML node
*   split              - A small function to split the children elemnt string with the blank.
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*     NoSqlDb.h
*     StrHelper.h
*
* Build Process:
* --------------
*   devenv Project1.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Jun 2017
* - added building document from XML file using XmlParser in constructor
* - added test to test function
*/
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include"../HelpSessionDemo/NoSqlDb/NoSqlDb.h"
#include "../StrHelper.h"

#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>

using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;
using Key = NoSqlDb<std::string>::Key;
using Keys = NoSqlDb<std::string>::Keys;

template <typename Data>
class TransXml {
public:  
	bool toXml(NoSqlDb<Data>& );//change element of database to XML element
	NoSqlDb<Data> fromXml(std::string );	// load element from xml file

private:
	void transXML(NoSqlDb<Data>&, SPtr , Key ); //write each node
	std::vector<std::string> split(std::string);//split a string with blank
};


template <typename Data>//change element of database to XML element
bool TransXml<Data>::toXml(NoSqlDb<Data>& db)
{
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("NoSqlDb");
	doc.docElement() = pRoot;

	Keys keys = db.keys();
	for (Key key : db.keys())
	{
		transXML(db, pRoot, key);
	}
	std::string xml;
	xml = doc.toString();
	std::ofstream out("../Result.xml");
	out << xml;
	out.close();
	std::cout << xml;
	return true;
}

template <typename Data> //return database from xml
NoSqlDb<Data> TransXml<Data>::fromXml(std::string path)
{
	NoSqlDb<Data> db;
	try {
		std::cout << "\n  Open XML file ";
		std::cout << "\n  Convert XML to Database" << std::endl;

		XmlDocument doc(path, XmlDocument::file);

		std::vector<SPtr>desc = doc.descendents("name").select();		  // fetch the name string from XML element
		std::vector<SPtr>desc1 = doc.descendents("category").select();	  //fetch the category	
		std::vector<SPtr>desc2 = doc.descendents("description").select();	  //fetch the description
		std::vector<SPtr>desc3 = doc.descendents("timeDate").select();      //fetch the timeDate
		std::vector<SPtr>desc4 = doc.descendents("children").select();      //fetch the Children
		std::vector<SPtr>desc5 = doc.descendents("data").select();	//fetch the Children

		for (unsigned int i = 0; i < desc.size(); i++)
		{
			Element<Data> x;
			std::string name_value = desc[i]->children()[0]->value();
			name_value = trim(name_value);
			x.name = name_value;

			std::string cateString = trim(desc1[i]->children()[0]->value());
			x.category = Convert<std::string>::fromString(cateString);

			std::string desString = trim(desc2[i]->children()[0]->value());
			x.description = desString; // Convert<std::string>::fromString(desString);

			std::string timeString = trim(desc3[i]->children()[0]->value());
			x.timeDate = Convert<std::string>::fromString(timeString);

			if (desc4[i]->children().size() != 0)
			{
				std::string chilString = trim(desc4[i]->children()[0]->value());
				x.children = split(chilString);
			}
			std::string dataString = trim(desc5[i]->children()[0]->value());
			x.data = Convert<Data>::fromString(dataString);

			db.save(x.name, x);
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n  something bad happend";
		std::cout << "\n  " << ex.what();
	}
	return db;
}

template<typename Data>// add each value to the node
void TransXml<Data>::transXML(NoSqlDb<Data>& db, SPtr pRoot, Key key)
{
	Element<Data> elem = db.value(key);
	SPtr dataValue = makeTaggedElement("DataValue");
	pRoot->addChild(dataValue);

	// make Property<string> name element and add to root
	SPtr pNameElem = makeTaggedElement("name");
	dataValue->addChild(pNameElem);
	SPtr pTextElem = makeTextElement(elem.name);
	pNameElem->addChild(pTextElem);

	// make category element and add to root
	SPtr categoryElem = makeTaggedElement("category");
	dataValue->addChild(categoryElem);
	SPtr pTextElem2 = makeTextElement(elem.category);
	categoryElem->addChild(pTextElem2);

	//make timeData element and add to root
	SPtr timeDateElem = makeTaggedElement("timeDate");
	dataValue->addChild(timeDateElem);
	SPtr pTextElem3 = makeTextElement(elem.timeDate);
	timeDateElem->addChild(pTextElem3);

	//make decription element and add to root
	SPtr descriptionElem = makeTaggedElement("description");
	dataValue->addChild(descriptionElem);
	SPtr pTextElem4 = makeTextElement(elem.description);
	descriptionElem->addChild(pTextElem4);

	//make <vector>children element and add to root
	std::string dataelement;
	std::unordered_set<std::string>vec = elem.children;
	for (auto v: vec)
	{
		dataelement += (v + " ");
	}
	SPtr childrenElem = makeTaggedElement("children");
	dataValue->addChild(childrenElem);
	SPtr pTextElem5 = makeTextElement(dataelement);
	childrenElem->addChild(pTextElem5);

	//make Property<Data> data element and add to root
	SPtr pDataElem = makeTaggedElement("data");
	dataValue->addChild(pDataElem);
	Data data = elem.data;
	std::string dataString = Convert<Data>::toString(data);
	pTextElem = makeTextElement(dataString);
	pDataElem->addChild(pTextElem);
}

template <typename Data>//split with element blank
std::vector<std::string> TransXml<Data>::split(std::string str)
{
	std::vector<std::string>result;
	std::string buf;
	std::stringstream ss(str);
	while (ss >> buf)
		result.push_back(buf);
	return result;
}

#endif
