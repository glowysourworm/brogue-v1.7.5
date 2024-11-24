#pragma once

#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	struct cellularAutomataRule : hashable
	{
		cellularAutomataRule()
		{
			count = 5;
			polarity = true;
			countPolarity = true;
			resultPolarity = true;
		}
		cellularAutomataRule(short acount, bool apolarity, bool acountPolarity, bool aresultPolarity)
		{
			count = acount;
			polarity = apolarity;
			countPolarity = acountPolarity;
			resultPolarity = aresultPolarity;
		}
		cellularAutomataRule(const cellularAutomataRule& copy)
		{
			count = copy.count;
			polarity = copy.polarity;
			countPolarity = copy.countPolarity;
			resultPolarity = copy.resultPolarity;
		}
		void operator=(const cellularAutomataRule& copy)
		{
			count = copy.count;
			polarity = copy.polarity;
			countPolarity = copy.countPolarity;
			resultPolarity = copy.resultPolarity;
		}

		short count;
		bool polarity;
		bool countPolarity;
		bool resultPolarity;
	};

	class cellularAutomataParameters
	{
	public:

		cellularAutomataParameters()
		{
			_fillRatio = 0.55f;
			_rules = new simpleList<cellularAutomataRule>();			
			_iterations = 5;
		}

		cellularAutomataParameters(float afillRatio, 
								   short asmoothingIterations, 
								   const cellularAutomataRule& rule1, 
								   const cellularAutomataRule& rule2)
		{
			_fillRatio = 0.55f;
			_rules = new simpleList<cellularAutomataRule>();
			_iterations = 5;

			_rules->add(rule1);
			_rules->add(rule2);
		}

		cellularAutomataParameters(const cellularAutomataParameters& copy)
		{
			_fillRatio = copy.getFillRatio();
			_rules = copy.getRules();
			_iterations = copy.getIterations();
		}

		void operator=(const cellularAutomataParameters& copy)
		{
			_fillRatio = copy.getFillRatio();
			_rules = copy.getRules();
			_iterations = copy.getIterations();
		}

	public:

		float getFillRatio() const
		{
			return _fillRatio;
		}

		short getIterations() const
		{
			return _iterations;
		}

		simpleList<cellularAutomataRule>* getRules() const
		{
			return _rules;
		}

	private:

		float _fillRatio;
		short _iterations;
		simpleList<cellularAutomataRule>* _rules;
	};
}