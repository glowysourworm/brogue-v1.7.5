#pragma once
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include <functional>

using namespace brogueHd::simple;

namespace brogueHd::backend
{
	template<isHashable EventSender, isHashable EventData>
	class brogueEventBase : hashableObject
	{
		using DelegateType = std::function<void(EventSender, EventData)>;

	public:

		brogueEventBase()
		{
			_delegates = new simpleHash<int, DelegateType*>();
			_tokenCounter = 0;
		};
		~brogueEventBase()
		{
			for (int index = 0; index < _delegates->count(); index++)
			{
				delete _delegates->get(index);
			}

			delete _delegates;
		}

	public:

		int subscribe(DelegateType listenerDelegate)
		{
			// User tokens should start at 1
			int token = ++_tokenCounter;

			_delegates->add(token, new DelegateType(listenerDelegate));

			return token;
		}
		void unSubscribe(int token)
		{
			if (!_delegates->contains(token))
				throw simpleException("Unknown event token:  brogueEvent.h");

			delete _delegates->get(token);

			_delegates->remove(token);
		}
		void publish(EventSender sender, EventData payload)
		{
			for (int index = 0; index < _delegates->count(); index++)
			{
				(*(_delegates->getAt(index)->value))(sender, payload);
			}
		}

	private:

		int _tokenCounter;

		simpleHash<int, DelegateType*>* _delegates;
	};
}