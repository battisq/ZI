#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

namespace Mathematics
{
	template<class Type> class TRow
	{
	private:
		Type * row;
		int cols;

	public:

		TRow(int _col, Type *_row) : cols(_col), row(_row) {}
		
		~TRow()
		{
			row = NULL;
		}

		Type & operator[](int _j)
		{
			if (_j < 0 || _j >= cols)
				throw std::out_of_range("Неверный индекс столбца для матрицы\n");

			return row[_j];
		}

		const Type & operator[](int _j) const
		{
			if (_j < 0 || _j >= cols)
				throw std::out_of_range("Неверный индекс столбца для матрицы\n");

			return row[_j];
		}
	};

	template <class Type> class TMatrix
	{
	private:
		int rows;
		int cols;
		Type *array;

	public:

		TMatrix() : rows(0), cols(0), array(NULL) {}

		explicit TMatrix(int _rank, const Type * _matrix = NULL) : TMatrix(_rank, _rank, _matrix) {}

		TMatrix(int _rows, int _cols, const Type *_matrix = NULL)
		{
			if (_rows < 0 || _cols < 0)
				throw std::length_error("Непреемлимый размер матрицы\n");

			rows = _rows;
			cols = _cols;
			array = (rows * cols) ? (new Type[rows * cols]) : NULL;

			if (_matrix)
				for (int i = 0; i < rows * cols; i++)
					array[i] = _matrix[i];
			else 
				for (int i = 0; i < rows * cols; i++)
					array[i] = Type();
		}

		TMatrix(int _rows, int _cols, Type _el, ...) : TMatrix(_rows, _cols, &_el) {}

		TMatrix(const TMatrix<Type> & _a) : TMatrix(_a.rows, _a.cols, _a.array) {}

		TMatrix(TMatrix<Type> && _a) : TMatrix()
		{
			std::swap(rows, _a.rows);
			std::swap(cols, _a.cols);
			std::swap(array, _a.array);
		}

		~TMatrix()
		{
			if (array != NULL)
				delete[] array;

			array = NULL;
			rows = cols = 0;
		}

		bool checkSum(const TMatrix<Type> & _a) const
		{
			return rows == _a.rows && cols == _a.cols;
		}

		bool checkMul(const TMatrix<Type> & _a) const
		{
			return cols == _a.rows;
		}

		bool checkSum(const TMatrix<Type> & _a, const TMatrix<Type> & _b)
		{
			return _a.rows == _b.rows && _a.cols == _b.cols;
		}

		bool checkMul(const TMatrix<Type> & _a, const TMatrix<Type> & _b)
		{
			return _a.cols == _b.rows;
		}

		const Type & maxEl(void) const
		{
			if (!cols && !rows)
				throw std::logic_error("Max элемент матрицы не может быть определён. Матрица не инициализированна\n");

			int position = 0;

			for (int i = 1; i < rows * cols; i++)
				if (array[i] > array[position])
					position = i;

			return array[position];
		}

		const Type & minEl(void) const
		{
			if (!cols && !rows)
				throw std::logic_error("Min элемент матрицы не может быть определён. Матрица не инициализированна\n");

			int position = 0;

			for (int i = 1; i < rows * cols; i++)
				if (array[i] < array[position])
					position = i;

			return array[position];
		}

		TRow<Type> operator [] (int _i)
		{
			if (_i < 0 || _i >= rows)
				throw std::out_of_range("Неверный индекс строки для матрицы\n");

			return TRow<Type>(cols, array + _i * cols);
		}

		const TRow<Type> operator [] (int _i) const
		{
			if (_i < 0 || _i >= rows)
				throw std::out_of_range("Неверный индекс строки для матрицы\n");

			return TRow<Type>(cols, array + _i * cols);
		}

		TMatrix<Type> & operator = (const TMatrix<Type> & _a)
		{
			if (rows*cols != _a.rows*_a.cols)
			{
				if (array)
					delete[] array;

				if (!_a.array)
					array = NULL;
				else
					array = new Type[_a.rows * _a.cols];
			}

			rows = _a.rows;
			cols = _a.cols;

			for (int i = 0; i < rows*cols; i++)
				array[i] = _a.array[i];

			return *this;
		}

		TMatrix<Type> & operator = (TMatrix<Type> && _a)
		{
			std::swap(cols, _a.cols);
			std::swap(rows, _a.rows);
			std::swap(array, _a.array);

			return *this;
		}

		TMatrix<Type> & operator = (const Type & _number)
		{
			if (rows != 1 || cols != 1)
			{
				rows = cols = 1;

				if (array != NULL)
					delete[] array;
	
				array = new Type[1];
			}

			array[0] = _number;

			return *this;

		}

		TMatrix<Type> & operator += (const TMatrix<Type> & _a)
		{
			if (!checkSum(_a))
			{
				if (!_a.cols*_a.rows)
					throw std::logic_error("Сложение невозможно. Матрица не инициализирована\n");
				else if (!cols*rows)									 
					throw std::logic_error("Сложение невозможно. Матрица не инициализирована\n");
				else
					throw std::logic_error("Сложение невозможно. Размер матриц не соответствует условию суммы\n");
			}

			for (int i = 0; i < rows*cols; i++)
				array[i] += _a.array[i];

			return *this;
		}

		TMatrix<Type> & operator -= (const TMatrix<Type> & _a)
		{
			if (!checkSum(_a))
			{
				if (!_a.cols*_a.rows)
					throw std::logic_error("Вычитание невозможно. Матрица не инициализирована\n");
				else if (!cols*rows)
					throw std::logic_error("Вычитание невозможно. Матрица не инициализирована\n");
				else
					throw std::logic_error("Вычитание невозможно. Размер матриц не соответствует условию разности\n");
			}

			for (int i = 0; i < rows*cols; i++)
				array[i] -= _a.array[i];

			return *this;
		}

		TMatrix<Type> & operator *= (const TMatrix<Type> & _a)//		
		{
			if (!checkMul(_a))
			{
				if (!_a.cols*_a.rows)
					throw std::logic_error("Умножение невозможно. Матрица не инициализирована\n");
				else if (!cols*rows)
					throw std::logic_error("Умножение невозможно. Матрица не инициализирована\n");
				else
					throw std::logic_error("Умножение невозможно. Размер матриц не соответствует условию произведения\n");
			}

			TMatrix<Type> newMatrix(rows, _a.cols);

			for (int i = 0; i < rows; i++)
				for (int j = 0; j < _a.cols; j++)
				{
					newMatrix.array[i * _a.cols + j] = this->array[i * cols] * _a.array[j];

					for (int k = 1; k < cols; k++)
						newMatrix.array[i * _a.cols + j] += this->array[i * cols + k] * _a.array[k * _a.cols + j];
				}

			*this = std::move(newMatrix);

			return *this;
		}

		TMatrix<Type> & operator *= (const Type &_number)
		{
			for (int i = 0; i < rows*cols; i++)
				array[i] *= _number;

			return *this;
		}

		int getRows(void) const
		{
			return rows;
		}

		int getCols(void) const
		{
			return cols;
		}

		friend std::ostream & operator << (std::ostream &_out, const TMatrix<Type> &_a)
		{
			std::streamsize width = _out.width();

			for (int i = 0; i < _a.rows; i++)
			{
				for (int j = 0; j < _a.cols; j++)
				{
					_out.width(width);
					_out << _a.array[i * _a.cols + j];

					if (!width)
						std::cout << " ";
				}
				_out << "\n";
			}

			return _out;
		}

		friend std::istream & operator >> (std::istream & _in, const TMatrix<Type> & _a)
		{
			for (int i = 0; i < _a.cols*_a.rows; i++)
			{
				_in >> _a.array[i];
			}

			return _in;
		}

		friend TMatrix<Type> operator + (const TMatrix<Type> &_a, const TMatrix<Type> &_b)
		{
			TMatrix<Type> c(_a);

			return c += _b;
		}

		friend TMatrix<Type> operator - (const TMatrix<Type> &_a, const TMatrix<Type> &_b)
		{
			TMatrix<Type> c(_a);

			return c -= _b;
		}

		friend TMatrix<Type> operator * (const TMatrix<Type> &_a, const TMatrix<Type> &_b)
		{
			TMatrix<Type> c(_a);

			return c *= _b;
		}

		friend TMatrix<Type> operator * (const TMatrix<Type> &_a, const Type & _number)
		{
			TMatrix<Type> c(_a);

			return c *= _number;
		}

		friend TMatrix<Type> operator * (const Type & _number, const TMatrix<Type> &_a)
		{
			TMatrix<Type> c(_a);

			return c *= _number;
		}

		friend bool operator > (const TMatrix<Type> &_a, const TMatrix<Type> &_b)
		{
			return _a.rows * _a.cols > _b.rows * _b.cols;
		}

		friend bool operator < (const TMatrix<Type> &_a, const TMatrix<Type> &_b)
		{
			return _a.rows * _a.cols < _b.rows * _b.cols;
		}
	};
}