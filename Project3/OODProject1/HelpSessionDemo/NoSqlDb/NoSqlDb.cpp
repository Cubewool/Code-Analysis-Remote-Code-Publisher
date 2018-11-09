/////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - key/value pair in-memory database                 //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////
//#define _CRT_SECURE_NO_WARNINGS
#include "../CppProperties/CppProperties.h"
#include "NoSqlDb.h"
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

#ifdef NOSQLDB
int main()
{
  std::cout << "\n  Demonstrating NoSql Helper Code";
  std::cout << "\n =================================\n";

  std::cout << "\n  Creating and saving NoSqlDb elements with string data";
  std::cout << "\n -------------------------------------------------------\n";

  NoSqlDb<StrData> db;

  Element<StrData> elem1;
  elem1.name = "elem1";
  elem1.category = "test";
  elem1.data = "elem1's StrData";
  elem1.description = "the first one";

  db.save(elem1.name, elem1);

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

  elem3.children = { elem1.name,elem2.name };//store the key in children

  db.save(elem3.name, elem3);

  std::cout << "\n  Retrieving elements from NoSqlDb<string>";
  std::cout << "\n ------------------------------------------\n";

  std::cout << "\n  size of db = " << db.count() << "\n";
  Keys keys = db.keys();
  for (Key key : keys)
  {
    std::cout << "\n  " << key << ":";
    std::cout << db.value(key).show();
  }
  std::cout << "\n\n";
  std::cout << "\n\n";
}
#endif // NOSQLDB