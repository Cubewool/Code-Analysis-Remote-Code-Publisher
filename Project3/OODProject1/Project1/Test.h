#pragma once
#ifndef TEST_H
#define TEST_H
///////////////////////////////////////////////////////////////////
// Test.h - Implement each requirement functions                 //
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
* This package is intended to implement the functions of each query.
* It uses C++11 constructs.
*
*  createElement          -create value and save it into element
*  demonstrateElement     -demonstrate the db
*  Req2                   -create string
*  Req2_                  -create Data type
*  Req3                   -add/delete key/value pair
*  Req4                   -edit and add/delete children/value
*  Req4_                  -edit Data type
*  Req5                   -toXMl and from XML
*  Req7                   -Queries
*  Req8                   -Query together
*  Req9                   -key union
*  Req10                  -XML show references
*  
* Required Files:
* ---------------
*   - Help.h
*     Query.h
*     Conver.h
*
* Build Process:
* --------------
*   devenv Project1.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Jun 2017
* - added requirment functions
*/
#include"../Demo/Help.h"
#include"../Query/Query.h"
#include"../HelpSessionDemo/Convert/Convert.h"

#include<vector>
#include<string>

template <typename Data>
class Test {
public:
	void createElement(NoSqlDb<Data>&db, Element<Data> &elem,
		std::string name, std::string category, Data data, std::string description, Keys key);
	void demonstrateElement(NoSqlDb<Data>&db);
	void Req2(NoSqlDb<Data>&db);//create string
	void Req2_(NoSqlDb<Data>&db);//create Data type
	void Req3(NoSqlDb<Data>&db);//add delete
	void Req4(NoSqlDb<Data>&db);//edit and add/delete children/value
	void Req4_(NoSqlDb<Data>&db);// edit Data type
	void Req5(NoSqlDb<Data>&db);//toXMl and from XML
	void Req7(NoSqlDb<Data>&db);//Query
	void Req8(NoSqlDb<Data>&db);//Query together
	void Req9(NoSqlDb<Data>&db);//key union
	void Req10(std::string path);//XML
};

template<typename Data> //create value of element
void Test<Data>::createElement(NoSqlDb<Data>&db, Element<Data> &elem,
	std::string name, std::string category, Data data, std::string description, Keys key)
{
	elem.name = name;
	elem.category = category;
	elem.data = data;
	elem.description = description;
	elem.children = key;
	db.save(elem.name, elem);
}

template<typename Data> //demonstrate each value
void Test<Data>::demonstrateElement(NoSqlDb<Data>&db) {
	std::cout << "\n  size of db = " << db.count() << "\n";
	Keys keys = db.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		std::cout << db.value(key).show();
	}
	std::cout << "\n\n";
}

template<typename Data> //implement requirment 2
void Test<Data>::Req2(NoSqlDb<Data>&db)
{
	std::cout << "Requirement 2#" << "\n";
	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";
	Element<Data>elem1, elem2, elem3;
	createElement(db, elem1, "elem1", "test1", "elem1StrData", "the first one", {});
	createElement(db, elem2, "elem2", "test2", "elem2StrData", "the second one", {});
	createElement(db, elem3, "elem3", "test3", "elem3StrData", "the third one", { elem1.name,elem2.name });
	demonstrateElement(db);
}

template<typename Data> // Int type of requirment 2#
void Test<Data>::Req2_(NoSqlDb<Data>&db)
{
	std::cout << "Requirement 2#" << "\n";
	std::cout << "\n  Creating and saving NoSqlDb elements with int data";
	std::cout << "\n -------------------------------------------------------\n";
	Element<Data> ielem1, ielem2, ielem3;
    createElement(db, ielem1, "ielem1", "test", 1, "111111", {});
	createElement(db, ielem2, "ielem2", "test", 2, "222222", {});
	createElement(db, ielem3, "ielem3", "test", 3, "333333", { ielem1.name,ielem2.name });
	demonstrateElement(db);
}

template<typename Data> //requirement 3#
void Test<Data>::Req3(NoSqlDb<Data>&db) {
	std::cout << "Requirement 3#" << "\n";
	std::cout << "Attemputing to remove item with key " << "elem2"<< " from db";
	std::cout << "\n---------------------------------------------------------------\n";
	db.deleting("elem2");
	demonstrateElement(db);
}

template <typename Data> //requirment 4#
void Test<Data>::Req4(NoSqlDb<Data>&db) {
	std::cout << "Requirement 4#" << "\n";
	std::cout << "\n  Changing text metadata";
	std::cout << "\n ----------------------------------------------------\n";
	std::cout << "String elem1 with data and description";
	db.changestr("elem1", "description", "the new elem1");
	db.changestr("elem1", "data", "111");
    demonstrateElement(db);

	std::cout << "Add children elem4 relationship to elem3";
	std::cout << "\n ----------------------------------------------------\n";
	db.addchildren("elem3", "elem4");
	demonstrateElement(db);

	std::cout << "\n  Deleting elem1 children relationship from elem3 ";
	std::cout << "\n ----------------------------------------------------\n";
    db.deletchildren("elem3", "elem1");
	demonstrateElement(db);

}

