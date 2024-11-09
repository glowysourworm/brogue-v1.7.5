#pragma once

#include "gridLocator.h"
#include "simpleException.h"
#include "simpleHash.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;

namespace brogueHd::frontend
{
	class brogueGlyphMap
	{
	public:

		static const int GlyphSheetColumns = 16;
		static const int GlyphSheetRows = 14;

		/// <summary>
		/// For some reason, the glyph sheets were made with a row offset. Could be missing glyphs.
		/// </summary>
		static const int GlyphSheetRowOffset = 2;

	public:

		static const int Empty = (int)' ';

		static const int FloorUnicode = 0x00b7;
		static const int ChasmUnicode = 0x2237;
		static const int TrapUnicode = 0x25c7;
		static const int FireUnicode = 0x22CF;
		static const int FoliageUnicode = 0x2648;
		static const int StatueUnicode = 0x00df;

		static const int AmuletUnicode = 0x2640;
		static const int ScrollUnicode = 0x266A;
		static const int RingUnicode = 0x26AA;
		static const int CharmUnicode = 0x03DF;
		static const int WeaponUnicode = 0x2191;
		static const int GemUnicode = 0x25cf;
		static const int TotemUnicode = 0x26b2;

		static const int TurretUnicode = 0x25cf;
		static const int UnicornUnicode = 0x00da;
		static const int ElectricCrystalUnicode = 0x00A4;

		static const int UpArrowUnicode = 0x2191;
		static const int DownArrowUnicode = 0x2193;
		static const int LeftArrowUnicode = 0x2190;
		static const int RightArrowUnicode = 0x2192;
		static const int UpTriangleUnicode = 0x2206;
		static const int DownTriangleUnicode = 0x2207;
		static const int OmegaUnicode = 0x03A9;
		static const int ThetaUnicode = 0x03B8;
		static const int LambdaUnicode = 0x03BB;
		static const int KoppaUnicode = 0x03DF;
		static const int LozengeUnicode = 0x29EB;
		static const int CrossProductUnicode = 0x2A2F;
		static const int BadMagicUnicode = 0x29F2;
		static const int GoodMagicUnicode = 0x29F3;

	public:

		brogueGlyphMap();
		~brogueGlyphMap();

		gridLocator getGlyphLocation(int character);

	protected:

		void initialize();

		simpleHash<int, gridLocator>* _glyphLocations;
	};

	brogueGlyphMap::brogueGlyphMap()
	{
		_glyphLocations = new simpleHash<int, gridLocator>();

		initialize();
	}
	brogueGlyphMap::~brogueGlyphMap()
	{
		delete _glyphLocations;
	}

