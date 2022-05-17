#pragma once

#include <IOStream.h>

constexpr size_t m_bufLen = 2'000'000;

class CyclicBuff {
    class exception : public std::runtime_error {
    public:
        explicit exception(const char* const message) throw()
            : std::runtime_error(message)
        {
        }
        const char* what() const throw()
        {
            return std::runtime_error::what();
        }
    };
public:
    void write(const unsigned char* data, size_t len);
    void read(unsigned char* data, size_t len);

private:
    unsigned char m_buff[m_bufLen];
    size_t m_writeHead = 0;
    size_t m_readHead = 0;
};

class RamIO : public IOStream {
    class exception : public std::runtime_error {
    public:
        explicit exception(const char* const message) throw()
            : std::runtime_error(message)
        {
        }
        const char* what() const throw()
        {
            return std::runtime_error::what();
        }
    };

public:
    //    RamIO(std::fstream&& file);
    //    template <class std::fstream>
    RamIO(CyclicBuff & buff);

    //    RamIO();
    //    ~RamIO() = default;
    //    RamIO(const RamIO& ramIO) = delete;
    //    RamIO(RamIO&& ramIO) = default;
    //    RamIO();

    void close() override;

    void write(const unsigned char* data, size_t len) const override;
    void read(unsigned char* data, size_t len) const override;

protected:
private:
    CyclicBuff & m_buff;
    //    std::fstream& mFile;
};
