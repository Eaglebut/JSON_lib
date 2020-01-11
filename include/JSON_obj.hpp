#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <String_functions.hpp>
#include <stdlib.h>

class JSON_obj
{

public:
    JSON_obj();
    ~JSON_obj();
    void parse_string(const std::string &JSON_string);
    void *operator[](const std::string key);
    std::string type_of(const std::string key);

private:
    std::map<std::string, JSON_obj> *JSONs = nullptr;
    std::map<std::string, std::string> *strings = nullptr;
    std::map<std::string, float> *numbers = nullptr;
    std::map<std::string, bool> *bools = nullptr;
    std::map<std::string, std::vector<float>> *number_arrays = nullptr;
    std::map<std::string, std::vector<bool>> *bool_arrays = nullptr;
    std::map<std::string, std::vector<std::string>> *string_arrays = nullptr;
    std::map<std::string, std::vector<JSON_obj>> *JSON_obj_arrays = nullptr;
    std::map<std::string, std::vector<JSON_obj>> *array_arrays = nullptr;

    std::string get_string(const std::string &JSON_string, size_t &i);
    float get_number(const std::string &JSON_string, size_t &i);
    bool get_bool(const std::string &JSON_string, size_t &i);
    bool skip_spaces(const std::string &JSON_string, size_t &i);
    std::vector<std::string> get_string_array(const std::string &JSON_string, size_t &i);
    std::vector<float> get_number_array(const std::string &JSON_string, size_t &i);
    std::vector<bool> get_bool_array(const std::string &JSON_string, size_t &i);
    std::vector<JSON_obj> get_JSON_array(const std::string &JSON_string, size_t &i);

};
