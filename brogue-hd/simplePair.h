#pragma once

#include "simple.h"

namespace brogueHd::simple
{
	template<isHashable K, typename V>
	struct simplePair : simpleStruct
	{
		simplePair(const K& key, const V& value)
		{
			_key = key;
			_value = value;
		}
		simplePair(const simplePair& pair)
		{
			_key = pair.getKey();
			_value = pair.getValue();
		}
		void set(const K& key, const V& value)
		{
			_key = key;
			_value = value;
		}
		void setValue(const V& value)
		{
			_value = value;
		}
		void operator=(const simplePair& pair)
		{
			_key = pair.getKey();
			_value = pair.getValue();
		}
		bool operator==(const simplePair& pair) const
		{
			return pair.getKey() == _key && pair.getValue() == _value;
		}
		bool operator!=(const simplePair& pair) const
		{
			return pair.getKey() != _key || pair.getValue() != _value;
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(_key);
		}

		K getKey() const
		{
			return _key;
		}
		V getValue() const
		{
			return _value;
		}

	private:

		K _key;
		V _value;
	};
}