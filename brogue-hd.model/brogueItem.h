#pragma once

#include "brogueObject.h"
#include "item.h"
#include "itemConstants.h"
#include <type_traits>

namespace brogueHd::model
{
	// Constraint for template function to help cast the brogue item details
	//
	template <typename T>
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

		template <itemBaseConstraint T>
		T getDetail() const;

	private:
		itemBase _detail;
	};

	brogueItem::brogueItem()
	{
	}

	brogueItem::~brogueItem()
	{
	}

	template <itemBaseConstraint T>
	T brogueItem::getDetail() const
	{
		return (T)_detail;
	}

	bool brogueItem::canBeDetected() const
	{
		return (_detail.category & itemCategory::WEAPON) != 0 ||
			(_detail.category & itemCategory::ARMOR) != 0 ||
			(_detail.category & itemCategory::POTION) != 0 ||
			(_detail.category & itemCategory::SCROLL) != 0 ||
			(_detail.category & itemCategory::RING) != 0 ||
			(_detail.category & itemCategory::CHARM) != 0 ||
			(_detail.category & itemCategory::WAND) != 0 ||
			(_detail.category & itemCategory::STAFF) != 0 ||
			(_detail.category & itemCategory::AMULET) != 0;
	}

	bool brogueItem::prenamedCategory() const
	{
		return (_detail.category & itemCategory::FOOD) != 0 ||
			(_detail.category & itemCategory::GOLD) != 0 ||
			(_detail.category & itemCategory::AMULET) != 0 ||
			(_detail.category & itemCategory::GEM) != 0 ||
			(_detail.category & itemCategory::KEY) != 0;
	}

	bool brogueItem::neverIdentifiable() const
	{
		return (_detail.category & itemCategory::FOOD) != 0 ||
			(_detail.category & itemCategory::CHARM) != 0 ||
			(_detail.category & itemCategory::GOLD) != 0 ||
			(_detail.category & itemCategory::AMULET) != 0 ||
			(_detail.category & itemCategory::GEM) != 0 ||
			(_detail.category & itemCategory::KEY) != 0;
	}

	bool brogueItem::canBeSwapped() const
	{
		return (_detail.category & itemCategory::WEAPON) != 0 ||
			(_detail.category & itemCategory::ARMOR) != 0 ||
			(_detail.category & itemCategory::STAFF) != 0 ||
			(_detail.category & itemCategory::RING) != 0 ||
			(_detail.category & itemCategory::CHARM) != 0;
	}
}
