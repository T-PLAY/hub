
#include <iostream>

#include <core/Macros.hpp>
#include <core/Traits.hpp>

int main() {
    const char * cstr = "bonjour";
    std::cout << "cstr: " << cstr << std::endl;
    // std::cout << "cstr: " << std::to_string(cstr) << std::endl;
    std::cout << "cstr: " << hub::to_string(cstr) << std::endl;

    const std::string str = "hello";
    std::cout << "str: " << str << std::endl;
    // std::cout << "str: " << std::to_string(str) << std::endl;
    std::cout << "str: " << hub::to_string(str) << std::endl;

    const int a = 5;
    std::cout << "int: " << std::to_string(a) << std::endl;
    std::cout << "int: " << hub::to_string(a) << std::endl;

    std::vector<int> ints{1, 2, 3};
    // std::cout << "ints: " << ints << std::endl;
    // std::cout << "ints: " << std::to_string(ints) << std::endl;
    std::cout << "ints: " << hub::to_string(ints) << std::endl;

    std::vector<unsigned char> uchars{'a', 'b', 'c'};
    // std::cout << "uchars: " << uchars << std::endl;
    // std::cout << "uchars: " << std::to_string(uchars) << std::endl;
    std::cout << "uchars: " << hub::to_string(uchars) << std::endl;

    std::vector<std::string> strings { "a", "b", "c"};
    // std::cout << "strings: " << strings << std::endl;
    std::cout << "strings: " << hub::to_string(strings) << std::endl;

    std::vector<std::vector<int>> intss { {1}, {1, 2}, {1, 2, 3}};
    // std::cout << "strings: " << strings << std::endl;
    std::cout << "intss: " << hub::to_string(intss) << std::endl;

    std::map<std::string, int> map {{"a", 1}, {"b", 2}};
    // std::cout << "map: " << map << std::endl;
    std::cout << "map: " << hub::to_string(map) << std::endl;

    std::pair<std::string, int> pair {"a", 1};
    // std::cout << "pair: " << pair << std::endl;
    std::cout << "pair: " << hub::to_string(pair) << std::endl;

    std::tuple<int, double, bool> tuple {5, 2.0, true};
    // std::cout << "tuple: " << tuple << std::endl;
    // std::cout << "tuple: " << hub::to_string(tuple) << std::endl;


    return 0;
}
