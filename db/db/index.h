#ifndef INDEX_H_INCLUDED
#define INDEX_H_INCLUDED

#include <list>
#include <string>
#include <set>
#include <string>
#include "util.h"

class BSONObj;
class FilterParser;

class Index {
	public:
		Index() {};
		virtual ~Index();

		Index(const Index& orig);
		BSONObj* key;
		djondb::string documentId;
		long posData;
		long indexPos;
};

class IndexAlgorithm {
	public:
		IndexAlgorithm(std::set<std::string> keys) {
			_keys = keys;
		}

		virtual ~IndexAlgorithm() {};
		virtual void add(const BSONObj& elem, djondb::string documentId, long filePos, long indexPos) = 0;
		virtual Index* find(BSONObj* const elem) = 0;
		virtual void remove(const BSONObj& elem) = 0;
		virtual std::list<Index*> find(FilterParser* parser) = 0;

		const std::set<std::string> keys() const {
			return _keys;
		}

	private:
		std::set<std::string> _keys;
};

#endif // INDEX_H_INCLUDED
