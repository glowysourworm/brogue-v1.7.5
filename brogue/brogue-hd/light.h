#pragma once

namespace brogueHd::backend::model::lighting
{
	struct light {

		short red;
		short green;
		short blue;

		light() {}
		light(short ared, short agreen, short ablue)
		{
			red = ared;
			green = agreen;
			blue = ablue;
		}
		bool compare(const light& light)
		{
			return red == light.red && blue == light.blue && green == light.green;
		}

	};
}