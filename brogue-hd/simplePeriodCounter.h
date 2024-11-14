#pragma once
#include <cstdint>

namespace brogueHd::frontend
{
	struct simplePeriodCounter
	{
		simplePeriodCounter()
		{
			_period = 0;
			_counter = 0;
			_periodNumber = 0;
		}
		simplePeriodCounter(uint32_t period)
		{
			_period = period;
			_counter = 0;
			_periodNumber = 0;
		}
		simplePeriodCounter(const simplePeriodCounter& copy)
		{
			_period = copy.getPeriod();
			_counter = copy.getCounter();
			_periodNumber = copy.getNumberOfPeriods();
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

		bool pending() const
		{
			return _counter >= _period;
		}

		void reset()
		{
			_counter = 0;
			_periodNumber++;
		}

		uint64_t getNumberOfPeriods() const
		{
			return _periodNumber;
		}


	private:

		uint32_t _period;
		uint32_t _counter;

		// How many periods has this counter ticked?
		uint64_t _periodNumber;
	};
}