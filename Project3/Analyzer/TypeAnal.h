#pragma once
///////////////////////////////////////////////////////////////////
// TypeAnal.h - store the type and global functions to type      //
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
* This package is intended to start type analysis from the AST tree.
* determine the global function and 5 different types, then store them 
* into typetable
* It uses C++11 constructs
*
* The elements defined in the companion package They are:
*   getType           -get the type table
*   getFile           -get the files
*   doTypeAnal        -start the type anaylsis 
*
*
* Build Process:
* --------------
*   devenv CodeAnalyzerEx.sln /debug rebuild
* Dependency :
*--------------
* Typetable.h
*
* Maintenance History:
* --------------------
* ver 1.0 : 8 Mar 2017
* - added isGlobal function to determine the glbal member
* - add search function to compare the global function
*/

#include "../Parser/ActionsAndRules.h"
#include"../Utilities/Utilities.h"
#include"TypeTable.h"
#include <iostream>
#include <functional>
#include<unordered_set>

#pragma warning (disable : 4101)  // disable warning re unused variable x, below

#ifndef  TYPEANAL_H
#define  TYPEANAL_H

namespace CodeAnalysis
{
	using FileList = std::unordered_set<std::string>;
	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;
		using Name = TypeTable::Name;
		using Names = TypeTable::Names;
		TypeTable& getType() {
			return table;
		} 
		FileList& getFile() {
			return files;
		}
		TypeAnal();
		void doTypeAnal();//start type analysis
	private:
		void isGlobal(ASTNode* pNode);//find out the global function without namespace
		bool search(ASTNode *pNode);//bool if the global function is under namespace
		void DFS(ASTNode* pNode);
		bool doDisplay(ASTNode* pNode);
		void toTable(ASTNode *pNode);//put into the table;
		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;
		TypeTable table;//type table
		FileList files;//files
		ASTNode *pNamespace;//store the namespace name
		std::vector<ASTNode*> vec;//store the global node
	};

	inline TypeAnal::TypeAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
		std::function<void()> test = [] { int x; };  // This is here to test detection of lambdas.
	} // It doesn't do anything useful for dep anal.

	inline bool TypeAnal::doDisplay(ASTNode* pNode)
	{
		static std::string toDisplay[] = {
			"function", "lambda", "class", "struct", "enum", "alias", "typedef"
		};
		for (std::string type : toDisplay)
		{
			if (pNode->type_ == type)
				return true;
		}
		return false;
	}

	//determine the global function
	inline void TypeAnal:: isGlobal(ASTNode* pNode) {		
		for (auto node : pNode->children_) {
			if (node->type_ == "class"|| 
				node->type_ == "struct"||
				node->type_ == "typedef"|| 
				node->type_== "enum"||
				node->type_== "alias"||
				(node->type_ == "function"&&node->name_ != "main")) {
				vec.push_back(node);	
			}
		}
	}


	inline void TypeAnal::DFS(ASTNode* pNode)
	{
		static std::string path = "";
		if (pNode->path_ != path)
		{
			std::cout << "\n    -- " << pNode->path_;/*<< "\\" << pNode->package_<<"\n";*/
			path = pNode->path_;
			files.insert(pNode->path_);
		}
		if (pNode->type_ == "namespace"||pNode->type_=="class")
		pNamespace = pNode;//
		if (doDisplay(pNode))
		{	
			toTable(pNode);//
		}
		for (auto pChild : pNode->children_)
			DFS(pChild);
	}

	inline bool TypeAnal::search(ASTNode*pNode) {
		for (unsigned int i = 0; i < vec.size(); i++) {
			if (pNode == vec.at(i))
				return true;
		}
		return false;
	}

	inline void TypeAnal::toTable(ASTNode* pNode) {
		Info information;
		information.File = pNode->path_;
		if (pNode->parentType_ == "namespace")
		{
			if (search(pNode)){
				information.Type = pNode->type_;
				information.Namespace = "Global namespace";
				table.save(pNode->name_, information);
			}
			else if(pNode->name_ != "main")
			{
				information.Type = pNode->type_;
				information.Namespace = pNamespace->name_;
				table.save(pNode->name_, information);
			}	
		}
		else /*if (pNode->type_ != "function")*/ {
			information.Type = pNode->type_;
			information.Namespace = pNamespace->name_;
			table.save(pNode->name_, information);
		}
	}


	inline void TypeAnal::doTypeAnal()
	{
		std::cout << "\n  starting type analysis:\n";
		std::cout << "\n  scanning AST and displaying important things:";
		std::cout << "\n -----------------------------------------------";
		ASTNode* pRoot = ASTref_.root();

    	isGlobal(Repository::getInstance()->getGlobalScope());
		DFS(pRoot);
		//Names names = table.names();
		//for (Name name : names) {
		//	std::cout << "\n " << name << "\n" << "-------------" << "\n";
		//	table.value(name).show();
		//}
	}
}
#endif // 


