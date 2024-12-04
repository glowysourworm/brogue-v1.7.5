#pragma once

#include <simple.h>
#include <simplePolygon.h>
#include <simpleList.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	class gridRegionOutline
	{
	public:

		gridRegionOutline(simplePolygon* exterior, const simpleList<simplePolygon*>& interiorPolygons);
		~gridRegionOutline();

		int getInteriorPolygonCount() const;

		simplePolygon* getExterior() const;
		simplePolygon* getInterior(int index) const;

	private:

		simplePolygon* _exterior;
		simpleList<simplePolygon*>* _interiorPolygons;
	};

	gridRegionOutline::gridRegionOutline(simplePolygon* exterior, const simpleList<simplePolygon*>& interiorPolygons)
	{
		_exterior = exterior;
		_interiorPolygons = new simpleList<simplePolygon*>(interiorPolygons);
	}
	gridRegionOutline::~gridRegionOutline()
	{
		delete _interiorPolygons;
	}
	int gridRegionOutline::getInteriorPolygonCount() const
	{
		return _interiorPolygons->count();
	}
	simplePolygon* gridRegionOutline::getExterior() const
	{
		return _exterior;
	}
	simplePolygon* gridRegionOutline::getInterior(int index) const
	{
		return _interiorPolygons->get(index);
	}
}