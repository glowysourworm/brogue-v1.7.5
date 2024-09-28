#pragma once

#include "broguedef.h"
#include "colorString.h"
#include <string>
#include "exceptionHandler.h"

using namespace std;

namespace brogueHd::backend::model::game
{
	struct messageData
	{
		colorString message;
		bool confirmed;

		messageData()
		{
			message = colorString("No Message", color(255,255,255,0, 0, 0, 0, false));
			confirmed = false;
		}
		messageData(const char* amessage, color textColor, bool aconfirmed)
		{
			// Check message length
			//brogueAssert(SIZEOF(amessage) <= COLS * 2);

			if (SIZEOF(amessage) > COLS * 2)
				brogueException::show("Message is outside the bounds of the display:  messageData.h");

			message = colorString(amessage, textColor);
			confirmed = aconfirmed;
		}

		void update(const char* amessage, color atextColor, bool aconfirmed)
		{
			message.updateAll(amessage, atextColor);
			confirmed = aconfirmed;
		}
	};
}