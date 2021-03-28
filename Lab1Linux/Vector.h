#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)



#include "Number.h"

template <typename T>
class Vector
{
public:
	Vector(Number<T> x=0, Number<T> y = 0);
	void add(const Vector<T>& right);
	template <typename T2>
	friend std::ostream& operator<<(std::ostream&, const Vector<T2>&);
	void printPolar();
private:
	Number<T> x;
	Number<T> y;
};

template<typename T>
Vector<T>::Vector(Number<T> x, Number<T> y) :x(x), y(y)
{
}

template<typename T>
void Vector<T>::add(const Vector<T>& right)
{
	this->x += right.x;
	this->y += right.y;
}

template<typename T>
void Vector<T>::printPolar()
{
	Number<T> radius = this->x * this->x;
	radius = radius + this->y * this->y;
	radius.sqrt();
	std::cout << "radius= " << radius << " radian= " << this->y / this->x;
}

template<typename T2>
std::ostream& operator<<(std::ostream&out , const Vector<T2>& vector)
{
	out << "x= " << vector.x << " y= " << vector.y;
	return out;
}
#endif
