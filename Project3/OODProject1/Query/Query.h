#pragma once
#ifndef QUERY_H
#define QUERY_H
///////////////////////////////////////////////////////////////////
// Query.h - contains the query function                         //
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
* 	showValue                 -show the value of a specified key
*	showChildren              -show the children of specified key
*   showKey                   -show the key of some queries
*	keysUnion                 -get the union of somen vectors
*	matchName                 -geting key of specified name element
*	matchCate                 -geting key of specified category element
*	matchData                 -geting key of specified  data element
*	matchtime                 -geting key of specified timeDate element
* Required Files:
* ---------------
*   - CppProperties.h
*     Convert.h
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
* - added query fuction
*/
#include"../HelpSessionDemo/NoSqlDb/NoSqlDb.h"
#include"../HelpSessionDemo/CppProperties/CppProperties.h"
#include"../HelpSessionDemo/Convert/Convert.h"

#include<vector>
#include<string>
#include<algorithm>
#include<set>
#include<iterator>

using Key = NoSqlDb<std::string> :: Key;
using Keys = NoSqlDb<std::string> :: Keys;

template <typename Data>
class Query {
public:
	Element<Data> showValue(NoSqlDb<Data>&, Key key);//show the value of a specified key
	std::vector<std::string> showChildren(NoSqlDb<Data>&, Key key );//show the children of specified key
	std::vector<Key> showKey(NoSqlDb<Data>&, std::string type, std::string str);//show the key of some queries
	std::vector<Key> keysUnion(std::vector<Key>&key1, std::vector<Key>&key2);// get the union of somen vectors
	std::vector<Key> showAll(NoSqlDb<Data>&, std::string pattern);
private:
	std::vector<Key> matchName(NoSqlDb<Data>&, std::string str); // geting key of specified name element
	std::vector<Key> matchCate(NoSqlDb<Data>&, std::string str); // geting key of specified category element
	std::vector<Key> matchData(NoSqlDb<Data>&, Data str);        // geting key of specified  data element
	std::vector<Key> matchtime(NoSqlDb<Data>&, std::string str); // geting key of specified timeDate element

};

template<typename Data> // show the value of a specified key
Element<Data> Query<Data>::showValue(NoSqlDb<Data>&db, Key key)
{
	std::cout << "\n    " << std::setw(8) << "The specified" << " : " << key;
	Element<Data> elem;
	elem = db.value(key);
	return elem;
}
template<typename Data>//return all keys when match a pattern
std::vector<Key> Query<Data>::showAll(NoSqlDb<Data>&db, std::string pattern) {
	if (pattern == "pattern")
		return db.keys();
	else{
		std::vector<Key>null = {};
		return null;
	}
}

template <typename Data> //show the children of specified key
std::vector<std::string> Query<Data>::showChildren(NoSqlDb<Data>&db, Key key) {
	std::cout << "\n    " << std::setw(8) << "The children of" << " : " << key << "\n";
	std::vector<std::string>vec;
	vec = db.value(key).children;
	return vec;
}

template<typename Data> // show the keys of specified pattern
std::vector<Key> Query<Data>::showKey(NoSqlDb<Data>&db, std::string type, std::string str) {
	std::cout << "\n    " << std::setw(8) << "The pattern type" << " : " << type;
	std::cout << "\n    " << std::setw(8) << "The specific string" << " : " << str;
	std::cout << "\n    " << std::setw(8) << "The matched key set" << " : ";
	std::vector<Key>keys;
	if (type == "name") {
		keys = matchName(db, str);
	}
	else if (type == "category") {
		keys = matchCate(db, str);
	}
	else if (type == "data") {
		keys = matchData(db, str);
	}
	else if (type == "timeDate") {
		keys = matchtime(db,str);
	}
	return keys;
}

template<typename Data> // get keys from specified strings of name
std::vector<Key> Query<Data>::matchName(NoSqlDb<Data>&db, std::string str)
{
	std::vector<Key> NameKeys;
	Keys keys = db.keys();
	for (Key key : keys) {
		std::string name = db.value(key).name;
		if (name.find(str) != std::string::npos)
			NameKeys.push_back(name);
	}
	return NameKeys;
}

template<typename Data> //get keys from specified strings of category
std::vector<Key> Query<Data>::matchCate(NoSqlDb<Data>&db, std::string str)
{
	std::vector<Key> CateKeys;
	Keys keys = db.keys();
	for (Key key : keys) {
		std::string cate = db.value(key).category;
		if (cate.find(str) != std::string::npos)
			CateKeys.push_back(db.value(key).name);
	}
	return CateKeys;
}

template<typename Data> // get keys from specified strings from data
std::vector<Key> Query<Data>::matchData(NoSqlDb<Data>&db, Data str)
{
	std::vector<Key> DataKeys;
	Keys keys = db.keys();
	for (Key key : keys) {
		std::string data = Convert<Data>::toString(db.value(key).data);
		if (data.find(str) != std::string::npos)
			DataKeys.push_back(db.value(key).name);
	}
	return DataKeys;
}

template<typename Data> //get keys from specified strings from timeDate
std::vector<Key> Query<Data>::matchtime(NoSqlDb<Data>&db, std::string str)
{
	std::vector<Key> timeKeys;
	Keys keys = db.keys();
	for (Key key : keys) {
		std::string time = db.value(key).timeDate;
		if (time.find(str) != std::string::npos)
			timeKeys.push_back(db.value(key).name);
	}
	return timeKeys;
}

template<typename Data> //return union keys from two vectors
std::vector<Key> Query<Data>::keysUnion(std::vector<Key>&key1, std::vector<Key>&key2)
{
	std::vector<Key> keys;
	std::sort(key1.begin(), key1.end());
	std::sort(key2.begin(), key2.end());
	std::set_union(key1.begin(), key1.end(), key2.begin(), key2.end(), std::back_inserter(keys));
	return keys;
}
#endif