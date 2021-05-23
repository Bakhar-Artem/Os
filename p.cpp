#include<iostream>
#include <cmath>
const int stopWord = -1;

int main() {
	int value;
	std::cin >> value;
	while (value != stopWord) {
		std::cout << pow(value,3) << " ";
		std::cin >> value;
	}
	std::cout << stopWord;
	return 0;
}