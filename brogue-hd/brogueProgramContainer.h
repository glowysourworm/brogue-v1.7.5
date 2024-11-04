#pragma once
#include "brogueProgram.h"
#include "brogueUIConstants.h"
#include "brogueUIResponseData.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleHash.h"
#include "simpleMouseState.h"

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
		brogueProgram* getUIProgram(brogueUIProgram programName);
		brogueProgram* getUIProgramAt(int index);
		int getUIProgramCount();

		void activateUIProgram(brogueUIProgram programName);
		void deactivateUIProgram(brogueUIProgram programName);

		void initialize();
		void run(int millisecondsLapsed);
		void showErrors();
		bool hasErrors();

		void update(const simpleMouseState& mouseState, int millisecondsLapsed);
		brogueUIResponseData& checkUpdate(const simpleMouseState& mouseState, int millisecondsLapsed);

		gridRect getSceneBoundaryUI() const
		{
			return _backgroundProgram->getSceneBoundaryUI();
		}

	private:

		brogueProgram* _backgroundProgram;
		simpleHash<brogueUIProgram, brogueProgram*>* _uiPrograms;
	};

	brogueProgramContainer::brogueProgramContainer()
	{
		_uiPrograms = new simpleHash<brogueUIProgram, brogueProgram*>();
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
		_uiPrograms->add(uiProgram->getProgramName(), uiProgram);
	}

	brogueProgram* brogueProgramContainer::getBackgroundProgram()
	{
		return _backgroundProgram;
	}

	brogueProgram* brogueProgramContainer::getUIProgram(brogueUIProgram programName)
	{
		return _uiPrograms->get(programName);
	}

	brogueProgram* brogueProgramContainer::getUIProgramAt(int index)
	{
		return _uiPrograms->getAt(index)->value;
	}

	int brogueProgramContainer::getUIProgramCount()
	{
		return _uiPrograms->count();
	}
	void brogueProgramContainer::activateUIProgram(brogueUIProgram programName)
	{
		_uiPrograms->get(programName)->setIsActive(true);
	}
	void brogueProgramContainer::deactivateUIProgram(brogueUIProgram programName)
	{
		_uiPrograms->get(programName)->setIsActive(false);
	}

	void brogueProgramContainer::initialize()
	{
		_backgroundProgram->initialize();

		_uiPrograms->iterate([] (brogueUIProgram programName, brogueProgram* program)
		{
			program->initialize();

			return iterationCallback::iterate;
		});
	}

	void brogueProgramContainer::run(int millisecondsLapsed)
	{
		_backgroundProgram->run(millisecondsLapsed);

		_uiPrograms->iterate([&millisecondsLapsed] (brogueUIProgram programName, brogueProgram* program)
		{
			if (program->getIsActive())
				program->run(millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}

	void brogueProgramContainer::showErrors()
	{
		_backgroundProgram->outputStatus();

		_uiPrograms->iterate([] (brogueUIProgram programName, brogueProgram* program)
		{
			program->outputStatus();

			return iterationCallback::iterate;
		});
	}

	bool brogueProgramContainer::hasErrors()
	{
		bool hasErrors = _backgroundProgram->hasErrors();

		_uiPrograms->iterate([&hasErrors] (brogueUIProgram programName, brogueProgram* program)
		{
			hasErrors |= program->hasErrors();

			return iterationCallback::iterate;
		});

		return hasErrors;
	}

	void brogueProgramContainer::update(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		if (_backgroundProgram->checkUpdate(mouseState, millisecondsLapsed).shouldUpdate)
			_backgroundProgram->update(mouseState, millisecondsLapsed);

		_uiPrograms->iterate([&mouseState, &millisecondsLapsed] (brogueUIProgram programName, brogueProgram* program)
		{
			if (program->getIsActive())
			{
				if (program->checkUpdate(mouseState, millisecondsLapsed).shouldUpdate)
					program->update(mouseState, millisecondsLapsed);
			}

			return iterationCallback::iterate;
		});
	}

	brogueUIResponseData& brogueProgramContainer::checkUpdate(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		brogueUIResponseData response;

		_uiPrograms->iterate([&mouseState, &millisecondsLapsed, &response] (brogueUIProgram programName, brogueProgram* program)
		{
			if (program->getIsActive())
			{
				brogueUIResponseData childResponse = program->checkUpdate(mouseState, millisecondsLapsed);

				if (childResponse.shouldUpdate)
					response = childResponse;
			}

			return iterationCallback::iterate;
		});

		// Use child response data first
		if (response.shouldUpdate)
			return response;

		return _backgroundProgram->checkUpdate(mouseState, millisecondsLapsed);
	}
}