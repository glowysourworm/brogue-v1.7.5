#pragma once

#include "brogueProgramSignature.h"
#include "brogueUIConstants.h"
#include "simple.h"

using namespace brogueHd::frontend;
using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	class brogueProgramControlData : public hashableObject
	{

	public:

		brogueProgramControlData()
		{
			_current = nullptr;
			_next = nullptr;
			_exitConditions = brogueProgramExitCondition::OnKeyboardEsc;
		}
		brogueProgramControlData(const brogueProgramSignature& current,
								 const brogueProgramSignature& next,
								 brogueProgramExitCondition conditions)
		{
			_current = new brogueProgramSignature(current);
			_next = new brogueProgramSignature(next);
			_exitConditions = conditions;
		}
		brogueProgramControlData(const brogueProgramControlData& copy)
		{
			copyImpl(copy);
		}
		~brogueProgramControlData() 
		{
			delete _current;
			delete _next;
		}

		void operator=(const brogueProgramControlData& copy)
		{
			delete _current;
			delete _next;

			copyImpl(copy);
		}

		brogueProgramSignature* getCurrent() const
		{
			return _current;
		}
		brogueProgramSignature* getNext() const
		{
			return _next;
		}
		brogueProgramExitCondition getExitConditions() const
		{
			return _exitConditions;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(_current->getHash(), _next->getHash(), _exitConditions);
		}


	private:

		brogueProgramSignature* _current;
		brogueProgramSignature* _next;
		brogueProgramExitCondition _exitConditions;

	private:

		void copyImpl(const brogueProgramControlData& copy)
		{
			_current = new brogueProgramSignature(*copy.getCurrent());
			_next = new brogueProgramSignature(*copy.getNext());
			_exitConditions = copy.getExitConditions();
		}
	};
}