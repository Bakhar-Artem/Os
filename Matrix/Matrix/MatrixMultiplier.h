#ifndef MATRIXMULTIPLIER_H
#define MATRIXMULTIPLIER_H
#include "Matrix.h"
#include <chrono>
#include <queue>
#include <thread>
#include <intsafe.h>

class Params
{
public:
	Params(const Matrix& first, const Matrix& second, Matrix& result, int i, int j) :first(first), second(second), result(result), i(i), j(j) {};
	~Params() {};
	const Matrix& getFirst()const { return first; };
	const Matrix& getSecond()const { return second; };
	Matrix& getResult() { return result; };
	int getI() {
		return i;
	}
	int getJ() { return j; };
	void setI(int i) { this->i = i; };
	void setJ(int j) { this->j = j; };
private:
	const Matrix& first;
	const Matrix& second;
	Matrix& result;
	int i;
	int j;
};
DWORD WINAPI threadMultiplyItoJ(LPVOID param) {
	Params* data = (Params*)param;
	const Matrix& first = data->getFirst();
	const Matrix& second = data->getSecond();
	Matrix& result = data->getResult();
	int i = data->getI();
	int j = data->getJ();

	double sum = 0;
	for (int s = 0; s < first.getN(); s++)
	{
		sum += first.getPos(i, s) * second.getPos(s, j);
	}
	result.setPos(i, j, sum);
	return 0;
}

class MatrixMultiplier
{
public:
	MatrixMultiplier():countOfThreads(0) {};
	MatrixMultiplier(int threads_):countOfThreads(threads_) {};


	Matrix DefaultMultiply(const Matrix& first, const Matrix& second) {
		if (first.getM() != second.getN()) {
			throw std::runtime_error("Impossible to multiply!");
		}
		std::vector<std::vector<double>> matrixV (first.getN()) ;
		for (int i = 0; i < first.getN(); i++)
		{
			matrixV[i].resize(second.getM());
		}
		Matrix matrix(matrixV, first.getN(), second.getM());
		for (int i = 0; i < first.getN(); i++)
		{
			for (int j = 0; j < second.getM(); j++)
			{
				matrix.setPos(i, j, multiplyItoJ(first,second,i, j));
			}
		}
		
		return matrix;
	}
	Matrix RowAndColMultiply(const Matrix& first,const Matrix& second) {
		HANDLE* handlesOfThread=new HANDLE[countOfThreads];
		int counter=0;
		Matrix result(first.getN(), second.getM());
		Params params(first, second, result, 0, 0);
		for (int i = 0; i < first.getN(); i++)
		{
			for (int j = 0; j <second.getM() ; j++)
			{
				params.setI(i);
				params.setJ(j);
				if (counter < countOfThreads) {
				handlesOfThread[counter]=(CreateThread(NULL, 0, &threadMultiplyItoJ, &params, 0, NULL));
				counter++;
				}
				else {
				int pos=WaitForMultipleObjects(countOfThreads,handlesOfThread,FALSE,INFINITE);
				handlesOfThread[pos] = (CreateThread(NULL, 0, &threadMultiplyItoJ, &params, 0, NULL));
				}
			}
			
		}
		WaitForMultipleObjects(countOfThreads, handlesOfThread, TRUE, INFINITE);
		return result;
	}
	~MatrixMultiplier() {};

private:
	int countOfThreads;
	double multiplyItoJ(const Matrix& first, const Matrix& second,int i, int j);


};

double MatrixMultiplier::multiplyItoJ(const Matrix& first,const Matrix& second,int i, int j) {
	double sum = 0;
	for (int s = 0; s < first.getN(); s++)
	{
		sum += first.getPos(i, s) * second.getPos(s, j);
	}
	return sum;
}

#endif // !MATRIXMULTIPLIER_H

