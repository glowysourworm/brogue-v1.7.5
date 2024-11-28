#pragma once
#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueImageQuad.h"
#include "brogueKeyboardState.h"
#include "brogueKeyboardState.h"
#include "brogueLine.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewCore.h"
#include "brogueViewGridCore.h"
#include "brogueViewPolygonCore.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleKeyboardState.h"
#include "simpleLine.h"
#include "simpleList.h"
#include "simpleMouseState.h"
#include "simpleOpenGL.h"

namespace brogueHd::frontend
{
	class brogueViewContainer
	{
	public:

		brogueViewContainer();
		~brogueViewContainer();

		template<isGLStream TStream>
		void addView(brogueViewCore<TStream>* view);

		int getCount();

		void initiateStateChange(brogueUIState fromState, brogueUIState toState);
		void clearStateChange();
		bool checkStateChange();

		void checkUpdate(const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed);

		void invalidate(const brogueKeyboardState& keyboardState,
						const brogueMouseState& mouseState);

		bool needsUpdate() const;
		void clearUpdate();
		void clearEvents();
		void update(int millisecondsLapsed, bool forceUpdate);



		template<isGLUniform TUniform>
		void setUniform(const char* name, const TUniform& value);
		void run();
		void activate();
		void deactivate();
		bool hasErrors();
		void showErrors();

		// Design Problem:  Two different view data streams (polygons v.s. 2D-grid)
		//
		void setGridProgram(const brogueUIProgramPartId& partId, const brogueCellDisplay& cell);
		void setPolygonProgram(const brogueUIProgramPartId& partId, const simpleLine<int>& line);

		gridRect getAggregateBoundary();

	private:

		void addViewImpl(brogueViewCore<brogueImageQuad>* view);
		void addViewImpl(brogueViewCore<brogueColorQuad>* view);
		void addViewImpl(brogueViewCore<brogueCellQuad>* view);
		void addViewImpl(brogueViewCore<brogueLine>* view);

	private:

		simpleHash<brogueUIProgramPartId, int>* _orderLookup;

		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueCellQuad>*>* _cellViews;
		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueColorQuad>*>* _colorViews;
		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueImageQuad>*>* _imageViews;
		simpleHash<brogueUIProgramPartId, brogueViewPolygonCore*>* _lineViews;
	};

