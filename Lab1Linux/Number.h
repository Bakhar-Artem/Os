#pragma once
#include <iostream>
#include <cmath>
template <typename T>
class Number
{
public:
	Number(const T& data) :data(data) {};
	Number(const Number& copy);
	Number& operator+(const Number&)const;
	Number& operator-(const Number&)const;
	Number& operator*(const Number&)const;
	Number<double>& operator/(const Number&)const;
	Number& operator=(const Number&);
	Number<double>& sqrt();
	template <typename T2>
	friend std::ostream& operator<<(std::ostream& , const Number<T2>&);
	friend Number& atan(const Number& x, const Number& y);
private:
	T data=0;
};


template <typename T>
Number<double>& atan(const Number<T>& x, const Number<T>& y) {
	return &(new Number<T>(y.data*1.0 / x.data));
}
template <typename T>
std::ostream& operator<<(std::ostream& out, const Number<T>& number) {
	out << number.data;
	return out;
}
template<typename T>
Number<T>::Number(const Number& copy)
{
	this->data = copy.data;
}
template<typename T>
Number<T>& Number<T>::operator+(const Number& right) const
{
	return *(new Number(this->data + right.data));
}

template<typename T>
Number<T>& Number<T>::operator-(const Number& right) const
{
	return *(new Number(this->data - right.data));
}


template<typename T>
Number<T>& Number<T>::operator*(const Number& right) const
{
	return *(new Number(this->data * right.data));
}


template<typename T>
Number<double>& Number<T>::operator/(const Number& right) const
{
	if (right.data != 0) {
		return *(new Number<double>(this->data*1.0 / right.data));
	}
	else return *(new Number<double>(0.0));
}

template<typename T>
Number<T>& Number<T>::operator=(const Number& right)
{
	if (this != &right) {
		this->data = right.data;
	}
	return *this;
}

template<typename T>
Number<double>& Number<T>::sqrt()
{
	return *(new Number<double>(std::sqrt(this->data)));

}

