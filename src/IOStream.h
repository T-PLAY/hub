#ifndef IOSTREAM_H
#define IOSTREAM_H

#include <any>
#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

//#define DEBUG_IOSTREAM

class IOStream {
public:
    IOStream() = default;
    IOStream(IOStream&& ioStream) = default;
    //    IOStream(const IOStream & ioStream) = delete;
    IOStream(IOStream& ioStream) = delete;
    //    IOStream(IOStream ioStream) = delete;
    IOStream& operator=(const IOStream& ioStream) = delete;
    IOStream&& operator=(const IOStream&& ioStream) = delete;
    IOStream& operator=(IOStream& ioStream) = delete;
    IOStream&& operator=(IOStream&& ioStream) = delete;

    virtual ~IOStream() = default;

//    template <class T>
//    static const auto getValue(const std::any& any);

    void write(const std::any& any) const;
    virtual void write(const unsigned char* data, size_t len) const = 0;
    void write(const char* str) const;

    template <class T>
    void write(const T& t) const;
    template <class T>
    void write(const std::list<T>& list) const;
    template <class T>
    void write(const std::vector<T>& vector) const;
    template <class T, class U>
    void write(const std::map<T, U>& map) const;

    void write(const std::string& str) const;

    ////////////////////////////////////////////////////////////////////////////

    void read(std::any& any) const;
    virtual void read(unsigned char* data, size_t len) const = 0;
    void read(char * str) const;

    template <class T>
    void read(T& t) const;
    template <class T>
    void read(std::list<T>& list) const;
    template <class T>
    void read(std::vector<T>& vector) const;
    template <class T, class U>
    void read(std::map<T, U>& map) const;

    void read(std::string& str) const;

    enum class Mode {
        NONE,
        INPUT,
        OUTPUT,
        INPUT_OUTPUT,
    };

    //    virtual IOStream(IOStream && iostream) = default;

    //    IOStream(const Mode& mode);
    //    virtual void setupMode(const Mode& mode) = 0;
    //    void setupMode(const Mode& mode);
    virtual void setupOutput(const std::string& sensorName) const;

private:
    Mode mMode = Mode::INPUT_OUTPUT;
    //    Mode mMode;
};

//template <class T>
//auto IOStream::getValue(const std::any& any) -> decltype (int)
//{
//    assert(any.has_value());
//    const auto & type = any.type();
////    type_info;

//    //    write(hashCode);
//    if (type == "double") {
//        const double* val = std::any_cast<double>(&any);
//        return val;
//        //        write(*val);

//    } else if (type == "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >") {
//        const std::string* val = std::any_cast<std::string>(&any);
//        //        write(*val);
//        return val;

//    } else {
//        auto name = any.type().name();
//        auto raw_name = any.type().raw_name();
//        assert(false);
//    }
//    return nullptr;
//}

template <class T>
void IOStream::write(const T& t) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write : " << typeid(T).name() << " '" << t << "'" << std::endl;
#endif

    //    assert(mMode != Mode::INPUT || std::is_enum<T>::value);
    assert(mMode == Mode::OUTPUT || mMode == Mode::INPUT_OUTPUT);
    write(reinterpret_cast<const unsigned char*>(&t), sizeof(T));
}

template <class T>
void IOStream::write(const std::list<T>& list) const
{
    int nbEl = list.size();
    write(nbEl);

    for (const T& el : list) {
        write(el);
    }
}

template <class T>
void IOStream::write(const std::vector<T>& vector) const
{
    int nbEl = static_cast<int>(vector.size());
    write(nbEl);

    for (const T& el : vector) {
        write(el);
    }
}

template <class T, class U>
void IOStream::write(const std::map<T, U>& map) const
{
    int nbKey = static_cast<int>(map.size());
    write(nbKey);

    for (const auto& pair : map) {
        const T& first = pair.first;
        const U& second = pair.second;

        write(first);
        write(second);
    }
}

///////////////////////////////////////////////////////////////////////////////

template <class T>
void IOStream::read(T& t) const
{

    //    assert(mMode != Mode::OUTPUT || std::is_enum<T>::value); // read only enum message
    assert(mMode == Mode::INPUT || mMode == Mode::INPUT_OUTPUT);
    read(reinterpret_cast<unsigned char*>(&t), sizeof(T));

#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read : " << typeid(T).name() << " '" << t << "'" << std::endl;
#endif
}

template <class T>
void IOStream::read(std::list<T>& list) const
{
    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        T el;
        read(el);
        list.push_back(std::move(el));
    }
}

template <class T>
void IOStream::read(std::vector<T>& vector) const
{
    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        T el;
        read(el);
        vector.push_back(std::move(el));
    }
}

//template <class T = std::string, class U = std::any>
template <class T, class U>
void IOStream::read(std::map<T, U>& map) const
{
    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        T name;
        read(name);
        U val;
        read(val);
        assert(map.find(name) == map.end());
        map[name] = val;
    }
}

#endif // IOSTREAM_H
