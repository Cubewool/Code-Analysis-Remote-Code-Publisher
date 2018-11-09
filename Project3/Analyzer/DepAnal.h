#pragma once
///////////////////////////////////////////////////////////////////
// DepAnal.h - Analysis the dependency of each files and store   //
// relationship into XML file                                    //
// table                                                         //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to start denpendency analysis from each file.
* Comparing with the type table element and store the dependency relationship
* into XML files.
* It uses C++11 constructs, NoSQLdb(Project1)
*
* The elements defined in the companion package They are:
*   DepAnal          - initialization the dependency analysis
*   doDepAnal        - start do the denpendency analysis
*   showDep          - show the relationship of the dependency db \
*   getDB            - return and get the db for dependency
*
* Build Process:
* --------------
*   devenv CodeAnalyzerEx.sln /debug rebuild
*
* Dependency :
*--------------
* TypeAnal.h
* Typetable.h
* NoSQLdb.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 8 Mar 2017
* - added isGlobal function to determine the glbal member
* - add search function to compare the global function
*/
#include "../Parser/ActionsAndRules.h"
#include"../Tokenizer/Tokenizer.h"
#include"../FileMgr/FileMgr.h"
#include"TypeTable.h"
#include"TypeAnal.h"
#include <iostream>
#include <functional>
#include<unordered_set>
#include "../OODProject1/HelpSessionDemo/NoSqlDb/NoSqlDb.h"
using namespace Scanner;
#ifndef  DEPANAL_H
#define  DEPANAL_H
namespace CodeAnalysis {
	class DepAnal {
	public:
		DepAnal(TypeAnal);//initializaion
		void doDepAnal(); //start do dependency analysis
		void showDep();   //show the dependency from the db
		NoSqlDb<std::string> &getDB() {
			return db;
		}
		std::unordered_set<std::string> getFiles(){ return files; }
	private:
		void findDep(std::string tok, std::string file, Element<std::string>&elem);//find the dependecny of each file
		std::unordered_set<std::string> files;
		TypeTable _typeTable;
		NoSqlDb< std::string> db;
		int number;
		std::string Class_;
	};
}
#endif // ! DEPANAL_H

