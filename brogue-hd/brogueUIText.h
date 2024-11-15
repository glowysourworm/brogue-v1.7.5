#pragma once
#include "brogueUIConstants.h"
#include "color.h"
#include "colorString.h"
#include "grid.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleString.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueUIText
	{
	public:

		brogueUIText(const gridRect& sceneBoundary, const gridRect& boundary, const simpleString& text);
		brogueUIText(const gridRect& sceneBoundary, const gridRect& boundary, const colorString& text);
		~brogueUIText();
		brogueUIText(const brogueUIText& copy);

		gridRect getBoundary() const;
		color getForeground(short column, short row) const;
		char getCharacter(short column, short row) const;

		void setLine(int row, const colorString& text, brogueTextAlignment alignment);
		void setLine(int row, const simpleString& text, brogueTextAlignment alignment);
		void setLine(int row, const simpleString& text, const color& foreground, const color& hotkeyForeground, int hotkeyIndex, brogueTextAlignment alignment);
		void setText(const colorString& text);
		void setText(const simpleString& text);

		void set(int column, int row, const color& foreground, char character);

	private:

		int calculateTextOffset(brogueTextAlignment alignment, int lineCount);

	private:

		grid<color>* _foregroundGrid;
		grid<char>* _characterGrid;
	};

	brogueUIText::brogueUIText(const gridRect& sceneBoundary, const gridRect& boundary, const colorString& text)
	{
		_foregroundGrid = new grid<color>(sceneBoundary, boundary);
		_characterGrid = new grid<char>(sceneBoundary, boundary);

		setText(text);
	}
	brogueUIText::brogueUIText(const gridRect& sceneBoundary, const gridRect& boundary, const simpleString& text)
	{
		_foregroundGrid = new grid<color>(sceneBoundary, boundary);
		_characterGrid = new grid<char>(sceneBoundary, boundary);

		setText(text);
	}
	brogueUIText::~brogueUIText()
	{
		delete _foregroundGrid;
		delete _characterGrid;
	}
	brogueUIText::brogueUIText(const brogueUIText& copy)
	{
		brogueUIText* that = this;

		_foregroundGrid = new grid<color>(copy.getBoundary(), copy.getBoundary());
		_characterGrid = new grid<char>(copy.getBoundary(), copy.getBoundary());

		copy.getBoundary().iterate([&that, &copy] (short column, short row)
		{
			that->set(column, row, copy.getForeground(column, row), copy.getCharacter(column, row));

			return iterationCallback::iterate;
		});
	}

	int brogueUIText::calculateTextOffset(brogueTextAlignment alignment, int textLength)
	{
		gridRect boundary = this->getBoundary();
		int textBeginIndex = 0;

		switch (alignment)
		{
			case brogueTextAlignment::Left:
				break;
			case brogueTextAlignment::Right:
				textBeginIndex = boundary.right() - textLength - boundary.column;
				break;
			case brogueTextAlignment::Center:
				textBeginIndex = ((boundary.right() - textLength - boundary.column) / 2) + 1;
				break;
			default:
				throw simpleException("Unhandled brogueTextAlignment:  brogueUIText.h");
		}

		return textBeginIndex;
	}
	gridRect brogueUIText::getBoundary() const
	{
		return _characterGrid->getRelativeBoundary();
	}
	color brogueUIText::getForeground(short column, short row) const
	{
		return _foregroundGrid->get(column, row);
	}
	char brogueUIText::getCharacter(short column, short row) const
	{
		return _characterGrid->get(column, row);
	}
	void brogueUIText::set(int column, int row, const color& foreground, char character)
	{
		_foregroundGrid->set(column, row, foreground, true);
		_characterGrid->set(column, row, character, true);
	}
	void brogueUIText::setLine(int row, const colorString& text, brogueTextAlignment alignment)
	{
		if (this->getBoundary().top() > row ||
			this->getBoundary().bottom() < row)
			throw simpleException("Index is outside the bounds of the brogueTextView:  brogueTextView::setLine");

		// TEXT index
		int textOffset = calculateTextOffset(alignment, text.getCount());

		// Iterate the text and set the brogueViewBase grid cell
		for (int index = textOffset; index < text.getCount() + textOffset; index++)
		{
			int columnIndex = this->getBoundary().left() + index;

			_foregroundGrid->set(columnIndex, row, text.getColor(index - textOffset), true);
			_characterGrid->set(columnIndex, row, text.getChar(index - textOffset), true);
		}
	}
	void brogueUIText::setLine(int row, const simpleString& text, brogueTextAlignment alignment)
	{
		colorString colorText(text.c_str(), colors::white());

		setLine(row, colorText, alignment);
	}
	void brogueUIText::setLine(int row, const simpleString& text, const color& foreground, const color& hotkeyForeground, int hotkeyIndex, brogueTextAlignment alignment)
	{
		colorString result(text.c_str(), foreground);

		result.set(hotkeyIndex, text.get(hotkeyIndex), hotkeyForeground);

		setLine(row, result, alignment);
	}
	void brogueUIText::setText(const colorString& text)
	{
		if (this->getBoundary().area() < text.getCount())
			throw simpleException("Not enough room to display the text:  brogueTextView::setText");

		// Empty String
		if (text.getCount() == 0)
			return;

		gridRect boundary = this->getBoundary();
		simpleString paragraph = text.getString();
		simpleArray<simpleString> words = paragraph.split(' ');

		int wordIndex = 0;
		int paragraphIndex = 0;

		for (int rowIndex = boundary.top(); rowIndex <= boundary.bottom() && paragraphIndex < text.getCount(); rowIndex++)
		{
			int wordIndexTemp = wordIndex;
			int wordLength = 0;

			// Count words to fit into the row
			while (wordLength < boundary.width && wordIndexTemp < words.count())
			{
				wordLength += words.get(wordIndexTemp).count();

				if (wordLength < boundary.width)
					wordIndexTemp++;
			}

			// Words found
			if (wordIndexTemp != wordIndex)
			{
				// Add back in spaces from the split
				int stringLength = wordLength + (wordIndexTemp - wordIndex);

				setLine(rowIndex, text.getSubstring(paragraphIndex, stringLength), brogueTextAlignment::Left);

				paragraphIndex += stringLength;
			}

			wordIndex = wordIndexTemp;
		}
	}
	void brogueUIText::setText(const simpleString& text)
	{
		if (this->getBoundary().area() < text.count())
			throw simpleException("Not enough room to display the text:  brogueTextView::setText");

		if (text.count() == 0)
			return;

		setText(colorString(text.c_str(), colors::white()));
	}
}