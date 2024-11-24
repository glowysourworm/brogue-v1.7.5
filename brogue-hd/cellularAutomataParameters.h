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
		}
		cellularAutomataRule(short acount, bool apolarity, bool acountPolarity)
		{
			count = acount;
			polarity = apolarity;
			countPolarity = acountPolarity;
		}
		cellularAutomataRule(const cellularAutomataRule& copy)
		{
			count = copy.count;
			polarity = copy.polarity;
			countPolarity = copy.countPolarity;
		}
		void operator=(const cellularAutomataRule& copy)
		{
			count = copy.count;
			polarity = copy.polarity;
			countPolarity = copy.countPolarity;
		}

		/// <summary>
		/// Count at which to apply the rule
		/// </summary>
		short count;

		/// <summary>
		/// Polarity of the cell to check (false for wall). This is the polarity for which
		/// to apply the rule. If it doesn't match the cell, the rule will not be applied.
		/// </summary>
		bool polarity;

		/// <summary>
		/// Count polarity - set false to count walls
		/// </summary>
		bool countPolarity;
	};

	class cellularAutomataParameters
	{
	public:

		cellularAutomataParameters()
		{
			_fillRatio = 0.55f;
			_rules = new simpleList<cellularAutomataRule>();			
			_iterations = 5;
			_resultPolarity = true;
		}

		cellularAutomataParameters(float fillRatio, 
								   short smoothingIterations, 
								   bool resultPolarity,
								   const cellularAutomataRule& rule1, 
								   const cellularAutomataRule& rule2)
		{
			_fillRatio = fillRatio;
			_rules = new simpleList<cellularAutomataRule>();
			_iterations = smoothingIterations;
			_resultPolarity = resultPolarity;

			_rules->add(rule1);
			_rules->add(rule2);
		}

		cellularAutomataParameters(const cellularAutomataParameters& copy)
		{
			_fillRatio = copy.getFillRatio();
			_rules = copy.getRules();
			_iterations = copy.getIterations();
			_resultPolarity = copy.getResultPolarity();
		}

		void operator=(const cellularAutomataParameters& copy)
		{
			_fillRatio = copy.getFillRatio();
			_rules = copy.getRules();
			_iterations = copy.getIterations();
			_resultPolarity = copy.getResultPolarity();
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

		bool getResultPolarity() const
		{
			return _resultPolarity;
		}

	private:

		float _fillRatio;
		short _iterations;
		bool _resultPolarity;
		simpleList<cellularAutomataRule>* _rules;
	};
}