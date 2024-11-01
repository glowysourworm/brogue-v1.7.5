#pragma once

#include "simpleList.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	struct simpleKeyState
	{
	public:
		simpleKeyState()
		{
			_keys = new simpleList<int>();
			_modifiers = 0;
		}
		simpleKeyState(const simpleKeyState& copy)
		{
			_keys = copy.getKeys();
			_modifiers = copy.getModifiers();
		}
		~simpleKeyState()
		{
			delete _keys;
		}

		void addKey(int key)
		{
			if (!_keys->contains(key))
				_keys->add(key);
		}
		void removeKey(int key)
		{
			if (_keys->contains(key))
				_keys->remove(key);
		}
		bool hasKey(int key)
		{
			return _keys->contains(key);
		}

		void setModifiers(int modifiers)
		{
			_modifiers = modifiers;
		}

		int getModifiers() const
		{
			return _modifiers;
		}

	protected:

		simpleList<int>* getKeys() const
		{
			return _keys;
		}

	private:

		simpleList<int>* _keys;
		
		int _modifiers;
	};
}