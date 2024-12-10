#pragma once

#include "color.h"
#include "simple.h"

namespace brogueHd::model
{
	using namespace simple;
	using namespace brogueHd::component;

	struct brogueSymbolTemplate : public simpleStruct
	{
	public:

		brogueSymbolTemplate()
		{
			_foreground = colors::transparent();
			_background = colors::transparent();
			_symbolUTF8 = brogueGlyphMap::Empty;
		}
		brogueSymbolTemplate(const brogueSymbolTemplate& copy)
		{
			_foreground = copy.getForeground();
			_background = copy.getBackground();
			_symbolUTF8 = copy.getSymbolUTF8();
		}
		brogueSymbolTemplate(const color& foreground, const color& background, int symbolUTF8)
		{
			_foreground = foreground;
			_background = background;
			_symbolUTF8 = symbolUTF8;
		}
		void operator=(const brogueSymbolTemplate& copy)
		{
			_foreground = copy.getForeground();
			_background = copy.getBackground();
			_symbolUTF8 = copy.getSymbolUTF8();
		}
		bool operator==(const brogueSymbolTemplate& other)
		{
			return compare(other);
		}
		bool operator!=(const brogueSymbolTemplate& other)
		{
			return !compare(other);
		}

		color getForeground() const
		{
			return _foreground;
		}
		color getBackground() const
		{
			return _background;
		}
		int getSymbolUTF8() const
		{
			return _symbolUTF8;
		}

	private:

		bool compare(const brogueSymbolTemplate& other)
		{
			return _foreground == other.getForeground() &&
				_background == other.getBackground() &&
				_symbolUTF8 == other.getSymbolUTF8();
		}

	private:

		color _foreground;
		color _background;
		int _symbolUTF8;

	};
}
