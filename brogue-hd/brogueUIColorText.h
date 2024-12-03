#pragma once

#include "simple.h"
#include "colorString.h"
#include "brogueUITagAction.h"

namespace brogueHd::frontend
{
	class brogueUIColorText : simpleObject
	{
	public:
		brogueUIColorText(const colorString& text, const brogueUITagAction& tagAction)
		{
			_text = new colorString(text);
			_tagAction = new brogueUITagAction(tagAction);
		}

		~brogueUIColorText()
		{
			delete _text;
			delete _tagAction;
		}

		colorString getText() const
		{
			return *_text;
		}

		brogueUITagAction getAction() const
		{
			return *_tagAction;
		}

		size_t getHash() const override
		{
			return hashGenerator::combineHash(_text->getHash(), _tagAction->getHash());
		}

	private:
		colorString* _text;
		brogueUITagAction* _tagAction;
	};
}
