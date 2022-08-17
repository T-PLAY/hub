
#ifndef STREAM_H
#define STREAM_H

#include <any>
#include <IOStream.h>
#include <exception>
#include <iostream>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#include "socket.h"
#include "export.h"
#include "Header.hpp"
#include "Acquisition.hpp"



class SRC_API Stream {
public:

    class exception : public std::runtime_error
    {
      public:
        explicit exception( const char* const message ) throw() : std::runtime_error( message ) {}
        const char* what() const throw() { return std::runtime_error::what(); }
    };


protected:
    Stream(const Header & header, IOStream& ioStream);
    Stream(IOStream& ioStream);
     ~Stream();

    Stream(const Stream& stream) = delete;
    Stream(Stream&& stream) = delete;

    Stream& operator=(const Stream& stream) = delete;
    Stream& operator=(Stream&& stream) = delete;

protected:

protected:
     friend class Header;
	//std::unique_ptr<Header> m_header; // pimpl
  Header * m_header;
  //Header& m_header;

    //std::string mSensorName = "";
    //Format mFormat = Format::NONE;
    //std::vector<int> mDims;
    //MetaData mMetaData;

    IOStream& mIOStream;

public:

	const Header & getHeader() const;
    IOStream& getIOStream() const;

     SRC_API friend std::ostream& operator<<( std::ostream& os, const Stream& stream );
};

//////////////////////////// InputStream ///////////////////////////////////////

class OutputStream;

class SRC_API InputStream : public Stream {
public:
     InputStream(const std::string& sensorName, const std::string& syncSensorName = "");
    template <class IOStreamT>
    InputStream(IOStreamT&& ioStream);
    template <class IOStreamT>
    InputStream(IOStreamT& ioStream) = delete;

     ~InputStream();

public:
     Acquisition getAcquisition() const;
     std::vector<Acquisition> getAllAcquisition();

public:
    SRC_API friend std::ostream& operator<<( std::ostream& os, const InputStream& inputStream );
};

template <class IOStreamT>
InputStream::InputStream(IOStreamT&& ioStream) :
    Stream( {}, *std::move( new IOStreamT( std::move( ioStream ) ) ) ) {

    m_header->read( mIOStream );
    //mIOStream.read(mSensorName);
    //mIOStream.read(mFormat);
    //mIOStream.read(mDims);
    //mIOStream.read(mMetaData);

    //mAcquisitionSize = computeAcquisitionSize(m_header->mFormat, m_header->mDims);
}

//////////////////////////// OutputStream ///////////////////////////////////////

class SRC_API OutputStream : public Stream {
public:
     OutputStream(const Header & header, ClientSocket&& ioStream = ClientSocket());
    template <class IOStreamT>
    OutputStream(const Header & header, IOStreamT&& ioStream);
    template <class IOStreamT>
    OutputStream(const Header & header, IOStreamT& ioStream) = delete;

    OutputStream(const OutputStream& outputStream) = delete;
    OutputStream(OutputStream&& outputStream) = delete;

public:
     void operator<<(const Acquisition& acquisition) const;

private:
};

template <class IOStreamT>
OutputStream::OutputStream(const Header & header, IOStreamT&& ioStream)
    : Stream(header, *std::move(new IOStreamT(std::move(ioStream))))
{
    mIOStream.setupOutput(m_header->getSensorName());

    m_header->write( mIOStream );
    //    mIOStream.write(mSensorName);
//    mIOStream.write(mFormat);
//    mIOStream.write(mDims);
//    mIOStream.write(metaData);
//
}

#endif // STREAM_H
