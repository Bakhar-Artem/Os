#pragma once
#include <iostream>
template <typename T>
class Number
{
public:
	Number(const T& data) :data(data) {};
	Number& operator+(const Number&)const;
	Number& operator-(const Number&)const;
	Number& operator*(const Number&)const;
	Number& operator/(const Number&)const;
	Number& operator=(const Number&);
	template <typename T2>
	friend std::ostream& operator<<(std::ostream& , const Number<T2>&);
private:
	T data;
};
template <typename T>
std::ostream& operator<<(std::ostream& out, const Number<T>& number) {
	out << number.data;
	return out;
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
}

