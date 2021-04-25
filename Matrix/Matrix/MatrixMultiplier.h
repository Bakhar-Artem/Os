#ifndef MATRIXMULTIPLIER_H
#define MATRIXMULTIPLIER_H
#include "Matrix.h"
#include <chrono>
#include <queue>
#include <thread>
#include <cmath>

class MatrixMultiplier;
std::vector<HANDLE> colAndRow;
HANDLE mutexForBlocks = CreateMutex(NULL, FALSE, NULL);
double multiplyItoJ(const Matrix& first, const Matrix& second, int i, int j);
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
class BlocksParams
{
public:
	BlocksParams(const Matrix& first, const Matrix& second, Matrix& result, int i1,int i2, int j1,int j2) :first(first), second(second), result(result), i1(i1),i2(i2), j1(j1),j2(j2) {};
	~BlocksParams() {};
	const Matrix& getFirst()const { return first; };
	const Matrix& getSecond()const { return second; };
	Matrix& getResult() { return result; };
	int getI1() {
		return i1;
	}
	int getJ1() { return j1; };
	int getI2() {
		return i2;
	}
	int getJ2() { return j2; };
	void setI1(int i1) { this->i1 = i1; };
	void setJ1(int j1) { this->j1 = j1; };
	void setI2(int i2) { this->i2 = i2; };
	void setJ2(int j2) { this->j2 = j2; };

private:
	const Matrix& first;
	const Matrix& second;
	Matrix& result;
	int i1;
	int i2;
	int j1;
	int j2;
};

DWORD WINAPI threadMultiplyItoJ(LPVOID param) {
	Params* data = (Params*)param;
	const Matrix& first = data->getFirst();
	const Matrix& second = data->getSecond();
	Matrix& result = data->getResult();
	int i = data->getI();
	int j = data->getJ();
	double sum = 0;
	for (int s = 0; s < first.getM(); s++)
	{
		sum += first.getPos(i, s) * second.getPos(s, j);
	}
	result.setPos(i, j, sum);
	ExitThread(0);
}

DWORD WINAPI threadMultiplyJtoI(LPVOID param) {
	
	Params* data = (Params*)param;
	const Matrix& first = data->getFirst();
	const Matrix& second = data->getSecond();
	Matrix& result = data->getResult();
	int i1 = data->getI();
	int j1 = data->getJ();
	for (int i = 0; i < result.getN(); i++)
	{
		WaitForSingleObject(colAndRow[i], INFINITE);
		for (int j = 0; j < result.getM(); j++)
		{
			double temp=result.getPos(i, j);
			temp += first.getPos(i,i1) * second.getPos(j,i);
			result.setPos(i, j, temp);
		}
		ReleaseMutex(colAndRow[i]);
	}
	ExitThread(0);
}

DWORD WINAPI threadMultiplyblocks(LPVOID param) {

	BlocksParams* data = (BlocksParams*)param;
	BlocksParams p1 = data[0];
	BlocksParams p2 = data[1];
	const Matrix& first = p1.getFirst();
	const Matrix& second = p1.getSecond();
	Matrix& result = p1.getResult();
	int i11 = p1.getI1();
	int j11 = p1.getJ1();
	int i12 = p1.getI2();
	int j12 = p1.getJ2();
	int i21 = p2.getI1();
	int j21 = p2.getJ1();
	int i22 = p2.getI2();
	int j22 = p2.getJ2();
	std::vector<std::vector<double>> firstMatrix(i12 - i11);
	for (size_t i = i11; i < i12; i++)
	{
		for (size_t j = j11; j < j12; j++)
		{
			firstMatrix[i].push_back(first.getPos(i, j));
		}
	}
	Matrix firstBlock(firstMatrix, i12 - i11, j12 - j11);
	std::vector<std::vector<double>> secondMatrix(i22 - i21);
	for (size_t i = i21; i < j12; i++)
	{
		for (size_t j = j21; j < j22; j++)
		{
			secondMatrix[i].push_back(first.getPos(i, j));
		}
	}
	Matrix secondBlock(secondMatrix, i22 - i21, j22 - j21);
	MatrixMultiplier multy;
	Matrix block(multy.DefaultMultiply(firstBlock, secondBlock));
	WaitForSingleObject(mutexForBlocks, INFINITE);
	for (int i = 0; i < block.getN(); i++)
	{
		for (size_t j = 0; j < block.getM(); j++)
		{
			int temp = result.getPos(i11, j21);
			temp += block.getPos(i, j);
			result.setPos(i11, j21,temp);
			j21++;
		}
		i11++;
	}
	ReleaseMutex(mutexForBlocks);
	ExitThread(0);
}


