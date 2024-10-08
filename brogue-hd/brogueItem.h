#pragma once

#include "brogueObject.h"
#include "itemConstants.h"
#include "item.h"
#include <functional>

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model
{
	// Constraint for template function to help cast the brogue item details
	//
	template<typename T>
	concept itemBaseConstraint = requires(T a)
	{
		{ a } -> std::convertible_to<itemBase>;
	};

	class brogueItem : public brogueObject
	{

	public:

		brogueItem();
		~brogueItem();

		bool canBeDetected() const;
		bool prenamedCategory() const;
		bool neverIdentifiable() const;
		bool canBeSwapped() const;

		template<itemBaseConstraint T>
		T getDetail() const;

	private:
		
		itemBase _detail;
	};
}

