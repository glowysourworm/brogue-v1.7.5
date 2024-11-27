#pragma once

#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewPolygonCore.h"
#include "eventController.h"
#include "graph.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleLine.h"
#include "simpleList.h"
#include "simplePoint.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueGraphView : public brogueViewPolygonCore
	{
	public:

		brogueGraphView(brogueCoordinateConverter* coordinateConverter,
						eventController* eventController,
						resourceController* resourceController,
						const brogueUIProgramPartId& partId,
						const brogueUIData& uiData);
		~brogueGraphView();

	public:

		void setGraph(graph<gridLocator, gridLocatorEdge>* graph);

	};

	brogueGraphView::brogueGraphView(brogueCoordinateConverter* coordinateConverter,
									 eventController* eventController,
									 resourceController* resourceController,
									 const brogueUIProgramPartId& partId,
									 const brogueUIData& uiData)
		: brogueViewPolygonCore(coordinateConverter, eventController, resourceController, partId, uiData)
	{}
	brogueGraphView::~brogueGraphView()
	{}

	void brogueGraphView::setGraph(graph<gridLocator, gridLocatorEdge>* agraph)
	{
		brogueUIData* uiData = this->getUIData();
		brogueGraphView* that = this;

		agraph->iterate([&that, &uiData] (const gridLocator& node, const simpleList<gridLocatorEdge>& edges)
		{
			simplePoint<int> vertex(brogueCellDisplay::CellWidth(uiData->getZoomLevel()) * node.column,
									brogueCellDisplay::CellHeight(uiData->getZoomLevel()) * node.row);

			for (int index = 0; index < edges.count(); index++)
			{
				gridLocator adjacentNode = edges.get(index).node1 == node ? edges.get(index).node2 : edges.get(index).node1;

				simplePoint<int> adjacentVertex(brogueCellDisplay::CellWidth(uiData->getZoomLevel()) * adjacentNode.column,
												brogueCellDisplay::CellHeight(uiData->getZoomLevel()) * adjacentNode.row);

				simpleLine<int> edgeUI(vertex, adjacentVertex);

				// Look for existing edge in the graph (tries both point configurations)
				if (!that->containsLine(edgeUI))
				{
					that->addLine(edgeUI);
				}
			}

			return iterationCallback::iterate;
		});
	}
}