class MatrixMultiplier
{
public:
	MatrixMultiplier():countOfThreads(0) {};
	MatrixMultiplier(int threads_):countOfThreads(threads_) {};


	Matrix& DefaultMultiply(const Matrix& first, const Matrix& second) {
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
	Matrix RowAndColMultiply(const Matrix& first, const Matrix& second) {
		HANDLE* handlesOfThread = new HANDLE[countOfThreads];
		int counter=0;
		Matrix result(first.getN(), second.getM());
		
		for (int i = 0; i < first.getN(); i++)
		{
			for (int j = 0; j <second.getM() ; j++)
			{
				Params* params= new Params(first, second, result,i, j);
				if (counter < countOfThreads) {
				handlesOfThread[counter]=(CreateThread(NULL, 0, &threadMultiplyItoJ, params, 0, NULL));
				counter++;
				}
				else {
				int pos=WaitForMultipleObjects(countOfThreads,handlesOfThread,FALSE,INFINITE);
				handlesOfThread[pos] = (CreateThread(NULL, 0, &threadMultiplyItoJ, params, 0, NULL));
				}
			}
			
		}
		WaitForMultipleObjects(countOfThreads, handlesOfThread, TRUE, INFINITE);

		return result;
	}
	Matrix ColAndRowMultiply(const Matrix& first, const Matrix& second) {
	HANDLE* handlesOfThread = new HANDLE[countOfThreads];
	int counter = 0;
	colAndRow = std::vector<HANDLE>(first.getN());
	for (size_t i = 0; i < colAndRow.size(); i++)
	{
		colAndRow[i] = CreateMutex(NULL, FALSE, NULL);
	}
	Matrix result(first.getN(), second.getM());
	for (int i = 0; i < first.getN(); i++){
		for (int j = 0; j < second.getM(); j++)
		{
			Params* params = new Params(first, second, result, i, j);
			if (counter < countOfThreads) {
				handlesOfThread[counter] = (CreateThread(NULL, 0, &threadMultiplyItoJ, params, 0, NULL));
				counter++;
			}
			else {
				int pos = WaitForMultipleObjects(counter, handlesOfThread, FALSE, INFINITE);
				handlesOfThread[pos] = (CreateThread(NULL, 0, &threadMultiplyItoJ, params, 0, NULL));
			}
		}
	}
		WaitForMultipleObjects(counter, handlesOfThread, TRUE, INFINITE);

		return result;
	}
	Matrix BlocksMultiply(const Matrix& first, const Matrix& second) {// для перемножения блоков сложно придумать что-то адекватное, поэтому я изначально передаю матрицы, у которых число строк и столбцов это квадрат какого-либо натурального числа 
		HANDLE* handlesOfThread = new HANDLE[countOfThreads];
		int counter = 0;
		Matrix result(first.getN(), second.getM());
		int k11 = pow(first.getN(), 1. / 2);
		int k12 = pow(first.getM(), 1. / 2);
		int k21 = pow(second.getN(), 1. / 2);
		int k22 = pow(second.getM(), 1. / 2);
		for (int i = 0; i < first.getN(); i+=k11) {
			for (int j = 0; j < second.getM(); j+=k22)
			{
				BlocksParams* params = new BlocksParams[2]{ BlocksParams(first, second, result, i,i+k11, j,j+k12),BlocksParams(first, second, result, i,i + k21, j,j + k22) };
					
				if (counter < countOfThreads) {
					handlesOfThread[counter] = (CreateThread(NULL, 0, &threadMultiplyblocks, params, 0, NULL));
					counter++;
				}
				else {
					int pos = WaitForMultipleObjects(counter, handlesOfThread, FALSE, INFINITE);
					handlesOfThread[pos] = (CreateThread(NULL, 0, &threadMultiplyblocks, params, 0, NULL));
				}
			}
		}
		WaitForMultipleObjects(counter, handlesOfThread, TRUE, INFINITE);

		return result;
	}
	~MatrixMultiplier() {};
private:
	int countOfThreads;
};

double multiplyItoJ(const Matrix& first,const Matrix& second,int i, int j) {
	double sum = 0;
	for (int s = 0; s < first.getM(); s++)
	{
		sum += first.getPos(i, s) * second.getPos(s, j);
	}
	return sum;
}

#endif // !MATRIXMULTIPLIER_H

