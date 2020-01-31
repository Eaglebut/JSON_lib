#include <iostream>
#include "JSON_obj.hpp"
#include <fstream>
#include <vector>

using namespace std;

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
    JSON_object object;
    object.parse_string(json_string);
    std::cout << *static_cast<std::string *>(object["squadName"]) << std::endl;
}

int main()
{
    test_JSON_obj();

    return 0;
}