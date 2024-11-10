#pragma once
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMainProgram.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueViewContainer.h"
#include "brogueViewProgram.h"
#include "eventController.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleKeyboardState.h"
#include "simpleList.h"
#include "simpleMouseState.h"

namespace brogueHd::frontend
{
	class brogueProgramContainer
	{
	public:

		brogueProgramContainer(resourceController* resourceController,
							   eventController* eventController,
							   brogueGlyphMap* glyphMap,
							   const gridRect& sceneBoundaryUI,
							   const int zoomLevel,
							   const simpleList<brogueViewContainer*>& viewList);
		~brogueProgramContainer();

		brogueKeyboardState getKeyboardState(const simpleKeyboardState& mouse) const;
		brogueMouseState getMouseState(const simpleMouseState& mouse) const;

		void setMode(BrogueGameMode gameMode);

		void activateUIProgram(brogueUIProgram programName);
		void deactivateUIProgram(brogueUIProgram programName);
		bool isProgramActive(brogueUIProgram programName);

		void initialize();
		void run(int millisecondsLapsed);
		void showErrors();
		bool hasErrors();

		/// <summary>
		/// Following a similar pipeline to the brogueView, checkUpdate checks program parameters needed
		/// to update the view - also calling the view's checkUpdate method. (creating the pipeline)
		/// </summary>
		void checkUpdate(const simpleKeyboardState& keyboardState,
						 const simpleMouseState& mouseState,
						 int millisecondsLapsed);
		void clearUpdate();
		void clearEvents();
		bool needsUpdate();

		/// <summary>
		/// Following a similar pipeline to the brogueView, update checks program parameters needed
		/// to update the view - also calling the view's update method. (creating the pipeline)
		/// </summary>
		void update(const simpleKeyboardState& keyboardState,
					const simpleMouseState& mouseState,
					int millisecondsLapsed);

		gridRect getSceneBoundaryUI() const;

	private:

		resourceController* _resourceController;
		brogueGlyphMap* _glyphMap;

		brogueMainProgram* _mainProgram;

		bool _initialized;
	};

	brogueProgramContainer::brogueProgramContainer(resourceController* resourceController, 
												   eventController* eventController,
												   brogueGlyphMap* glyphMap, 
												   const gridRect& sceneBoundaryUI,
												   const int zoomLevel,
												   const simpleList<brogueViewContainer*>& viewList)
	{
		_resourceController = resourceController;
		_glyphMap = glyphMap;
		_mainProgram = new brogueMainProgram(resourceController, eventController, glyphMap, sceneBoundaryUI, zoomLevel, viewList);
		_initialized = false;
	}
	brogueProgramContainer::~brogueProgramContainer()
	{
		delete _mainProgram;
		
		_initialized = false;
	}

	brogueKeyboardState brogueProgramContainer::getKeyboardState(const simpleKeyboardState& keyboard) const
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::getKeyboardState");

		return _mainProgram->calculateKeyboardState(keyboard);
	}
	brogueMouseState brogueProgramContainer::getMouseState(const simpleMouseState& mouse) const
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::getMouseState");

		return _mainProgram->calculateMouseState(mouse);
	}
	void brogueProgramContainer::activateUIProgram(brogueUIProgram programName)
	{
		_mainProgram->activateUIProgram(programName);
	}
	void brogueProgramContainer::deactivateUIProgram(brogueUIProgram programName)
	{
		_mainProgram->deactivateUIProgram(programName);
	}
	bool brogueProgramContainer::isProgramActive(brogueUIProgram programName)
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::hasErrors");

		return _mainProgram->isProgramActive(programName);
	}
	void brogueProgramContainer::initialize()
	{
		_mainProgram->initialize();

		_initialized = true;
	}
	void brogueProgramContainer::setMode(BrogueGameMode gameMode)
	{
		_mainProgram->setMode(gameMode);
	}
	gridRect brogueProgramContainer::getSceneBoundaryUI() const
	{
		return _mainProgram->getSceneBoundaryUI();
	}

	void brogueProgramContainer::run(int millisecondsLapsed)
	{
		if (!_initialized)
			throw simpleException("Trying to run brogue program container without initializing the programs:  brogueProgramContainer::run");

		_mainProgram->run(millisecondsLapsed);
	}

	void brogueProgramContainer::showErrors()
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::showErrors");

		_mainProgram->showErrors();
	}

	bool brogueProgramContainer::hasErrors()
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::hasErrors");

		return _mainProgram->hasErrors();
	}

	void brogueProgramContainer::checkUpdate(const simpleKeyboardState& keyboardState,
											 const simpleMouseState& mouseState,
											 int millisecondsLapsed)
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::checkUpdate");

		_mainProgram->checkUpdate(keyboardState, mouseState, millisecondsLapsed);
	}
	void brogueProgramContainer::clearUpdate()
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::clearUpdate");

		_mainProgram->clearUpdate();
	}
	void brogueProgramContainer::clearEvents()
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::clearEvents");

		_mainProgram->clearEvents();
	}
	bool brogueProgramContainer::needsUpdate()
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::needsUpdate");

		return _mainProgram->needsUpdate();
	}
	void brogueProgramContainer::update(const simpleKeyboardState& keyboardState,
										const simpleMouseState& mouseState,
										int millisecondsLapsed)
	{
		if (!_initialized)
			throw simpleException("Brogue Program Container must first be initialized:  brogueProgramContainer::update");

		_mainProgram->update(keyboardState, mouseState, millisecondsLapsed);
	}
}