#include<iostream>
const int stopWord = -1;

int main() {
	int value;
	std::cin >> value;
	while (value != stopWord) {
		std::cout << value + 2 << " ";
		std::cin >> value;
	}
	std::cout << stopWord;
	return 0;
}