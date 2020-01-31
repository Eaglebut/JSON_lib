#include <iostream>
#include "JSON_obj.hpp"
#include <fstream>
#include <vector>

using namespace std;

void test_JSON_obj()
{
    JSON_object object;
    object.from_file("test.json");
    std::cout << object.to_string() << std::endl;
}

int main()
{
    test_JSON_obj();

    return 0;
}