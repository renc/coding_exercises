
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <limits>
#include <fstream>

namespace cj
{
	class CRBCfg {
	public:
		struct MarketData
		{
			struct Server
			{
				std::string m_ServerName;
				std::string m_host;
				std::string m_port;

				std::string toString() const { return m_ServerName + "=" + m_host + ":" + m_port; }
			};
			std::map<std::string, Server> m_serverList;
		};
		
		void addParameter(const std::string& key, const std::string& value);
		const std::string& getString(const std::string& key) const;
		char getChar(const std::string& key) const;
		// todo: bool getLong(const std::string& key, long &result) const would be better
		long getLong(const std::string& key) const;
		float getFloat(const std::string& key) const;
		double getDouble(const std::string& key) const;

		MarketData& getMarketData() { return m_marketData; }
		const MarketData& getMarketData() const { return m_marketData; }
		
		// after reading the file, please call this func;
		void buildMappingKey();
		std::vector<std::string> getVector(const std::string& key) const;
		std::vector<std::string> getKey(const std::string& key) const { return getVector(key);}

private:
		// One market data block should be enough, it support mult servers
		MarketData m_marketData;
		std::unordered_map<std::string, std::string> m_parameters;
		const std::string m_failFinding{ "FailFinding" };
	};

	std::string trim(const std::string& str);
	bool extractKey(const std::string& sLine, std::string& key);
	bool extractLineKeyValue(const std::string& sLine, std::string& key, std::string& value);
	std::vector<std::string> splitString(const std::string& input_string, char delimiter);

	bool readCfgFile(const std::string& filename, CRBCfg& cfg);

} // end of namespace cj