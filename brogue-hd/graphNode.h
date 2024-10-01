#pragma once

#include "brogueGlobal.h"

namespace brogueHd::component
{
	struct graphNode : hashable
	{
	public:
		graphNode(){};

		virtual bool operator==(const graphNode& node) const
		{
			return false;
		}
		virtual bool operator!=(const graphNode& node) const
		{
			return false;
		}
		virtual size_t getHash() const override
		{
			return 0;
		}
	};
}

MAKE_HASHABLE_STRUCT(brogueHd::component::graphNode)