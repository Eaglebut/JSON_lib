#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <stdio.h>

class JSON_object
{
private:
    enum Type
    {
        ARRAY,
        OBJECT
    };

    Type type = OBJECT;
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

    static std::string get_array_string(const std::string &JSON_string, size_t &i);
    static JSON_object get_string_array(const std::string &JSON_string, size_t &i);
    static JSON_object get_number_array(const std::string &JSON_string, size_t &i);
    static JSON_object get_bool_array(const std::string &JSON_string, size_t &i);
    static JSON_object get_JSON_array(const std::string &JSON_string, size_t &i);
    static JSON_object get_array_array(const std::string &JSON_string);

    static JSON_object get_array(const std::string &JSON_string, size_t &i);

    static std::string number_to_string(const float number);

public:
    void parse_string(const std::string &JSON_string);
    void from_file(const std::string &filename);

    void *operator[](const std::string &key);
    std::string type_of(const std::string &key);

    std::string &string(const std::string &key);
    bool &boolean(const std::string &key);
    float &number(const std::string &key);
    JSON_object &array(const std::string &key);
    JSON_object &JSON(const std::string &key);

    bool insert(const std::string &key, const std::string &string);
    bool insert(const std::string &key, const float &number);
    bool insert(const std::string &key, const bool &boolean);
    bool insert(const std::string &key, const JSON_object &JSON);
    bool insert(const std::string &key);
    bool insert_array(const std::string &key, const JSON_object &JSON);

    std::string to_string();

    static std::string object_to_string(const JSON_object &object);
    static std::string array_to_string(const JSON_object &array);
};
