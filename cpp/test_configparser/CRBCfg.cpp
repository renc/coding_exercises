// Please use gcc or g++ to compile.
// Please use version 4.8 or above
// Please avoid using any external libs or downloaded code
// 
// sample suggested compilation cmds
// /usr/bin/g++ -std=c++11 -c -Wall -Werror -ggdb3  -Iinclude -o bin//obj/CfgTest.o CfgTest.cpp
// /usr/bin/g++ -Wall -Werror -o bin//CfgTest path/CfgTest.o <your class>  -lrt -lm -lstdc++ 

// TODO: include files as needed
#include "CRBCfg.hpp"

namespace cj {

	bool bDebug = false;

	void CRBCfg::addParameter(const std::string& key, const std::string& value)
	{
		m_parameters.emplace(key, value);
	}
	const std::string& CRBCfg::getString(const std::string& key) const
	{
		std::unordered_map<std::string, std::string>::const_iterator cit = m_parameters.find(key);
		if (cit != m_parameters.cend())
		{
			return m_parameters.at(key);
		}
		return m_failFinding;
	}
	char CRBCfg::getChar(const std::string& key) const
	{
		// todo: std::string_view ?
		const std::string& value = getString(key);
		if (value == m_failFinding)
		{
			return ' ';
		}
		else if (value.size() == 0)
		{
			return ' ';
		}
		return value[0];
	}
	// todo: bool getLong(const std::string& key, long &result) const would be better
	long CRBCfg::getLong(const std::string& key) const
	{
		const std::string& value = getString(key);
		long result = 0;
		try
		{
			result = std::stol(value);
		}
		catch (const std::invalid_argument& ex)
		{
			std::cout << "Error: getLong(" << key << "), invalid_argument ex:" << ex.what() << std::endl;
		}
		catch (const std::out_of_range& ex)
		{
			std::cout << "Error: getLong(" << key << "), out_of_range ex:" << ex.what() << "; max=" << std::numeric_limits<long>::max() << std::endl;
		}
		return result;
	}

	float CRBCfg::getFloat(const std::string& key) const
	{
		const std::string& value = getString(key);
		float result = 0;
		try
		{
			result = std::stof(value);
		}
		catch (const std::invalid_argument& ex)
		{
			std::cout << "Error: getFloat(" << key << "), invalid_argument ex:" << ex.what() << std::endl;
		}
		catch (const std::out_of_range& ex)
		{
			std::cout << "Error: getFloat(" << key << "), out_of_range ex:" << ex.what() << "; max=" << std::numeric_limits<float>::max() << std::endl;
		}
		return result;
	}
	double CRBCfg::getDouble(const std::string& key) const
	{
		const std::string& value = getString(key);
		double result = 0;
		try
		{
			result = std::stod(value);
		}
		catch (const std::invalid_argument& ex)
		{
			std::cout << "Error: getDouble(" << key << "), invalid_argument ex:" << ex.what() << std::endl;
		}
		catch (const std::out_of_range& ex)
		{
			std::cout << "Error: getDouble(" << key << "), out_of_range ex:" << ex.what() << "; max=" << std::numeric_limits<double>::max() << std::endl;
		}
		return result;
	}

	void CRBCfg::buildMappingKey()
	{
		// build mapping key
		for (const auto& s : getMarketData().m_serverList)
		{
			const auto& server = s.second;

			std::string key = "CRB.MarketData." + server.m_ServerName + ".Host";
			addParameter(key, server.m_host);

			key = "CRB.MarketData." + server.m_ServerName + ".Port";
			addParameter(key, server.m_port);
		}
	}
	std::vector<std::string> CRBCfg::getVector(const std::string& key) const
	{
		std::vector<std::string> sections = splitString(key, '.');
		if (sections.size() > 0)
		{
			if (sections[0] == "CRB")
			{
				if (sections.size() > 1)
				{
					if (sections[1] == "MarketData")
					{
						if (sections.size() == 2)
						{
							std::vector<std::string> serverList;
							for (const auto& s : getMarketData().m_serverList)
							{
								const auto& server = s.second;
								serverList.push_back(server.toString());
							}
							return serverList;
						}
						else if (sections.size() > 2)
						{
							if (sections[2] == "ServerList")
							{
								std::vector<std::string> serverListNames;
								for (const auto& s : getMarketData().m_serverList)
								{
									const auto& server = s.second;
									serverListNames.push_back(server.m_ServerName);
								}
								return serverListNames;
							}
						}
					}
					else if (sections[1] == "MaybeOtherSection")
					{
					}
				}
			}
		}
		return std::vector<std::string>{};
	}

	enum class CfgLevel {
		Global = 0,
		InsideCRB,
		InsideMarketData,
		InsideServer
	};

