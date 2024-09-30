#pragma once

#include "brogueGlobal.h"
#include "color.h"

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model
{
	/// <summary>
	/// Base class for anything on the map that takes just a single tile. This contains just the display 
	///	information for any dungeon object. Also, wants to handle data serialization.
	/// </summary>
	class brogueObject : public hashableObject
	{
	public:

		brogueObject(){}

	public:
		
		size_t getHash() const override 
		{
			return hashGenerator::generateHash(character, opacity);
		}

	public:

		char character;
		color foreColor;
		color backColor;
		char opacity;
	};
}
MAKE_HASHABLE_CLASS(brogueHd::backend::model::brogueObject)