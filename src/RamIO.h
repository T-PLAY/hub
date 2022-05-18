#pragma once

#include <IOStream.h>

//constexpr size_t m_bufLen = 1'000'000;

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
    CyclicBuff(size_t size = 1'000'000);
    CyclicBuff(const CyclicBuff & buff) = delete;
    CyclicBuff(CyclicBuff && buff) = default;

    CyclicBuff & operator=(const CyclicBuff & buff) = delete;
    CyclicBuff & operator=(CyclicBuff && buff) = delete;

    ~CyclicBuff();

    void write(const unsigned char* data, size_t len);
    void read(unsigned char* data, size_t len);
    void close();

private:
    unsigned char * const m_buff;
    const size_t m_buffLen;

    size_t m_writeHead = 0;
    size_t m_readHead = 0;

    bool m_outputStreamWantsToClose = false;
    bool m_inputStreamClose = false;
//    bool m_endBuff = false;
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
