#pragma once

#include "simple.h"

namespace brogueHd::frontend
{
	class brogueUIMouseData : hashableObject
	{
	public:

		brogueUIMouseData()
		{
			_mouseUpCapture = false;
			_mouseDownCapture = false;
			_mouseUpCaptureLast = false;
			_mouseDownCaptureLast = false;
			_mouseLastPressed = false;
			_mouseLastOver = false;
			_mousePressed = false;
			_mouseOver = false;
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
		void setUpdate(bool mousePressed, bool mouseOver)
		{
			// Last Cycle
			_mouseLastPressed = _mousePressed;
			_mouseLastOver = _mouseOver;

			// Current Cycle
			_mousePressed = mousePressed;
			_mouseOver = mouseOver;

			// (see getMouseClick())
			_mouseDownCaptureLast = _mouseDownCapture;
			_mouseUpCaptureLast = _mouseUpCapture;

			_mouseDownCapture = mousePressed && mouseOver;
			_mouseUpCapture = !mousePressed && mouseOver;
		}

		void clear()
		{
			_mouseDownCapture = false;
			_mouseUpCapture = false;

			_mouseDownCaptureLast = false;
			_mouseUpCaptureLast = false;

			_mouseOver = false;
			_mousePressed = false;
			_mouseLastPressed = false;
			_mouseLastOver = false;
		}

	public:

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
		bool getMousePressedChanged() const
		{
			return _mousePressed != _mouseLastPressed;
		}
		bool getMouseClick() const
		{
			return _mouseDownCaptureLast && _mouseUpCapture;
		}
		bool getMouseUp() const
		{
			return _mouseUpCapture;
		}
		bool getMouseDown() const
		{
			return _mouseDownCapture;
		}

	protected:

		bool getMousePressedLast() const
		{
			return _mouseLastPressed;
		}
		bool getMouseOverLast() const
		{
			return _mouseLastOver;
		}
		bool getMousePressed() const
		{
			return _mousePressed;
		}
		bool getMouseUpCaptureLast() const
		{
			return _mouseUpCaptureLast;
		}
		bool getMouseDownCaptureLast() const
		{
			return _mouseDownCaptureLast;
		}

	private:

		void copyImpl(const brogueUIMouseData& copy)
		{
			_mouseUpCapture = copy.getMouseUp();
			_mouseDownCapture = copy.getMouseDown();
			_mouseUpCaptureLast = copy.getMouseUpCaptureLast();
			_mouseDownCaptureLast = copy.getMouseDownCaptureLast();
			_mouseLastPressed = copy.getMousePressedLast();
			_mouseLastOver = copy.getMouseOverLast();
			_mousePressed = copy.getMousePressed();
			_mouseOver = copy.getMouseOver();
		}

	private:

		// Some real time data
		bool _mouseDownCapture;
		bool _mouseDownCaptureLast;
		bool _mouseUpCapture;
		bool _mouseUpCaptureLast;
		bool _mouseLastPressed;
		bool _mouseLastOver;
		bool _mousePressed;
		bool _mouseOver;
	};
}