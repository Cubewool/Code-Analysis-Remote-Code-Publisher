#pragma once

///////////////////////////////////////////////////////////////////
// TypeTable.h - key/value pair in-memory database and defition  //
// functions to store the name/type element to typetable         //
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
* implement the function to store element values
* It uses C++11 constructs,
*
* The elements defined in the companion package They are:
*   show                -show the element of value
*   save                -save the element to the key/value pair
*   value               -get the key/value pair value.
*
*
* Build Process:
* --------------
*   devenv CodeAnalyzerEx.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 8 Mar 2017
* - added test to test function
*/
#ifndef TYPETABLE_H
#define TYPETABLE_H

#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "../Utilities/Utilities.h"
namespace CodeAnalysis {
	struct  Info
	{
		std::string Type;
		std::string Namespace;
		std::string File;
	};

	class Information {
	public:
		using Elem = std::vector<Info>;
		Elem& elem() { return _elem; }
		void show();//show the info
	private:
		Elem _elem;
	};

	class TypeTable {
	public:
		using Name = std::string;
		using Names = std::vector<Name>;

		Names names();//constructor
		bool save(Name name, Info info);//save the element information
		Information value(Name name);//get the value of key
	private:
		using Item = std::pair<Name, Information>;
		std::unordered_map<Name, Information>store;
	};
}
#endif // !TYPETABLE_H
