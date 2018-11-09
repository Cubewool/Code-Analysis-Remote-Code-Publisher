///////////////////////////////////////////////////////////////////
// Query.cpp - include test code to implement the queries        //
//                                                               //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////
#include"../HelpSessionDemo/CppProperties/CppProperties.h"
#include"../HelpSessionDemo/NoSqlDb/NoSqlDb.h"
#include"../HelpSessionDemo/Convert/Convert.h"
#include"Query.h"

#include<vector>
#include<string>
#include<algorithm>
#include<set>
#include<iterator>


#ifdef QUERY
int main() {
	using StrData = std::string;
	using intData = int;
	NoSqlDb<StrData> db;
	Element<StrData> elem1;
	elem1.name = "elem1";
	elem1.category = "11111";
	elem1.data = "elem1's StrData";
	elem1.description = "the first one";
	db.save(elem1.name, elem1);
	Element<StrData> elem2;
	elem2.name = "elem2";
	elem2.category = "test2";
	elem2.data = "elem2's StrData";
	elem2.description = "the second one";
	db.save(elem2.name, elem2);
	Query<StrData> query;
	std::cout << "the query 1"<<"\n";
	std::cout << "----------------------------";
	std::cout << query.showValue(db, elem1.name).show(); 
	std::cout << "\n" << "the query 4: Name" << "\n";
	std::cout << "----------------------------";
	std::vector<std::string>vec4;
	vec4 = query.showKey(db, "name", "elem");
	for (unsigned int i = 0; i < vec4.size(); i++) {
		std::cout << vec4.at(i)+" ";
	}
	std::cout << "\n";
	std::cout << "\n" << "the query 5: category" << "\n";
	std::cout << "----------------------------";
	std::vector<std::string>vec5;
	vec5 = query.showKey(db, "category", "test");
	for (unsigned int i = 0; i < vec5.size(); i++) {
		std::cout << vec5.at(i) + " ";	}
	std::cout << "\n";
	std::cout << "\n" << "the query 6: data" << "\n";
	std::cout << "----------------------------";
	std::cout << "\n"<< "show the string:" ;
	std::vector<std::string>vec6;
	vec6 = query.showKey(db, "data", "elem1's StrData");
	for (unsigned int i = 0; i < vec6.size(); i++) {
		std::cout << vec6.at(i) + " ";
	}
	std::cout << "\n";
	return 0;
}
#endif // QUERY