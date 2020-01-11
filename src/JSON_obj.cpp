#include "JSON_obj.hpp"
#include <string>
#include <map>
#include <vector>
#include <iostream>

JSON_obj::JSON_obj()
{
}

JSON_obj::~JSON_obj()
{
}

void JSON_obj::parse_string(const std::string &JSON_string)
{
    int num_of_basic_objects = 0;
    bool is_key = true;
    std::string curr_key;
    std::string curr_string;

    for (size_t i = 0; i < JSON_string.length(); i++)
    {

        if (JSON_string[i] == '"' && is_key)
        {
            i++;
            curr_key = "";
            while (JSON_string[i] != '"')
            {
                curr_key += JSON_string[i];
                i++;
            }
            i++;
            is_key = false;
        }

        if (JSON_string[i] == ':' && !is_key)
        {
            i++;
            skip_spaces(JSON_string, i);

            if (JSON_string[i] == '"') // если значение - строка
            {
                if (strings == nullptr)
                {
                    strings = new std::map<std::string, std::string>;
                }
                strings->insert(std::pair(curr_key, get_string(JSON_string, i)));
                i++;
                is_key = true;
            }

            if (JSON_string[i] == '0' || std::atof(&JSON_string[i]) != 0.0) // если значение - число
            {
                if (numbers == nullptr)
                    numbers = new std::map<std::string, float>;
                auto test = numbers->insert(std::pair(curr_key, get_number(JSON_string, i)));
                i++;
                is_key = true;
            }

            if (JSON_string[i] == 't' || JSON_string[i] == 'T' || JSON_string[i] == 'f' || JSON_string[i] == 'F') //если значение bool
            {

                if (bools == nullptr)
                    bools = new std::map<std::string, bool>;

                auto test = bools->insert(std::pair(curr_key, get_bool(JSON_string, i)));
                i++;
                is_key = true;
            }

            if (JSON_string[i] == '[') //если значение - массив
            {
                i++;
                skip_spaces(JSON_string, i);

                if (JSON_string[i] == '"') // если значения массива - строки
                {
                    if (string_arrays == nullptr)
                    {
                        string_arrays = new std::map<std::string, std::vector<std::string>>;
                    }

                    auto test = string_arrays->insert(std::pair(curr_key, get_string_array(JSON_string, i)));
                    i++;
                    is_key = true;
                }
                if (JSON_string[i] == '0' || std::atof(&JSON_string[i]) != 0.0) // если значения - числа
                {
                    if (number_arrays == nullptr)
                        number_arrays = new std::map<std::string, std::vector<float>>;
                    number_arrays->insert(std::pair(curr_key, get_number_array(JSON_string, i)));
                    i++;
                    is_key = true;
                }
                if (JSON_string[i] == 't' || JSON_string[i] == 'T' || JSON_string[i] == 'f' || JSON_string[i] == 'F') //если значение bool
                {

                    if (bool_arrays == nullptr)
                        bool_arrays = new std::map<std::string, std::vector<bool>>;

                    bool_arrays->insert(std::pair(curr_key, get_bool_array(JSON_string, i)));
                    i++;

                    is_key = true;
                }
                i++;
            }
        }
    }
}
void *JSON_obj::operator[](const std::string key)
{
    try
    {
        return &strings->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &numbers->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &bools->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &JSONs->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }

    try
    {
        return &string_arrays->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &number_arrays->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &bool_arrays->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &JSON_obj_arrays->at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    throw std::out_of_range("object not found");
}

std::string JSON_obj::type_of(std::string key)
{
    try
    {
        strings->at(key);
        return "String";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        numbers->at(key);
        return "Number";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        bools->at(key);
        return "Bool";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        JSONs->at(key);
        return "JSON";
    }
    catch (const std::out_of_range &e)
    {
    }

    throw std::out_of_range("object not found");
}

std::string JSON_obj::get_string(const std::string &JSON_string, size_t &i)
{
    i++;

    std::string curr_string = "";
    while (JSON_string[i] != '"')
    {
        curr_string += JSON_string[i];
        i++;
    }
    return curr_string;
}
float JSON_obj::get_number(const std::string &JSON_string, size_t &i)
{
    std::string curr_string = "";
    while (JSON_string[i] != ' ' && JSON_string[i] != ',')
    {
        curr_string += JSON_string[i];
        i++;
    }
    return std::atof(curr_string.c_str());
}
bool JSON_obj::get_bool(const std::string &JSON_string, size_t &i)
{
    std::string curr_string = "";
    while (JSON_string[i] != ' ' && JSON_string[i] != ',')
    {
        curr_string += JSON_string[i];
        i++;
    }
    if (curr_string == "True" || curr_string == "true" || curr_string == "TRUE")
    {
        return true;
    }
    else if (curr_string == "False" || curr_string == "false" || curr_string == "FALSE")
    {
        return false;
    }
    else
        throw std::out_of_range("not a bool");
}

bool JSON_obj::skip_spaces(const std::string &JSON_string, size_t &i)
{
    while (JSON_string[i] == ' ' || JSON_string[i] == '\n')
    {
        i++;
    }
}

std::vector<std::string> JSON_obj::get_string_array(const std::string &JSON_string, size_t &i)
{

    std::vector<std::string> arr;
    while (JSON_string[i] != ']')
    {
        if (JSON_string[i] == '"')
        {
            arr.push_back(get_string(JSON_string, i));
        }
        i++;
    }
    return arr;
}
std::vector<float> JSON_obj::get_number_array(const std::string &JSON_string, size_t &i)
{
}
std::vector<bool> JSON_obj::get_bool_array(const std::string &JSON_string, size_t &i)
{
}
std::vector<JSON_obj> JSON_obj::get_JSON_array(const std::string &JSON_string, size_t &i)
{
}