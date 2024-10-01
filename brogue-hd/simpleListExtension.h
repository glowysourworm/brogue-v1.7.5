#pragma once

#include "simpleList.h"
#include "simpleHash.h"

namespace brogueHd::component
{
	template<typename T>
	class simpleListExtension
	{
	public:

		static void distinctPairs(const simpleList<T>& collection1, const simpleList<T>& collection2, simpleListPairDelegate<T> callback)
		{
			simpleHash<T, simpleHash<T, T>> lookup;

			for (int index1 = 0; index1 < collection1.count(); index1++)
			{
				for (int index2 = 0; index2 < collection2.count(); index2++)
				{
					// Ignore equal items
					if (collection1[index1] == collection2[index2])
						continue;

					// Ignore duplicate pairs (item1, item2)
					if (lookup.contains(collection1[index1]) &&
						lookup[collection1[index1]].contains(collection2[index2]))
						continue;

					// Ignore duplicate pairs (item2, item1)
					if (lookup.contains(collection2[index2]) &&
						lookup[collection2[index2]].contains(collection1[index1]))
						continue;

					else
					{
						// RESULT
						callback(collection1[index1], collection2[index2]);

						// Store lookup 1 -> 2
						if (lookup.contains(collection1[index1]))
							lookup[collection1[index1]].add(collection2[index2], collection2[index2]);

						else
						{
							simpleHash<T, T> hash;
							hash.add(collection2[index2], collection2[index2]);

							lookup.add(collection1[index1], hash);
						}

						// Store lookup 2 -> 1
						if (lookup.contains(collection2[index2]))
							lookup[collection2[index2]].add(collection1[index1], collection1[index1]);

						else
						{
							simpleHash<T, T> hash;
							hash.add(collection1[index1], collection1[index1]);

							lookup.add(collection2[index2], hash);;
						}
					}
				}
			}
		}
	};
}