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
    enum class Type {
        INT = 0,
        DOUBLE,
        STRING,
        CONST_CHAR_PTR,
        COUNT
    };
    static constexpr char const* type2string[static_cast<int>(Type::COUNT)] = {
        "int",
        "double",
        "std::string",
        "const char *",
    };
    friend std::ostream& operator<<(std::ostream& os, const Type& type)
    {
        os << type2string[(int)type];
        return os;
    }

public:
    IOStream() = default;
    IOStream(IOStream&& ioStream) = default;
    IOStream(IOStream& ioStream) = delete;
    IOStream& operator=(const IOStream& ioStream) = delete;
    IOStream&& operator=(const IOStream&& ioStream) = delete;
    IOStream& operator=(IOStream& ioStream) = delete;
    IOStream&& operator=(IOStream&& ioStream) = delete;

    virtual ~IOStream() = default;
//    virtual ~IOStream();

    virtual void close() = 0;

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
    void read(char* str) const;

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

    virtual void setupOutput(const std::string& sensorName) const;

private:
    Mode mMode = Mode::INPUT_OUTPUT;
};

// template <class T>
// auto IOStream::getValue(const std::any& any) -> decltype (int)
//{

//}

template <class T>
void IOStream::write(const T& t) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write " << typeid(T).name() << " '" << t << "' : start" << std::endl;
#endif

    assert(mMode == Mode::OUTPUT || mMode == Mode::INPUT_OUTPUT);
    write(reinterpret_cast<const unsigned char*>(&t), sizeof(T));
}

template <class T>
void IOStream::write(const std::list<T>& list) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write std::list : start" << std::endl;
#endif

    int nbEl = list.size();
    write(nbEl);

    for (const T& el : list) {
        write(el);
    }
}

template <class T>
void IOStream::write(const std::vector<T>& vector) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write std::vector : start" << std::endl;
#endif

    int nbEl = static_cast<int>(vector.size());
    write(nbEl);

    for (const T& el : vector) {
        write(el);
    }
}

template <class T, class U>
void IOStream::write(const std::map<T, U>& map) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] write std::map : start" << std::endl;
#endif

    int nbKey = static_cast<int>(map.size());
    write(nbKey);
#ifdef  DEBUG_IOSTREAM
    std::cout << "[IOStream] map : nbEl = " << nbKey << std::endl;
#endif

    for (const auto& pair : map) {
        const T& first = pair.first;
#ifdef DEBUG_IOSTREAM
        std::cout << "[IOStream] map : name = " << first << std::endl;
#endif
        const U& second = pair.second;

        write(first);
        write(second);
    }
}

///////////////////////////////////////////////////////////////////////////////

template <class T>
void IOStream::read(T& t) const
{

    assert(mMode == Mode::INPUT || mMode == Mode::INPUT_OUTPUT);
    read(reinterpret_cast<unsigned char*>(&t), sizeof(T));

#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read " << typeid(T).name() << " '" << t << "' : end" << std::endl;
#endif
}

template <class T>
void IOStream::read(std::list<T>& list) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read std::list : start" << std::endl;
#endif

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
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read std::vector : start" << std::endl;
#endif

    int nbEl;
    read(nbEl);

    for (int i = 0; i < nbEl; ++i) {
        T el;
        read(el);
        vector.push_back(std::move(el));
    }
}

// template <class T = std::string, class U = std::any>
template <class T, class U>
void IOStream::read(std::map<T, U>& map) const
{
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] read std::map : start" << std::endl;
#endif

    int nbEl;
    read(nbEl);
#ifdef DEBUG_IOSTREAM
    std::cout << "[IOStream] map : nbEl = " << nbEl << std::endl;
#endif

    for (int i = 0; i < nbEl; ++i) {
        T name;
        read(name);
#ifdef DEBUG_IOSTREAM
        std::cout << "[IOStream] map : name = " << name << std::endl;
#endif
        U val;
        read(val);
        //        std::cout << "[IOStream] map : val = " << val << std::endl;
        assert(map.find(name) == map.end());
        map[name] = val;
    }
}

#endif // IOSTREAM_H
