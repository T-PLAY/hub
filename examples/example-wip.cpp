
#include <iostream>

#include <core/Macros.hpp>
#include <core/Traits.hpp>

#include "serializer.h"

int main() {
    // std::cout << "void: " << TYPE_NAME(void) << std::endl;
    // std::cout << "void: " << hub::typeName((void)) << std::endl;

    std::vector<unsigned char> data(100);
    zpp::serializer::memory_output_archive out(data);
    zpp::serializer::memory_input_archive in(data);

    data.clear();
    data.reserve(1000);
    out.reset(0);
    in.reset(0);

    assert(out.offset() == 0);
    assert(in.offset() == 0);
    assert(data.size() == 0);

    std::string str = "gauthier";
    int a = 5;

    out(str);
    assert(data.size() == str.size() + 4);
    assert(out.offset() == str.size() + 4);
    out(a);
    assert(in.offset() == 0);

    std::string str_read;
    in(str_read);

    assert(data.size() == 4);
    assert(in.offset() == 0);

    assert(str == str_read);
    return 0;
}
