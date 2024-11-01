#pragma once

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
		~simplePeriodCounter(){}

		uint32_t getPeriod() const
		{
			return _period;
		}
		uint32_t getCounter() const
		{
			return _counter;
		}

		bool update(int lapsed)
		{
			_counter += lapsed;

			if (_counter >= _period)
			{
				_counter = 0;
				return true;
			}

			return false;
		}

	private:

		uint32_t _period;
		uint32_t _counter;
	};
}