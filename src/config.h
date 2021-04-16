#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <algorithm>

namespace JINFENG{

class ConfigVarBase{
public:
	typedef std::shared_ptr<ConfigVarBase> ptr;
	ConfigVarBase(std::string name, std::string description):m_name(name), m_description(description)
	{
		std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
	}

	virtual ~ConfigVarBase(){}

	const std::string getName(){return m_name;}

	const std::string getDescription(){return m_description;}

	virtual std::string toString()=0;

	virtual bool fromString(const std::string& val) = 0;

	virtual std::string getTypeName() const = 0;

private:
	std::string m_name;
	std::string m_description;
};

template<class F, class T>
class LexicalCast{
public:
	T operator()(const F& val)
	{
		return boost::lexical_cast<T>(val);
	}
};


template<class T>
class LexicalCast<std::string, std::vector<T>>{
public:
	std::vector<T> operator()(std::string& val)
	{
		YAML::Node node = YAML::Load(val);
		std::vector<T> vec;
		std::stringstream ss;
		for(auto begin=node.begin(); begin!=node.end(); begin++)
		{
			ss.str("");
			ss<<*begin;
			vec.push_back(LexicalCast<std::string, T>()(ss.str()));
		}
		return vec;
	}
};

template<class T>
class LexicalCast<std::vector<T>, std::string>{
public:
	std::string operator()(std::vector<T>& vec)
	{
		YAML::Node node;
		for(auto& item: vec)
		{
			node.push_back(YAML::Load(LexicalCast<T, std::string>()(item)));
		}
		std::stringstream ss;
		ss<<node;
		return ss.str();
	}
};

template<class T>
class LexicalCast<std::map<std::string, T>, std::string>{
public:
	std::string operator()(std::map<std::string, T> m)
	{
		YAML::Node node;
		for(auto& item: m)
		{
			node[item.first] = YAML::Load(LexicalCast<T, std::string>()(item.second));
		}
		std::stringstream ss;
		ss<<node;
		return ss.str();
	}
};

template<class T>
class LexicalCast<std::string, std::map<std::string, T>>{
public:
	std::map<std::string, T> operator()(std::string& str)
	{
		YAML::Node node = YAML::Load(str);
		std::map<std::string, T> m;
		std::stringstream ss;
		for(auto begin=node.begin(); begin!=node.end(); begin++)
		{
			ss.str("");
			ss<<begin->second;
			m.insert(std::make_pair(begin->first.Scalar(), LexicalCast<std::string, T>()(ss.str())));
		}
		return m;
	}
};



template<class T, class FromStr=LexicalCast<std::string, T>, class ToStr=LexicalCast<T, std::string>>
class ConfigVar: public ConfigVarBase{
public:
	typedef std::shared_ptr<ConfigVar> ptr;
	typedef std::function<void(const T& oldval, const T& newval)> on_change_cb;

	ConfigVar(std::string name, T val, std::string description=""):ConfigVarBase(name, description), m_val(val)
	{
	}

	std::string toString() override
	{
		try{
			return ToStr()(m_val);
		}
		catch(std::exception& e)
		{
			cout<<e.what()<<endl;
		}
		return "";
	}

	bool fromString(const std::string& val) override
	{
		try{
			m_val = FromStr()(val);
			return true;
		}
		catch(std::exception)
		{
			cout<<e.what()<<endl;
		}
		return false;
	}

	T getValue()
	{
		return m_val;
	}

	void setValue(const T& v)
	{
		if(v==m_val)
			return;
		for(auto& item: m_cbs)
		{
			item.second(m_val, v);
		}

		m_val = v;
	}

	uint64_t addListener(on_change_cb cb)
	{
		static uint64_t s_fun_id = 0;
		++s_fun_id;
		m_cbs[s_fun_id] = cb;
		return s_fun_id;
	}

	void delListener(uint64_t key)
	{
		m_cbs.erase(key);
	}

	on_change_cb getListener(uint64_t key)
	{
		auto it = m_cbs.find(key);
		if(it==m_cbs.end())
			return nullptr;
		return it->second;
	}

	void clearListener()
	{
		m_cbs.clear();
	}



private:
	T m_val;
	std::map<uint64_t, on_change_cb> m_cbs;
};

class Config{
public:
	typedef std::shared_ptr<Config> ptr;

};





};
