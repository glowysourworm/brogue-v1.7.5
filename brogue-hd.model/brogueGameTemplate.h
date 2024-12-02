#pragma once
#include "brogueLevelTemplate.h"
#include <simpleList.h>

namespace brogueHd::model
{
	using namespace simple;

	/// <summary>
	/// Primary data set for the game generation
	/// </summary>
	class brogueGameTemplate
	{
	public:
		brogueGameTemplate()
		{
			_levelTemplates = new simpleList<brogueLevelTemplate*>();
		}

		brogueGameTemplate(const brogueGameTemplate& copy)
		{
			_levelTemplates = new simpleList(*copy.getLevelTemplates());
		}

		~brogueGameTemplate()
		{
			delete _levelTemplates;
		}

		int getLevelCount() const
		{
			return _levelTemplates->count();
		}

		brogueLevelTemplate* getLevel(int depth)
		{
			return _levelTemplates->get(depth - 1);
		}

		void addLevel(brogueLevelTemplate* level)
		{
			_levelTemplates->add(level);
		}

	private:
		simpleList<brogueLevelTemplate*>* getLevelTemplates() const
		{
			return _levelTemplates;
		}

	private:
		simpleList<brogueLevelTemplate*>* _levelTemplates;
	};
}
