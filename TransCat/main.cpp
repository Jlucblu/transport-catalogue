#include "transport_catalogue.h"
#include "json_reader.h"

#include <cassert>
#include <iostream>
#include <fstream>


using namespace std;
namespace jr = json_reader;


int CompareFiles(ifstream& expectedFile, ifstream& outputFile) {
	std::string expectedLine;
	std::string outputLine;

	int lineIndex = 0;

	while (std::getline(expectedFile, expectedLine) && std::getline(outputFile, outputLine)) {
		++lineIndex;
		if (expectedLine != outputLine) {
			std::cout << lineIndex << " | ";
			continue;
		}
	}

	if (!expectedFile.eof() || !outputFile.eof()) {
		return lineIndex;
	}

	return -1; // ���� ����������� �� �������
}

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
	//std::ifstream file;
	//std::ofstream result;

	//file.open("TransCat\\tests\\input2.json");
	//result.open("TransCat\\tests\\result.json");

	//if (!file) {
	//	std::cout << "No File!\n"s;
	//	return;
	//}

	//tc::TransportCatalogue tc;
	//rh::RequestHandler rh(tc);
	//mr::MapRenderer mr;
	//jr::JSONReader jr(tc, rh, mr, file, result);

	//jr.MakeBase();

	//file.close();
	//result.close();

	//system("pause");
}

void Test8() {
	//std::ifstream file;
	//std::ofstream result;
	//std::ifstream compareresult;

	//file.open("..\\TransCat\\tests\\tests sprint 12\\s12_final_opentest_3.json");
	//result.open("..\\TransCat\\tests\\tests sprint 12\\result.json");
	//compareresult.open("..\\TransCat\\tests\\tests sprint 12\\s12_final_opentest_3_answer.json");

	//if (!file) {
	//	std::cout << "No File!\n"s;
	//	system("pause");
	//	return;
	//}

	//tc::TransportCatalogue tc;
	//rh::RequestHandler rh(tc);
	//mr::MapRenderer mr;
	//tr::TransportRouter tr(tc);
	//jr::JSONReader jr(tc, rh, mr, tr, file, result);

	//jr.MakeBase();

	//result.close();
	//file.close();

	//std::ifstream myresultread("..\\TransCat\\tests\\tests sprint 12\\result.json");
	//CompareFiles(myresultread, compareresult);
	//myresultread.close();

	////std::ifstream finalresult;
	////std::ifstream output;

	////finalresult.open("TransCat\\tests\\result.xml");
	////output.open("TransCat\\tests\\output4.txt");

	////std::cout << CompareFiles(finalresult, output) << endl;

	////finalresult.close();
	////output.close();

 //	system("pause");
}

//int main() {
//	Test6();
//	Test7();
//	Test8();
//}

using namespace std::literals;

void PrintUsage(std::ostream& stream = std::cerr) {
	stream << "Usage: transport_catalogue [make_base|process_requests]\n"sv;
}

int main(int argc, char* argv[]) {
	//if (argc != 2) {
	//	PrintUsage();
	//	return 1;
	//}

	//const std::string_view mode(argv[1]);

	std::string_view mode = "process_requests"sv;

	std::ifstream file;
	file.open("C:\\Users\\Legion\\Desktop\\C++\\transport-catalogue\\CMakeBuild\\Tests\\s14_1_opentest_1_process_requests.json");
		if (!file) {
		std::cout << "No File!\n"s;
		system("pause");
		return 3;
		}

	tc::TransportCatalogue tc;
	rh::RequestHandler rh(tc);
	mr::MapRenderer mr;
	tr::TransportRouter tr(tc);
	sr::Serialization ser(tc, mr, tr);
	sr::Deserialization deser(tc, mr, tr);
	jr::JSONReader jr(tc, rh, mr, tr, ser, deser, file);

	if (mode == "make_base"sv) {

		jr.MakeBase();

	}
	else if (mode == "process_requests"sv) {

		jr.ProcessRequests();

	}
	else {
		PrintUsage();
		return 1;
	}
}