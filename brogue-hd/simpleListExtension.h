#pragma once

#include "simple.h"
#include "simpleHash.h"
#include "simpleList.h"

namespace brogueHd::component
{
	template<isHashable T>
	class simpleListExtension
	{
	public:

		static void distinctPairs(const simpleList<T>& collection1, const simpleList<T>& collection2, simpleListPairDelegate<T> callback)
		{
			simpleHash<T, simpleHash<T, T>*> lookup;

			for (int index1 = 0; index1 < collection1.count(); index1++)
			{
				for (int index2 = 0; index2 < collection2.count(); index2++)
				{
					T key1 = collection1.get(index1);
					T key2 = collection2.get(index2);

					// Ignore equal items
					if (key1 == key2)
						continue;

					// Ignore duplicate pairs (item1, item2)
					if (lookup.contains(key1) &&
						lookup.get(key1)->contains(key2))
						continue;

					// Ignore duplicate pairs (item2, item1)
					if (lookup.contains(key2) &&
						lookup.get(key2)->contains(key1))
						continue;

					else
					{
						// RESULT
						callback(key1, key2);

						// Store lookup 1 -> 2
						if (lookup.contains(key1))
							lookup.get(key1)->add(key2, key2);

						else
						{
							simpleHash<T, T>* hash = new simpleHash<T, T>();
							hash->add(key2, key2);

							lookup.add(key1, hash);
						}

						// Store lookup 2 -> 1
						if (lookup.contains(key2))
							lookup.get(key2)->add(key1, key1);

						else
						{
							simpleHash<T, T>* hash = new simpleHash<T, T>();
							hash->add(key1, key1);

							lookup.add(key2, hash);
						}
					}
				}
			}

			// Memory Cleanup
			lookup.iterate([] (const T& key, simpleHash<T, T>* value)
			{
				delete value;
				return iterationCallback::iterate;
			});
		}
	};
}