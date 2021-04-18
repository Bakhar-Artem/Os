#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <vector>
#include <windows.h>
#include <mutex>

std::mutex lock;
class Matrix
{
public:
	Matrix(const std::vector<std::vector<double>>& matrix_, int n_, int m_) :matrix(matrix_), n(n_), m(m_) {};
	Matrix(int n_, int m_) : n(n_), m(m_) {
		matrix.resize(n);
		for (int i = 0; i < n; i++)
		{
			matrix[i].resize(m);
		}
	};
	~Matrix() {};
	bool checkPos(int i, int j)const {
		bool flag = (i >= n || i < 0 || j >= m || j < 0);
		return flag;
	}
	double getPos(int i, int j)const {
		if (checkPos(i, j)) {
			throw std::runtime_error("Out of matrix");
		}
		return matrix[i][j];
	}
	void setPos(int i, int j,double value) {
		if (checkPos(i, j)) {
			throw std::runtime_error("Out of matrix");
		}
		matrix[i][j]=value;
	}
	int getN()const {
		return n;
	}
	int getM()const {
		return m;
	}
	void printMatrix() {
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				std::cout << matrix[i][j] << " ";
			}
			std::cout << "\n";
		}
	}

private:
	std::vector<std::vector<double>> matrix;
	const int n;
	const int m;
};

#endif // !MATRIX_H
