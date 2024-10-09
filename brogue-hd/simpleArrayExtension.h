#pragma once

#include "simpleArray.h"
#include "simpleList.h"

namespace brogueHd::simple
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

		template<typename T, typename TResult>
		static simpleArray<TResult> select(const simpleArray<T>& input, simpleArraySelector<T, TResult> selector)
		{
			int count = input.count();
			simpleArray<TResult> result(count);

			for (int index = 0; index < input.count(); index++)
			{
				result.set(index, selector(input.get(index)));
			}

			return result;
		}
	};
}