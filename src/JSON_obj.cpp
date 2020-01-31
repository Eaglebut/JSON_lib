#include "JSON_obj.hpp"

void JSON_object::parse_string(const std::string &JSON_string)
{
    bool is_key = true;
    std::string curr_key;
    std::string curr_string;
    bool in_branches = false;
    std::string JSON_string_no_spaces = "";

    for (size_t i = 0; i < JSON_string.length(); i++)
    {
        if (JSON_string[i] == '"')
            in_branches = !in_branches;
        if (!in_branches && (JSON_string[i] == ' ' || JSON_string[i] == '\n'))
        {
            continue;
        }
        else
        {
            JSON_string_no_spaces += JSON_string[i];
        }
    }

    for (size_t i = 0; i < JSON_string_no_spaces.length(); i++)
    {
        if (JSON_string_no_spaces[i] == '"' && is_key)
        {
            curr_key = get_key(JSON_string_no_spaces, i);
            is_key = false;
        }

        if (is_key && JSON_string_no_spaces[i] == '[')
        {
            arrays.insert({"0", get_array(JSON_string_no_spaces, i)});
        }

        if (JSON_string_no_spaces[i] == ':' && !is_key)
        {
            i++;

            if (JSON_string_no_spaces[i] == '"') // если значение - строка
            {
                strings.insert({curr_key, get_string(JSON_string_no_spaces, i)});
                is_key = true;
            }

            if (JSON_string_no_spaces[i] == '0' || std::atof(&JSON_string_no_spaces[i]) != 0.0) // если значение - число
            {
                numbers.insert({curr_key, get_number(JSON_string_no_spaces, i)});
                is_key = true;
            }

            if (JSON_string_no_spaces[i] == 't' || JSON_string_no_spaces[i] == 'T' || JSON_string_no_spaces[i] == 'f' || JSON_string_no_spaces[i] == 'F') //если значение bool
            {
                bools.insert({curr_key, get_bool(JSON_string_no_spaces, i)});
                is_key = true;
            }

            if (JSON_string_no_spaces[i] == '{') //если значение - json объект
            {
                JSONs.insert({curr_key, get_JSON(JSON_string_no_spaces, i)});
                is_key = true;
            }

            if (JSON_string_no_spaces[i] == '[') //если значение - массив
            {
                arrays.insert({curr_key, get_array(JSON_string_no_spaces, i)});
                is_key = true;
            }
            i--;
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
        JSON_object arr = arrays.at(key);
        if (!arr.strings.empty())
        {
            std::vector<std::string *> *return_vector = new std::vector<std::string *>;
            for (auto &elem : arr.strings)
            {
                return_vector->push_back(&elem.second);
            }
            return return_vector;
        }
        else if (!arr.numbers.empty())
        {
            std::vector<float *> *return_vector = new std::vector<float *>;
            for (auto &elem : arr.numbers)
            {
                return_vector->push_back(&elem.second);
            }
            return return_vector;
        }
        else if (!arr.bools.empty())
        {
            std::vector<bool *> *return_vector = new std::vector<bool *>;
            for (auto &elem : arr.bools)
            {
                return_vector->push_back(&elem.second);
            }
            return return_vector;
        }
        else if (!arr.JSONs.empty())
        {
            std::vector<JSON_object *> *return_vector = new std::vector<JSON_object *>;
            for (auto &elem : arr.JSONs)
            {
                return_vector->push_back(&elem.second);
            }
            return return_vector;
        }
        else if (!arr.arrays.empty())
        {
            std::vector<JSON_object *> *return_vector = new std::vector<JSON_object *>;
            for (auto &elem : arr.arrays)
            {
                return_vector->push_back(&elem.second);
            }
            return return_vector;
        }
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
        return "string";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        numbers.at(key);
        return "number";
    }
    catch (const std::out_of_range &e)
    {
    }
    try
    {
        bools.at(key);
        return "bool";
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
    try
    {
        arrays.at(key);
        return "array";
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
    i++;

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
    std::string str;
    str = JSON_string.substr(start_i, end_i - start_i);
    obj.parse_string(str);
    return obj;
}

JSON_object JSON_object::get_string_array(const std::string &JSON_string, size_t &i)
{

    JSON_object arr;
    std::string elem;
    size_t iterator = 0;
    while (JSON_string[i] != ']')
    {
        if (JSON_string[i] == '"')
        {
            elem = get_string(JSON_string, i);
            arr.strings.insert({std::to_string(iterator), elem});
            iterator++;
        }
        if (JSON_string[i] != ']')
            i++;
    }

    return arr;
}
JSON_object JSON_object::get_number_array(const std::string &JSON_string, size_t &i)
{
    JSON_object arr;
    float elem;
    size_t iterator = 0;

    while (JSON_string[i] != ']')
    {
        elem = get_number(JSON_string, i);
        arr.numbers.insert({std::to_string(iterator), elem});
        i++;

        iterator++;
    }

    return arr;
}
JSON_object JSON_object::get_bool_array(const std::string &JSON_string, size_t &i)
{
    JSON_object arr;
    bool elem;
    size_t iterator = 0;

    while (JSON_string[i] != ']')
    {
        elem = get_bool(JSON_string, i);
        arr.bools.insert({std::to_string(iterator), elem});
        i++;

        iterator++;
    }

    return arr;
}
JSON_object JSON_object::get_JSON_array(const std::string &JSON_string, size_t &i)
{
    JSON_object arr;
    JSON_object elem;
    size_t iterator = 0;
    while (JSON_string[i] != ']')
    {
        elem = get_JSON(JSON_string, i);
        arr.JSONs.insert({std::to_string(iterator), elem});

        iterator++;
        if (JSON_string[i] != ']')
            i++;
    }

    return arr;
}

JSON_object JSON_object::get_array(const std::string &JSON_string, size_t &i)
{
    JSON_object arr;
    size_t num_of_branches = 1;
    i++;
    JSON_object elem;
    size_t iterator = 0;

    while (num_of_branches != 0)
    {

        if (JSON_string[i] == '[')
            num_of_branches++;
        if (JSON_string[i] == ']')
            num_of_branches--;
        if (JSON_string[i] == '"') // если значения массива - строки
        {
            arr = get_string_array(JSON_string, i);
            return arr;
        }
        if (JSON_string[i] == '0' || std::atof(&JSON_string[i]) != 0.0) // если значения - числа
        {
            arr = get_number_array(JSON_string, i);
            return arr;
        }
        if (JSON_string[i] == 't' || JSON_string[i] == 'T' || JSON_string[i] == 'f' || JSON_string[i] == 'F') //если значения bool
        {
            arr = get_bool_array(JSON_string, i);
            return arr;
        }
        if (JSON_string[i] == '{') //если значения - json объект
        {
            arr = get_JSON_array(JSON_string, i);
            return arr;
        }
        if (JSON_string[i] == '[')
        {
            arr.arrays.insert({std::to_string(iterator), get_array(JSON_string, i)});
            iterator++;
            num_of_branches--;
        }
        i++;
    }

    return arr;
}

std::string JSON_object::get_key(const std::string &JSON_string, size_t &i)
{
    std::string key = "";
    while (JSON_string[i] != '"')
    {
        i++;
    }
    i++;
    while (JSON_string[i] != '"')
    {
        key += JSON_string[i];
        i++;
    }
    i++;

    return key;
}
