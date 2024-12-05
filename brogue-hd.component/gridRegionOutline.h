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

		gridRegionOutline(simplePolygon* exterior, const simpleList<simplePolygon<int>*>& interiorPolygons);
		~gridRegionOutline();

		int getInteriorPolygonCount() const;

		simplePolygon<int>* getExterior() const;
		simplePolygon<int>* getInterior(int index) const;

	private:

		simplePolygon<int>* _exterior;
		simpleList<simplePolygon<int>*>* _interiorPolygons;
	};

	gridRegionOutline::gridRegionOutline(simplePolygon<int>* exterior, const simpleList<simplePolygon<int>*>& interiorPolygons)
	{
		_exterior = exterior;
		_interiorPolygons = new simpleList<simplePolygon<int>*>(interiorPolygons);
	}
	gridRegionOutline::~gridRegionOutline()
	{
		delete _interiorPolygons;
	}
	int gridRegionOutline::getInteriorPolygonCount() const
	{
		return _interiorPolygons->count();
	}
	simplePolygon<int>* gridRegionOutline::getExterior() const
	{
		return _exterior;
	}
	simplePolygon<int>* gridRegionOutline::getInterior(int index) const
	{
		return _interiorPolygons->get(index);
	}
}