#pragma once

namespace brogueHd::model
{
	struct linearGenerationParameters
	{
		int minDepth;
		int maxDepth;
		int frequency;
		int minNumberIntercept; // actually intercept * 100
		int minNumberSlope; // actually slope * 100
		int maxNumber;
	};
}
