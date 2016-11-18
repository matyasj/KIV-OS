#include"my_string.h"

std::string ltrim(const std::string& s) {
	static const std::regex lws{ "^[[:space:]]*", std::regex_constants::extended };
	return std::regex_replace(s, lws, "");
}

std::string rtrim(const std::string& s) {
	static const std::regex tws{ "[[:space:]]*$", std::regex_constants::extended };
	return std::regex_replace(s, tws, "");
}

/**
* klasicky javovsky trim
*/
std::string trim(const std::string& s) {
	return ltrim(rtrim(s));
}
/**
* rezdeli zadaný string dle uvedeného oddìlovaèe
* @param str - string
* @param delimiter - oddelovac
* @return vektor stringu, rozdeleny dle oddelovace
*/
std::vector<std::string> split_string(std::string str, std::string delimiter) {
	std::vector<std::string> data;
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		data.push_back(trim(str.substr(0, pos)));
		str.erase(0, pos + delimiter.length());
	}
	str = trim(str);
	if (!str.empty()) {
		data.push_back(str);
	}
	return data;
}
/**
* rezdeli zadaný string dle prvniho !! uvedeného oddìlovaèe.
* @param str - string
* @param delimiter - oddelovac
* @return vektor stringu s max delkou dve, rozdele dle prvniho oddelovace
*/
std::vector<std::string> split_first_string(std::string str, std::string delimiter) {
	std::vector<std::string> data;
	size_t pos = 0;
	if ((pos = str.find(delimiter)) != std::string::npos) {
		data.push_back(trim(str.substr(0, pos)));
		str.erase(0, pos + delimiter.length());
	}
	data.push_back(trim(str));
	return data;
}
/**
* rezdeli zadaný string dle prvniho !! uvedeného oddìlovaèe.
* @param str - string
* @param delimiter - pole oddelovacu
* @return vektor stringu s max delkou dve, rozdele dle prvniho oddelovace. Posledni string je oddelovac.
*/
std::vector<std::string> split_first_string_more(std::string str, std::vector<std::string> delimiters) {
	std::vector<std::string> data;
	size_t pos = 0;
	size_t first = std::string::npos;
	std::string type_of_delimeters="";
	int i;
	for (i = 0;i < delimiters.size();i++) {
		if ((pos = str.find(delimiters.at(i))) != std::string::npos) {
			if ((first == std::string::npos) || (pos < first)) {
				first = pos;
				type_of_delimeters = delimiters.at(i);
			}
		}
	}
	if (first != std::string::npos) {
		data.push_back(trim(str.substr(0, first)));
		str.erase(0, first + (type_of_delimeters).length());
	}
	data.push_back(trim(str));
	data.push_back(type_of_delimeters);
	return data;
}