#pragma once

#include <simple.h>

#include "gridRect.h"
#include "gridLocator.h"

namespace brogueHd::component
{
	using namespace simple;

	/// <summary>
	/// Grid rect component that carries the adjacency of a rectangle from
	/// the rectangle packing - which would be the face of the adjacent rectangle.
	/// </summary>
	class gridRectAdjacency : public gridRect
	{
	public:
		gridRectAdjacency()
		{
			_adjacency = brogueCompass::None;
		}
		gridRectAdjacency(const gridRect& rect, brogueCompass adjacency) : gridRect(rect)
		{
			_adjacency = adjacency;
		}
		gridRectAdjacency(const gridRectAdjacency& copy)
		{
			this->width = copy.width;
			this->height = copy.height;
			this->column = copy.column;
			this->row = copy.row;

			_adjacency = copy.getAdjacency();
		}
		~gridRectAdjacency()
		{
			
		}

		void operator=(const gridRectAdjacency& copy)
		{
			this->width = copy.width;
			this->height = copy.height;
			this->column = copy.column;
			this->row = copy.row;

			_adjacency = copy.getAdjacency();
		}
		bool operator==(const gridRect& other)
		{
			return compare(other);
		}
		bool operator==(const gridRectAdjacency& other)
		{
			return compare(other);
		}
		bool operator!=(const gridRect& other)
		{
			return !compare(other);
		}
		bool operator!=(const gridRectAdjacency& other)
		{
			return !compare(other);
		}
		brogueCompass getAdjacency() const
		{
			return _adjacency;
		}

		size_t getHash() const override
		{
			size_t hash = hashGenerator::generateHash(_adjacency);
			size_t baseHash = gridRect::getHash();
			return hashGenerator::combineHash(hash, baseHash);
		}

	private:

		bool compare(const gridRect& rect)
		{
			return this->width == rect.width &&
				this->height == rect.height &&
				this->column == rect.column &&
				this->row == rect.row;
		}
		bool compare(const gridRectAdjacency& rect)
		{
			return this->width == rect.width &&
				this->height == rect.height &&
				this->column == rect.column &&
				this->row == rect.row &&
				this->getAdjacency() == rect.getAdjacency();
		}

	private:

		brogueCompass _adjacency;
	};
}