#pragma once

#include "brogueProgramControlData.h"
#include "brogueProgramSignature.h"
#include "brogueUIConstants.h"
#include "brogueUIResponseData.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleList.h"

using namespace brogueHd::simple;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueProgramController
	{
	public:

		brogueProgramController()
		{
			_stateTransitionLookup = new simpleHash<brogueUIView, simpleList<brogueProgramControlData*>*>();
			_signatureLookup = new simpleHash<brogueUIView, brogueProgramSignature*>();
			_exitConditionLookup = new simpleHash<brogueUIView, brogueProgramExitCondition>();
		}
		~brogueProgramController()
		{
			for (int index = 0; index < _stateTransitionLookup->count(); index++)
			{
				for (int listIndex = 0; listIndex < _stateTransitionLookup->getAt(index)->value->count(); listIndex++)
					delete _stateTransitionLookup->getAt(index)->value->get(listIndex);

				delete _stateTransitionLookup->getAt(index)->value;
			}
			for (int index = 0; index < _signatureLookup->count(); index++)
			{
				delete _signatureLookup->getAt(index);
			}

			delete _signatureLookup;
			delete _exitConditionLookup;
			delete _stateTransitionLookup;
		}

		bool wasExitConditionMet(const brogueUIResponseData& response)
		{
			// Current State -> Sender (View) (of current state) -> What are the program transitions? 
			//
			if (_signatureLookup->contains(response.signature.view))
			{
				brogueProgramExitCondition condition = _exitConditionLookup->get(response.signature.view);

				if ((condition | brogueProgramExitCondition::OnKeyboardHotkey) && response.keyboard.getAsciiHotkey() > 0)
					return true;

				else if ((condition | brogueProgramExitCondition::OnKeyboardAny) && response.keyboard.getAsciiHotkey() > 0)
					return true;

				else if ((condition | brogueProgramExitCondition::OnMouseButton) && response.response.mouseLeftRegistered)
					return true;

				else
					return false;
			}
			else
				simpleException::show("brogueUIView not registered with the brogueProgramController");
		}

		brogueUIProgram getNextProgram(const brogueUIResponseData& response)
		{
			brogueUIProgram result = brogueUIProgram::ContainerControlledProgram;

			// Current State -> Sender (View) (of current state) -> What are the program transitions? 
			//
			if (_stateTransitionLookup->contains(response.signature.view))
			{
				_stateTransitionLookup->get(response.signature.view)->forEach([&response, &result] (brogueProgramControlData* entry)
				{
					brogueProgramExitCondition condition = entry->getExitConditions();

					if ((condition | brogueProgramExitCondition::OnKeyboardHotkey) && response.keyboard.getAsciiHotkey() > 0)
					{
						result = entry->getNext()->program;
						return iterationCallback::breakAndReturn;
					}
					else if ((condition | brogueProgramExitCondition::OnKeyboardAny) && response.keyboard.getAsciiHotkey() > 0)
					{
						result = entry->getNext()->program;
						return iterationCallback::breakAndReturn;
					}
					else if ((condition | brogueProgramExitCondition::OnMouseButton) && response.response.mouseLeftRegistered)
					{
						result = entry->getNext()->program;
						return iterationCallback::breakAndReturn;
					}
					return iterationCallback::iterate;
				});
			}

			if (result == brogueUIProgram::ContainerControlledProgram)
				throw simpleException("Program not properly registered with brogueProgramController (see state transitions for the UI)");

			return result;
		}

		void addViewControl(const brogueProgramControlData& controlData)
		{
			// Current State -> Sender (View) (of current state) -> What are the program transitions? 
			//

			if (!_stateTransitionLookup->contains(controlData.getCurrent()->view))
				_stateTransitionLookup->add(controlData.getCurrent()->view, new simpleList<brogueProgramControlData*>());

			_stateTransitionLookup->get(controlData.getCurrent()->view)->add(new brogueProgramControlData(controlData));

			if (!_signatureLookup->contains(controlData.getCurrent()->view))
				_signatureLookup->add(controlData.getCurrent()->view, controlData.getCurrent());

			if (!_signatureLookup->contains(controlData.getNext()->view))
				_signatureLookup->add(controlData.getNext()->view, controlData.getNext());

			// Exit from the current state -> next state
			if (!_exitConditionLookup->contains(controlData.getCurrent()->view))
				_exitConditionLookup->add(controlData.getCurrent()->view, controlData.getExitConditions());
		}

	private:

		//brogueProgramExitCondition getExitCondition(const brogueUIResponseData& response)
		//{
		//	// Current State -> Sender (View) (of current state) -> What are the program transitions? 
		//	//
		//	if (_signatureLookup->contains(response.signature.view))
		//	{
		//		brogueProgramExitCondition condition = _exitConditionLookup->get(response.signature.view);

		//		if ((condition | brogueProgramExitCondition::OnKeyboardHotkey) && response.keyboard.getAsciiHotkey() > 0)
		//			return true;

		//		else if ((condition | brogueProgramExitCondition::OnKeyboardAny) && response.keyboard.getAsciiHotkey() > 0)
		//			return true;

		//		else if ((condition | brogueProgramExitCondition::OnMouseButton) && response.response.mouseLeftRegistered)
		//			return true;

		//		else
		//			return false;
		//	}
		//	else
		//		simpleException::show("brogueUIView not registered with the brogueProgramController");
		//}

	private:

		simpleHash<brogueUIView, simpleList<brogueProgramControlData*>*>* _stateTransitionLookup;
		simpleHash<brogueUIView, brogueProgramSignature*>* _signatureLookup;
		simpleHash<brogueUIView, brogueProgramExitCondition>* _exitConditionLookup;
	};
}