#pragma once

#include "brogueGlobal.h"
#include "colorString.h"
#include <color.h>
#include <simple.h>
#include <simpleException.h>

namespace brogueHd::backend::model
{
	using namespace simple;
	using namespace brogueHd::frontend;

	struct messageData : simpleStruct
	{
	public:
		colorString message;
		bool confirmed;

		messageData()
		{
			message = colorString("No Message", color(1, 1, 1, 1));
			confirmed = false;
		}

		messageData(const char* amessage, color textColor, bool aconfirmed)
		{
			// Check message length
			//brogueAssert(SIZEOF(amessage) <= COLS * 2);

			if (sizeof(amessage) > COLS * 2)
				throw simpleException("Message is outside the bounds of the display:  messageData.h");

			message = colorString(amessage, textColor);
			confirmed = aconfirmed;
		}

		void update(const char* amessage, color atextColor, bool aconfirmed)
		{
			message = colorString(amessage, atextColor);
			confirmed = aconfirmed;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(message, confirmed);
		}
	};
}
