#pragma once

namespace brogueHd::backend::model
{
	struct linearGenerationParameters
	{
		int minDepth;
		int maxDepth;
		int frequency;
		int minNumberIntercept; // actually intercept * 100
		int minNumberSlope;	  // actually slope * 100
		int maxNumber;
	};
}