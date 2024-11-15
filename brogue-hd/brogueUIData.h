#pragma once

#include "color.h"
#include "colorGradient.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueUIData
	{
	public:

		brogueUIData()
		{
			_background = new colorGradient();
			_hoverBackground = new colorGradient();
			_pressedBackground = new colorGradient();

			_background = new colorGradient(colors::transparent(), colors::transparent(), brogueGradientType::Horizontal);
			_hoverBackground = new colorGradient(colors::transparent(), colors::transparent(), brogueGradientType::Horizontal);
			_pressedBackground = new colorGradient(colors::transparent(), colors::transparent(), brogueGradientType::Horizontal);

			_zoomLevel = 0;
			_zIndex = 0;
		};
		brogueUIData(const brogueUIData& copy)
		{
			_background = new colorGradient();
			_hoverBackground = new colorGradient();
			_pressedBackground = new colorGradient();

			copyImpl(copy);
		}
		void operator=(const brogueUIData& copy)
		{
			_background = new colorGradient();
			_hoverBackground = new colorGradient();
			_pressedBackground = new colorGradient();

			copyImpl(copy);
		}
		~brogueUIData()
		{
			delete _background;
			delete _hoverBackground;
			delete _pressedBackground;
		}
		brogueUIData(int zoomLevel, const color& background)
			: brogueUIData(zoomLevel, background, background, brogueGradientType::Horizontal)
		{}
		brogueUIData(int zoomLevel, const color& background, const color& mouseHoverBackground)
			: brogueUIData(zoomLevel, background, background, mouseHoverBackground, mouseHoverBackground, brogueGradientType::Horizontal)
		{}
		brogueUIData(int zoomLevel, const color& background1, const color& background2, brogueGradientType gradientType)
			: brogueUIData(zoomLevel, background1, background2, background1, background2, gradientType)
		{}
		brogueUIData(int zoomLevel,
					 const color& background1,
					 const color& background2,
					 const color& mouseBackground1,
					 const color& mouseBackground2,
					 brogueGradientType gradientType)
			: brogueUIData(zoomLevel, background1, background2, mouseBackground1, mouseBackground2, mouseBackground1, mouseBackground2, gradientType)
		{}
		brogueUIData(int zoomLevel,
					 const color& gradient1,
					 const color& gradient2,
					 const color& mouseBackground1,
					 const color& mouseBackground2,
					 const color& mousePressed1,
					 const color& mousePressed2,
					 brogueGradientType gradientType)
		{
			_zoomLevel = zoomLevel;
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

	private:

		void copyImpl(const brogueUIData& copy)
		{
			_background->set(copy.getBackground());
			_hoverBackground->set(copy.getHoverBackground());
			_pressedBackground->set(copy.getPressedBackground());
			_zoomLevel = copy.getZoomLevel();
			_zIndex = copy.getZIndex();
		}

	private:

		colorGradient* _background;
		colorGradient* _hoverBackground;
		colorGradient* _pressedBackground;
		int _zoomLevel;
		int _zIndex;
	};
}