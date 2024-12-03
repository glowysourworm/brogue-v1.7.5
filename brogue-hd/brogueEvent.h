#pragma once
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include <functional>

namespace brogueHd::backend
{
	using namespace simple;

	template <isHashable EventSender, isHashable EventData>
	class brogueEventBase : simpleObject
	{
		using DelegateType = std::function<void(const EventSender&, const EventData&)>;

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
		int subscribe(const DelegateType& listenerDelegate)
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

		void publish(const EventSender& sender, const EventData& payload)
		{
			for (int index = 0; index < _delegates->count(); index++)
			{
				(*(_delegates->getAt(index)->getValue()))(sender, payload);
			}
		}

	private:
		int _tokenCounter;

		simpleHash<int, DelegateType*>* _delegates;
	};
}
