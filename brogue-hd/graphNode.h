#pragma once

#include "simple.h"

using namespace brogueHd::simple;

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