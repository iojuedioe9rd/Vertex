#pragma once

// Defile a type alias for a 2D array using 'using', templated on both rows, cols, and type T
template <typename T, int rows, int cols>
using TwoDimArray = T[rows][cols];


// Function to convert a 1D array to a 2D array and return it by reference
template <typename T, int rows, int cols>
TwoDimArray<T, rows, cols>& convert1DTo2D(T(&oneDimArray)[rows * cols])
{
	T twoDimArray[rows, cols];

	// Fill the 2D array using the values from the 1D array
	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			twoDimArray[row][col] = oneDimArray[row * cols + col];
		}
	}

	// Return the reference to the 2D array
	return twoDimArray;
}