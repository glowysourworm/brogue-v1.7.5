#pragma once

#include "cellularAutomataParameters.h"
#include "gridDefinitions.h"
#include "randomGenerator.h"
#include "gridRect.h"
#include <functional>

using namespace std;

using namespace brogueHd::backend::generator;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model::construction
{
	/// <summary>
	/// Constructs the primary grid for the level
	/// </summary>
	template<isGridLocator T>
	class gridConstructor
	{
	public:

		gridConstructor(const gridRect& boundary, gridDelegates<T>::constructor cellConstructor);
		~gridConstructor();

		/// <summary>
		/// Returns the new grid pointer (does not delete)
		/// </summary>
		/// <returns></returns>
		grid<T>* getResult() const;

		/// <summary>
		/// Sets value for rectangle on the grid
		/// </summary>
		void setRectangle(short x, short y, short width, short height);

		/// <summary>
		/// Sets value for a circle on the grid
		/// </summary>
		void setCircle(short x, short y, short radius);

		/// <summary>
		/// Sets the intersection of the grid with the other grid
		/// </summary>
		void setIntersection(const grid<T>* otherGrid);

		/// <summary>
		/// Sets the union of the grid with the other grid
		/// </summary>
		void setUnion(const grid<T>* otherGrid, bool copyCells = false);

		/// <summary>
		/// Sets value of grid based on provided predicate
		/// </summary>
		/// <param name="predicate">function that gets set based on the current grid (column, row, and value)</param>
		void setFor(function<bool(short, short, T)> predicate, bool existingPermissive = true);

		/// <summary>
		/// Generates cellular automata with the specified parameters. NOTE*** Smoothing Passes needs to be less than or equal to 10.
		/// </summary>
		void generateCellularAutomata(randomGenerator* randomGenerator, cellularAutomataParameters parameters);

	private:

		/// <summary>
		/// Inserts (guarded) new cell at the specified location
		/// </summary>
		void insertNew(short column, short row);

		/// <summary>
		/// Removes (guarded) existing cell at the specified location
		/// </summary>
		void remove(short column, short row);

		/// <summary>
		/// Iteration of cellular automata using the given parameters
		/// </summary>
		void gridConstructor<T>::cellularAutomataIteration(cellularAutomataParameters parameters);

	private:

		grid<T>* _grid;

		gridDelegates<T>::constructor _cellConstructor;
	};
}