template <typename Data> // Data type of requirment 4#
void Test<Data>::Req4_(NoSqlDb<Data>&db) {
	std::cout << "Requirement 4#" << "\n";
	std::cout << "\n  Changing text metadata";
	std::cout << "\n ----------------------------------------------------\n";
	std::cout << "Int ielem2 with data \n";
	db.changestr("ielem2", "data", 222);
	demonstrateElement(db);
}

template<typename Data> // requirement 5#
void Test<Data>::Req5(NoSqlDb<Data>&db) {
	std::cout << "Requirement 5#" << "\n";
	std::cout << "\n  To XML file and from a XML file";
	std::cout << "\n ----------------------------------------------------\n";
	TransXml<Data> trans;
	trans.toXml(db);
	std::cout << "\n\n";
	NoSqlDb<Data> newdb = trans.fromXml("../Database.xml");
	demonstrateElement(newdb);
}

template<typename Data> //requirment 6#
void Test<Data>::Req7(NoSqlDb<Data>&db) {
	std::cout << "Requirement 7#" << "\n";
	std::cout << "\n ----------------------------------------------------\n";
	std::cout << "Make query for the string Database db\n";
	Query<Data> query;
	std::cout << "the query 1" << "\n";//query 1#
	std::cout << "----------------------------";
	std::cout << query.showValue(db, "elem1").show();
	std::cout << "\n" << "the query 2" << "\n";//query 2#
	std::cout << "----------------------------";
	std::vector<std::string> vec;
	vec = query.showChildren(db, "elem3");
	for (unsigned int i = 0; i < vec.size(); i++) {
		std::cout << std::setw(9) << vec.at(i) << "\n";	}
	std::cout << "\n\n" << "the query 3: defaults to all keys" << "\n";//query 3#
	std::cout << "----------------------------\n";
	std::vector<std::string>vec3;
	vec3 = query.showAll(db, "pattern");
	for (unsigned int i = 0; i < vec3.size(); i++) {
		std::cout << vec3.at(i) + " ";
	}
	std::cout << "\n\n" << "the query 4: Name" << "\n";//query 4#
	std::cout << "----------------------------";
	std::vector<std::string>vec4;
	vec4 = query.showKey(db, "name", "elem");
	for (unsigned int i = 0; i < vec4.size(); i++) {
		std::cout << vec4.at(i) + " ";	}
	std::cout << "\n\n" << "the query 5: category" << "\n";//query 5#
	std::cout << "----------------------------";
	std::vector<std::string>vec5;
	vec5 = query.showKey(db, "category", "test4");
	for (unsigned int i = 0; i < vec5.size(); i++) {
		std::cout << vec5.at(i) + " ";	}
	std::cout << "\n\n" << "the query 6: data" << "\n";//query 6#
	std::cout << "----------------------------";
	std::cout << "\n" << "show the string:";
	std::vector<std::string>vec6;
	vec6 = query.showKey(db, "data", "elem3StrData");
	for (unsigned int i = 0; i < vec6.size(); i++) {
		std::cout << vec6.at(i) + " ";	}
	std::cout << "\n\n" << "the query 7: time" << "\n";//qurey 7#
	std::cout << "----------------------------";
	std::cout << "\n" << "show the string:";
	std::vector<std::string>vec7;
	vec7 = query.showKey(db, "timeDate", "2017");
	for (unsigned int i = 0; i < vec7.size(); i++) {
		std::cout << vec7.at(i) + " ";	}
}

template<typename Data> //requirement 8#
void Test<Data>::Req8(NoSqlDb<Data>&db) {
	std::cout << "\n\n";
	std::cout << "Requirement 8#" << "\n";
	std::cout << "\n  Repeat query 1 to show the value of the set of keys of the query 4";
	std::cout << "\n ----------------------------------------------------\n";
	Query<Data> query;
	std::vector<std::string>vec4;
	vec4= query.showKey(db, "name", "elem");
	for (unsigned int i = 0; i < vec4.size(); i++) {
		std::cout << query.showValue(db, vec4.at(i)).show();
	}
	std::cout << "\n\n";
}
 
template<typename Data> //requirment 9#
void Test<Data>::Req9(NoSqlDb<Data>&db) {
	std::cout << "Requirement 9#" << "\n";
	std::cout << "\n Forming a union of keys from query 5 and query 6";
	std::cout << "\n ----------------------------------------------------\n";
	Query<Data> query;
	std::vector<std::string>vec5;
	vec5 = query.showKey(db, "category", "test4");
	for (unsigned int i = 0; i < vec5.size(); i++) {
		std::cout << vec5.at(i) + " ";
	}
	std::vector<std::string>vec6;
	vec6 = query.showKey(db, "data", "elem3StrData");
	for (unsigned int i = 0; i < vec6.size(); i++) {
		std::cout << vec6.at(i) + " ";
	}
	std::cout << "\n\nThe uion of the keys: ";
	std::vector<std::string>newkeys;
	newkeys = query.keysUnion(vec5, vec6);
	for (std::string newkey : newkeys)
		std::cout << newkey << ' ';
	std::cout << "\n\n";
}

template<typename Data> //requirment 10#
void Test<Data>::Req10(std::string path) {
	std::cout << "Requirement 10#" << "\n";
	std::cout << "\n  Show the form of an XML file";
	std::cout << "\n ----------------------------------------------------\n";
	TransXml<Data> trans;
	NoSqlDb<Data> newdb = trans.fromXml(path);
	demonstrateElement(newdb);
	std::cout << "\n\n";
}

#endif

