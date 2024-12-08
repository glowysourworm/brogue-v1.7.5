#pragma once

#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewPolygonCore.h"
#include "eventController.h"
#include "simpleGraph.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleLine.h"
#include "simpleList.h"
#include "simplePoint.h"

namespace brogueHd::frontend
{
	using namespace simple;
	using namespace brogueHd::component;

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
		void setGraph(simpleGraph<gridLocatorNode<gridLocator>, gridLocatorEdge<gridLocator>>* graph);
	};

	brogueGraphView::brogueGraphView(brogueCoordinateConverter* coordinateConverter,
	                                 eventController* eventController,
	                                 resourceController* resourceController,
	                                 const brogueUIProgramPartId& partId,
	                                 const brogueUIData& uiData)
		: brogueViewPolygonCore(coordinateConverter, eventController, resourceController, partId, uiData)
	{
	}

	brogueGraphView::~brogueGraphView()
	{
	}

	void brogueGraphView::setGraph(simpleGraph<gridLocatorNode<gridLocator>, gridLocatorEdge<gridLocator>>* agraph)
	{
		brogueUIData* uiData = this->getUIData();
		brogueGraphView* that = this;

		agraph->iterate([&that, &uiData](const gridLocatorNode<gridLocator>& node, const simpleList<gridLocatorEdge<gridLocator>>& edges)
		{
			simplePoint<int> vertex(brogueCellDisplay::CellWidth(uiData->getZoomLevel()) * node.column,
			                        brogueCellDisplay::CellHeight(uiData->getZoomLevel()) * node.row);

			for (int index = 0; index < edges.count(); index++)
			{
				gridLocatorNode<gridLocator> adjacentNode = edges.get(index).node1 == node
								                           ? edges.get(index).node2
								                           : edges.get(index).node1;

				simplePoint<int> adjacentVertex(
					brogueCellDisplay::CellWidth(uiData->getZoomLevel()) * adjacentNode.column,
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
