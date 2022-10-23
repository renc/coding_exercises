// Please use gcc or g++ to compile.
// Please use version 4.8 or above
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "CRBCfg.hpp"

namespace cj 
{
	// utility function
	void trim_byRef(std::string& str);
	std::vector<std::string> splitString(const std::string& input_string, char delimiter);

	void CRBCfg::addParameter(const std::string& key, const std::string& value)
	{
		m_parameters.emplace(key, value);
	}

	const std::string& CRBCfg::queryFailString() const
	{
		static std::string failFinding{ "FailFinding" };
		return failFinding;
	}
	const std::string& CRBCfg::getString(const std::string& key) const
	{
		const auto cit = m_parameters.find(key);
		if (cit != m_parameters.cend())
		{
			return m_parameters.at(key);
		}
		return queryFailString();
	}
	char CRBCfg::getChar(const std::string& key) const
	{
		// todo: std::string_view ?
		const std::string& value = getString(key);
		if (value == queryFailString())
		{
			return '\0';
		}
		else if (value.size() == 0)
		{
			return '\0';
		}
		return value[0];
	}
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
	std::vector<std::string> CRBCfg::getVector(const std::string& key) const
	{
		auto it = m_parameters.find(key);
		if (it == m_parameters.end())
		{
			return std::vector<std::string>{};
		}

		return splitString(it->second, ',');
	}

	std::vector<std::string> CRBCfg::getKey(const std::string& key) const
	{
		std::vector<std::string> result;
		std::vector<std::string> sections = splitString(key, '.');
		if (sections.size())
		{
			// step 1. find the path according to the accumulated key
			const TreeNode* prevNode = &m_rootNode;
			for (std::size_t iLevel = 0; iLevel < sections.size(); ++iLevel)
			{
				const std::string& sCurrentKey = sections[iLevel];
				bool bFound = false;
				for (std::size_t iNodeIdx = 0; iNodeIdx < prevNode->m_children.size(); ++iNodeIdx)
				{
					if (sCurrentKey == prevNode->m_children[iNodeIdx]->m_nodeName)
					{
						bFound = true;
						prevNode = prevNode->m_children[iNodeIdx];
						break;
					}
				}
				if (false == bFound)
				{
					// fail to find this key in this level, stop
					std::cout << "Error: fail to find value of key=" << key << std::endl;
					return result; // empty;
				}
			}
			// step 2. find all the leaves nodes from here
			std::string sKey; // begin with empty string
			DFS(prevNode, sKey, result);
			
		}
		return result;
	}
	
	void CRBCfg::DFS(const TreeNode* currentNode, const std::string& sKey, std::vector<std::string>& result) const
	{
		if (nullptr != currentNode)
		{
			if (currentNode->m_children.size())
			{
				for (const auto& child : currentNode->m_children)
				{
					DFS(child, sKey.length() ? (sKey + "." + child->m_nodeName) : child->m_nodeName, result);
				}
			}
			else
			{
				result.push_back(sKey);
			}
		}
	}

