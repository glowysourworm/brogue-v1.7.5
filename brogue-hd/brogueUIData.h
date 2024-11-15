#pragma once

#include "brogueUIConstants.h"
#include "brogueUIProgramPartId.h"
#include "brogueUITagAction.h"
#include "color.h"
#include "colorGradient.h"
#include "colorString.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleMath.h"
#include "simpleString.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	struct brogueUIData : hashableObject
	{
		brogueUIData()
		{
			_id = brogueUIProgramPartId();
			_text = default_value::value<colorString>();
			_boundary = default_value::value<gridRect>();
			_background = default_value::value<colorGradient>();
			_hoverBackground = default_value::value<colorGradient>();
			_pressedBackground = default_value::value<colorGradient>();
			_tagAction = default_value::value<brogueUITagAction>();

			_alignment = brogueTextAlignment::Left;
			_hotkeyIndex = -1;
			_zoomLevel = 0;
			_zIndex = 0;

			_hasMouseInteraction = false;
			_mouseClickCapture = false;
			_mouseDownCapture = false;
			_mouseUpCapture = false;
			_mouseLastPressed = false;
			_mouseLastOver = false;
			_mousePressed = false;
			_mouseOver = false;
			_needsUpdate = false;
		};
		brogueUIData(const brogueUIData& copy)
		{
			copyImpl(copy);
		}
		void operator=(const brogueUIData& copy)
		{
			copyImpl(copy);
		}
		~brogueUIData()
		{
		}
		brogueUIData(brogueUIProgramPartId partId, const gridRect& boundary, int zoomLevel)
			: brogueUIData(partId, boundary, zoomLevel, colors::transparent(), colors::transparent())
		{}
		brogueUIData(brogueUIProgramPartId partId, const gridRect& boundary, int zoomLevel, const color& background)
			: brogueUIData(partId, boundary, zoomLevel, background, background)
		{}
		brogueUIData(brogueUIProgramPartId partId, const gridRect& boundary, int zoomLevel, const color& background1, const color& background2, brogueGradientType gradientType)
			: brogueUIData(partId, boundary, zoomLevel, background1, background2, background1, background2, gradientType)
		{}
		brogueUIData(brogueUIProgramPartId partId, const gridRect& boundary, int zoomLevel, const color& background, const color& mouseHoverBackground)
			: brogueUIData(partId, boundary, zoomLevel, background, background, mouseHoverBackground, mouseHoverBackground, brogueGradientType::Horizontal)
		{}
		brogueUIData(brogueUIProgramPartId partId,
					 const gridRect& boundary,
					 int zoomLevel,
					 const color& background1,
					 const color& background2,
					 const color& mouseBackground1,
					 const color& mouseBackground2,
					 brogueGradientType gradientType)
			: brogueUIData(partId, boundary, zoomLevel, "", background1, background2, mouseBackground1, mouseBackground2, mouseBackground1, mouseBackground2, gradientType, brogueTextAlignment::Left)
		{}
		brogueUIData(brogueUIProgramPartId partId,
					 const gridRect& boundary,
					 int zoomLevel,
					 const colorString& text,
					 const color& background,
					 brogueTextAlignment alignment)
			: brogueUIData(partId, boundary, zoomLevel, text, background, background, background, background, background, background, brogueGradientType::Horizontal, alignment)
		{}
		brogueUIData(brogueUIProgramPartId partId,
					 const gridRect& boundary,
					 int zoomLevel,
					 const colorString& text,
					 const color& gradient1,
					 const color& gradient2,
					 brogueTextAlignment alignment)
			: brogueUIData(partId, boundary, zoomLevel, text, gradient1, gradient2, gradient1, gradient2, gradient1, gradient2, brogueGradientType::Horizontal, alignment)
		{}
		brogueUIData(brogueUIProgramPartId partId,
					 const gridRect& boundary,
					 int zoomLevel,
					 const colorString& text,
					 const color& gradient1,
					 const color& gradient2,
					 const color& mouseBackground1,
					 const color& mouseBackground2,
					 const color& mousePressed1,
					 const color& mousePressed2,
					 brogueGradientType gradientType,
					 brogueTextAlignment alignment)
		{
			_id = partId;
			_text = text;
			_boundary = boundary;
			_background = colorGradient(gradient1, gradient2, gradientType);
			_hoverBackground = colorGradient(mouseBackground1, mouseBackground2, gradientType);
			_pressedBackground = colorGradient(mousePressed1, mousePressed2, gradientType);
			_alignment = alignment;
			_hotkeyIndex = -1;
			_zoomLevel = zoomLevel;
			_zIndex = 0;
			_hasMouseInteraction = false;

			// Set during setUIParameters()
			_tagAction = brogueUITagAction();
			_mouseClickCapture = false;
			_mouseDownCapture = false;
			_mouseUpCapture = false;
			_mouseLastPressed = false;
			_mouseLastOver = false;
			_mousePressed = false;
			_mouseOver = false;
			_needsUpdate = false;
		}


	public:

		brogueUIProgramPartId getId() const
		{
			return _id;
		}

		/// <summary>
		/// Sets ancillary parameters (that wouldn't fit nicely into the ctor's)
		/// </summary>
		void setUIParameters(bool hasMouseInteraction,
							 int zoomLevel,
							 int zIndex)
		{
			setUIParameters(brogueUITagAction(brogueUIAction::None, ""), -1, hasMouseInteraction, zoomLevel, zIndex);
		}

		/// <summary>
		/// Sets ancillary parameters (that wouldn't fit nicely into the ctor's)
		/// </summary>
		void setUIParameters(const brogueUITagAction& tagAction,
							 int hotkeyIndex,							 
							 bool hasMouseInteraction,
							 int zoomLevel,
							 int zIndex)
		{
			_hotkeyIndex = hotkeyIndex;
			_hasMouseInteraction = hasMouseInteraction;
			_zoomLevel = zoomLevel;
			_zIndex = zIndex;

			_tagAction.desiredState = tagAction.desiredState;
			_tagAction.fileName = tagAction.fileName;
			_tagAction.action = tagAction.action;
		}

		/// <summary>
		/// Returns true if a mouse click has occurred
		/// </summary>
		void setUpdate(bool mousePressed, bool mouseOver, bool forceUpdate = false)
		{
			// Reset Mouse Capture if mouse-up occurs out-of-bounds
			if (!mousePressed && !mouseOver)
			{
				_mouseDownCapture = false;
				_mouseUpCapture = false;
				_mouseClickCapture = false;
			}

			_mouseDownCapture = _mouseDownCapture || (!_mouseLastPressed && mousePressed && mouseOver);
			_mouseUpCapture = _mouseUpCapture || (_mouseLastPressed && !mousePressed && mouseOver);
			_mouseClickCapture = _mouseClickCapture || (_mouseDownCapture && _mouseUpCapture);

			// Last Cycle
			_mouseLastPressed = _mousePressed;
			_mouseLastOver = _mouseOver;

			// Current Cycle
			_mousePressed = mousePressed;
			_mouseOver = mouseOver;

			// Set Update Flags
			_needsUpdate = _needsUpdate || (_hasMouseInteraction && ((mouseOver && !_mouseOver) || (mousePressed && !_mousePressed)) || forceUpdate);
		}

		bool needsUpdate() const
		{
			return _needsUpdate;
		}
		void forceUpdate()
		{
			_needsUpdate = true;
		}
		bool getMouseLeave() const
		{
			return _mouseLastOver && !_mouseOver;
		}
		bool getMouseEnter() const
		{
			return !_mouseLastOver && _mouseOver;
		}
		bool getMouseClick() const
		{
			return _mouseClickCapture;
		}
		bool getMouseUp() const
		{
			return _mouseUpCapture;
		}
		bool getMouseDown() const
		{
			return _mouseDownCapture;
		}
		bool getMousePressedLast() const
		{
			return _mouseLastPressed;
		}
		bool getMouseOverLast() const
		{
			return _mouseLastOver;
		}
		void clearUpdate()
		{
			_needsUpdate = false;
		}
		void clearCapture()
		{
			_mouseDownCapture = false;
			_mouseUpCapture = false;
			_mouseClickCapture = false;

			_mouseOver = false;
			_mousePressed = false;
			_mouseLastPressed = false;
			_mouseLastOver = false;
		}

		gridRect getBounds() const
		{
			return _boundary;
		}

		colorString getColorString() const
		{
			return _text;
		}

		brogueUITagAction getAction() const
		{
			return _tagAction;
		}

	public:

		color calculateGradient(const gridLocator& location)
		{
			return calculateGradient(location.column, location.row);
		}

		color calculateGradient(int column, int row)
		{
			if (_mouseOver && this->getMouseDown() && _hasMouseInteraction)
				return calculateGradientImpl(column, row, _pressedBackground);

			else if (_mouseOver && _hasMouseInteraction)
				return calculateGradientImpl(column, row, _hoverBackground);

			else
				return calculateGradientImpl(column, row, _background);
		}

		color getTextColor(int column, int row)
		{
			int textIndex = calculateTextOffset();
			int offsetColumn = column - _boundary.column;

			if (offsetColumn >= textIndex && offsetColumn - textIndex < _text.getCount())
				return _text.getColor(offsetColumn - textIndex);

			else
				return colors::transparent();
		}

		char getText(int column, int row)
		{
			int textIndex = calculateTextOffset();
			int offsetColumn = column - _boundary.column;

			if (offsetColumn >= textIndex && offsetColumn - textIndex < _text.getCount())
				return _text.getChar(offsetColumn - textIndex);
			else
				return colorString::Empty;
		}

		colorGradient getBackground() const
		{
			return _background;
		}
		colorGradient getHoverBackground() const
		{
			return _hoverBackground;
		}
		colorGradient getPressedBackground() const
		{
			return _pressedBackground;
		}

		bool getIsHotkey(int column, int row)
		{
			if (_hotkeyIndex == -1)
				return false;

			int textIndex = calculateTextOffset();
			int offsetColumn = column - _boundary.column;

			return offsetColumn - textIndex == _hotkeyIndex;
		}

		int getZIndex() const
		{
			return _zIndex;
		}
		int getZoomLevel() const
		{
			return _zoomLevel;
		}
		bool getHasMouseInteraction() const
		{
			return _hasMouseInteraction;
		}
		bool getMousePressed() const
		{
			return _mousePressed;
		}
		bool getMouseOver() const
		{
			return _mouseOver;
		}

	private:

		void copyImpl(const brogueUIData& copy)
		{
			_id = copy.getId();

			_text = copy.getText();
			_boundary = copy.getBoundary();
			_background = copy.getBackground();
			_hoverBackground = copy.getHoverBackground();
			_pressedBackground = copy.getPressedBackground();
			_tagAction = copy.getAction();

			_alignment = copy.getAlignment();
			_hotkeyIndex = copy.getHotkeyIndex();
			_zoomLevel = copy.getZoomLevel();
			_zIndex = copy.getZIndex();

			_hasMouseInteraction = copy.getHasMouseInteraction();
			_mouseClickCapture = copy.getMouseClick();
			_mouseUpCapture = copy.getMouseUp();
			_mouseDownCapture = copy.getMouseDown();
			_mouseLastPressed = copy.getMousePressedLast();
			_mouseLastOver = copy.getMouseOverLast();
			_mousePressed = copy.getMousePressed();
			_mouseOver = copy.getMouseOver();
			_needsUpdate = copy.needsUpdate();
		}

		color calculateGradientImpl(int column, int row, const colorGradient& gradient)
		{
			short menuColumn = column - _boundary.column;
			short menuRow = row - _boundary.row;

			switch (gradient.gradientType)
			{
				case brogueGradientType::Horizontal:
				{
					if (menuColumn < (_boundary.width / 2.0f))
						return gradient.getColor((_boundary.width - (2 * (menuColumn))) / (float)_boundary.width);
					else
						return gradient.getColor((2 * ((menuColumn)-(_boundary.width / 2.0f))) / (float)_boundary.width);
				}
				break;
				case brogueGradientType::Vertical:
				{
					if (menuRow < (_boundary.height / 2.0f))
						return gradient.getColor((_boundary.height - (2 * (menuRow))) / (float)_boundary.height);
					else
						return gradient.getColor((2 * ((menuRow)-(_boundary.height / 2.0f))) / (float)_boundary.height);
				}
				break;
				case brogueGradientType::Circular:
				{
					// This was made slightly non-linear to match Brogue v1.7.5
					//
					float dx = simpleMath::abs(column - _boundary.centerX()) / ((float)_boundary.width / 2.0f);
					float dy = simpleMath::abs(row - _boundary.centerY()) / ((float)_boundary.height / 2.0f);
					float weight = simpleMath::sqrt((dx * dx) + (dy * dy));

					return gradient.getColor(weight);
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
					textBeginIndex = _boundary.right() - _text.getCount() - _boundary.column;
					break;
				case brogueTextAlignment::Center:
					textBeginIndex = ((_boundary.right() - _text.getCount() - _boundary.column) / 2) + 1;
					break;
				default:
					simpleException::show("Unhandled brogueTextAlignment:  brogueUIData.h");
			}

			return textBeginIndex;
		}

	protected:

		colorString getText() const
		{
			return _text;
		}
		gridRect getBoundary() const
		{
			return _boundary;
		}
		brogueTextAlignment getAlignment() const
		{
			return _alignment;
		}
		int getHotkeyIndex() const
		{
			return _hotkeyIndex;
		}

	private:

		// Adding some program identification data
		brogueUIProgramPartId _id;

		colorString _text;
		gridRect _boundary;
		colorGradient _background;
		colorGradient _hoverBackground;
		colorGradient _pressedBackground;

		brogueTextAlignment _alignment;
		int _hotkeyIndex;
		int _zoomLevel;
		int _zIndex;
		bool _hasMouseInteraction;

		// Some real time data
		brogueUITagAction _tagAction;
		bool _mouseClickCapture;
		bool _mouseDownCapture;
		bool _mouseUpCapture;
		bool _mouseLastPressed;
		bool _mouseLastOver;
		bool _mousePressed;
		bool _mouseOver;
		bool _needsUpdate;
	};
}