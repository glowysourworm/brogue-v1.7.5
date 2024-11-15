#pragma once

#include "simple.h"

namespace brogueHd::frontend
{
	class brogueUIMouseData : hashableObject
	{
	public:

		brogueUIMouseData()
		{
			_mouseClickCapture = false;
			_mouseUpCapture = false;
			_mouseDownCapture = false;
			_mouseLastPressed = false;
			_mouseLastOver = false;
			_mousePressed = false;
			_mouseOver = false;
			_needsUpdate = false;
		}
		~brogueUIMouseData()
		{
		}
		brogueUIMouseData(const brogueUIMouseData& copy)
		{
			copyImpl(copy);
		}

	public:

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
			_needsUpdate = _needsUpdate || (((mouseOver && !_mouseOver) || (mousePressed && !_mousePressed)) || forceUpdate);
		}

		bool needsUpdate() const
		{
			return _needsUpdate;
		}
		void forceUpdate()
		{
			_needsUpdate = true;
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

	public:

		bool getMousePressed() const
		{
			return _mousePressed;
		}
		bool getMouseOver() const
		{
			return _mouseOver;
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

	private:

		void copyImpl(const brogueUIMouseData& copy)
		{
			_mouseClickCapture = copy.getMouseClick();
			_mouseUpCapture = copy.getMouseUp();
			_mouseDownCapture = copy.getMouseDown();
			_mouseLastPressed = copy.getMousePressedLast();
			_mouseLastOver = copy.getMouseOverLast();
			_mousePressed = copy.getMousePressed();
			_mouseOver = copy.getMouseOver();
			_needsUpdate = copy.needsUpdate();
		}

	private:

		// Some real time data
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