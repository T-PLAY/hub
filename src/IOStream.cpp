#include "IOStream.h"

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
        //        const char* data = str.data();
        write((const unsigned char*)str, strLen);
    }
}

//enum class ANY_TYPE {
//    NONE,
//    DOUBLE,
//};

void IOStream::write(const std::any& any) const
{
    //    std::any;
    assert(any.has_value());
    //    std::string hashCode = any.type().name();
    //    const type_info & type = any.type();
    const auto& hashCode = any.type().hash_code();
    write(hashCode);

    //    double val = my_any_cast<>(&any);
    //    const auto * val = getValue(any);
    //    double d = std::get<double>(any);
    //    auto d = any::get<double>(any);

    //    if (hashCode == "double") {
    if (hashCode == typeid(double).hash_code()) {
        const double* val = std::any_cast<double>(&any);
        write(*val);

    }
    //    else if (hashCode == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
    else if (hashCode == typeid(std::string).hash_code()) {
        const std::string* val = std::any_cast<std::string>(&any);
        write(*val);

    } else if (hashCode == typeid(const char*).hash_code()) {
        const char* val = *std::any_cast<const char*>(&any);
        //        const char* val = "hello";
        write(val);

    } else {
        auto name = any.type().name();
        auto raw_name = any.type().raw_name();
        assert(false);
    }
}

//void IOStream::write(const std::map<std::string, std::any> &map) const
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
        //        unsigned char* tmp = new unsigned char[strLen + 1];
        read((unsigned char*)str, strLen);

        str[strLen] = 0;
        //        tmp[strLen] = 0;
        //        str = std::string((char*)tmp);
        //        delete[] tmp;
    }
}

void IOStream::read(std::any& any) const
{
    //    std::string hashCode;
    decltype(any.type().hash_code()) hashCode;
    read(hashCode);

    //    const type_info & type = any.type();
    //    read(t)
    //    type_info type;

    //    switch (hashCode) {

    //    default:
    //        assert(false);
    //    }
    //    if (hashCode == std::any((double)0.0).type().hash_code()) {
    //    if (hashCode == "double") {
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
        //        char val[256] = {0}; // max string length
        char* buff = new char[256];
        memset(buff, 0, 256);
        //        const char val[] = "demain";
        //        char * val = new char[256]; // max string length
        read(buff);
        std::cout << "val[256] = '" << buff << "'" << std::endl;
        int len = strlen(buff);
        std::cout << "len = " << len << std::endl;

        char* str = new char[len + 1];
        memcpy(str, buff, len);
        str[len] = 0;
        delete[] buff;

        //        std::string str = val;
        //        std::cout << "str = " << str << std::endl;
        any = std::any_cast<const char*>((const char*)str);
        //        delete [] str;
        //        any = str.c_str();
        //        any = std::any_cast<const char*>((const char*)"bonjour");

        //        const char* val2 = *std::any_cast<const char*>(&any);
        //        std::cout << "val2 : '" << val2 << "'" << std::endl;

    } else {
        auto name = any.type().name();
        auto raw_name = any.type().raw_name();
        assert(false);
    }
    assert(any.has_value());
}

//void IOStream::read(std::map<std::string, std::any> &map) const
//{

//}

void IOStream::setupOutput(const std::string& sensorName) const
{
    (void)sensorName;
#ifdef DEBUG_IOSTREAM
    std::cout << "IOStream::setOutputName(const std::string& sensorName)" << std::endl;
#endif
    ////        ClientSocket::Type clientType = ClientSocket::Type::STREAMER;
    ////        write(clientType);

    //        write(sensorName);
    //        Socket::Message mess;
    //        read(mess);
    //        if (mess == Socket::Message::FOUND) {
    //            throw Socket::exception((std::string("sensor '") + sensorName + "' is already attached to server").c_str());
    //        }
    //        assert(mess == Socket::Message::NOT_FOUND);

    assert(mMode == Mode::INPUT_OUTPUT);
    //    mMode = Mode::OUTPUT;
}

// IOStream::IOStream(const Mode& mode)
//     : mMode(mode)
//{
// }

// void IOStream::setupMode(const Mode& mode)
//{
#ifdef DEBUG_IOSTREAM
//     std::cout << "IOStream::setupMode(const Mode& mode)" << std::endl;
#endif
//     mMode = mode;
// }
