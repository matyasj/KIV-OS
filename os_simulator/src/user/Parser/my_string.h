#include<regex>

#ifndef myString_H
#define myString_H



/**
* klasicky javosky trim
*/
std::string trim(const std::string& s);
/**
* rezdeli zadaný string dle uvedeného oddìlovaèe
* @param str - string
* @param delimiter - oddelovac
* @return vektor stringu, rozdeleny dle oddelovace
*/
std::vector<std::string> split_string(std::string str, std::string delimiter);
/**
* rezdeli zadaný string dle prvniho !! uvedeného oddìlovaèe.
* @param str - string
* @param delimiter - oddelovac
* @return vektor stringu s max delkou dve, rozdele dle prvniho oddelovace
*/
std::vector<std::string> split_first_string(std::string str, std::string delimiter);
/**
* rezdeli zadaný string dle prvniho !! uvedeného oddìlovaèe.
* @param str - string
* @param delimiter - pole oddelovacu
* @return vektor stringu s max delkou dve, rozdele dle prvniho oddelovace
*/
std::vector<std::string> split_first_string_more(std::string str, std::vector<std::string> delimiters);

#endif // !my_string_h