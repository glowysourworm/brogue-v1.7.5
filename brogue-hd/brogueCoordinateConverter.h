#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueFlameMenuHeatView.h"
#include "brogueFlameQuad.h"
#include "brogueGlyphMap.h"
#include "brogueImageQuad.h"
#include "brogueUIConstants.h"
#include "color.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "openglQuadConverter.h"
#include "simpleException.h"
#include "simpleGlData.h"

namespace brogueHd::frontend
{
	class brogueCoordinateConverter
	{
	public:

		/// <summary>
		/// Creates an instance of the coordinate converter with the specified parameters quad maps. There should be
		/// one for the glyphs for a view; and one for the view itself.
		/// </summary>
		brogueCoordinateConverter(const openglQuadConverter& glyphConverter, const openglQuadConverter& viewConverter);
		brogueCoordinateConverter(const brogueCoordinateConverter& copy)
		{
			_viewConverter = copy.getViewConverter();
			_glyphConverter = copy.getGlyphConverter();
			_glyphMap = copy.getGlyphMap();
		}
		~brogueCoordinateConverter();

		brogueImageQuad createBrogueImageQuadScene(const brogueCellDisplay& cell, int column, int row);
		brogueCellQuad createBrogueCellQuadScene(const brogueCellDisplay& cell, int column, int row, openglBrogueCellOutputSelector outputSelector);
		brogueColorQuad createBrogueColorQuadScene(const brogueCellDisplay& cell, int column, int row);
		brogueFlameQuad createBrogueFlameQuadScene(const brogueFlameMenuHeatView* view,
													const brogueCellDisplay& cell,
													int column, int row);

		brogueImageQuad createBrogueImageQuadFrame();
		brogueColorQuad createBrogueColorQuadFrame(const color& theColor);

	public:

		openglQuadConverter getViewConverter() const
		{
			return _viewConverter;
		}
		openglQuadConverter getGlyphConverter() const
		{
			return _glyphConverter;
		}
		brogueGlyphMap* getGlyphMap() const
		{
			return _glyphMap;
		}

	private:

		openglQuadConverter _glyphConverter;
		openglQuadConverter _viewConverter;

		brogueGlyphMap* _glyphMap;
	};

	brogueCoordinateConverter::brogueCoordinateConverter(const openglQuadConverter& glyphMap, const openglQuadConverter& viewMap)
	{
		_glyphConverter = glyphMap;
		_viewConverter = viewMap;
		_glyphMap = new brogueGlyphMap();
	}

	brogueCoordinateConverter::~brogueCoordinateConverter()
	{
		delete _glyphMap;
	}

	brogueImageQuad brogueCoordinateConverter::createBrogueImageQuadScene(const brogueCellDisplay& cell, int column, int row)
	{
		simpleQuad quadXY = _viewConverter.createQuadNormalizedXY_FromLocator(column, row);
		simpleQuad quadUV = _viewConverter.createQuadNormalizedUV_FromLocator(column, row);

		return brogueImageQuad(quadXY, quadUV);
	}

	brogueCellQuad brogueCoordinateConverter::createBrogueCellQuadScene(const brogueCellDisplay& cell, int column, int row, openglBrogueCellOutputSelector outputSelector)
	{
		gridLocator glyphLocation = _glyphMap->getGlyphLocation(cell.character);

		simpleQuad quadXY = _viewConverter.createQuadNormalizedXY_FromLocator(column, row);
		simpleQuad quadUV = _viewConverter.createQuadNormalizedUV_FromLocator(column, row);
		simpleQuad glyphUV = _glyphConverter.createQuadNormalizedUV_FromLocator(glyphLocation);

		return brogueCellQuad(cell, quadXY, quadUV, glyphUV, outputSelector);
	}

	brogueColorQuad brogueCoordinateConverter::createBrogueColorQuadScene(const brogueCellDisplay& cell, int column, int row)
	{
		simpleQuad quadXY = _viewConverter.createQuadNormalizedXY_FromLocator(column, row);

		return brogueColorQuad(cell, quadXY);
	}
	brogueFlameQuad brogueCoordinateConverter::createBrogueFlameQuadScene(const brogueFlameMenuHeatView* view, 
																		  const brogueCellDisplay& cell, 
																		  int column, int row)
	{
		// Some of these will be out of bounds; but easily filtered out in the shader
		simpleQuad vertex = _viewConverter.createQuadNormalizedXY_FromLocator(column, row);
		simpleQuad texture = _viewConverter.createQuadNormalizedUV_FromLocator(column, row);

		// HARD-CODED LOGIC: Didn't want to mess up the design for new data streams. This is a quick solution for 
		//					 adding the view here.
		//
		if (view->isTheText(column, row))
			return brogueFlameQuad(view->FlameTitleColor1, view->FlameTitleColor2, view->FlameBottomColor3, vertex, texture);

		else if (row == view->getBoundary().bottom())
			return brogueFlameQuad(view->FlameBottomColor1, view->FlameBottomColor2, view->FlameBottomColor3, vertex, texture);

		else
			return brogueFlameQuad(colors::transparent(), colors::transparent(), colors::transparent(), vertex, texture);
	}
	brogueImageQuad brogueCoordinateConverter::createBrogueImageQuadFrame()
	{
		simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
		simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

		return brogueImageQuad(quadXY, quadUV);
	}

	brogueColorQuad brogueCoordinateConverter::createBrogueColorQuadFrame(const color& theColor)
	{
		simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
		simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

		return brogueColorQuad(theColor, quadXY);
	}
}