	brogueViewContainer::brogueViewContainer()
	{
		_orderLookup = new simpleHash<brogueUIProgramPartId, int>();
		_cellViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueCellQuad>*>();
		_colorViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueColorQuad>*>();
		_imageViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueImageQuad>*>();
		_lineViews = new simpleHash<brogueUIProgramPartId, brogueViewPolygonCore*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		_cellViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueCellQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_colorViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueColorQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_imageViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueImageQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_lineViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueLine>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});

		delete _cellViews;
		delete _colorViews;
		delete _imageViews;
		delete _lineViews;
		delete _orderLookup;
	}

	template<isGLStream TStream>
	void brogueViewContainer::addView(brogueViewCore<TStream>* view)
	{
		addViewImpl(view);
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueImageQuad>* view)
	{
		_imageViews->add(view->getId(), (brogueViewGridCore<brogueImageQuad>*)view);
		_orderLookup->add(view->getId(), _orderLookup->count());
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueColorQuad>* view)
	{
		_colorViews->add(view->getId(), (brogueViewGridCore<brogueColorQuad>*)view);
		_orderLookup->add(view->getId(), _orderLookup->count());
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueCellQuad>* view)
	{
		_cellViews->add(view->getId(), (brogueViewGridCore<brogueCellQuad>*)view);
		_orderLookup->add(view->getId(), _orderLookup->count());
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueLine>* view)
	{
		_lineViews->add(view->getId(), (brogueViewPolygonCore*)view);
		_orderLookup->add(view->getId(), _orderLookup->count());
	}

	int brogueViewContainer::getCount()
	{
		return _orderLookup->count();
	}

	gridRect brogueViewContainer::getAggregateBoundary()
	{
		gridRect boundary;

		simpleListAggregator<gridRect, gridRect> callback([] (const gridRect& seed, const gridRect& viewBounds)
		{
			gridRect nextBounds = seed;
			if (nextBounds == default_value::value<gridRect>())
				nextBounds = viewBounds;
			else
				nextBounds.expand(viewBounds);
			return nextBounds;
		});

		// Iterator...
		boundary = _cellViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueCellQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _imageViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueImageQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _colorViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueColorQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _lineViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueLine>* view)
		{
			return callback(seed, view->getBoundary());
		});

		return boundary;
	}

	void brogueViewContainer::setGridProgram(const brogueUIProgramPartId& partId, const brogueCellDisplay& cell)
	{
		if (_cellViews->contains(partId))
			_cellViews->get(partId)->set(cell);

		else if (_imageViews->contains(partId))
			_imageViews->get(partId)->set(cell);

		else if (_colorViews->contains(partId))
			_colorViews->get(partId)->set(cell);

		else
			throw simpleException("Program part not found:  brogueViewContainer.h");
	}
	void brogueViewContainer::setPolygonProgram(const brogueUIProgramPartId& partId, const simpleLine<int>& line)
	{
		if (_lineViews->contains(partId))
			_lineViews->get(partId)->addLine(line);

		else
			throw simpleException("Program part not found:  brogueViewContainer.h");
	}

	void brogueViewContainer::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->initiateStateChange(fromState, toState);

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->initiateStateChange(fromState, toState);

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->initiateStateChange(fromState, toState);

			//else if (_lineViews->contains(partId))
			//	_lineViews->get(partId)->initiateStateChange(fromState, toState);

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}

	void brogueViewContainer::clearStateChange()
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->clearStateChange();

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->clearStateChange();

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->clearStateChange();

			//else if (_lineViews->contains(partId))
			//	_lineViews->get(partId)->initiateStateChange(fromState, toState);

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	bool brogueViewContainer::checkStateChange()
	{
		bool result = false;

		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				result |= _imageViews->get(partId)->checkStateChange();

			else if (_colorViews->contains(partId))
				result |= _colorViews->get(partId)->checkStateChange();

			else if (_cellViews->contains(partId))
				result |= _cellViews->get(partId)->checkStateChange();

			//else if (_lineViews->contains(partId))
			//	_lineViews->get(partId)->initiateStateChange(fromState, toState);

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}

		return result;
	}

	void brogueViewContainer::checkUpdate(const brogueKeyboardState& keyboardState,
										  const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}

	void brogueViewContainer::invalidate(const brogueKeyboardState& keyboardState,
										 const brogueMouseState& mouseState)
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->invalidate(keyboardState, mouseState);

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->invalidate(keyboardState, mouseState);

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->invalidate(keyboardState, mouseState);

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->invalidate(keyboardState, mouseState);

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}

	bool brogueViewContainer::needsUpdate() const
	{
		bool result = false;

		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				result |= _imageViews->get(partId)->needsUpdate();

			else if (_colorViews->contains(partId))
				result |= _colorViews->get(partId)->needsUpdate();

			else if (_cellViews->contains(partId))
				result |= _cellViews->get(partId)->needsUpdate();

			else if (_lineViews->contains(partId))
				result |= _lineViews->get(partId)->needsUpdate();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}

		return result;
	}
	void brogueViewContainer::clearUpdate()
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->clearUpdate();

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->clearUpdate();

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->clearUpdate();

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->clearUpdate();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	void brogueViewContainer::clearEvents()
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->clearEvents();

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->clearEvents();

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->clearEvents();

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->clearEvents();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	void brogueViewContainer::update(int millisecondsLapsed, bool forceUpdate)
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->update(millisecondsLapsed, forceUpdate);

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->update(millisecondsLapsed, forceUpdate);

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->update(millisecondsLapsed, forceUpdate);

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->update(millisecondsLapsed, forceUpdate);

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	template<isGLUniform TUniform>
	void brogueViewContainer::setUniform(const char* name, const TUniform& value)
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
			{
				if (_imageViews->get(partId)->hasUniform(name))
					_imageViews->get(partId)->setUniform(name, value);
			}

			else if (_colorViews->contains(partId))
			{
				if (_colorViews->get(partId)->hasUniform(name))
					_colorViews->get(partId)->setUniform(name, value);
			}

			else if (_cellViews->contains(partId))
			{
				if (_cellViews->get(partId)->hasUniform(name))
					_cellViews->get(partId)->setUniform(name, value);
			}

			else if (_lineViews->contains(partId))
			{
				if (_lineViews->get(partId)->hasUniform(name))
					_lineViews->get(partId)->setUniform(name, value);
			}

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	void brogueViewContainer::run()
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->run();

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->run();

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->run();

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->run();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	void brogueViewContainer::activate()
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->activate();

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->activate();

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->activate();

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->activate();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	void brogueViewContainer::deactivate()
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->deactivate();

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->deactivate();

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->deactivate();

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->deactivate();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
	bool brogueViewContainer::hasErrors()
	{
		bool result = false;

		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				result |= _imageViews->get(partId)->hasErrors();

			else if (_colorViews->contains(partId))
				result |= _colorViews->get(partId)->hasErrors();

			else if (_cellViews->contains(partId))
				result |= _cellViews->get(partId)->hasErrors();

			else if (_lineViews->contains(partId))
				result |= _lineViews->get(partId)->hasErrors();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}

		return result;
	}
	void brogueViewContainer::showErrors()
	{
		for (int index = 0; index < _orderLookup->count(); index++)
		{
			brogueUIProgramPartId partId = _orderLookup->getAt(index)->key;

			if (_imageViews->contains(partId))
				_imageViews->get(partId)->showErrors();

			else if (_colorViews->contains(partId))
				_colorViews->get(partId)->showErrors();

			else if (_cellViews->contains(partId))
				_cellViews->get(partId)->showErrors();

			else if (_lineViews->contains(partId))
				_lineViews->get(partId)->showErrors();

			else
				throw simpleException("Program Part Id not found:  brogueViewContainer.h");
		}
	}
}