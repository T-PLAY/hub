#ifndef FILEIO_H
#define FILEIO_H

#include <IOStream.h>
#include <fstream>

class FileIO : public IOStream {
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
//    FileIO(std::fstream&& file);
    template <class FStream>
    FileIO(FStream&& file);

    void write(const unsigned char* data, size_t len) const override;
    void read(unsigned char* data, size_t len) const override;

protected:
    std::fstream& mFile;
};

template<class FStream>
FileIO::FileIO(FStream &&file)
    : mFile(*std::move(new FStream(std::move(file))))
{
    assert(mFile.is_open());
}

#endif // FILEIO_H
