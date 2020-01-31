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
            type = ARRAY;
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
            if (JSON_string_no_spaces[i] == 'n')
            {
                nulls.push_back(curr_key);
                is_key = true;
            }
            i--;
        }
    }
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
    while (isdigit(JSON_string[i]) || JSON_string[i] == '.')
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
        if (JSON_string[i] != ']')
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
        if (JSON_string[i] != ']')
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

std::string JSON_object::get_array_string(const std::string &JSON_string, size_t &i)
{
    std::string array_str = "[";
    size_t num_of_branches = 1;
    i++;

    while (num_of_branches != 0)
    {

        if (JSON_string[i] == '[')
            num_of_branches++;
        if (JSON_string[i] == ']')
            num_of_branches--;
        array_str += JSON_string[i];
        i++;
    }
    return array_str;
}

JSON_object JSON_object::get_array(const std::string &JSON_string, size_t &i)
{
    JSON_object arr;
    arr.type = ARRAY;
    size_t j = 1;

    std::string array_str = get_array_string(JSON_string, i);

    if (array_str[1] == '"') // если значения массива - строки
    {
        arr = get_string_array(array_str, j);
        return arr;
    }
    if (array_str[1] == '0' || std::atof(&array_str[1]) != 0.0) // если значения - числа
    {
        arr = get_number_array(array_str, j);
        return arr;
    }
    if (array_str[1] == 't' || array_str[1] == 'f') //если значения bool
    {
        arr = get_bool_array(array_str, j);
        return arr;
    }
    if (array_str[1] == '{') //если значения - json объект
    {
        arr = get_JSON_array(array_str, j);
        return arr;
    }
    if (array_str[1] == '[')
    {
        arr = get_array_array(array_str);
        return arr;
    }
}

JSON_object JSON_object::get_array_array(const std::string &JSON_string)
{
    // [[],[]]
    std::vector<std::string> str_array;
    JSON_object array;
    size_t k = 1;
    for (size_t i = 1; i < JSON_string.size() && JSON_string[i] != ']'; i++)
    {
        if (JSON_string[i] == '[')
        {
            str_array.push_back(get_array_string(JSON_string, i));
        }
    }

    for (size_t i = 0; i < str_array.size(); i++)
    {
        k = 0;
        if (str_array[i][1] == '[')
            array.arrays.insert({std::to_string(i), get_array_array(str_array[i])});
        else
            array.arrays.insert({std::to_string(i), get_array(str_array[i], k)});
    }
    return array;
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
void *JSON_object::operator[](const std::string &key)
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
std::string JSON_object::type_of(const std::string &key)
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

std::string &JSON_object::string(const std::string &key)
{
    return strings.at(key);
}
bool &JSON_object::boolean(const std::string &key)
{
    return bools.at(key);
}
float &JSON_object::number(const std::string &key)
{
    return numbers.at(key);
}
JSON_object &JSON_object::array(const std::string &key)
{
    return arrays.at(key);
}
JSON_object &JSON_object::JSON(const std::string &key)
{
    return JSONs.at(key);
}
void JSON_object::from_file(const std::string &filename)
{
    std::ifstream f(filename);
    if (!f.is_open())
    {
        throw std::invalid_argument("file not opened");
    }
    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    std::string s(size, ' ');
    f.seekg(0);
    f.read(&s[0], size);
    f.close();
    parse_string(s);
}

bool JSON_object::insert(const std::string &key, const std::string &string)
{
    return strings.insert({key, string}).second;
}
bool JSON_object::insert(const std::string &key, const float &number)
{
    return numbers.insert({key, number}).second;
}
bool JSON_object::insert(const std::string &key, const bool &boolean)
{
    return bools.insert({key, boolean}).second;
}
bool JSON_object::insert(const std::string &key, const JSON_object &JSON)
{
    return JSONs.insert({key, JSON}).second;
}
bool JSON_object::insert(const std::string &key)
{
    nulls.push_back(key);
    return true;
}
bool JSON_object::insert_array(const std::string &key, const JSON_object &JSON)
{
    return arrays.insert({key, JSON}).second;
}

std::string JSON_object::number_to_string(const float number)
{
    std::string str = std::to_string(number);
    for (size_t i = str.length() - 1; str[i] == '0'; i--)
    {
        str.erase(i);
    }
    if (str.back() == '.')
        str.erase(str.length() - 1);
    return str;
}

std::string JSON_object::to_string()
{

    if (type == ARRAY)
    {
        return array_to_string(*this);
    }
    else
    {
        return object_to_string(*this);
    }
}

std::string JSON_object::object_to_string(const JSON_object &object)
{
    std::string JSON_string = "{";

    for (auto &i : object.strings)
    {
        JSON_string += "\"" + i.first + "\":\"" + i.second + "\",";
    }
    for (auto &i : object.numbers)
    {
        JSON_string += "\"" + i.first + "\":" + number_to_string(i.second) + ",";
    }
    for (auto &i : object.bools)
    {
        JSON_string += "\"" + i.first + "\":" + (i.second ? "true" : "false") + ',';
    }
    for (auto &i : object.nulls)
    {
        JSON_string += "\"" + i + "\":null,";
    }
    for (auto &i : object.arrays)
    {
        JSON_string += "\"" + i.first + "\":" + array_to_string(i.second) + ",";
    }
    for (auto &i : object.JSONs)
    {
        JSON_string += "\"" + i.first + "\":" + object_to_string(i.second) + ",";
    }
    JSON_string[JSON_string.length() - 1] = '}';
    return JSON_string;
}

std::string JSON_object::array_to_string(const JSON_object &object)
{
    std::string JSON_string = "[";

    if (!object.strings.empty())
    {
        std::vector<std::string> *array = new std::vector<std::string>(object.strings.size());
        for (auto &i : object.strings)
        {
            (*array)[atoi(i.first.c_str())] = i.second;
        }
        for (auto &i : *array)
        {
            JSON_string += "\"" + i + "\",";
        }
        delete array;
    }
    else if (!object.numbers.empty())
    {
        std::vector<float> *array = new std::vector<float>(object.numbers.size());
        for (auto &i : object.numbers)
        {
            (*array)[atoi(i.first.c_str())] = i.second;
        }
        for (auto &i : *array)
        {
            JSON_string += number_to_string(i) + ",";
        }
        delete array;
    }
    else if (!object.bools.empty())
    {
        bool *array = new bool[object.bools.size()];
        for (auto i : object.bools)
        {
            array[atoi(i.first.c_str())] = i.second;
        }
        for (size_t i = 0; i < object.bools.size(); i++)
        {
            if (array[i])
            {
                JSON_string += "true,";
            }
            else
                JSON_string += "false,";
        }
        delete array;
    }
    else if (!object.arrays.empty())
    {
        std::vector<JSON_object> *array = new std::vector<JSON_object>(object.arrays.size());
        for (auto &i : object.arrays)
        {
            (*array)[atoi(i.first.c_str())] = i.second;
        }
        for (auto &i : *array)
        {
            JSON_string += array_to_string(i) + ",";
        }
        delete array;
    }
    else if (!object.JSONs.empty())
    {
        std::vector<JSON_object> *array = new std::vector<JSON_object>(object.JSONs.size());
        for (auto &i : object.JSONs)
        {
            (*array)[atoi(i.first.c_str())] = i.second;
        }
        for (auto &i : *array)
        {
            JSON_string += object_to_string(i) + ",";
        }
        delete array;
    }

    JSON_string[JSON_string.length() - 1] = ']';
    return JSON_string;
}
