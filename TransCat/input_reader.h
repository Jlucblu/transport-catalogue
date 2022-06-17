#pragma once
#include <string>
#include <iostream>


// ввод запросов
std::string ReadLine() {
	std::string s;
	std::getline(std::cin, s);
	return s;
}

// ввод кол-ва запросов
int ReadNumber() {
	int x;
	std::cin >> x;
	ReadLine();
	return x;
}