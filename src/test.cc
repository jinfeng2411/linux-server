#include<iostream>
#include"config.h"
using namespace std;
int main()
{
	map<string, int> m = {};
	m["name"] = 11;
	m["age"] = 22;
	string str = JINFENG::LexicalCast<map<string, int>, std::string>()(m);
	YAML::Node node = YAML::Load(str);


	map<string, int> copy = JINFENG::LexicalCast<std::string, map<string, int>>()(str);
	for(auto& item: copy)
	{
		cout<<item.first<<" : "<<item.second<<endl;
	}

}
