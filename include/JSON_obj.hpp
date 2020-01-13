#pragma once

#include <string>
#include <map>
#include <vector>
#include <String_functions.hpp>

class JSON_basic
{
protected:
    std::map<std::string, std::string> strings;
    std::map<std::string, float> numbers;
    std::map<std::string, bool> bools;
    std::vector<std::string> nulls;
};

struct Array_elem
{
    JSON_basic JSON;
    std::string string;
    float number;
    bool bool_elem;
    std::vector<Array_elem> array;
    std::string type;
};

class JSON_object : private JSON_basic
{
public:
    void *operator[](const std::string key);
    std::string type_of(const std::string key);
    void parse_string(const std::string &JSON_string);

private:
    std::map<std::string, JSON_object> JSONs;
    std::map<std::string, std::vector<Array_elem>> arrays;

    
    std::vector<Array_elem> get_string_array(const std::string &JSON_string, size_t &i);
    std::vector<Array_elem> get_number_array(const std::string &JSON_string, size_t &i);
    std::vector<Array_elem> get_bool_array(const std::string &JSON_string, size_t &i);
    std::vector<Array_elem> get_JSON_array(const std::string &JSON_string, size_t &i);

    std::vector<Array_elem> get_array(const std::string &JSON_string, size_t &i);

    std::string get_string(const std::string &JSON_string, size_t &i);
    float get_number(const std::string &JSON_string, size_t &i);
    bool get_bool(const std::string &JSON_string, size_t &i);
    JSON_object get_JSON(const std::string &JSON_string, size_t &i);

    void skip_spaces(const std::string &JSON_string, size_t &i);
};