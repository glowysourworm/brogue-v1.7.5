#pragma once

#include "simpleArray.h"
#include "simpleList.h"

namespace brogueHd::component
{
	class simpleArrayExtension
	{		
	public:

		template<typename T>
		static simpleArray<T> whereArray(const simpleArray<T>& input, simpleArrayPredicate<T> predicate)
		{
			simpleList<T> result;

			for (int index = 0; index < input.count(); index++)
			{
				if (predicate(input.get(index)))
					result.add(input.get(index));
			}

			return simpleArray<T>(result.toArray());
		}
	};
}