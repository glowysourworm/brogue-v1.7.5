#pragma once
#include "brogueProgram.h"
#include "brogueViewProgram.h"

namespace brogueHd::frontend::opengl
{
	class brogueProgramContainer
	{
	public:
		
		brogueProgramContainer();
		~brogueProgramContainer();

		void setBackground(brogueProgram* backgroundProgram);
		void addUIProgram(brogueProgram* uiProgram, bool isActive);

		brogueProgram* getBackgroundProgram();
		brogueProgram* getUIProgram(int index);
		int getUIProgramCount();
		void activateUIProgram(int index);
		void deactivateUIProgram(int index);

		void initialize();
		void run(int millisecondsLapsed);
		void showErrors();
		bool hasErrors();

		void update(const simpleMouseState& mouseState, int millisecondsLapsed);
		bool shouldUpdate(const simpleMouseState& mouseState, int millisecondsLapsed);

		gridRect getSceneBoundaryUI() const
		{
			return _backgroundProgram->getSceneBoundaryUI();
		}

	private:

		brogueProgram* _backgroundProgram;
		simpleHash<brogueProgram*, bool>* _uiPrograms;
	};

	brogueProgramContainer::brogueProgramContainer()
	{
		_uiPrograms = new simpleHash<brogueProgram*, bool>();
	}

	brogueProgramContainer::~brogueProgramContainer()
	{
		delete _uiPrograms;
	}

	void brogueProgramContainer::setBackground(brogueProgram* backgroundProgram)
	{
		_backgroundProgram = backgroundProgram;
	}

	void brogueProgramContainer::addUIProgram(brogueProgram* uiProgram, bool isActive)
	{	
		_uiPrograms->add(uiProgram, isActive);
	}

	brogueProgram* brogueProgramContainer::getBackgroundProgram()
	{
		return _backgroundProgram;
	}

	brogueProgram* brogueProgramContainer::getUIProgram(int index)
	{
		return _uiPrograms->getAt(index)->key;
	}

	int brogueProgramContainer::getUIProgramCount()
	{
		return _uiPrograms->count();
	}
	void brogueProgramContainer::activateUIProgram(int index)
	{
		_uiPrograms->getAt(index)->value = true;
	}
	void brogueProgramContainer::deactivateUIProgram(int index)
	{
		_uiPrograms->getAt(index)->value = false;
	}

	void brogueProgramContainer::initialize()
	{
		_backgroundProgram->initialize();

		_uiPrograms->iterate([] (brogueProgram* program, bool isActive)
		{
			program->initialize();

			return iterationCallback::iterate;
		});
	}

	void brogueProgramContainer::run(int millisecondsLapsed)
	{
		_backgroundProgram->run(millisecondsLapsed);

		_uiPrograms->iterate([&millisecondsLapsed] (brogueProgram* program, bool isActive)
		{
			if (isActive)
				program->run(millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}

	void brogueProgramContainer::showErrors()
	{
		_backgroundProgram->outputStatus();

		_uiPrograms->iterate([] (brogueProgram* program, bool isActive)
		{
			program->outputStatus();

			return iterationCallback::iterate;
		});
	}

	bool brogueProgramContainer::hasErrors()
	{
		bool hasErrors = _backgroundProgram->hasErrors();

		_uiPrograms->iterate([&hasErrors] (brogueProgram* program, bool isActive)
		{
			hasErrors |= program->hasErrors();

			return iterationCallback::iterate;
		});

		return hasErrors;
	}

	void brogueProgramContainer::update(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		if (_backgroundProgram->shouldUpdate(mouseState, millisecondsLapsed))
			_backgroundProgram->update(mouseState, millisecondsLapsed);

		_uiPrograms->iterate([&mouseState, &millisecondsLapsed] (brogueProgram* program, bool isActive)
		{
			if (isActive)
			{
				if (program->shouldUpdate(mouseState, millisecondsLapsed))
					program->update(mouseState, millisecondsLapsed);
			}

			return iterationCallback::iterate;
		});
	}

	bool brogueProgramContainer::shouldUpdate(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		bool result = false;

		_uiPrograms->iterate([&mouseState, &millisecondsLapsed, &result] (brogueProgram* program, bool isActive)
		{
			if (isActive)
			{
				result |= program->shouldUpdate(mouseState, millisecondsLapsed);
			}


			return iterationCallback::iterate;
		});

		return result || _backgroundProgram->shouldUpdate(mouseState, millisecondsLapsed);
	}
}