#pragma once

namespace brogueHd::backend::model
{
	struct linearGenerationParameters
	{
		short minDepth;
		short maxDepth;
		short frequency;
		short minNumberIntercept; // actually intercept * 100
		short minNumberSlope;	  // actually slope * 100
		short maxNumber;
	};
}