	bool readCfgFile(const std::string& filename, CRBCfg& cfg)
	{
		// check file exist or not

		std::ifstream inFile;
		inFile.open(filename);

		if (inFile.is_open())
		{
			CfgLevel cfgLevel = CfgLevel::Global;
			std::string currentServerName;
			std::uint32_t iLineCount = 0;
			while (inFile)
			{
				std::string sLine;
				std::getline(inFile, sLine);
				if (bDebug) std::cout << "line" << iLineCount++ << "=[" << sLine << "]\n";
				sLine = trim(sLine);
				
				if (sLine.empty())
				{
					//std::cout << "Empty line.\n";
				}
				else
				{
					if (cfgLevel == CfgLevel::Global)
					{
						if (sLine.find("CRB") == sLine.npos)
						{
							std::string key, value;
							if (extractLineKeyValue(sLine, key, value))
							{
								cfg.addParameter(key, value);
							}
						}
						else
						{
							cfgLevel = CfgLevel::InsideCRB;
						}
					}
					else if (cfgLevel == CfgLevel::InsideCRB)
					{
						std::string sSectionName;
						if (extractKey(sLine, sSectionName))
						{
							if (sSectionName == "MarketData")
							{
								cfgLevel = CfgLevel::InsideMarketData;
							}
							else if (sSectionName == "OrderEntry")
							{
								// for example, different section
							}
							else if (sLine == "}")
							{
								// do not need this , to end a section ?
								cfgLevel = CfgLevel::Global;
							}
						}
						else if (sLine == "}")
						{
							cfgLevel = CfgLevel::Global;
						}
					}
					else if (cfgLevel == CfgLevel::InsideMarketData)
					{
						if (sLine.find("ServerList") != std::string::npos)
						{
							std::string key, value;
							extractLineKeyValue(sLine, key, value);
							std::vector<std::string> serverNames = splitString(value, ',');
							for (const auto& serverName : serverNames)
							{
								CRBCfg::MarketData::Server server;
								server.m_ServerName = serverName;
								cfg.getMarketData().m_serverList[serverName] = server;
							}
						}
						else if (sLine == "}")
						{
							cfgLevel = CfgLevel::InsideCRB;
						}
						else
						{
							std::string sServerName;
							if (extractKey(sLine, sServerName))
							{
								if (cfg.getMarketData().m_serverList.find(sServerName) != std::cend(cfg.getMarketData().m_serverList))
								{
									cfgLevel = CfgLevel::InsideServer;
									currentServerName = sServerName;
								}
							}
						}
					}
					else if (cfgLevel == CfgLevel::InsideServer)
					{
						if (cfg.getMarketData().m_serverList.find(currentServerName) != std::cend(cfg.getMarketData().m_serverList))
						{
							auto& server = cfg.getMarketData().m_serverList[currentServerName];

							if (sLine.find("Host") != std::string::npos)
							{
								std::string hostKey, hostValue;
								extractLineKeyValue(sLine, hostKey, hostValue);
								server.m_host = hostValue;
							}
							else if (sLine.find("Port") != std::string::npos)
							{
								std::string portKey, portValue;
								extractLineKeyValue(sLine, portKey, portValue);
								server.m_port = portValue;
							}
						}
						if (sLine == "}")
						{
							cfgLevel = CfgLevel::InsideMarketData;
							currentServerName = "";
						}
					}
				}
			}
			if (bDebug) std::cout << "End of file.\n";

			cfg.buildMappingKey(); // finish reading
		}
		else
		{
			std::cout << "Fail to open file.\n";
		}
		return true;
	}

	std::string ltrim(const std::string& str) {
		std::string s(str);

		s.erase(
			s.begin(),
			std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c); })
		);

		return s;
	}
	void ltrim_byRef(std::string& s) {
		s.erase(
			s.begin(),
			std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c); })
		);
	}

	std::string rtrim(const std::string& str) {
		std::string s(str);

		s.erase(
			std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); }).base(),
			s.end()
		);

		return s;
	}

	void rtrim_byRef(std::string& s) {
		s.erase(
			std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); }).base(),
			s.end()
		);
	}

	std::string trim(const std::string& str)
	{
		return rtrim(ltrim(str));
	}

	void trim_byRef(std::string& str)
	{
		ltrim_byRef(str);
		rtrim_byRef(str);
	}

	bool extractKey(const std::string& sLine, std::string& key)
	{
		std::size_t iBegin = sLine.find('{');
		if (iBegin == std::string::npos)
		{
			return false;
		}

		//key = trim(sLine.substr(0, iBegin));
		key = sLine.substr(0, iBegin);
		trim_byRef(key);
		return true;
	}
	
	bool extractLineKeyValue(const std::string& sLine, std::string& key, std::string& value)
	{
		std::size_t iBegin = sLine.find('{');
		std::size_t iEnd = sLine.find('}', iBegin);
		if (iBegin == std::string::npos || iEnd == std::string::npos)
		{
			return false;
		}
		// todo: iEnd should be the end of string ?

		key = trim(sLine.substr(0, iBegin));
		value = trim(sLine.substr(iBegin + 1, iEnd - iBegin - 1));
		return true;
	}
	
	std::vector<std::string> splitString(const std::string& input_string, char delimiter)
	{
		std::vector<std::string> splits;

		size_t i = 0;
		size_t pos = input_string.find(delimiter);

		while (pos != std::string::npos) {
			std::string subValue = input_string.substr(i, pos - i);
			splits.push_back(subValue);

			i = pos + 1;
			pos = input_string.find(delimiter, i);
		}

		splits.push_back(input_string.substr(i, std::min(pos, input_string.length()) - i + 1));

		return splits;
	}
} // end of namespace cj