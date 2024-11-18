#pragma once
#include "brogueLevelProfile.h"
#include "dungeonConstants.h"
#include "simple.h"

namespace brogueHd::backend::model
{
	class brogueLevelTemplate 
	{
	public:
		brogueLevelTemplate()
		{
			_profile = nullptr;
		}
		brogueLevelTemplate(short depth, levelTypes types) 
		{
			_profile = new brogueLevelProfile(depth, types);
		};
		~brogueLevelTemplate() 
		{
			delete _profile;
		};

		brogueLevelProfile* getProfile() const
		{
			return _profile;
		}

	private:

		brogueLevelProfile* _profile;
	};
}