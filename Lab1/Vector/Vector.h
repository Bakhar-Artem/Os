#pragma once
#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif
#include "Number.h"
#include "Polar.h"
template <typename T>
class Vector
{
public:
	Vector(Number<T> x=0, Number<T> y = 0);
	void add(const Vector<T>& right);

private:
	Number<T> x;
	Number<T> y;
	Polar polar;
};

template<typename T>
Vector<T>::Vector(Number<T> x, Number<T> y) :x(x), y(y)
{
	Number<double> radius;
	radius = x * x;
	radius = radius + y * y;
	radius.sqrt();
	Number <double> radian(x, y);
	this->polar = Polar(radius, radian);
}

template<typename T>
void Vector<T>::add(const Vector<T>& right)
{
	this->x += right.x;
	this->y += right.y;
}
