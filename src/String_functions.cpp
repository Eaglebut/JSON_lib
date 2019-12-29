#include "String_functions.hpp"

std::string *String_functions::sub_string(const std::string &str, size_t begin, size_t end)
{
    std::string *sub_str = new std::string;
    *sub_str = "";
    if (begin <= end)
    {
        for (size_t i = begin; i < end; i++)
        {
            *sub_str += str[i];
        }
    }
    else
        for (size_t i = begin; i > end; i--)
        {
            *sub_str += str[i];
        }
    return sub_str;
}

std::vector<std::string> *String_functions::split(const std::string &str, char separator)
{
    std::vector<std::string> *res_vec = new std::vector<std::string>;
    res_vec->push_back("");
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] == separator)
        {
            res_vec->push_back("");
            i++;
        }
        res_vec->back() += str[i];
    }
    return res_vec;
}
