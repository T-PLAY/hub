#include "IOStream.h"

#include <cstring>
#include <iostream>

void IOStream::write(const std::string& str) const
{
    int strLen = static_cast<int>(str.size());
    write(strLen);

    if (strLen > 0) {
        const char* data = str.data();
        write((const unsigned char*)data, strLen);
    }
}

void IOStream::write(const char* str) const
{
    int strLen = strlen(str);
    write(strLen);

    if (strLen > 0) {
        write((const unsigned char*)str, strLen);
    }
}

// enum class ANY_TYPE {
// };

void IOStream::write(const std::any& any) const
{
    assert(any.has_value());
    const auto& hashCode = any.type().hash_code();
    write(hashCode);

    if (hashCode == typeid(double).hash_code()) {
        const double* val = std::any_cast<double>(&any);
        write(*val);

    } else if (hashCode == typeid(std::string).hash_code()) {
        const std::string* val = std::any_cast<std::string>(&any);
        write(*val);

    } else if (hashCode == typeid(const char*).hash_code()) {
        const char* val = *std::any_cast<const char*>(&any);
        write(val);

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
    decltype(any.type().hash_code()) hashCode;
    read(hashCode);

    if (hashCode == typeid(double).hash_code()) {
        double val;
        read(val);
        any = std::any_cast<double>(val);
    }
    //	else if (hashCode == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
    else if (hashCode == typeid(std::string).hash_code()) {
        std::string val;
        read(val);
        any = std::any_cast<std::string>(val);

    } else if (hashCode == typeid(const char*).hash_code()) {
        char* buff = new char[256];
        memset(buff, 0, 256);
        read(buff);
        std::cout << "val[256] = '" << buff << "'" << std::endl;
        int len = strlen(buff);
        std::cout << "len = " << len << std::endl;

        char* str = new char[len + 1];
        memcpy(str, buff, len);
        str[len] = 0;
        delete[] buff;

        any = std::any_cast<const char*>((const char*)str);

    } else {
        auto name = any.type().name();
        auto raw_name = any.type().name();
        assert(false);
    }
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
