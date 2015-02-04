#ifndef ARRAY_2D_HPP
#define ARRAY_2D_HPP

#include <iostream>
#include <iomanip>

namespace Engine
{
	/**
	 * Encapsulation of a 2D array.
	 */
	class Array2D
	{
		public:
			/**
			 * Creates a Array2D object.
			 */
			Array2D()
			{
				// assign the array size
				n = 0;
			}

			/**
			 * Creates a Array2D object and allocates array.
			 * \param nParam the width and height of the array.
			 */
			explicit Array2D(int nParam)
			{
				// assign the array size
				n = nParam;

				// allocate the array
				allocate();
			}

			/**
			 * Destroys this Array2D object.
			 */
			virtual ~Array2D()
			{
				if (matrix) delete [] matrix;
			}

			/**
			 * Allocates the array onto the heap.
			 */
			void allocate()
			{
				if (matrix) delete [] matrix;
				matrix = new int[n*n];
			}

			/**
			 * Print the values currently held in the allocated array.
			 */
			void print()
			{
				for (int x = 0; x < n; x++)
				{
					for (int y = 0; y < n; y++)
					{
						if (getElement(x, y) < 0)
						{
							std::cout << std::setw(2) << std::left << "[" << getElement(x, y) << "]";
						} else {
							std::cout << std::setw(3) << std::left << "[" << getElement(x, y) << "]";
						}
					}
					std::cout << std::endl;
				}
			}

			/**
			 * Fill the two dimensional array with a particular value.
			 * \aparam fillValueParam the value to assign to every element of the array.
			 */
			void fill(int fillValueParam)
			{
				for (int x = 0; x < n; x++)
				{
					for (int y = 0; y < n; y++)
					{
						setElement(x, y, fillValueParam);
					}
				}
			}

			/**
			 * Sets the value for an element in the array.
			 * \param rowOffsetParam x offset in the array.
			 * \param columnOffsetParam y offset in the array.
			 * \param newValue the new value that coordinate [x][y] will contain.
			 */
			void setElement(int rowOffsetParam, int columnOffsetParam, int newValue)
			{
				int temp = columnOffsetParam + (rowOffsetParam * n); // determine element
				// deprecated: testing purposes
				//cout << "xOffsetParam: " << rowOffsetParam << endl;
				//cout << "yOffsetParam: " << columnOffsetParam << endl;
				//cout << "newValue: "<< newValue << endl;
				//cout << "temp: " << temp << endl;
				matrix[temp] = newValue;
			}

			/**
			 * Sets the value of an array element.
			 * \param rowOffsetParam x offset in the array.
			 * \param columnOffsetParam y offset in the array.
			 * \return array element in the coordinate [x][y] in the array.
			 */
			int getElement(int rowOffsetParam, int columnOffsetParam)
			{
				int temp = columnOffsetParam + (rowOffsetParam * n); // determine element
				return matrix[temp];
			}

			/**
			 * Gets the size of the array.
			 * \param nParam the width and height of the array.
			 */
			void setSize(int nParam)
			{
				n = nParam;
			}

			/**
			 * Gets the size of the array.
			 * \return the width and height of the array.
			 */
			int getSize()
			{
				return n;
			}

		protected:

		private:
			int n;
			int* matrix;
	};
}

#endif