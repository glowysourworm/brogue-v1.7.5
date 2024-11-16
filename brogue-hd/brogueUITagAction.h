#pragma once
#include "brogueUIConstants.h"
#include "simple.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	struct brogueUITagAction : hashable
	{
		brogueUITagAction()
		{
			action = brogueUIAction::None;
			desiredState = brogueUIState::MainMenu;
			tag = default_value::value<simpleString>();
		}
		brogueUITagAction(brogueUIState stateChange)
		{
			action = brogueUIAction::StateChange;
			desiredState = stateChange;
			tag = default_value::value<simpleString>();
		}
		brogueUITagAction(brogueUIAction fileAction, simpleString fileChosen)
		{
			action = fileAction;
			tag = fileChosen;
			desiredState = brogueUIState::MainMenu;
		}
		brogueUITagAction(const brogueUITagAction& copy)
		{
			action = copy.action;
			tag = copy.tag;
			desiredState = copy.desiredState;
		}
		void set(const brogueUITagAction& copy)
		{
			action = copy.action;
			tag = copy.tag;
			desiredState = copy.desiredState;
		}
		void operator=(const brogueUITagAction& copy)
		{
			action = copy.action;
			tag = copy.tag;
			desiredState = copy.desiredState;
		}
		bool operator==(const brogueUITagAction& other)
		{
			return compare(other);
		}
		bool operator!=(const brogueUITagAction& other)
		{
			return !compare(other);
		}

		/// <summary>
		/// File name from the chosen file
		/// </summary>
		simpleString tag;

		/// <summary>
		/// Change of state action
		/// </summary>
		brogueUIState desiredState;

		/// <summary>
		/// Action to perform - which is the purpose of the UI control
		/// </summary>
		brogueUIAction action;

	private:

		bool compare(const brogueUITagAction& other)
		{
			return action == other.action &&
				desiredState == other.desiredState &&
				tag == other.tag;
		}
	};
}