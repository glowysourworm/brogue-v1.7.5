#pragma once

#include "color.h"
#include "colorGradient.h"
#include "colorString.h"
#include "gridLocator.h"
#include "gridRect.h"

using namespace brogueHd::backend::model::game;

namespace brogueHd::frontend::ui
{
	enum brogueTextAlignment
	{
		Left = 0,
		Center = 1,
		Right = 2
	};

	struct brogueUIData : hashable
	{
		brogueUIData()
		{
			_text = nullptr;
			_boundary = nullptr;
			_background = nullptr;
			_renderOffset = nullptr;
			_hoverBackground = nullptr;
			_alignment = brogueTextAlignment::Left;
			_hotkeyChar = 'a';
			_hotkeyIndex = -1;
			_padding = 0;

			_hasMouseInteraction = false;
			_isVisible = false;
		};
		brogueUIData(const brogueUIData& copy)
		{
			_text = copy.getText();
			_boundary = copy.getBoundary();
			_background = copy.getBackground();
			_hoverBackground = copy.getHoverBackground();
			_renderOffset = copy.getRenderOffsetPtr();
			_alignment = copy.getAlignment();
			_hotkeyChar = copy.getHotkeyChar();
			_hotkeyIndex = copy.getHotkeyIndex();
			_padding = copy.getPadding();
			_hasMouseInteraction = copy.getHasMouseInteraction();
			_isVisible = copy.getIsVisible();
		}
		~brogueUIData()
		{
			delete _text;
			delete _boundary;
			delete _background;
			delete _hoverBackground;
			delete _renderOffset;
		}
		brogueUIData(const gridRect& boundary)
			: brogueUIData(boundary, colors::transparent(), colors::transparent())
		{}
		brogueUIData(const gridRect& boundary, const color& background)
			: brogueUIData(boundary, background, background)
		{}
		brogueUIData(const gridRect& boundary, const color& background1, const color& background2, brogueGradientType gradientType)
			: brogueUIData(boundary, background1, background2, background1, background2, gradientType)
		{}
		brogueUIData(const gridRect& boundary, const color& background, const color& mouseHoverBackground)
			: brogueUIData(boundary, background, background,  mouseHoverBackground, mouseHoverBackground, brogueGradientType::Horizontal)
		{}
		brogueUIData(const gridRect& boundary, 
					 const color& background1, 
					 const color& background2, 
					 const color& mouseBackground1, 
					 const color& mouseBackground2, 
					 brogueGradientType gradientType)
			: brogueUIData(boundary, "", background1, background2, mouseBackground1, mouseBackground2, gradientType, brogueTextAlignment::Left)
		{}

		brogueUIData(const gridRect& boundary, 
					 const colorString& text, 
					 const color& gradient1, 
					 const color& gradient2, 
					 brogueTextAlignment alignment)
			: brogueUIData(boundary, text, gradient1, gradient2, gradient1, gradient2, brogueGradientType::Horizontal, alignment)
		{}
		brogueUIData(const gridRect& boundary, 
					 const colorString& text, 
					 const color& gradient1, 
					 const color& gradient2, 
					 const color& mouseBackground1,
					 const color& mouseBackground2,
					 brogueGradientType gradientType,
					 brogueTextAlignment alignment)
		{
			_boundary = new gridRect(boundary);
			_text = new colorString(text);
			_background = new colorGradient(gradient1, gradient2, gradientType);
			_hoverBackground = new colorGradient(mouseBackground1, mouseBackground2, gradientType);
			_renderOffset = new gridLocator(0,0);
			_alignment = alignment;
			_hotkeyChar = '\0';
			_hotkeyIndex = -1;
			_padding = 0;
			_hasMouseInteraction = false;
			_isVisible = false;
		}

	public:

		/// <summary>
		/// Sets ancillary parameters (that wouldn't fit nicely into the ctor's)
		/// </summary>
		void setUIParameters(char hotkey, int hotkeyIndex, bool hasMouseInteraction, bool isVisible, int padding)
		{
			_hotkeyChar = hotkey;
			_hotkeyIndex = hotkeyIndex;
			_hasMouseInteraction = hasMouseInteraction;
			_isVisible = isVisible;
			_padding = padding;
		}

		gridRect getBounds() const
		{
			return *_boundary;
		}

		colorString getColorString() const
		{
			return *_text;
		}

	public:

		color calculateGradient(const gridLocator& location, bool isMouseOver)
		{
			return calculateGradient(location.column, location.row, isMouseOver);
		}

		color calculateGradient(int column, int row, bool isMouseOver)
		{
			if (isMouseOver && _hasMouseInteraction)
				return calculateGradientImpl(column, row, _hoverBackground);
			else
				return calculateGradientImpl(column, row, _background);
		}

