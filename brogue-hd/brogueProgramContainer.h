#pragma once
#include "brogueProgram.h"
#include "brogueUIConstants.h"
#include "brogueUIResponseData.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleKeyboardState.h"
#include "simpleMouseState.h"

namespace brogueHd::frontend::opengl
{
	class brogueProgramContainer
	{
	public:

		brogueProgramContainer(brogueUIContainer containerName);
		~brogueProgramContainer();

		void setBackground(brogueProgram* backgroundProgram);
		void addUIProgram(brogueProgram* uiProgram);

		brogueProgram* getBackgroundProgram();
		brogueProgram* getUIProgram(brogueUIProgram programName);
		brogueProgram* getUIProgramAt(int index);
		int getUIProgramCount();

		void activateUIProgram(brogueUIProgram programName);
		void deactivateUIProgram(brogueUIProgram programName);
		void deactivateCurrentUIProgram();

		void initialize();
		void run(int millisecondsLapsed);
		void showErrors();
		bool hasErrors();

		/// <summary>
		/// Following a similar pipeline to the brogueView, checkUpdate checks program parameters needed
		/// to update the view - also calling the view's checkUpdate method. (creating the pipeline)
		/// </summary>
		virtual void checkUpdate(brogueUIResponseData& response,
								 const simpleKeyboardState& keyboardState,
								 const simpleMouseState& mouseState,
								 int millisecondsLapsed);

		/// <summary>
		/// Following a similar pipeline to the brogueView, update checks program parameters needed
		/// to update the view - also calling the view's update method. (creating the pipeline)
		/// </summary>
		virtual void update(const simpleKeyboardState& keyboardState,
							const simpleMouseState& mouseState,
							int millisecondsLapsed);

		gridRect getSceneBoundaryUI() const
		{
			return _backgroundProgram->getSceneBoundaryUI();
		}

	private:

		brogueUIContainer _containerName;
		brogueProgram* _backgroundProgram;
		simpleHash<brogueUIProgram, brogueProgram*>* _uiPrograms;
	};

	brogueProgramContainer::brogueProgramContainer(brogueUIContainer containerName)
	{
		_backgroundProgram = nullptr;
		_uiPrograms = new simpleHash<brogueUIProgram, brogueProgram*>();
		_containerName = containerName;
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
	void brogueProgramContainer::deactivateCurrentUIProgram()
	{
		brogueUIProgram activeProgram = _uiPrograms->firstOrDefaultKey([] (brogueUIProgram uiProgram, brogueProgram* program)
		{
			return program->getIsActive();
		});

		if (activeProgram != brogueUIProgram::ContainerControlledProgram)
			deactivateUIProgram(activeProgram);

		else
			throw simpleException("Unhandled program type, or program not found:  brogueProgramContainer::deactivateCurrentUIProgram");
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

	void brogueProgramContainer::checkUpdate(brogueUIResponseData& response,
											 const simpleKeyboardState& keyboardState,
											 const simpleMouseState& mouseState,
											 int millisecondsLapsed)
	{
		// CRITICAL!  Don't forget to mark the program name! Otherwise, the main controller won't know 
		//			  how to process the message
		//
		//			  Mark the reponse. (see views to make sure the response is being marked properly)
		//
		response.signature.container = _containerName;

		// Pass the response down the pipeline -> background program
		_backgroundProgram->checkUpdate(response, keyboardState, mouseState, millisecondsLapsed);

		// Iterate UI Programs
		_uiPrograms->iterate([&keyboardState, &mouseState, &millisecondsLapsed, &response] (brogueUIProgram programName, brogueProgram* program)
		{
			if (program->getIsActive())
			{
				program->checkUpdate(response, keyboardState, mouseState, millisecondsLapsed);
			}

			return iterationCallback::iterate;
		});
	}

	void brogueProgramContainer::update(const simpleKeyboardState& keyboardState,
										const simpleMouseState& mouseState,
										int millisecondsLapsed)
	{
		_backgroundProgram->update(keyboardState, mouseState, millisecondsLapsed);

		_uiPrograms->iterate([&keyboardState, &mouseState, &millisecondsLapsed] (brogueUIProgram programName, brogueProgram* program)
		{
			if (program->getIsActive())
			{
				program->update(keyboardState, mouseState, millisecondsLapsed);
			}

			return iterationCallback::iterate;
		});
	}
}