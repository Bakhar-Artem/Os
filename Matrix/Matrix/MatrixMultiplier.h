#ifndef MATRIXMULTIPLIER_H
#define MATRIXMULTIPLIER_H
#include "Matrix.h"
#include <chrono>
#include <queue>
#include <thread>

void threadMultiplyItoJ(const Matrix& first, const Matrix& second, Matrix& result, int i, int j) {
	double sum = 0;
	for (int s = 0; s < first.getN(); s++)
	{
		sum += first.getPos(i, s) * second.getPos(s, j);
	}
	result.setPos(i, j, sum);
	ExitThread(0);
}

class MatrixMultiplier
{
public:
	MatrixMultiplier():threads(0) {};
	MatrixMultiplier(int threads_):threads(threads_) {};

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
	Matrix RowAndColMultiply(const Matrix& first, const Matrix& second) {
		std::queue<std::thread> queueOfThread;
		int counter=0;
		Matrix result(first.getN(), second.getM());
		for (int i = 0; i < first.getN(); i++)
		{
			for (int j = 0; j <second.getM() ; j++)
			{
			if (counter < threads) {
				queueOfThread.push(std::thread(threadMultiplyItoJ, std::cref(first), std::cref(second), std::ref(result), i, j));
				counter++;
			}
			else {
				queueOfThread.front().join();
				queueOfThread.pop();
				queueOfThread.push(std::thread(threadMultiplyItoJ, std::cref(first), std::cref(second), std::ref(result), i, j));
			}
			}
			
		}
		while (!queueOfThread.empty()) {
			queueOfThread.front().join();
			queueOfThread.pop();
		}
		return result;

	}
	~MatrixMultiplier() {};

private:
	int threads;
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

