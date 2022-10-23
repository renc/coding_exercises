
#ifndef crbcfg_hpp
#define crbcfg_hpp
 
#include <string>
#include <vector>
#include <unordered_map> 
#include <limits>
#include <memory> 
#include <iostream>

namespace cj
{
	struct TreeNode 
	{
		explicit TreeNode(const std::string& str) : m_nodeName(str) {}
		~TreeNode()
		{
			for (auto child : m_children)
			{
				if (nullptr != child)
				{
					//std::cout << "kill " << child->m_nodeName << "\n"; // for debug
					delete child;
				}
			}
			m_children.clear();
		}
		std::string m_nodeName;
		std::vector<TreeNode*> m_children;
	};

	class CRBCfg 
	{
	public:		
		CRBCfg() : m_rootNode("Root") {}
		void	addParameter(const std::string& key, const std::string& value);
		
		char	getChar(const std::string& key) const;
		long	getLong(const std::string& key) const;
		float	getFloat(const std::string& key) const;
		double	getDouble(const std::string& key) const;
		const std::string&		 getString(const std::string& key) const;
		const std::string&       queryFailString() const;
		std::vector<std::string> getVector(const std::string& key) const;

		TreeNode* getRootNode() { return &m_rootNode; };
		std::vector<std::string> getKey(const std::string& key) const;
		
	private:
		void DFS(const TreeNode* currentNode, const std::string& sKey, std::vector<std::string>& result) const;
		std::unordered_map<std::string, std::string> m_parameters;
		TreeNode m_rootNode;
	};

	bool readCfgFile(const std::string& filename, CRBCfg& cfg);
} // end of namespace cj

#endif // !crbcfg_hpp
