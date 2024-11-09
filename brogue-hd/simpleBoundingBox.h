#pragma once

namespace brogueHd::frontend
{
	struct simpleBoundingBox
	{
		float x;
		float y;
		float width;
		float height;

		simpleBoundingBox()
		{
			x = 0;
			y = 0;
			width = 0;
			height = 0;
		}
		simpleBoundingBox(float ax, float ay, float awidth, float aheight)
		{
			x = ax;
			y = ay; 
			width = awidth;
			height = aheight;
		}
		simpleBoundingBox(const simpleBoundingBox& copy)
		{
			x = copy.x;
			y = copy.y;
			width = copy.width;
			height = copy.height;
		}
	};
}