#include "JSON_obj.hpp"

void JSON_object::parse_string(const std::string &JSON_string)
{
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
                strings.insert(std::pair(curr_key, get_string(JSON_string, i)));
                i++;
                is_key = true;
            }

            if (JSON_string[i] == '0' || std::atof(&JSON_string[i]) != 0.0) // если значение - число
            {
                numbers.insert(std::pair(curr_key, get_number(JSON_string, i)));
                i++;
                is_key = true;
            }

            if (JSON_string[i] == 't' || JSON_string[i] == 'T' || JSON_string[i] == 'f' || JSON_string[i] == 'F') //если значение bool
            {
                bools.insert(std::pair(curr_key, get_bool(JSON_string, i)));
                i++;
                is_key = true;
            }

            if (JSON_string[i] == '{') //если значение - json объект
            {
                JSONs.insert(std::pair(curr_key, get_JSON(JSON_string, i)));
                i++;
                is_key = true;
            }

            if (JSON_string[i] == '[') //если значение - массив
            {
                auto test = arrays.insert(std::pair(curr_key, get_array(JSON_string, i)));
                i++;
                is_key = true;
            }
        }
    }
}
void *JSON_object::operator[](const std::string key)
{
    try
    {
        return &strings.at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &numbers.at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &bools.at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        return &JSONs.at(key);
    }
    catch (const std::out_of_range &e)
    {
    }

    try
    {
        return &arrays.at(key);
    }
    catch (const std::out_of_range &e)
    {
    }
    throw std::out_of_range("object not found");
}

std::string JSON_object::type_of(std::string key)
{
    try
    {
        strings.at(key);
        return "String";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        numbers.at(key);
        return "Number";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        bools.at(key);
        return "Bool";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        JSONs.at(key);
        return "JSON";
    }
    catch (const std::out_of_range &e)
    {
    }

    throw std::out_of_range("object not found");
}

std::string JSON_object::get_string(const std::string &JSON_string, size_t &i)
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
float JSON_object::get_number(const std::string &JSON_string, size_t &i)
{
    std::string curr_string = "";
    while (JSON_string[i] != ' ' && JSON_string[i] != ',')
    {
        curr_string += JSON_string[i];
        i++;
    }
    return std::atof(curr_string.c_str());
}
bool JSON_object::get_bool(const std::string &JSON_string, size_t &i)
{
    std::string curr_string = "";
    while (JSON_string[i] != ' ' && JSON_string[i] != ',' && JSON_string[i] != '\n' && JSON_string[i] != ']')
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

JSON_object JSON_object::get_JSON(const std::string &JSON_string, size_t &i)
{
    JSON_object obj;
    size_t start_i = i;
    size_t end_i;
    size_t num_of_branches = 1;
    i++;
    for (; num_of_branches != 0; i++)
    {
        if (JSON_string[i] == '{')
            num_of_branches++;
        if (JSON_string[i] == '}')
            num_of_branches--;
    }
    end_i = i;
    obj.parse_string(String_functions::sub_string(JSON_string, start_i, end_i));
    return obj;
}

void JSON_object::skip_spaces(const std::string &JSON_string, size_t &i)
{
    while (JSON_string[i] == ' ' || JSON_string[i] == '\n')
    {
        i++;
    }
}

std::vector<Array_elem> JSON_object::get_string_array(const std::string &JSON_string, size_t &i)
{

    std::vector<Array_elem> arr;
    Array_elem elem;
    elem.type = "string";
    while (JSON_string[i] != ']')
    {
        if (JSON_string[i] == '"')
        {
            elem.string = get_string(JSON_string, i);
            arr.push_back(elem);
        }
        i++;
    }
    return arr;
}
std::vector<Array_elem> JSON_object::get_number_array(const std::string &JSON_string, size_t &i)
{
    std::vector<Array_elem> arr;
    Array_elem elem;
    elem.type = "number";

    while (JSON_string[i] != ']')
    {
        elem.number = get_number(JSON_string, i);
        arr.push_back(elem);
        i++;
        skip_spaces(JSON_string, i);
    }
    return arr;
}
std::vector<Array_elem> JSON_object::get_bool_array(const std::string &JSON_string, size_t &i)
{
    std::vector<Array_elem> arr;
    Array_elem elem;
    elem.type = "bool";
    while (JSON_string[i] != ']')
    {
        elem.bool_elem = get_bool(JSON_string, i);
        arr.push_back(elem);
        i++;
        skip_spaces(JSON_string, i);
    }
    return arr;
}
std::vector<Array_elem> JSON_object::get_JSON_array(const std::string &JSON_string, size_t &i)
{
    std::vector<Array_elem> arr;
    Array_elem elem;
    elem.type = "JSON";
    while (JSON_string[i] != ']')
    {
        elem.JSON = get_JSON(JSON_string, i);
        arr.push_back(elem);

        i++;
        skip_spaces(JSON_string, i);
    }
    return arr;
}

std::vector<Array_elem> JSON_object::get_array(const std::string &JSON_string, size_t &i)
{
    std::vector<Array_elem> arr;
    size_t num_of_branches = 1;
    i++;
    Array_elem elem;

    while (num_of_branches != 0)
    {
        skip_spaces(JSON_string, i);
        if (JSON_string[i] == '[')
            num_of_branches++;
        if (JSON_string[i] == ']')
            num_of_branches--;
        if (JSON_string[i] == '"') // если значения массива - строки
        {
            arr = get_string_array(JSON_string, i);
            i++;
            return arr;
        }
        if (JSON_string[i] == '0' || std::atof(&JSON_string[i]) != 0.0) // если значения - числа
        {
            arr = get_number_array(JSON_string, i);
            i++;
            return arr;
        }
        if (JSON_string[i] == 't' || JSON_string[i] == 'T' || JSON_string[i] == 'f' || JSON_string[i] == 'F') //если значения bool
        {
            arr = get_bool_array(JSON_string, i);
            i++;
            return arr;
        }
        if (JSON_string[i] == '{') //если значения - json объект
        {
            arr = get_JSON_array(JSON_string, i);
            i++;
            return arr;
        }
        if (JSON_string[i] == '[')
        {
            elem.type = "array";
            elem.array = get_array(JSON_string, i);
            arr.push_back(elem);
            i++;
            num_of_branches--;
        }
        i++;
    }
    return arr;
}
