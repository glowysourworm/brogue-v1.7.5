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
		void addUIProgram(brogueProgram* uiProgram);

		brogueProgram* getBackgroundProgram();
		brogueProgram* getUIProgram(int index);
		int getUIProgramCount();

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
		simpleList<brogueProgram*>* _uiPrograms;
	};

	brogueProgramContainer::brogueProgramContainer()
	{
		_uiPrograms = new simpleList<brogueProgram*>();
	}

	brogueProgramContainer::~brogueProgramContainer()
	{
		delete _uiPrograms;
	}

	void brogueProgramContainer::setBackground(brogueProgram* backgroundProgram)
	{
		_backgroundProgram = backgroundProgram;
	}

	void brogueProgramContainer::addUIProgram(brogueProgram* uiProgram)
	{	
		_uiPrograms->add(uiProgram);
	}

	brogueProgram* brogueProgramContainer::getBackgroundProgram()
	{
		return _backgroundProgram;
	}

	brogueProgram* brogueProgramContainer::getUIProgram(int index)
	{
		return _uiPrograms->get(index);
	}

	int brogueProgramContainer::getUIProgramCount()
	{
		return _uiPrograms->count();
	}

	void brogueProgramContainer::initialize()
	{
		_backgroundProgram->initialize();

		_uiPrograms->forEach([] (brogueProgram* program)
		{
			program->initialize();

			return iterationCallback::iterate;
		});
	}

	void brogueProgramContainer::run(int millisecondsLapsed)
	{
		_backgroundProgram->run(millisecondsLapsed);

		_uiPrograms->forEach([&millisecondsLapsed] (brogueProgram* program)
		{
			program->run(millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}

	void brogueProgramContainer::showErrors()
	{
		_backgroundProgram->outputStatus();

		_uiPrograms->forEach([] (brogueProgram* program)
		{
			program->outputStatus();

			return iterationCallback::iterate;
		});
	}

	bool brogueProgramContainer::hasErrors()
	{
		bool hasErrors = _backgroundProgram->hasErrors();

		_uiPrograms->forEach([&hasErrors] (brogueProgram* program)
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

		_uiPrograms->forEach([&mouseState, &millisecondsLapsed] (brogueProgram* program)
		{
			if (program->shouldUpdate(mouseState, millisecondsLapsed))
				program->update(mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}

	bool brogueProgramContainer::shouldUpdate(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		bool result = false;

		_uiPrograms->forEach([&result, &mouseState, &millisecondsLapsed] (brogueProgram* program)
		{
			result |= program->shouldUpdate(mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});

		return result || _backgroundProgram->shouldUpdate(mouseState, millisecondsLapsed);
	}
}