#pragma once

#include "simple.h"
#include "simpleException.h"
#include "colorString.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model::game
{
	struct messageData : hashable
	{
	public:

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

			if (sizeof(amessage) > COLS * 2)
				simpleException::showCstr("Message is outside the bounds of the display:  messageData.h");

			message = colorString(amessage, textColor);
			confirmed = aconfirmed;
		}

		void update(const char* amessage, color atextColor, bool aconfirmed)
		{
			message.updateAll(amessage, atextColor);
			confirmed = aconfirmed;
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(message, confirmed);
		}
	};
}