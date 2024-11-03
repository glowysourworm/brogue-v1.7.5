#pragma once

namespace brogueHd::frontend::opengl
{
	struct simpleMouseState
	{
	public:
		simpleMouseState()
		{
			_x = 0;
			_y = 0;
			_scrolldXPending = 0;
			_scrolldYPending = 0;
			_leftButton = 0;
			_rightButton= 0;
			_middleButton = 0;
		}
		simpleMouseState(const simpleMouseState& copy)
		{
			_x = copy.getX();
			_y = copy.getY();
			_scrolldXPending = copy.getScrolldXPending();
			_scrolldYPending = copy.getScrolldYPending();
			_leftButton = copy.getLeftButton();
			_rightButton = copy.getRightButton();
			_middleButton = copy.getMiddleButton();
		}
		~simpleMouseState(){}

		double getX() const
		{
			return _x;
		}
		double getY() const
		{
			return _y;
		}
		double getScrolldXPending() const
		{
			return _scrolldXPending;
		}
		double getScrolldYPending() const
		{
			return _scrolldYPending;
		}
		int getLeftButton() const
		{
			return _leftButton;
		}
		int getRightButton() const
		{
			return _rightButton;
		}
		int getMiddleButton() const
		{
			return _middleButton;
		}

		/// <summary>
		/// Must acknowledge that scroll delta has been read; and 
		/// it will be reset for the next iteration.
		/// </summary>
		void resetScroll()
		{
			_scrolldXPending = 0;
			_scrolldYPending = 0;
		}

		void updatePosition(double x, double y)
		{
			_x = x;
			_y = y;
		}
		void updateButtons(int left, int right, int middle)
		{
			_leftButton = left;
			_rightButton = right;
			_middleButton = middle;
		}
		void updateScroll(double dx, double dy)
		{
			// Go ahead and drop the pending scroll. It's okay if we miss some frames.
			//
			_scrolldXPending = dx;
			_scrolldYPending = dy;
		}

	private:
		
		double _x;
		double _y;
		int _leftButton;
		int _rightButton;
		int _middleButton;

		double _scrolldXPending;
		double _scrolldYPending;
	};
}