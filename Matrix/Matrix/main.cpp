
#include "Matrix.h"
#include "MatrixMultiplier.h"
#include <chrono>
#include <fstream>

void readMatrix(std::vector<Matrix>& matrixArray) {
	std::ifstream fin("data.txt");
	int n, m;
	fin >> n >> m;
	std::vector<std::vector<double>> vector(n);
	for (int i = 0; i < n; i++)
	{
		vector[i].resize(m);
	}
	double value;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			fin >> value;
			vector[i][j] = value;
		}
	}
	Matrix matrix1(vector, n, m);
	fin >> n >> m;
	std::vector<std::vector<double>> vector2(n);
	for (int i = 0; i < n; i++)
	{
		vector2[i].resize(m);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			fin >> value;
			vector2[i][j] = value;
		}
	}
	Matrix matrix2(vector2, n, m);
	matrixArray.push_back(matrix1);
	matrixArray.push_back(matrix2);
}
int main() {
	std::vector<Matrix> matrixArray;
	readMatrix(matrixArray);
	matrixArray[0].printMatrix();
	std::cout << "\n";
	matrixArray[1].printMatrix();
	std::cout << "\n";


	MatrixMultiplier multiplier(0);
	auto start = std::chrono::steady_clock::now();
	Matrix result=multiplier.DefaultMultiply(matrixArray[0], matrixArray[1]);
	auto finish = std::chrono::steady_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
	std::cout << time.count() << " ms\n";
	result.printMatrix();
	std::cout << "\n";


	MatrixMultiplier multiplier2(5);
	start = std::chrono::steady_clock::now();
	Matrix result2 = multiplier2.RowAndColMultiply(matrixArray[0], matrixArray[1]);
	finish = std::chrono::steady_clock::now();
	time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
	std::cout << time.count() << " ms\n";
	result2.printMatrix();
	std::cout << "\n";
	return 0;
}