		color getTextColor(int column, int row)
		{
			int textIndex = calculateTextOffset();
			int offsetColumn = column - _boundary->column;

			if (offsetColumn >= textIndex && offsetColumn - textIndex < _text->getCount())
				return _text->getColor(offsetColumn - textIndex);

			else
				return colors::transparent();
		}

		char getText(int column, int row)
		{
			int textIndex = calculateTextOffset();
			int offsetColumn = column - _boundary->column;

			if (offsetColumn >= textIndex && offsetColumn - textIndex < _text->getCount())
				return _text->getChar(offsetColumn - textIndex);
			else
				return colorString::Empty;
		}

		bool getIsHotkey(int column, int row)
		{
			if (_hotkeyIndex == -1)
				return false;

			int textIndex = calculateTextOffset();
			int offsetColumn = column - _boundary->column;

			return offsetColumn - textIndex == _hotkeyIndex;
		}

		gridRect getPaddedBoundary() const
		{
			return gridRect(_boundary->column + _padding, 
							_boundary->row + _padding, 
							_boundary->width - (2 * _padding),
							_boundary->height - (2 * _padding));
		}

		gridLocator getRenderOffset() const
		{
			return *_renderOffset;
		}

		void setRenderOffset(short column, short row)
		{
			_renderOffset->column = column;
			_renderOffset->row = row;
		}

	private:

		color calculateGradientImpl(int column, int row, colorGradient* gradient)
		{
			short menuColumn = column - _boundary->column;
			short menuRow = row - _boundary->row;

			switch (gradient->gradientType)
			{
			case brogueGradientType::Horizontal:
			{
				if (menuColumn < (_boundary->width / 2.0f))
					return gradient->getColor((_boundary->width - (2 * (menuColumn))) / (float)_boundary->width);
				else
					return gradient->getColor((2 * ((menuColumn)-(_boundary->width / 2.0f))) / (float)_boundary->width);
			}
			break;
			case brogueGradientType::Vertical:
			{
				if (menuRow < (_boundary->height / 2.0f))
					return gradient->getColor((_boundary->height - (2 * (menuRow))) / (float)_boundary->height);
				else
					return gradient->getColor((2 * ((menuRow)-(_boundary->height / 2.0f))) / (float)_boundary->height);
			}
			break;
			case brogueGradientType::Circular:
			{
				// This was made slightly non-linear to match Brogue v1.7.5
				//
				float dx = simpleMath::abs(column - _boundary->centerX()) / ((float)_boundary->width / 2.0f);
				float dy = simpleMath::abs(row - _boundary->centerY()) / ((float)_boundary->height / 2.0f);
				float weight = simpleMath::sqrt((dx * dx) + (dy * dy));

				return gradient->getColor(weight);
			}
			break;
			default:
				simpleException::show("Unhandled brogueGradientType:  brogueUIData.h");
			}
		}

		int calculateTextOffset()
		{
			int textBeginIndex = 0;

			switch (_alignment)
			{
			case brogueTextAlignment::Left:
				break;
			case brogueTextAlignment::Right:
				textBeginIndex = _boundary->right() - _text->getCount() - _boundary->column;
				break;
			case brogueTextAlignment::Center:
				textBeginIndex = ((_boundary->right() - _text->getCount() - _boundary->column) / 2) + 1;
				break;
			default:
				simpleException::show("Unhandled brogueTextAlignment:  brogueUIData.h");
			}

			return textBeginIndex;
		}

	protected:

		colorString* getText() const
		{
			return _text;
		}
		gridRect* getBoundary() const
		{
			return _boundary;
		}
		colorGradient* getBackground() const
		{
			return _background;
		}
		colorGradient* getHoverBackground() const
		{
			return _hoverBackground;
		}
		gridLocator* getRenderOffsetPtr() const
		{
			return _renderOffset;
		}
		brogueTextAlignment getAlignment() const
		{
			return _alignment;
		}
		char getHotkeyChar() const
		{
			return _hotkeyChar;
		}
		int getHotkeyIndex() const
		{
			return _hotkeyIndex;
		}
		int getPadding() const
		{
			return _padding;
		}
		bool getIsVisible() const
		{
			return _isVisible;
		}
		bool getHasMouseInteraction() const
		{
			return _hasMouseInteraction;
		}

	private:

		colorString* _text;
		gridRect* _boundary;
		gridLocator* _renderOffset;
		colorGradient* _background;
		colorGradient* _hoverBackground;
		brogueTextAlignment _alignment;
		char _hotkeyChar;
		int _hotkeyIndex;
		int _padding;

		bool _hasMouseInteraction;
		bool _isVisible;
	};
}