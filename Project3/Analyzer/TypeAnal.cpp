///////////////////////////////////////////////////////////////////
// TypeAnal.cpp - test the type analysis and put into type       //
// table                                                         //
//                                                               //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////

#include "TypeAnal.h"
using namespace CodeAnalysis;
using namespace Utilities;

#ifdef TEST_TYPEANAL
int main()
{
	AbstrSynTree& ASTref_ = Repository::getInstance()->AST()
		ASTNode* pRoot = ASTref_.root();
	extractGlobalFunctions(Repository::getInstance()->getGlobalScope());
	DFS(pRoot);
	TypeAnal ta;
	ta.doTypeAnal();
}
#endif