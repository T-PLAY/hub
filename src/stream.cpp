
#include "stream.h"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>
#include <socket.h>

//#define DEBUG_STREAM


Stream::Stream( const Header & header,
                IOStream& ioStream) :
    m_header (header),
    //mSensorName( sensorName ),
    //mFormat( format ),
    //mDims( dims ),
    mIOStream( ioStream )
    //mAcquisitionSize( computeAcquisitionSize( m_header->getFormat(), m_header->getDims() ) ) {}
	{}

Stream::Stream( IOStream& ioStream ) : 
    mIOStream( ioStream ) {};
 
Stream::~Stream() {
#ifdef DEBUG_MSG
    std::cout << "[Stream] ~Stream()" << std::endl;
#endif
    mIOStream.close();
    delete &mIOStream;
}


std::ostream& operator<<( std::ostream& os, const Stream& stream ) {
    //        os << stream.mSensorName << stream.mFormat << stream.mDims << stream.mAcquisitionSize;
    os << stream.m_header.getSensorName();
    return os;
}

///////////////////////////////////////////////////////////////////////////////


//size_t Stream::getAcquisitionSize() const {
//    return m_header->getAcquisitionSize();
//}

const Header & Stream::getHeader() const {
    return m_header;
}

IOStream& Stream::getIOStream() const {
    return mIOStream;
}

///////////////////////////////////////////////////////////////////////////////

InputStream::InputStream( const std::string& sensorName, const std::string& syncSensorName ) :
    Stream( {},
            *std::move( new ClientSocket( sensorName, syncSensorName ) )
            ) {

    m_header.read( mIOStream );

}

InputStream::~InputStream() {
#ifdef DEBUG_MSG
    std::cout << "[InputStream] ~InputStream()" << std::endl;
#endif
}

Acquisition InputStream::getAcquisition() const {
    long long start, end;
    unsigned char* data = new unsigned char[m_header.getAcquisitionSize()];

    mIOStream.read( start );
    mIOStream.read( end );
    mIOStream.read( data, m_header.getAcquisitionSize() );

#ifdef DEBUG_STREAM
    std::cout << "[InputStream] read acq :  " << acquisition << std::endl;
#endif

    return Acquisition( start, end, data, m_header.getAcquisitionSize() );
    delete[] data;
}

std::vector<Acquisition> InputStream::getAllAcquisition() {
    std::vector<Acquisition> acqs;

    try {
        int nReadAcqs = 0;
        while ( true ) {
            acqs.emplace_back( getAcquisition() );
            ++nReadAcqs;
        }
    }
    catch ( Stream::exception& e ) {
        std::cout << "[stream] catch stream exception : " << e.what() << std::endl;
        throw;
    }
    catch ( std::exception& e ) {
        std::cout << "[stream] catch exception : " << e.what() << std::endl;
    }

    return acqs;
}


std::ostream& operator<<( std::ostream& os, const InputStream& inputStream ) {
    os << "metadata:" << Header::metaData2string( inputStream.m_header.getMetaData() );
    return os;
}

////////////////////////////////// OutputStream /////////////////////////////////////////////

OutputStream::OutputStream( const Header & header,
                            ClientSocket&& ioStream ) :
    Stream( header, *std::move( new ClientSocket( std::move( ioStream ) ) )) {

    mIOStream.setupOutput( m_header.getSensorName() );

    m_header.write( mIOStream );
}

void OutputStream::operator<<( const Acquisition& acquisition ) const {
    assert( acquisition.mData != nullptr );
    //assert( acquisition.mSize == mAcquisitionSize );

    assert( acquisition.mBackendTimestamp <= acquisition.mBackendTimeOfArrival );

    mIOStream.write( acquisition.mBackendTimestamp );
    mIOStream.write( acquisition.mBackendTimeOfArrival );
    mIOStream.write( acquisition.mData, m_header.getAcquisitionSize() );
}

