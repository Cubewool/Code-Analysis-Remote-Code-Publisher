#pragma once
#ifndef NOSQLDB_H
#define NOSQLDB_H
/////////////////////////////////////////////////////////////////////
// NoSqlDb.h - key/value pair in-memory database and defition      //
// functions to write and changing Database                        //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* This package is intended to definition the key/value pair and definition the value of each key.
* implement the function changing Database element values
* Changing the key is forbidden. It uses C++11 constructs,
*
* The elements defined in the companion package They are:
*   show                -show the element of value
*   save                -save the element to the key/value pair
*   deleting            -deleting the key/value pair from the map
*   changestr           -changing the text or data type element content
*   addchildren         -add children relationship to the exstied value.
*   deletchildren       -deleting the chilkey from the parent key.
*   value               -get the key/value pair value.
*   count               -get the size of database.
*
* Required Files:
* ---------------
*    -Conver.h
*     CppProperties.h
*
* Build Process:
* --------------
*   devenv Project1.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 2 Jun 2017
* - added changeing str/children functions
* - added deleting key/value pair functions
* - added test to test function
*/
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <unordered_set>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <sstream>
#include <iterator>
#include <algorithm>
#include "../Convert/Convert.h"
#include"../../../FileSystem/FileSystem.h"
#include "../CppProperties/CppProperties.h"

/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
// - it needs to store child data, something for you to implement
//
template<typename Data>
class Element
{
public:
  using Name = std::string;
  using Category = std::string;
  using TimeDate = std::string;
  using Children = std::unordered_set<std::string>;//children store keys
  using Description = std::string;// text description
  using File = FileSystem::Path;

  Property<Name> name;            // metadata
  Property<Category> category;    // metadata
  Property<TimeDate> timeDate;    // metadata
  Property<Data> data;            // data
  Property<Description>description;//description metadata
  Property<Children> children;    //children

  void addvect(std::string str) {
	  std::unordered_set<std::string> temp = children;
	  temp.insert(str);
	  children = temp;
  }

  std::string show();
};

template<typename Data>
std::string Element<Data>::show()
{
  // show children when you've implemented them

  std::ostringstream out;
  out.setf(std::ios::adjustfield, std::ios::left);
  out << "\n    " << std::setw(8) << "name"     << " : " << File::getName(name);
  //out << "\n    " << std::setw(8) << "category" << " : " << category;
  //out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
  //out << "\n    " << std::setw(8) << "data"     << " : " << data;
  /*-----show children-----*/
  out << "\n    " << std::setw(8) << "children" << " : " <<children;
  /*------show description------*/
  // out << "\n    " << std::setw(8) << "description" << " : " << description<<std::endl;
  return out.str();
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - you will need to add query capability
//   That should probably be another class that uses NoSqlDb
//
template<typename Data>
class NoSqlDb
{
public:
  using Key = std::string;
  using Keys = std::vector<Key>;

  Keys keys();
  bool save(Key key, Element<Data> elem);
  bool deleting(Key key);//delate the key/value pair
  bool changestr(Key key, std::string , Data);// edite/replace the value
  //bool addchildren(Key key, std::string);//addition and deletion the children
  bool deletchildren(Key key, std::string);//delete the children relationship
  Element<Data> value(Key key);//get the value
  size_t count();//get the database size
private:
  using Item = std::pair<Key, Element<Data>>;
  std::unordered_map<Key,Element<Data>> store; 
};


template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
  Keys keys;
  for (Item item : store)
  {
    keys.push_back(item.first);
  }
  return keys;
}

template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
  if (store.find(key) != store.end())
	  return false;
  /*---------time-date string record the time-----*/
  struct std::tm ti;
  time_t now = time(0);
  localtime_s(&ti, &now);
  std::ostringstream oss;
  oss << std::put_time(&ti, "%m-%d-%Y %H:%M:%S");
  auto str = oss.str();
  elem.timeDate = str;

  store[key] = elem;
  return true;
}

template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
  if (store.find(key) != store.end())
    return store[key];
  return Element<Data>();
}


template<typename Data>
size_t NoSqlDb<Data>::count()
{
  return store.size();
}
template<typename Data>
bool NoSqlDb<Data>::deleting(Key key)//delate the exsit key/value pair
{
	if (store.find(key) != store.end()) {
		store.erase(key);//delate the key
		for (Key foundkey : this->keys())//delate the parent relationship
		{
			std::vector<std::string>temp = store.at(foundkey).children;
			for (std::vector<std::string>::iterator it = temp.begin(); it != temp.end();) {
				if (*it == key) {
					it = temp.erase(it);
					store.at(foundkey).children = temp;
				}
				else ++it;
			}
		}
		return true;
	}
	else return false;
}

template<typename Data> //changing the element with string/data type content
bool NoSqlDb<Data>::changestr(Key key, std::string type, Data str)
{
	if (store.find(key) != store.end()) {
		if (type == "data") {
			store.at(key).data = str;
			return true;
		}
		else {
			std::string dataString = Convert<Data>::toString(str);
			if (type == "category") {
				store.at(key).category = dataString;
				return true;
			}
			else if (type == "description") {
				store.at(key).description = dataString;
				return true;
			};
		}return true;
	}
	else return false;
}

template<typename Data> //deleting relasionship from children
bool NoSqlDb<Data>::deletchildren(Key key, std::string str)
{
	if (store.find(key) != store.end()) {
		std::vector<std::string> temp = store.at(key).children;
		temp.erase(std::remove(temp.begin(), temp.end(), str), temp.end());
		store.at(key).children = temp;
		return true;
	}
	else return false;
}
/////////////////////////////////////////////////////////////////////
// Persistance class
// - you will need to implement that using the XmlDocument class
//   from Handouts/Repository

#endif // !NOSQLDB_H
