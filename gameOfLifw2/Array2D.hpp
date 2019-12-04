#pragma once
#ifndef ARRAY2D_H_
#define ARRAY2D_H_
template <class T>
class Array2D
{
private:
	T** elems;
	int ROWS, COLS;

	void setArr2D(const int& rows, const int& cols)
	{
		this->ROWS = rows;
		this->COLS = cols;

		elems = new T * [COLS];
		for (int i = 0; i < COLS; i++)
		{
			elems[i] = new T[ROWS];
		}
	}

	void deallocate()
	{
		if (NULL == elems)
		{
			return;
		}

		for (int i = 0; i < COLS; i++)
		{
			delete[] elems[i];
		}
		delete[] elems;

		ROWS = 0;
		COLS = 0;
		elems = NULL;
	}
	//check if values passed are valid
	void checkRange(int row, int col)
	{
		if (col >= this->getCols() || col < 0 || row >= this->getRows() || row < 0)
		{
			throw InvalidCoordinate();
		}
	}

public:
	Array2D()
	{
		elems = NULL;
		ROWS = 0;
		COLS = 0;
	}

	Array2D(const int& rows, const int& cols)
	{
		elems = NULL;
		setArr2D(rows, cols);
	}
	//Copy constructors
	Array2D(const Array2D& arr)
	{
		COLS = arr.COLS;
		ROWS = arr.ROWS;
		setArr2D(ROWS, COLS);
		for (int col = 0; col < COLS; col++)
		{
			for (int row = 0; row < ROWS; row++)
			{
				elems[col][row] = arr.elems[col][row];
			}
		}
	}
	Array2D& operator=(const Array2D& arr)
	{
		if (this != &arr)
		{
			COLS = arr.COLS;
			ROWS = arr.ROWS;
			setArr2D(ROWS, COLS);
			for (int col = 0; col < COLS; col++)
			{
				for (int row = 0; row < ROWS; row++)
				{
					elems[col][row] = arr.elems[col][row];
				}
			}
		}
		return *this;
	}
	~Array2D()
	{
		deallocate();
	}

	T getElem(const int& row, const int& col)
	{
		checkRange(row, col);
		return elems[col][row];
	}

	void setElem(const T& elem, const int& row, const int& col)
	{
		checkRange(row, col);
		elems[col][row] = elem;
	}

	void setArr(const T& elem)
	{

		for (int col = 0; col < COLS; col++)
		{
			for (int row = 0; row < ROWS; row++)
			{
				elems[col][row] = elem;
			}
		}
	}

	int getRows()
	{
		return ROWS;
	}

	int getCols()
	{
		return COLS;
	}
	//exceptions class
	class InvalidCoordinate
	{
	};
};
#endif