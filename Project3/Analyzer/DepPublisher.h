#pragma once
///////////////////////////////////////////////////////////////////
// DepPublisher.h - Create HTM file from Dependency db           //
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
* This package is intended to Create HTM file using the information from
* the Dependency DataBase, and add Collapse function.
* It uses C++11 constructs
*
* The elements defined in the companion package They are:
*   FileAnaly         -get Dependency information
*   CreateIndex       -Create the Index files
*
* Build Process:
* --------------
*   devenv CodeAnalyzerEx.sln /debug rebuild
* Dependency :
*--------------
* NoSqlDb.h
* DepAnal.h
* TypeAnal.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 3 Apr 2017
* - added DepFile function to get the dependency information
* - added CreateIndex function to create file.
*/
#include"../OODProject1/HelpSessionDemo/NoSqlDb/NoSqlDb.h"
#include"../Analyzer/DepAnal.h"
#include"../Analyzer/TypeAnal.h"
#include<string>
#include<unordered_set>
#include<iostream>

#ifndef  DEPPUBLISH_H
#define  DEPPUBLISH_H

using namespace CodeAnalysis;
struct FileDep //store the dependency infromation
{
	std::string path;
	std::string Name;
	std::unordered_set<std::string>Dep;
};

class DepPublisher {
public:
	std::vector<FileDep> FileAnaly(NoSqlDb<std::string>&db);//get the dependencies.
	void CreateIndex(NoSqlDb<std::string>&db);
private:
	std::unordered_set<std::string> noparents;
	int i = 0;
	std::string collapse(std::string str);
	std::string getCode(std::string path);
	void CreateHTM(FileDep &File);//create the htm file.
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
	
};
#endif