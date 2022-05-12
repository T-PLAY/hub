#include "IOStream.h"

#include <cstring>
#include <iostream>

void IOStream::write(const std::string& str) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write std::string : start" << std::endl;
#endif

    int strLen = static_cast<int>(str.size());
    write(strLen);

    if (strLen > 0) {
        const char* data = str.data();
        write((const unsigned char*)data, strLen);
    }
}

void IOStream::write(const char* str) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write const char* : start" << std::endl;
#endif

    int strLen = strlen(str);
    write(strLen);

    if (strLen > 0) {
        write((const unsigned char*)str, strLen);
    }
}

// enum class ANY_TYPE {
// };

// hash of type info for 64 bit only, no conversion for 32 bits (same value in uint64_t)
uint64_t typeInfoHash64(const char _DecoratedName[1])
{
    //#ifdef _WIN64
    //static_assert(sizeof(size_t) == 8, "This code is for 64-bit size_t.");
    uint64_t const fnv_offset_basis = 14695981039346656037ULL;
    uint64_t const fnv_prime = 1099511628211ULL;
    //#else
    //    static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
    //    size_t const fnv_offset_basis = 2166136261U;
    //    size_t const fnv_prime = 16777619U;
    //#endif

    uint64_t value = fnv_offset_basis;
    for (char const* it = _DecoratedName + 1; *it != '\0'; ++it) {
        value ^= static_cast<uint64_t>(static_cast<unsigned char>(*it));
        value *= fnv_prime;
    }

    //#ifdef _WIN64
    static_assert(sizeof(uint64_t) == 8, "This code is for 64-bit size_t.");
    value ^= value >> 32;
    //#else
    //    static_assert(sizeof(size_t) == 4, "This code is for 32-bit size_t.");
    //#endif

    return value;
}


//IOStream::~IOStream()
//{
//    close();
//}

void IOStream::write(const std::any& any) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write std::any : start" << std::endl;
    std::cout << "any raw name = '" << any.type().raw_name() << "'" << std::endl;
#endif

    assert(any.has_value());
    //    const auto& hashCode = any.type().hash_code();

    //    const uint64_t& hashCodeTmp = any.type().hash_code();
    //    const uint64_t& hashCodeTmp = typeInfoHash64(any.type().raw_name());
    //    assert(sizeof(hashCodeTmp) == 8);
    //    write(hashCodeTmp);
    //    //    any.type() == typeid (double);
    const auto& anyType = any.type();

    //    if (hashCode == typeid(double).hash_code()) {
    if (anyType == typeid(double)) {
        write(Type::DOUBLE);
        const double* val = std::any_cast<double>(&any);
        assert(sizeof(double) == 8);
        write(*val);

        //    } else if (hashCode == typeid(std::string).hash_code()) {
    } else if (anyType == typeid(std::string)) {
        write(Type::STRING);
        const std::string* val = std::any_cast<std::string>(&any);
        write(*val);

        //    } else if (hashCode == typeid(const char*).hash_code()) {
    } else if (anyType == typeid(const char*)) {
        write(Type::CONST_CHAR_PTR);
        const char* val = *std::any_cast<const char*>(&any);
        assert(sizeof(char) == 1);
        write(val);

        //    } else if (hashCode == typeid(int).hash_code()) {
    } else if (anyType == typeid(int)) {
        write(Type::INT);
        const int* val = std::any_cast<int>(&any);
        assert(sizeof(int) == 4);
        write(*val);

    } else {
        auto name = any.type().name();
        auto raw_name = any.type().name();
        assert(false);
    }
}

// void IOStream::write(const std::map<std::string, std::any> &map) const
//{

//}

void IOStream::read(std::string& str) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read std::string : start" << std::endl;
#endif

    int strLen = 0;
    read(strLen);

    if (strLen == 0) {
        str = "";

    } else {
        unsigned char* tmp = new unsigned char[strLen + 1];
        read(tmp, strLen);
        tmp[strLen] = 0;
        str = std::string((char*)tmp);
        delete[] tmp;
    }
}

