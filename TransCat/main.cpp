#include "transport_catalogue.h"
#include "json_reader.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace std;
namespace jr = json_reader;


void Test6() {
	//std::string line = "TransCat\\tests\\tsC_case1_input.txt";
	//std::ifstream in;
	//in.open(line);
	//if (in.is_open()) {
	//	UpdateCatalogue(in, tc);
	//	PrintInfo(in, cout, tc);
	//	}
	//system("pause");
	//in.close();


	//UpdateCatalogue(cin, tc);
	//PrintInfo(cin, cout, tc);
}

void Test7() {
	std::ifstream file;
	std::ofstream result;

	file.open("TransCat\\tests\\input2.json");
	result.open("TransCat\\tests\\result.json");

	if (!file) {
		std::cout << "No File!\n"s;
		return;
	}

	tc::TransportCatalogue tc;
	rh::RequestHandler rh(tc);
	mr::MapRenderer mr;
	jr::JSONReader jr(tc, rh, mr, file, result);

	jr.ParseBaseRequest();

	file.close();
	result.close();

	system("pause");
}

void Test8() {
	std::ifstream file;
	std::ofstream result;

	file.open("TransCat\\tests\\input4.json");
	result.open("TransCat\\tests\\result.xml");

	if (!file) {
		std::cout << "No File!\n"s;
		return;
	}

	tc::TransportCatalogue tc;
	rh::RequestHandler rh(tc);
	mr::MapRenderer mr;
	jr::JSONReader jr(tc, rh, mr, file, result);

	jr.ParseBaseRequest();

	file.close();
	result.close();

 	system("pause");
}

int main() {
	//Test6();
	//Test7();
	Test8();
}