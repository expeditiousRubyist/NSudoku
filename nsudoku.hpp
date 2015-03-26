// N-Sudoku Puzzle Solver
// Copyright (C) 2015 Rubyist

#ifndef __NSUDOKU_HPP__
#define __NSUDOKU_HPP__

#include <bitset>
#include <functional>

namespace sudoku {
	// Given the board state M, return true iff placing element t at index i
	// would not cause a duplicate entry in one of the rows, columns or subgrids
	template <typename IntegralType, std::size_t N>
	bool valid_move(const IntegralType *M, std::size_t i, IntegralType t) {
		constexpr auto sq =  N *  N; // N^2 constant (square)
		constexpr auto cb = sq *  N; // N^3 constant (cube)
		constexpr auto ts = sq * sq; // N^4 constant (tesseract)
		
		// Base indices - represents the offset of the first element of each of
		// i's column, row, and subgrid
		auto cbi = i % sq;
		auto rbi = i - cbi;
		auto sbi = (cbi - (cbi % N)) + (rbi - (rbi % cb));
		
		// Check if element in row
		for (auto j = rbi; j < rbi + sq; ++j)
			if (M[j] == t) return false;

		// Check if element in column
		for (auto j = cbi; j < ts; j += sq)
			if (M[j] == t) return false;

		// Check if element in subgrid
		for (auto ri = sbi; ri < sbi + cb; ri += sq)
			for (auto ci = 0; ci < N; ++ci)
				if (M[ri + ci] == t) return false;

		// If we have not failed by now, we can place a value there
		return true;
	}

	// Return true iff the board has been solved
	template <typename IntegralType, std::size_t N>
	bool is_solved(const IntegralType *M) {
		constexpr auto sq =  N *  N; // N^2 constant (square)
		constexpr auto cb = sq *  N; // N^3 constant (cube)
		constexpr auto ts = sq * sq; // N^4 constant (tesseract)
		
		// Set of booleans for each of the numbers in range 1..N^2 to tell
		// whether or not we found it in each of the rows, columns, and subgrids
		std::bitset<sq + 1> value_found;

		// Check for each row whether it contains all numbers 1..N^2
		for (auto ri = 0; ri < ts; ri += sq) {
			value_found.set(0, true);
			for (auto ci = 0; ci < sq; ++ci)
				value_found.set(M[ri+ci], true);
			if (value_found.all())
				value_found.reset();
			else return false;
		}

		// Do the same for columns
		for (auto ci = 0; ci < sq; ++ci) {
			value_found.set(0, true);
			for (auto ri = 0; ri < ts; ri += sq)
				value_found.set(M[ri+ci], true);
			if (value_found.all())
				value_found.reset();
			else return false;
		}
		
		// Do the same for all NxN subgrids
		for (auto sri = 0; sri < ts; sri += cb)
		for (auto sci = sri; sci < sri + sq; sci += N) {
			value_found.set(0, true);
			for (auto ri = sci; ri < sci + cb; ri += sq)
			for (auto ci = 0; ci < N; ++ci)
				value_found.set(M[ri + ci], true);
			if (value_found.all())
				value_found.reset();
			else return false;
		}

		// If we've passed all the tests, the board is solved!
		return true;
	}

	// Return true iff the board is in a valid initial state
	template <typename IntegralType, std::size_t N>
	bool solvable(IntegralType *M) {
		constexpr auto sq =  N *  N; // N^2 constant (square)
		constexpr auto ts = sq * sq; // N^4 constant (tesseract)

		// For each index, if there is a value already filled, temporarily
		// replace it with 0 and test if the value would work there. If any
		// value fails, return false.
		for (auto i = 0; i < ts; ++i) {
			auto temp = M[i];
			if (temp) {
				M[i] = 0;
				bool valid = valid_move<IntegralType, N>(M, i, temp);
				M[i] = temp;
				if (!valid) return false;
			}
		}

		// Board is solvable, return true
		return true;
	}
	
	// Solve the board, returning true when it is solved, or returning false
	// if the board was unsolvable to begin with
	template <typename IntegralType, std::size_t N>
	bool solve(IntegralType *M) {
		constexpr auto sq =  N *  N; // N^2 constant (square)
		constexpr auto ts = sq * sq; // N^4 constant (tesseract)

		// Helper function to do recursive backtracking over the grid
		std::function<bool(int)> attempt = [&](std::size_t i) -> bool {
			// Check if we've solved the puzzle if we're past the last element
			if (i == ts) return is_solved<IntegralType, N>(M);

			// Go to the next element if we already have an element here
			if (M[i]) return attempt(i+1);

			// Otherwise, try each number in 1..N^2 for the current space
			// If it works, try the next index
			for (IntegralType t = 1; t <= sq; ++t) {
				if (valid_move<IntegralType, N>(M, i, t)) {
					M[i] = t;
					if (attempt(i+1)) return true;
				}
			}

			// If it didn't work, we reset the solution to 0 and backtrack
			M[i] = 0;
			return false;
		};

		// Start helper function on index 0
		return attempt(0);
	}
}

#endif // __NSUDOKU_HPP__