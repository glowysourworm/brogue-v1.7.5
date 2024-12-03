#pragma once

#include <color.h>
#include <simple.h>

namespace brogueHd::model
{
	using namespace simple;

	/// <summary>
	/// Base class for anything on the map that takes just a single tile. This contains just the display 
	///	information for any dungeon object. Also, wants to handle data serialization.
	/// </summary>
	class brogueObject : public simpleObject
	{
	public:
		brogueObject()
		{
		}

	public:
		bool operator==(const brogueObject& other) const
		{
			return character == other.character &&
				foreColor == other.foreColor &&
				backColor == other.backColor &&
				opacity == other.opacity;
		}

		bool operator!=(const brogueObject& other) const
		{
			return character != other.character ||
				foreColor != other.foreColor ||
				backColor != other.backColor ||
				opacity != other.opacity;
		}

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