	void brogueGlyphMap::initialize()
	{
		// Glyph Sheet:  UI Coordinates -> left to right, top to bottom. [0, 0], [1, 0], ...

		// Row 1
		_glyphLocations->add(' ', gridLocator(0, 0));
		_glyphLocations->add('!', gridLocator(1, 0));
		_glyphLocations->add('"', gridLocator(2, 0));
		_glyphLocations->add('#', gridLocator(3, 0));
		_glyphLocations->add('$', gridLocator(4, 0));
		_glyphLocations->add('%', gridLocator(5, 0));
		_glyphLocations->add('&', gridLocator(6, 0));
		_glyphLocations->add('\'', gridLocator(7, 0));
		_glyphLocations->add('(', gridLocator(8, 0));
		_glyphLocations->add(')', gridLocator(9, 0));
		_glyphLocations->add('*', gridLocator(10, 0));
		_glyphLocations->add('+', gridLocator(11, 0));
		_glyphLocations->add(',', gridLocator(12, 0));
		_glyphLocations->add('-', gridLocator(13, 0));
		_glyphLocations->add('.', gridLocator(14, 0));
		_glyphLocations->add('/', gridLocator(15, 0));

		// Row 2
		_glyphLocations->add('0', gridLocator(0, 1));
		_glyphLocations->add('1', gridLocator(1, 1));
		_glyphLocations->add('2', gridLocator(2, 1));
		_glyphLocations->add('3', gridLocator(3, 1));
		_glyphLocations->add('4', gridLocator(4, 1));
		_glyphLocations->add('5', gridLocator(5, 1));
		_glyphLocations->add('6', gridLocator(6, 1));
		_glyphLocations->add('7', gridLocator(7, 1));
		_glyphLocations->add('8', gridLocator(8, 1));
		_glyphLocations->add('9', gridLocator(9, 1));
		_glyphLocations->add(':', gridLocator(10, 1));
		_glyphLocations->add(';', gridLocator(11, 1));
		_glyphLocations->add('<', gridLocator(12, 1));
		_glyphLocations->add('=', gridLocator(13, 1));
		_glyphLocations->add('>', gridLocator(14, 1));
		_glyphLocations->add('?', gridLocator(15, 1));

		// Row 3
		_glyphLocations->add('@', gridLocator(0, 2));
		_glyphLocations->add('A', gridLocator(1, 2));
		_glyphLocations->add('B', gridLocator(2, 2));
		_glyphLocations->add('C', gridLocator(3, 2));
		_glyphLocations->add('D', gridLocator(4, 2));
		_glyphLocations->add('E', gridLocator(5, 2));
		_glyphLocations->add('F', gridLocator(6, 2));
		_glyphLocations->add('G', gridLocator(7, 2));
		_glyphLocations->add('H', gridLocator(8, 2));
		_glyphLocations->add('I', gridLocator(9, 2));
		_glyphLocations->add('J', gridLocator(10, 2));
		_glyphLocations->add('K', gridLocator(11, 2));
		_glyphLocations->add('L', gridLocator(12, 2));
		_glyphLocations->add('M', gridLocator(13, 2));
		_glyphLocations->add('N', gridLocator(14, 2));
		_glyphLocations->add('O', gridLocator(15, 2));

		// Row 4
		_glyphLocations->add('P', gridLocator(0, 3));
		_glyphLocations->add('Q', gridLocator(1, 3));
		_glyphLocations->add('R', gridLocator(2, 3));
		_glyphLocations->add('S', gridLocator(3, 3));
		_glyphLocations->add('T', gridLocator(4, 3));
		_glyphLocations->add('U', gridLocator(5, 3));
		_glyphLocations->add('V', gridLocator(6, 3));
		_glyphLocations->add('W', gridLocator(7, 3));
		_glyphLocations->add('X', gridLocator(8, 3));
		_glyphLocations->add('Y', gridLocator(9, 3));
		_glyphLocations->add('Z', gridLocator(10, 3));
		_glyphLocations->add('[', gridLocator(11, 3));
		_glyphLocations->add('\\', gridLocator(12, 3));
		_glyphLocations->add(']', gridLocator(13, 3));
		_glyphLocations->add('^', gridLocator(14, 3));
		_glyphLocations->add('_', gridLocator(15, 3));

		// Row 5
		_glyphLocations->add('`', gridLocator(0, 4));
		_glyphLocations->add('a', gridLocator(1, 4));
		_glyphLocations->add('b', gridLocator(2, 4));
		_glyphLocations->add('c', gridLocator(3, 4));
		_glyphLocations->add('d', gridLocator(4, 4));
		_glyphLocations->add('e', gridLocator(5, 4));
		_glyphLocations->add('f', gridLocator(6, 4));
		_glyphLocations->add('g', gridLocator(7, 4));
		_glyphLocations->add('h', gridLocator(8, 4));
		_glyphLocations->add('i', gridLocator(9, 4));
		_glyphLocations->add('j', gridLocator(10, 4));
		_glyphLocations->add('k', gridLocator(11, 4));
		_glyphLocations->add('l', gridLocator(12, 4));
		_glyphLocations->add('m', gridLocator(13, 4));
		_glyphLocations->add('n', gridLocator(14, 4));
		_glyphLocations->add('o', gridLocator(15, 4));

		// Row 6
		_glyphLocations->add('p', gridLocator(0, 5));
		_glyphLocations->add('q', gridLocator(1, 5));
		_glyphLocations->add('r', gridLocator(2, 5));
		_glyphLocations->add('s', gridLocator(3, 5));
		_glyphLocations->add('t', gridLocator(4, 5));
		_glyphLocations->add('u', gridLocator(5, 5));
		_glyphLocations->add('v', gridLocator(6, 5));
		_glyphLocations->add('w', gridLocator(7, 5));
		_glyphLocations->add('x', gridLocator(8, 5));
		_glyphLocations->add('y', gridLocator(9, 5));
		_glyphLocations->add('z', gridLocator(10, 5));
		_glyphLocations->add('{', gridLocator(11, 5));
		_glyphLocations->add('|', gridLocator(12, 5));
		_glyphLocations->add('}', gridLocator(13, 5));
		_glyphLocations->add('~', gridLocator(14, 5));

		// Row 7 (Brogue v1.7.5 didn't use the entire glyph sheet)
		_glyphLocations->add(FloorUnicode, gridLocator(0, 6));
		_glyphLocations->add(ChasmUnicode, gridLocator(1, 6));
		_glyphLocations->add(TrapUnicode, gridLocator(2, 6));
		_glyphLocations->add(FireUnicode, gridLocator(3, 6));
		_glyphLocations->add(FoliageUnicode, gridLocator(4, 6));
		_glyphLocations->add(AmuletUnicode, gridLocator(5, 6));
		_glyphLocations->add(ScrollUnicode, gridLocator(6, 6));
		_glyphLocations->add(RingUnicode, gridLocator(7, 6));
		_glyphLocations->add(WeaponUnicode, gridLocator(8, 6));
		_glyphLocations->add(GemUnicode, gridLocator(9, 6));
		_glyphLocations->add(TotemUnicode, gridLocator(10, 6));
		//_glyphLocations->add(TurretUnicode, gridLocator(11, 6));		// Duplicate
		_glyphLocations->add(BadMagicUnicode, gridLocator(12, 6));
		_glyphLocations->add(GoodMagicUnicode, gridLocator(13, 6));

		// Row 8 (Brogue v1.7.5 didn't use the entire glyph sheet)
		//_glyphLocations->add(UpArrowUnicode, gridLocator(0, 7));		// Weapon
		_glyphLocations->add(DownArrowUnicode, gridLocator(1, 7));
		_glyphLocations->add(LeftArrowUnicode, gridLocator(2, 7));
		_glyphLocations->add(RightArrowUnicode, gridLocator(3, 7));
		_glyphLocations->add(UpTriangleUnicode, gridLocator(4, 7));
		_glyphLocations->add(DownTriangleUnicode, gridLocator(5, 7));
		_glyphLocations->add(OmegaUnicode, gridLocator(6, 7));
		_glyphLocations->add(ThetaUnicode, gridLocator(7, 7));
		_glyphLocations->add(LambdaUnicode, gridLocator(8, 7));
		_glyphLocations->add(CharmUnicode, gridLocator(9, 7));
		_glyphLocations->add(ElectricCrystalUnicode, gridLocator(10, 7));
		_glyphLocations->add(CrossProductUnicode, gridLocator(11, 7));
	}

	gridLocator brogueGlyphMap::getGlyphLocation(int character)
	{
		if (!_glyphLocations->contains(character))
			simpleException::show("Glyph not found:  brogueGlyph::getGlyphLocation");

		// Forgot the offset while setting these up
		gridLocator location = _glyphLocations->get(character);

		return gridLocator(location.column, location.row + this->GlyphSheetRowOffset);
	}
}