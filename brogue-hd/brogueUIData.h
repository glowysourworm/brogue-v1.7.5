#pragma once

#include "color.h"
#include "colorGradient.h"
#include "gridRect.h"
#include "simpleException.h"
#include "simpleMath.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueUIData
	{
	public:

		brogueUIData()
		{
			_viewBoundary = new gridRect();
			_viewParentBoundary = new gridRect();
			_background = new colorGradient();
			_hoverBackground = new colorGradient();
			_pressedBackground = new colorGradient();

			_zoomLevel = 0;
			_zIndex = 0;
		};
		brogueUIData(const brogueUIData& copy)
		{
			_viewBoundary = new gridRect();
			_viewParentBoundary = new gridRect();
			_background = new colorGradient();
			_hoverBackground = new colorGradient();
			_pressedBackground = new colorGradient();

			copyImpl(copy);
		}
		void operator=(const brogueUIData& copy)
		{
			copyImpl(copy);
		}
		~brogueUIData()
		{
			delete _viewBoundary;
			delete _viewParentBoundary;
			delete _background;
			delete _hoverBackground;
			delete _pressedBackground;
		}
		brogueUIData(const gridRect& viewBoundary, const gridRect& sceneBoundary, int zoomLevel, const color& background)
			: brogueUIData(viewBoundary, sceneBoundary, zoomLevel, background, background, brogueGradientType::Horizontal)
		{
		}
		brogueUIData(const gridRect& viewBoundary, const gridRect& sceneBoundary, int zoomLevel, const color& background, const color& mouseHoverBackground)
			: brogueUIData(viewBoundary, sceneBoundary, zoomLevel, background, background, mouseHoverBackground, mouseHoverBackground, brogueGradientType::Horizontal)
		{
		}
		brogueUIData(const gridRect& viewBoundary, const gridRect& sceneBoundary, int zoomLevel, const color& background1, const color& background2, brogueGradientType gradientType)
			: brogueUIData(viewBoundary, sceneBoundary, zoomLevel, background1, background2, background1, background2, gradientType)
		{
		}
		brogueUIData(const gridRect& viewBoundary, const gridRect& sceneBoundary, int zoomLevel,
					 const color& background1,
					 const color& background2,
					 const color& mouseBackground1,
					 const color& mouseBackground2,
					 brogueGradientType gradientType)
			: brogueUIData(viewBoundary, sceneBoundary, zoomLevel, background1, background2, mouseBackground1, mouseBackground2, mouseBackground1, mouseBackground2, gradientType)
		{
		}
		brogueUIData(const gridRect& viewBoundary,
					 const gridRect& sceneBoundary,
					 int zoomLevel,
					 const color& gradient1,
					 const color& gradient2,
					 const color& mouseBackground1,
					 const color& mouseBackground2,
					 const color& mousePressed1,
					 const color& mousePressed2,
					 brogueGradientType gradientType)
		{
			_zoomLevel = zoomLevel;
			_zIndex = -1;									// TODO: ZIndex
			_viewBoundary = new gridRect(viewBoundary);
			_viewParentBoundary = new gridRect(sceneBoundary);
			_background = new colorGradient(gradient1, gradient2, gradientType);
			_hoverBackground = new colorGradient(mouseBackground1, mouseBackground2, gradientType);
			_pressedBackground = new colorGradient(mousePressed1, mousePressed2, gradientType);
		}


		int getZIndex() const
		{
			return _zIndex;
		}
		int getZoomLevel() const
		{
			return _zoomLevel;
		}
		gridRect getBoundary() const
		{
			return *_viewBoundary;
		}
		gridRect getParentBoundary() const
		{
			return *_viewParentBoundary;
		}
		colorGradient getBackground() const
		{
			return *_background;
		}
		colorGradient getHoverBackground() const
		{
			return *_hoverBackground;
		}
		colorGradient getPressedBackground() const
		{
			return *_pressedBackground;
		}

	public:

		color calculateGradient(int column, int row, bool hasMouseInteraction, bool mouseOver, bool mouseDown) const
		{
			if (mouseOver && mouseDown && hasMouseInteraction)
				return calculateGradientImpl(column, row, *_pressedBackground);

			else if (mouseOver && hasMouseInteraction)
				return calculateGradientImpl(column, row, *_hoverBackground);

			else
				return calculateGradientImpl(column, row, *_background);
		}

	private:

		color calculateGradientImpl(int column, int row, const colorGradient& gradient) const
		{
			gridRect boundary = this->getBoundary();

			int menuColumn = column - boundary.column;
			int menuRow = row - boundary.row;

			switch (gradient.gradientType)
			{
				case brogueGradientType::Horizontal:
				{
					if (menuColumn < (boundary.width / 2.0f))
						return gradient.getColor((boundary.width - (2 * (menuColumn))) / (float)boundary.width);
					else
						return gradient.getColor((2 * ((menuColumn)-(boundary.width / 2.0f))) / (float)boundary.width);
				}
				break;
				case brogueGradientType::Vertical:
				{
					if (menuRow < (boundary.height / 2.0f))
						return gradient.getColor((boundary.height - (2 * (menuRow))) / (float)boundary.height);
					else
						return gradient.getColor((2 * ((menuRow)-(boundary.height / 2.0f))) / (float)boundary.height);
				}
				break;
				case brogueGradientType::Circular:
				{
					// This was made slightly non-linear to match Brogue v1.7.5
					//
					float dx = simpleMath::abs(column - boundary.centerX()) / ((float)boundary.width / 2.0f);
					float dy = simpleMath::abs(row - boundary.centerY()) / ((float)boundary.height / 2.0f);
					float weight = simpleMath::squareRoot((dx * dx) + (dy * dy));

					return gradient.getColor(weight);
				}
				break;
				default:
					throw simpleException("Unhandled brogueGradientType:  brogueUIData.h");
			}
		}

	private:

		void copyImpl(const brogueUIData& copy)
		{
			_viewBoundary->set(copy.getBoundary());
			_viewParentBoundary->set(copy.getParentBoundary());
			_background->set(copy.getBackground());
			_hoverBackground->set(copy.getHoverBackground());
			_pressedBackground->set(copy.getPressedBackground());
			_zoomLevel = copy.getZoomLevel();
			_zIndex = copy.getZIndex();
		}

	private:

		gridRect* _viewBoundary;
		gridRect* _viewParentBoundary;
		colorGradient* _background;
		colorGradient* _hoverBackground;
		colorGradient* _pressedBackground;
		int _zoomLevel;
		int _zIndex;
	};
}