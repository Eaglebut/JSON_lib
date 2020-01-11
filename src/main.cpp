#include <iostream>
#include "JSON_obj.hpp"
#include "String_functions.hpp"
#include <fstream>
#include <vector>

using namespace std;
using namespace String_functions;

std::string readFile(const std::string &fileName)
{
    std::ifstream f(fileName);
    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    std::string s(size, ' ');
    f.seekg(0);
    f.read(&s[0], size);
    return s;
}

void test_JSON_obj()
{
    std::string json_string = readFile("test.json");
    JSON_obj *object = new JSON_obj;
    object->parse_string(json_string);
    JSON_obj object_o = *object;

    std::cout << *static_cast<string *>(object_o["squadName"]) << std::endl;
    std::cout << *static_cast<float *>(object_o["formed"]) << std::endl;
    std::cout << *static_cast<bool *>(object_o["active"]) << std::endl;
    std::cout << object_o.type_of("squadName") << std::endl;
    std::cout << object_o.type_of("formed") << std::endl;
    std::cout << object_o.type_of("active") << std::endl;
}

void test_String_functions()
{
    string str = "12345 13234 123";
    std::cout << sub_string(str, 1, 3) << std::endl;

    vector<string> vec = split(str, ' ');
    for (auto &i : vec)
    {
        std::cout << i << std::endl;
    }
}

int main()
{
    //test_String_functions();
    test_JSON_obj();
    return 0;
}