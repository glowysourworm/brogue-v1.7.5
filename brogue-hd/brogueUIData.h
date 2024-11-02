#pragma once
#include "colorString.h"
#include "color.h"
#include "gridLocator.h"

namespace brogueHd::frontend::ui
{
	enum brogueTextAlignment
	{
		Left = 0,
		Center = 1,
		Right = 2
	};

	enum brogueGradientType
	{
		Horizontal = 0,
		Vertical = 1,
		Circular = 2
	};

	struct brogueUIData : hashable
	{
		brogueUIData(){};
		brogueUIData(const brogueUIData& copy)
		{
			text = copy.text;
			gradient1 = copy.gradient1;
			gradient2 = copy.gradient2;
			alignment = copy.alignment;
			padding = copy.padding;
			hotkeyIndex = copy.hotkeyIndex;
			boundary = copy.boundary;
			paddedBoundary = copy.paddedBoundary;
			gradientType = copy.gradientType;
		}
		brogueUIData(const gridRect& aboundary, const color& background, int apadding)
		{
			text = colorString("", colors::white());
			gradient1 = background;
			gradient2 = background;
			alignment = brogueTextAlignment::Left;
			gradientType = brogueGradientType::Horizontal;
			padding = apadding;
			hotkeyIndex = -1;
			boundary = aboundary;
			paddedBoundary = gridRect(aboundary);
		}
		brogueUIData(const gridRect& aboundary, const color& background1, const color& background2, brogueGradientType agradientType, int apadding)
		{
			text = colorString("", colors::white());
			gradient1 = background1;
			gradient2 = background2;
			alignment = brogueTextAlignment::Left;
			gradientType = agradientType;
			padding = apadding;
			hotkeyIndex = -1;
			boundary = aboundary;
			paddedBoundary = gridRect(aboundary);
		}
		brogueUIData(const gridRect& aboundary,  const char* atext, const color& agradient1, const color& agradient2, brogueTextAlignment aalignment)
		{
			text = colorString(atext, colors::white());
			gradient1 = agradient1;
			gradient2 = agradient2;
			alignment = aalignment;
			gradientType = brogueGradientType::Horizontal;
			padding = 0;
			hotkeyIndex = -1;
			boundary = aboundary;
			paddedBoundary = gridRect(aboundary);

		}
		brogueUIData(const gridRect& aboundary, const colorString& atext, const color& abackground, brogueTextAlignment aalignment, brogueGradientType agradientType, int apadding, int ahotkeyIndex)
		{
			text = atext;
			gradient1 = abackground;
			gradient2 = abackground;
			alignment = aalignment;
			gradientType = agradientType;
			padding = apadding;
			hotkeyIndex = ahotkeyIndex;
			boundary = aboundary;
			paddedBoundary = gridRect(aboundary.column + apadding, aboundary.row + apadding, aboundary.width - (2 * apadding), aboundary.height - (2 * apadding));
		}
		brogueUIData(const gridRect& aboundary, const colorString& atext, const color& agradient1, const color& agradient2, brogueTextAlignment aalignment, int apadding, int ahotkeyIndex)
		{
			text = atext;
			gradient1 = agradient1;
			gradient2 = agradient2;
			alignment = aalignment;
			gradientType = brogueGradientType::Horizontal;
			padding = apadding;
			hotkeyIndex = ahotkeyIndex;
			boundary = aboundary;
			paddedBoundary = gridRect(aboundary.column + apadding, aboundary.row + apadding, aboundary.width - (2 * apadding), aboundary.height - (2 * apadding));
		}

		color calculateGradient(const gridLocator& location)
		{
			return calculateGradient(location.column, location.row);
		}

		color calculateGradient(int column, int row)
		{
			short menuColumn = column - boundary.column;
			short menuRow = row - boundary.row;

			switch (gradientType)
			{
			case brogueGradientType::Horizontal:
			{
				if (menuColumn < (boundary.width / 2.0f))
					return color::interpolate(gradient1, gradient2, (boundary.width - (2 * (menuColumn))) / (float)boundary.width);
				else
					return color::interpolate(gradient1, gradient2, (2 * ((menuColumn) - (boundary.width / 2.0f))) / (float)boundary.width);
			}
			break;
			case brogueGradientType::Vertical:
			{
				if (menuRow < (boundary.height / 2.0f))
					return color::interpolate(gradient1, gradient2, (boundary.height - (2 * (menuRow))) / (float)boundary.height);
				else
					return color::interpolate(gradient1, gradient2, (2 * ((menuRow) - (boundary.height / 2.0f))) / (float)boundary.height);
			}
			break;
			case brogueGradientType::Circular:
			{
				// This was made slightly non-linear to match Brogue v1.7.5
				//
				float dx = simpleMath::abs(column - boundary.centerX()) / ((float)boundary.width / 2.0f);
				float dy = simpleMath::abs(row - boundary.centerY()) / ((float)boundary.height / 2.0f);
				float weight = simpleMath::sqrt((dx * dx) + (dy * dy));

				return color::interpolate(gradient1, gradient2, weight);
			}
			break;
			default:
				simpleException::show("Unhandled brogueGradientType:  brogueUIData.h");
			}
		}

		color getTextColor(int column, int row)
		{
			int textIndex = calculateTextOffset();
			int offsetColumn = column - boundary.column;

			if (offsetColumn >= textIndex && offsetColumn - textIndex < text.getCount())
				return text.getColor(offsetColumn - textIndex);

			else
				return colors::transparent();
		}

		char getText(int column, int row)
		{
			int textIndex = calculateTextOffset();
			int offsetColumn = column - boundary.column;

			if (offsetColumn >= textIndex && offsetColumn - textIndex < text.getCount())
				return text.getChar(offsetColumn - textIndex);
			else
				return colorString::Empty;
		}

		bool getIsHotkey(int column, int row)
		{
			if (hotkeyIndex == -1)
				return false;

			int textIndex = calculateTextOffset();
			int offsetColumn = column - boundary.column;

			return offsetColumn - textIndex == hotkeyIndex;
		}

		gridRect boundary;
		gridRect paddedBoundary;
		int padding;
		colorString text;
		color gradient1;
		color gradient2;
		brogueTextAlignment alignment;
		brogueGradientType gradientType;
		int hotkeyIndex;

	private:

		int calculateTextOffset()
		{
			int textBeginIndex = 0;

			switch (alignment)
			{
			case brogueTextAlignment::Left:
				break;
			case brogueTextAlignment::Right:
				textBeginIndex = boundary.right() - text.getCount() - boundary.column;
				break;
			case brogueTextAlignment::Center:
				textBeginIndex = ((boundary.right() - text.getCount() - boundary.column) / 2) + 1;
				break;
			default:
				simpleException::show("Unhandled brogueTextAlignment:  brogueUIData.h");
			}

			return textBeginIndex;
		}
	};
}