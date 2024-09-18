#pragma once

#include "brogueItem.h"

namespace brogueHd::backend::model
{
	brogueItem::brogueItem()
	{

	}
	brogueItem::~brogueItem()
	{

	}

	template<itemBaseConstraint T>
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