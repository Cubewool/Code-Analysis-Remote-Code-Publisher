///////////////////////////////////////////////////////////////////
// TypeTable.cpp - key/value pair to store type table            //
//                                                               //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////

#include"TypeTable.h"
#include<vector>
using namespace CodeAnalysis;

using Name = TypeTable::Name;
using Names = TypeTable::Names;

void Information::show() {
		std::vector<Info>::iterator iter = elem().begin();
		for (iter; iter != elem().end(); ++iter) {
			std::cout << "type      : " << iter->Type << "\n";
			std::cout << "namespace : " << iter->Namespace << "\n";
			std::cout << "file      : " << iter->File << "\n";
		}
}

TypeTable::Names TypeTable::names() {
	Names names;
	for (Item item : store) {
		names.push_back(item.first);
	}
	return names;
}

bool TypeTable::save(Name name, Info info) {
	if (store.find(name) == store.end())
	{
		Information elem;
		elem.elem().push_back(info);
		store[name] = elem;
		return true;

	}
	else if (store.find(name) != store.end())
	{
		Information elem;
		elem = value(name);
		elem.elem().push_back(info);
		store[name] = elem;
		return true;
	}
	return false;
}

Information TypeTable::value(Name name) {
	if (store.find(name) != store.end())
		return store[name];
	return Information();
}

#ifdef TEST_TYPETABLE
int main() {
	TypeTable table;
	Element myelem;

	Info information1;
	information1.File = "A.h";
	information1.Name = "A";
	information1.Namespace = "CodeAnalysis";

	myelem.elem().push_back(information1);

	Info information2;
	information2.File = "B.h";
	information2.Name = "B";
	information2.Namespace = "CodeAnalysis";
	myelem.elem().push_back(information2);

	table.save("class", myelem);

	Types types = table.types();
	for (Type type : types)
	{
		std::cout << "\n " << type << "\n" << "--------" << "\n";
		table.value(type).show();
	}

	std::cout << "\n\n";
	system("pause");
	return 0;
}
#endif // TEST_TYPETABLE
