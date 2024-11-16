#pragma once
#include <chrono>

namespace brogueHd::simple
{
	class simpleTimer
	{
	private:

		using clock = std::chrono::time_point<std::chrono::system_clock>;

	public:

		void mark()
		{
			_last = std::chrono::system_clock::now();
		}

		int markMilliseconds()
		{
			clock end = std::chrono::system_clock::now();

			// Try the vendor -> stopwatch.hpp file to decipher this namespace.. or probably just read.
			// Not obvious how to get seconds; but this conversion gives you seconds - which has double
			// precision.
			//
			std::chrono::duration<double> elapsed_seconds = end - _last;

			_last += end - _last;

			// Apparently, this is seconds
			return elapsed_seconds.count() * 1000;
		}

	private:

		clock _last;
	};
}