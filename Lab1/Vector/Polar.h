#pragma once
#include "Number.h"

class Polar
{
public:
	Polar(Number<double> radius,Number<double> rad);
	Polar& operator=(const Polar& right);
private:
	Number<double> radius;
	Number<double> radian;
};

Polar::Polar(Number<double> radius, Number<double> rad):radius(radius), radian(rad)
{
}

Polar& Polar::operator=(const Polar& right)
{
	if (this != &right) {
		this->radius = right.radius;
		this->radian = right.radian;
	}
	return *this;
}
