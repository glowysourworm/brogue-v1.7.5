#pragma once
#include <cstdint>

namespace brogueHd::frontend::opengl
{
	struct simplePeriodCounter
	{
		simplePeriodCounter()
		{
			_period = 0;
			_counter = 0;
		}
		simplePeriodCounter(uint32_t period)
		{
			_period = period;
			_counter = 0;
		}
		simplePeriodCounter(const simplePeriodCounter& copy)
		{
			_period = copy.getPeriod();
			_counter = copy.getCounter();
		}
		~simplePeriodCounter() {}

		uint32_t getPeriod() const
		{
			return _period;
		}
		uint32_t getCounter() const
		{
			return _counter;
		}

		bool update(int lapsed, bool autoReset = true)
		{
			_counter += lapsed;

			if (_counter >= _period)
			{
				if (autoReset)
					reset();

				return true;
			}

			return false;
		}

		void reset()
		{
			_counter = 0;
		}

	private:

		uint32_t _period;
		uint32_t _counter;
	};
}