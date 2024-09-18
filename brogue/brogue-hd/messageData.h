#pragma once

#include "broguedef.h"
#include "colorString.h"
#include <string>

using namespace std;

namespace brogueHd
{
	struct messageData
	{
		colorString message;
		bool confirmed;

		messageData(char* amessage, color textColor, bool aconfirmed)
		{
			// Check message length
			brogueAssert(SIZEOF(amessage) <= COLS * 2);

			message = colorString(amessage, textColor);
			confirmed = aconfirmed;
		}

		void update(char* amessage, color atextColor, bool aconfirmed)
		{
			message.updateAll(amessage, atextColor);
			confirmed = aconfirmed;
		}
	};
}