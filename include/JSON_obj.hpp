#pragma once
#include <string>
#include <map>
#include <vector>

enum type
{
    string,
    number,
    tbool,
    null,
    array,
    JSON
};

class JSON_object
{
private:
    std::map<std::string, std::string> strings;
    std::map<std::string, float> numbers;
    std::map<std::string, bool> bools;
    std::vector<std::string> nulls;
    std::map<std::string, JSON_object> arrays;
    std::map<std::string, JSON_object> JSONs;

protected:
    static std::string get_key(const std::string &JSON_string, size_t &i);

    static std::string get_string(const std::string &JSON_string, size_t &i);
    static float get_number(const std::string &JSON_string, size_t &i);
    static bool get_bool(const std::string &JSON_string, size_t &i);
    static JSON_object get_JSON(const std::string &JSON_string, size_t &i);

    static JSON_object get_string_array(const std::string &JSON_string, size_t &i);
    static JSON_object get_number_array(const std::string &JSON_string, size_t &i);
    static JSON_object get_bool_array(const std::string &JSON_string, size_t &i);
    static JSON_object get_JSON_array(const std::string &JSON_string, size_t &i);

    static JSON_object get_array(const std::string &JSON_string, size_t &i);

public:
    void parse_string(const std::string &JSON_string);
    void *operator[](const std::string key);
    std::string type_of(const std::string key);
};
