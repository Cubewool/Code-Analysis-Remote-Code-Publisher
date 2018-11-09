///////////////////////////////////////////////////////////////////
// DepPublisher.cpp -test the DepPulisher.h                      //
//                                                               //
//                                                               //
// Ver 1.0                                                       //
//                                                               //
// Platform:    Dell XPS 13, Win 10, Visual Studio 2015          //
// Author:      Jingyi Yang, SUID:208607946                      //
//              jyang34@syr.edu                                  //
///////////////////////////////////////////////////////////////////

#include"DepPublisher.h"
#include"../FileSystem/FileSystem.h"
#include<cstring>
#include <sstream>
#include <fstream>
#include <string>
using namespace FileSystem;
std::vector<FileDep> DepPublisher::FileAnaly(NoSqlDb<std::string>&db) {
	Path path;
	std::vector<FileDep> result;
	for (auto key : db.keys()) {
		FileDep myFile;
		myFile.Name = path.getName(key);
		myFile.Dep = db.value(key).children;
		myFile.path = key;
		result.push_back(myFile);

		if (db.value(key).children.getValue().size() == 0)
			noparents.insert(myFile.Name);
	}
	return result;
}

std::string DepPublisher::getCode(std::string path) {
	std::ifstream file(path);
	std::string str, newstr;
	while (std::getline(file, str)) {
		replaceAll(str, "<", "&lt");
		replaceAll(str, ">", "&gt");
		str=collapse(str);
		newstr += str + "\n";
	}
	return newstr;
}

void DepPublisher::replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); 
	}
}

std::string DepPublisher::collapse(std::string str) {
	
		std::string button = "<button onclick=\"myFunction(0)\">-</button>";
		std::string Start = "<div id=\"0\">";
		std::string End = "</div>";
		std::string id = std::to_string(i);
		button=button.replace(28,1, id);
		Start=Start.replace(9, 1,id);
		if (str.find("{") != std::string::npos) {	
			size_t Pos = str.find("{");	
				str.insert(Pos + 1, Start);
				str.insert(0, button);
				i++;
		}		
		if (str.find("}") != std::string::npos) {
				size_t Pos_ = str.find("}");
				str.insert(Pos_, End);		
		}
		return str;
}

void DepPublisher::CreateHTM(FileDep &File) {
	Path path;
	Directory dic;
	std::string str1 = "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"><script type=\"text/javascript\" src=\"Hide.js\"></script></head><body><h3>";
	std::string str2 = File.Name;
	std::string str3 = "</h3><hr /><div class=\"indent\"><h4>Dependencies:</h4>";
	std::string str4;
	for (auto dep : File.Dep) {
		std::string temp = "<a href=\"" + path.getName(dep) + ".htm\">" + path.getName(dep) + "</a><br>";
		std::cout << path.getName(dep) << "\n";
		str4 += temp;
	}
	std::string str5 = "</div><hr / ><pre>";
	std::string _str = getCode(File.path);

	std::string str = str1 + str2 + str3 + str4 + str5 + _str + "</pre></body></html>";
	if (!dic.exists("../Web"))
	{ dic.create("../Web"); }
	std::ofstream out("../Web/" + str2 + ".htm");
	out << str;
	out.close();
	std::ofstream outparent;
	if (!dic.exists("../Web/Noparent"))
		dic.create("../Web/Noparent");
	outparent.open("../Web/Noparent/noparent.txt");
	for (auto noparent:noparents) {
		outparent << noparent;
		outparent << "\n";
	}
	outparent.close(); 
}

void DepPublisher::CreateIndex(NoSqlDb<std::string>&db) {
	Path path;
	Directory dic;
	std::string head= "<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\"></head>";
	head += "<body><h3>Index</h3>";
	head += "<hr /><div class=\"indent\">";
	for (auto file : FileAnaly(db))
	{
		CreateHTM(file);
		head += "<a href=\"" + path.getName(file.path) + ".htm\"" + "target=\"_blank\">" + path.getName(file.path).substr(0, file.path.size() - 4) + "</a><br>";
	}
	head += "</pre></body></html>";
	if (!dic.exists("../Web"))
	{
		dic.create("../Web");
	}
	std::string cur = FileSystem::Directory::getCurrentDirectory();
	std::ofstream out("../Web/Index.htm");
	out << head;
	out.close();
}

#ifdef DEP_PUBLISHER
int main()
{
	NoSqlDb<std::string> db;
	Element<std::string> elem1;
	elem1.name = "../Test/DepAnal.cpp";
	elem1.children.insert(" ../Test/DepAnal.h");
	db.save(elem1.name, elem1);

	Element<std::string> elem2;
	elem2.name = " ../Test/DepAnal.h";
	elem2.children.insert("../Test/DepAnal.cpp");
	db.save(elem2.name, elem2);

	DepPublisher pu;
	for (auto file : pu.FileAnaly(db)) {
		pu.CreateHTM(file);
	}
}
#endif