void IOStream::read(char* str) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read char* : start" << std::endl;
#endif

    int strLen = 0;
    read(strLen);

    if (strLen == 0) {
        str[0] = 0;

    } else {
        read((unsigned char*)str, strLen);

        str[strLen] = 0;
    }
}

void IOStream::read(std::any& any) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read std::any : start" << std::endl;
#endif

    //    uint64_t hashCode;
    //    assert(sizeof(hashCode) == 8);
    //    read(hashCode);

    Type type;
    read(type);

    //    decltype(any.type().hash_code()) hashCode = hashCodeTmp;

    //    const uint64_t& hashCodeTmp = typeInfoHash64(any.type().raw_name());
    //    if (hashCode == typeid(double).hash_code()) {
    switch (type) {
    case Type::INT: {
        assert(sizeof(int) == 4);
        int val;
        read(val);
        any = std::any_cast<int>(val);
    } break;

    case Type::DOUBLE: {
        double val;
        assert(sizeof(double) == 8);
        read(val);
        any = std::any_cast<double>(val);
    } break;

    case Type::STRING: {
        std::string val;
        read(val);
        any = std::any_cast<std::string>(val);
    } break;

    case Type::CONST_CHAR_PTR: {
        assert(sizeof(char) == 1);
        char* buff = new char[256];
        memset(buff, 0, 256);
        read(buff);
        //        std::cout << "val[256] = '" << buff << "'" << std::endl;
        int len = strlen(buff);
        //        std::cout << "len = " << len << std::endl;

        char* str = new char[len + 1];
        memcpy(str, buff, len);
        str[len] = 0;
        delete[] buff;

        any = std::any_cast<const char*>((const char*)str);

    } break;

    default:
        auto name = any.type().name();
        auto raw_name = any.type().name();
        assert(false);
    }
    //    if (hashCode == typeInfoHash64(typeid(double).raw_name())) {
    //        double val;
    //        assert(sizeof(double) == 8);
    //        read(val);
    //        any = std::any_cast<double>(val);
    //    }
    //    //	else if (hashCode == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
    //    //    else if (hashCode == typeid(std::string).hash_code()) {
    //    else if (hashCode == typeInfoHash64(typeid(std::string).raw_name())) {
    //        std::string val;
    //        read(val);
    //        any = std::any_cast<std::string>(val);

    //        //    } else if (hashCode == typeid(const char*).hash_code()) {
    //    } else if (hashCode == typeInfoHash64(typeid(const char*).raw_name())) {
    //        assert(sizeof(char) == 8);
    //        char* buff = new char[256];
    //        memset(buff, 0, 256);
    //        read(buff);
    //        //        std::cout << "val[256] = '" << buff << "'" << std::endl;
    //        int len = strlen(buff);
    //        //        std::cout << "len = " << len << std::endl;

    //        char* str = new char[len + 1];
    //        memcpy(str, buff, len);
    //        str[len] = 0;
    //        delete[] buff;

    //        any = std::any_cast<const char*>((const char*)str);

    //        //    } else if (hashCode == typeid(int).hash_code()) {
    //    } else if (hashCode == typeInfoHash64(typeid(int).raw_name())) {
    //        assert(sizeof(int) == 4);
    //        int val;
    //        read(val);
    //        any = std::any_cast<int>(val);

    //    } else {
    //        auto name = any.type().name();
    //        auto raw_name = any.type().name();
    //        assert(false);
    //    }
    assert(any.has_value());
}

// void IOStream::read(std::map<std::string, std::any> &map) const
//{

//}

void IOStream::setupOutput(const std::string& sensorName) const
{
    (void)sensorName;
#ifdef DEBUG_IOSTREAM
    std::cout << "IOStream::setOutputName(const std::string& sensorName)" << std::endl;
#endif

    assert(mMode == Mode::INPUT_OUTPUT);
}

// IOStream::IOStream(const Mode& mode)
//{
// }

// void IOStream::setupMode(const Mode& mode)
//{
#ifdef DEBUG_IOSTREAM
#endif
// }