	bool readCfgFile(const std::string& filename, CRBCfg& cfg)
	{
		std::ostringstream ss; // the content of the whole file

		std::ifstream inFile;
		inFile.open(filename);
		if (inFile.is_open())
		{
			ss << inFile.rdbuf();
			inFile.close();
			// reference: performance compare of diff methods
			// https://github.com/Sqeaky/CppFileToStringExperiments 
			// https://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html  
		}
		else
		{
			std::cout << "Fail to open file.\n";
		}

		// until C++20, std::ostringstream.str() returns const copy.
		// start C++20, std::ostringstream.str() returns const &, no copy.
		const std::string& sContent(ss.str());

		std::int32_t iLevel = 0; // the depth of the block, cout the pair of {}
		std::size_t iKeyBegin{ std::numeric_limits<std::size_t>::max() };// 
		std::vector<std::string> stackKeys; // key0  key1  key2 ...
		std::vector<TreeNode*> stackNodes; // track the current path of a tree, this can be removed(disabled) if no query of middle level key
		stackNodes.push_back(cfg.getRootNode());
		for (std::size_t iPos = 0, iCount = sContent.size(); iPos < iCount; ++iPos)
		{
			char c = sContent[iPos];

			if (std::isspace(static_cast<unsigned char>(c))) 
			{
				// do nothing, ignore the space
			} 
			else
			{
				if ('{' != c && '}' != c)
				{
					if (std::numeric_limits<std::size_t>::max() == iKeyBegin)
					{
						// begin a new key
						iKeyBegin = iPos;
					}
				}
				else if ('{' == c)
				{
					if (std::numeric_limits<std::size_t>::max() == iKeyBegin)
					{
						std::cout << "Error: this block does not have a key.\n";
						++iLevel;
					}
					else
					{
						std::string sKey = sContent.substr(iKeyBegin, iPos - iKeyBegin);
						trim_byRef(sKey);
						stackKeys.push_back(sKey);
						
						TreeNode* node = new TreeNode(sKey);
						stackNodes.back()->m_children.push_back(node);
						stackNodes.push_back(node);

						++iLevel; // go into a depther level 
						iKeyBegin = std::numeric_limits<std::size_t>::max(); // reset, wait to next key or value
					}
				}
				else if ('}' == c)
				{
					if (std::numeric_limits<std::size_t>::max() != iKeyBegin)
					{
						// we reach a leaf
						std::string sLeaf = sContent.substr(iKeyBegin, iPos - iKeyBegin);
						trim_byRef(sLeaf);

						std::string sAccumulatedKey;
						for (std::size_t i = 0; i < stackKeys.size(); ++i)
						{
							sAccumulatedKey += stackKeys[i];
							if (stackKeys.size() - 1 != i)
							{
								sAccumulatedKey += ".";
							}
						}
						cfg.addParameter(sAccumulatedKey, sLeaf);
						iKeyBegin = std::numeric_limits<std::size_t>::max(); // reset
						--iLevel; // change from leaf level to leaf's parent

						if (stackKeys.size()) stackKeys.pop_back();// pop up this leaf's parent 

						TreeNode* node = new TreeNode(sLeaf);
						stackNodes.back()->m_children.push_back(node);
						if (stackNodes.size() > 1) stackNodes.pop_back();
					}
					else
					{
						// we are in the case of prev_}  current_}
						// without key, so all children of this current node has finished
						--iLevel;
						if (stackKeys.size())
						{
							stackKeys.pop_back();// pop up the current node
						}
						if (stackNodes.size() > 1)
						{
							stackNodes.pop_back();
						}
					}
					if (iLevel < 0)
					{
						std::cout << "Error: this format has more } than {\n";
						iLevel = 0; // otherwise the comming blocks cannot be process
					}
				}
			}
		} // end of parsering content
		
		if (0 == iLevel)
		{
			// have left all the brocks 
		}
		else if (iLevel > 0)
		{
			std::cout << "Error: this format has more { than }\n";
		}

		return true;
	}
	 
	void ltrim_byRef(std::string& s) 
	{
		s.erase(
			s.begin(),
			std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c); })
		);
	} 

	void rtrim_byRef(std::string& s) 
	{
		s.erase(
			std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); }).base(),
			s.end()
		);
	} 

	void trim_byRef(std::string& str)
	{
		ltrim_byRef(str);
		rtrim_byRef(str);
	}

	std::vector<std::string> splitString(const std::string& input_string, char delimiter)
	{
		std::vector<std::string> splits;

		size_t i = 0;
		size_t pos = input_string.find(delimiter);

		while (pos != std::string::npos) 
		{
			std::string subValue = input_string.substr(i, pos - i);
			splits.push_back(subValue);

			i = pos + 1;
			pos = input_string.find(delimiter, i);
		}

		splits.push_back(input_string.substr(i, std::min(pos, input_string.length()) - i + 1));

		return splits;
	}
} // end of namespace cj