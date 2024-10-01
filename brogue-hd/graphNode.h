#pragma once

#include "brogueGlobal.h"

namespace brogueHd::component
{
	struct graphNode : hashable
	{
		graphNode(){};

		virtual size_t getHash() const override
		{
			return 0;
		}
	};
}

MAKE_HASHABLE_STRUCT(brogueHd::component::graphNode)