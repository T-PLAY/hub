#ifndef IOSTREAM_H
#define IOSTREAM_H

#include <list>
#include <string>
#include <vector>

class IOStream {
public:
    virtual ~IOStream() = default;

    virtual void write(const unsigned char* data, size_t len) const = 0;

    template <class T>
    void write(const T& t) const;
    template <class T>
    void write(const std::list<T>& list) const;
    template <class T>
    void write(const std::vector<T>& vector) const;

    void write(const std::string& str) const;

    virtual void read(unsigned char* data, size_t len) const = 0;

    template <class T>
    void read(T& t) const;
    template <class T>
    void read(std::list<T>& list) const;
    template <class T>
    void read(std::vector<T>& vector) const;

    void read(std::string& str) const;
};

template <class T>
void IOStream::write(const T& t) const
{
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

///////////////////////////////////////////////////////////////////////////////

template <class T>
void IOStream::read(T& t) const
{
    read(reinterpret_cast<unsigned char*>(&t), sizeof(T));
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

#endif // IOSTREAM_H
