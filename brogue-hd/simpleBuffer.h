#pragma once

#include <cstdint>

#include "simpleException.h"

namespace brogueHd::simple
{
	/// <summary>
	/// Class to support a 32-bit word buffer
	/// </summary>
	class simpleBuffer
	{
	public:

		enum bufferByte : int
		{
			byte1 = 1,
			byte2 = 2,
			byte3 = 3,
			byte4 = 4
		};

		const size_t WordSize = 4;

		const uint32_t Int32_Byte1 = 0x000000FF;
		const uint32_t Int32_Byte2 = 0x0000FF00;
		const uint32_t Int32_Byte3 = 0x00FF0000;
		const uint32_t Int32_Byte4 = 0xFF000000;

	public:

		simpleBuffer();
		simpleBuffer(char* buffer, size_t bufferSize, bool checkWordAlignment);
		simpleBuffer(const simpleBuffer& copy);
		~simpleBuffer();

		uint8_t  decode8(int offset, bufferByte byteNumber);
		uint16_t decode16(int offset, bufferByte byteNumber);
		uint16_t decode16(int offset);
		uint32_t decode32(int offset);

		void encode8(int offset, bufferByte byteNumber, uint8_t value);
		void encode16(int offset, bufferByte byteNumber, uint16_t value);
		void encode32(int offset, uint32_t value);

	public:

		char getChar(int index)
		{
			return _buffer[index];
		}
		char* getBuffer() const
		{
			return _buffer;
		}
		size_t getBufferSize() const
		{
			return _bufferSize;
		}
		bool getCheckWordAlignment() const
		{
			return _checkWordAlignment;
		}

	private:

		uint32_t maskAndShift(uint8_t value, bufferByte byteNumber);
		uint32_t maskAndShift(uint16_t value, bufferByte byteNumber);

		uint32_t getWord(int offset);
		uint32_t getMask(bufferByte byteNumber);
		uint8_t getByte(uint32_t word, bufferByte byteNumber);

	private:

		char* _buffer;
		size_t _bufferSize;
		bool _checkWordAlignment;
	};

	simpleBuffer::simpleBuffer()
	{
		_buffer = nullptr;
		_bufferSize = 0;
		_checkWordAlignment = true;
	}
	simpleBuffer::simpleBuffer(char* buffer, size_t bufferSize, bool checkWordAlignment)
	{
		_buffer = buffer;
		_bufferSize = bufferSize;
		_checkWordAlignment = checkWordAlignment;
	}
	simpleBuffer::simpleBuffer(const simpleBuffer& copy)
	{
		_buffer = copy.getBuffer();
		_bufferSize = copy.getBufferSize();
		_checkWordAlignment = copy.getCheckWordAlignment();
	}
	simpleBuffer::~simpleBuffer()
	{

	}

	uint8_t simpleBuffer::decode8(int offset, bufferByte byteNumber)
	{
		if (_checkWordAlignment && offset % this->WordSize != 0)
			throw simpleException("Word alignment incorrect:  simpleBuffer.h");

		return (getWord(offset) & getMask(byteNumber)) >> ((byteNumber - 1) * 8);
	}
	uint16_t simpleBuffer::decode16(int offset, bufferByte byteNumber)
	{
		if (_checkWordAlignment &&
			(offset % this->WordSize != 0))
			throw simpleException("Word alignment incorrect:  simpleBuffer.h");

		uint32_t word = getWord(offset);

		return word & getMask(byteNumber) | word & getMask((bufferByte)(byteNumber + 1));
	}
	uint16_t simpleBuffer::decode16(int offset)
	{
		if (_checkWordAlignment &&
			(offset % this->WordSize != 0))
			throw simpleException("Word alignment incorrect:  simpleBuffer.h");

		return (uint16_t)(_buffer[offset]) |
			(uint16_t)(_buffer[offset + 1] << 8);
	}
	uint32_t simpleBuffer::decode32(int offset)
	{
		if (_checkWordAlignment && (offset % this->WordSize != 0))
			throw simpleException("Word alignment incorrect:  simpleBuffer.h");

		return getWord(offset);
	}

	void simpleBuffer::encode8(int offset, bufferByte byteNumber, uint8_t value)
	{
		if (_checkWordAlignment && offset % this->WordSize != 0)
			throw simpleException("Word alignment incorrect:  simpleBuffer.h");

		_buffer[offset + byteNumber - 1] = value;
	}
	void simpleBuffer::encode16(int offset, bufferByte byteNumber, uint16_t value)
	{
		if (_checkWordAlignment &&
			(offset % this->WordSize != 0))
			throw simpleException("Word alignment incorrect:  simpleBuffer.h");

		uint32_t output = maskAndShift(value, byteNumber);

		_buffer[offset + byteNumber - 1] &= ~getMask(byteNumber) | output;
		_buffer[offset + byteNumber] &= ~getMask((bufferByte)(byteNumber + 1)) | output;
	}
	void simpleBuffer::encode32(int offset, uint32_t value)
	{
		if (_checkWordAlignment && offset % this->WordSize != 0)
			throw simpleException("Word alignment incorrect:  simpleBuffer.h");

		for (int index = 0; index < 4; index++)
		{
			_buffer[offset + index] = getByte(value, (bufferByte)(index + 1));
		}
	}

	uint32_t simpleBuffer::getWord(int offset)
	{
		return (uint32_t)(_buffer[offset]) |
			(uint32_t)(_buffer[offset + 1] << 8) |
			(uint32_t)(_buffer[offset + 2] << 16) |
			(uint32_t)(_buffer[offset + 3] << 24);
	}
	uint32_t simpleBuffer::getMask(bufferByte byteNumber)
	{
		switch (byteNumber)
		{
			case bufferByte::byte1:
				return this->Int32_Byte1;
			case bufferByte::byte2:
				return this->Int32_Byte2;
			case bufferByte::byte3:
				return this->Int32_Byte3;
			case bufferByte::byte4:
				return this->Int32_Byte4;
			default:
				throw simpleException("Unhandled bufferByte type:  simpleBuffer::getMask");
		}
	}
	uint32_t simpleBuffer::maskAndShift(uint8_t value, bufferByte byteNumber)
	{
		switch (byteNumber)
		{
			case bufferByte::byte1:
				return value;
			case bufferByte::byte2:
				return ((uint32_t)value) << 8;
			case bufferByte::byte3:
				return ((uint32_t)value) << 16;
			case bufferByte::byte4:
				return ((uint32_t)value) << 24;
			default:
				throw simpleException("Unhandled bufferByte type:  simpleBuffer::getMask");
		}
	}
	uint32_t simpleBuffer::maskAndShift(uint16_t value, bufferByte byteNumber)
	{
		switch (byteNumber)
		{
			case bufferByte::byte2:
				return value;
			case bufferByte::byte3:
				return ((uint32_t)value) << 8;
			case bufferByte::byte1:
			case bufferByte::byte4:
				throw simpleException("Alignment Exception:  simpleBuffer.h");
				throw;
			default:
				throw simpleException("Unhandled bufferByte type:  simpleBuffer::getMask");
				throw;
		}
	}
	uint8_t simpleBuffer::getByte(uint32_t word, bufferByte byteNumber)
	{
		uint32_t mask = getMask(byteNumber);
		uint32_t result = word & mask;

		switch (byteNumber)
		{
			case bufferByte::byte1:
				return result;
			case bufferByte::byte2:
				return result >> 8;
			case bufferByte::byte3:
				return result >> 16;
			case bufferByte::byte4:
				return result >> 24;
			default:
				throw simpleException("Unhandled bufferByte type:  simpleBuffer::getMask");
		}
	}
}