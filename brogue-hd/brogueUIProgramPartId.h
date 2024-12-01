#pragma once
#include "brogueUIConstants.h"
#include "simple.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	struct brogueUIProgramPartId : simpleStruct
	{
		brogueUIProgramPartId()
		{
			_name = brogueUIProgram::ContainerControlledProgram;
			_partName = brogueUIProgramPart::Background;
			_partNumber = -1;
		}
		brogueUIProgramPartId(brogueUIProgram name, brogueUIProgramPart partName, int partNumber)
		{
			_name = name;
			_partName = partName;
			_partNumber = partNumber;
		}
		brogueUIProgramPartId(const brogueUIProgramPartId& copy)
		{
			_name = copy.getName();
			_partName = copy.getPartName();
			_partNumber = copy.getPartNumber();
		}
		void operator=(const brogueUIProgramPartId& copy)
		{
			_name = copy.getName();
			_partName = copy.getPartName();
			_partNumber = copy.getPartNumber();
		}
		void set(const brogueUIProgramPartId& copy)
		{
			_name = copy.getName();
			_partName = copy.getPartName();
			_partNumber = copy.getPartNumber();
		}
		bool operator==(const brogueUIProgramPartId& other)
		{
			return compare(other);
		}
		bool operator!=(const brogueUIProgramPartId& other)
		{
			return !compare(other);
		}

		brogueUIProgram getName() const
		{
			return _name;
		}
		brogueUIProgramPart getPartName() const
		{
			return _partName;
		}
		int getPartNumber() const
		{
			return _partNumber;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(_name, _partName, _partNumber);
		}

	private:

		brogueUIProgram _name;
		brogueUIProgramPart _partName;
		int _partNumber;

	private:

		bool compare(const brogueUIProgramPartId& other)
		{
			return _name == other.getName() &&
				_partName == other.getPartName() &&
				_partNumber == other.getPartNumber();
		}

	};
}