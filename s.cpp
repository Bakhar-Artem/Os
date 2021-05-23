#include<iostream>
const int stopWord = -1;

int main() {
	int value;
	int sum = 0;
	std::cin >> value;
	while (value != stopWord) {
		sum += value;
		std::cin >> value;
	}
	std::cout << sum;
	return 0;
}