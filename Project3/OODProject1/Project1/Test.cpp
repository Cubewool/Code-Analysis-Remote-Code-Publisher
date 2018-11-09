///////////////////////////////////////////////////////////////////
// Test.cpp - Demonstrate each requirment                        //
//                                                               //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////


#include"../HelpSessionDemo/CppProperties/CppProperties.h"
#include"../HelpSessionDemo/NoSqlDb/NoSqlDb.h"
#include"../Demo/Help.h"
#include"../Query/Query.h"
#include"../HelpSessionDemo/Convert/Convert.h"

#include"Test.h"
#include<vector>
#include<string>

#ifdef Test



int main() {
	std::cout << "\n  Demonstrating Code";
	std::cout << "\n =================================\n";

	using StrData = std::string;
	using intData = int;
	NoSqlDb<StrData> db;
	NoSqlDb<intData> idb;
	Test<StrData>test; //string type database
	Test<intData> itest;//Int type database

	test.Req2(db); itest.Req2_(idb);

	Element<StrData>elem4;//add new element
	test.createElement(db, elem4, "elem4", "test4", "elem4StrData", "the fourth one", {});
	std::cout << "\n  Requirement 3#: \n Adding element " << elem4.name << " to NoSqlDb<string>";
	std::cout << "\n ------------------------------------------\n";
	test.demonstrateElement(db);
	test.Req3(db);

	itest.Req4_(idb); test.Req4(db);

	test.Req5(db); 
	test.Req7(db);
	test.Req8(db);
	test.Req9(db);
	test.Req10("../PackStruc.xml");
	
	system("pause");
	return 0;
}
#endif // Test
