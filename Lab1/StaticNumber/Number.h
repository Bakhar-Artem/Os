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
	Number& operator/(const Number&)const;
	Number& operator=(const Number&);
	void sqrt();
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
Number<T>& Number<T>::operator/(const Number& right) const
{
	if (right.data != 0) {
		return *(new Number(this->data + right.data));
	}
	else return *(new Number(0));
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
void Number<T>::sqrt()
{
	this->data=std::sqrt(this->data);
}

