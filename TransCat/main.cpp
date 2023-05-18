#include "transport_catalogue.h"
#include "json_reader.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace std;
using namespace transport_catalogue;
using namespace json_reader;


void Test7() {
	std::ifstream file;
	std::ofstream result;

	file.open("TransCat\\tests\\input2.json");
	result.open("TransCat\\tests\\result.json");

	if (!file) {
		std::cout << "No File!\n"s;
		return;
	}

	TransportCatalogue tc;
	JSONReader rh(tc, file, result);

	rh.ParseBaseRequest();

	file.close();
	result.close();

	system("pause");
}


int main() {
	Test7();
	

	/*
	std::string line = "TransCat\\tests\\tsC_case1_input.txt";
	std::ifstream in;
	in.open(line);
	if (in.is_open()) {
		UpdateCatalogue(in, tc);
		PrintInfo(in, cout, tc);
		}
	system("pause");
	in.close();
	*/
	
	/*
	UpdateCatalogue(cin, tc);
	PrintInfo(cin, cout, tc);
	*/
}