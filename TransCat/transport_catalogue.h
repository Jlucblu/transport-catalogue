#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <deque>


struct BusStop {
	std::string name_;
	double latitude_;
	double longitude_;
};

struct BusRoute {
	std::string number_;
	std::vector<std::string> stops_;
	bool circle;
};

class Hasher {
public:
	size_t operator() (const std::string name) {
		return hasher_(name);
	}
private:
	std::hash<std::string> hasher_;
};