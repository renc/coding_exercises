#include <list> 
#include <unordered_map>
#include <string>

template<typename K, typename V>
class LRUCache {
public:
	typedef std::pair<K, V> ItemType;
	typedef std::list<ItemType > ListType;

	LRUCache(int iMaxCapacity) : capacity(iMaxCapacity) {}

	// head is the LRU item, tail is the most recent used, 
	// if the list is full, remove the head item, insert new item to tail
	void insert(K k, V v) {
		// support the same k without multiple values?
		// or support one k and update k's value?

		if (mapK2Loc.find(k) == mapK2Loc.end())
		{
			// item is not in the cache 
			if (list.size() == capacity) {
				ItemType item = list.front();
				list.pop_front();
				mapK2Loc.erase(item.first);
			}
			list.push_back(ItemType(k, v));
		}
		else {
			// found , to move the item from current location to the tail,
			// erase() or remove()? both invalidate the current item's iterator. 
			ItemType item = list.front();
			list.erase(list.begin());

			item.second = v; // support only one k, and update k's value.
			list.push_back(item); 
		}
		mapK2Loc[k] = --list.end();
	}

	bool find(const K &k)
	{
		if (mapK2Loc.find(k) == mapK2Loc.end())
		{
			// item is not in the cache 
			return false;
		}

		ItemType item = list.front();
		list.erase(list.begin());
		list.push_back(item);
		mapK2Loc[k] = --list.end();

		return true;
	}
private:
	int capacity = 0;
	ListType list;
	std::unordered_map<K, typename ListType::iterator > mapK2Loc;
};
int main()
{
	LRUCache<std::string,int> cache(2);
	cache.insert("k1", 0);
	cache.find("k1");
	cache.insert("k2", 0);
	cache.insert("k3", 0);
	return 0;
}