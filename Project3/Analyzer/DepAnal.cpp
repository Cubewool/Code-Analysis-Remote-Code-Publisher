///////////////////////////////////////////////////////////////////
// DepTable.cpp - Analysis the file dependency                   //
//                                                               //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////

#include "DepAnal.h"
#include "../Parser/ActionsAndRules.h"
#include"../Tokenizer/Tokenizer.h"
#include"../Utilities/Utilities.h"
#include"../FileSystem/FileSystem.h"
#include"TypeTable.h"
#include"TypeAnal.h"
#include<fstream>
#include<iostream>
using namespace CodeAnalysis;
using namespace Scanner;
using File = FileSystem::Path;
using stringhelper = Utilities::StringHelper;

DepAnal::DepAnal(TypeAnal table) :
	files(table.getFile()),
	_typeTable(table.getType()) {}

void DepAnal::doDepAnal() {
	for (auto file : files) {
		std::ifstream in(file);
		if (!in.good())
		{
			std::cout << "\n  ERROR! can't open " << file << "\n\n";
			return;
		}
		Toker toker_;
		toker_.returnComments(false);
		toker_.attach(&in);

		Element<std::string>elem;
		elem.name = file;
		std::string Namespace;
		std::string get = File::getExt(file);
		
		while (in.good()) // loop until the end of stream
		{
			Namespace = toker_.getTok();
			if (Namespace == "\n") // skip empty line
				continue;

			if (Namespace == "class")
			{
				Namespace = toker_.getTok();
				Class_ = Namespace;
				Namespace = toker_.getTok();
				if (Namespace == "{")
					number++;
				Namespace = toker_.getTok();
			}
			if (number != 0 && Namespace == "{")
				number++;
			if (number != 0 && Namespace == "}")
				number--;
			if (number == 0)
				Class_ = "";
			findDep(Namespace, file, elem); // find dep for each token
		}
		db.save(elem.name, elem);
	}
}

void DepAnal::findDep(std::string tok, std::string file, Element<std::string>&elem) {
	auto temp = _typeTable.value(tok);
	if (temp.elem().size() == 0)
		return;
	for (auto t : temp.elem()) {
		if (t.Type!="alias"&&t.Type!="function"&&file!=t.File) {
			elem.addvect(t.File);
		}
		else if (t.Type == "function"&&file != t.File&&t.Namespace==Class_) {
			elem.addvect(t.File);
		}
	}
}

void DepAnal::showDep() {
	std::cout << "\n the # of files : " << db.count() << "\n";
	std::vector<std::string> keys = db.keys();
	for (auto key : keys)
	{
		stringhelper::title(File::getName(key));
		std::cout << db.value(key).show()<<"\n";
	}
	std::cout << "\n\n";
}

#ifdef TEST_DEPANAL
int main()
{	Util::Title("Testing ");
	putline();
    if (argc < 2){
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}
	for (int i = 1; i<argc; ++i){
		std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
		std::string msg = "Processing file" + fileSpec;
		Util::title(msg);
		putline();
		ConfigParseForCodeAnal configure;
		Parser* pParser = configure.Build();
		try{
			if (pParser){
				if (!configure.Attach(argv[i])){
					std::cout << "\n  could not open file " << fileSpec << std::endl;
					continue;
				}
			}
			else{
				std::cout << "\n\n  Parser not built\n\n";
				return 1;
			}
			while (pParser->next())
				pParser->parse();
			std::cout << "\n";
			Repository* pRepo = Repository::getInstance();
			ASTNode* pGlobalScope = pRepo->getGlobalScope();
			TreeWalk(pGlobalScope);
		}
		catch (std::exception& ex){
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	std::cout << "\n";
}
#endif
