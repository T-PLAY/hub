#include "Sensor.hpp"

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <numeric>

#include "Socket.hpp"

namespace hub {
//#define DEBUG_STREAM

Sensor::Sensor( const Header& header, IO& ioStream ) :
    m_header( header ),
    // mSensorName( sensorName ),
    // mFormat( format ),
    // mDims( dims ),
    mIO( ioStream )
// mAcquisitionSize( computeAcquisitionSize( m_header->getFormat(), m_header->getDims() ) ) {}
{}

Sensor::Sensor( IO& ioStream ) : mIO( ioStream ) {};

Sensor::~Sensor() {
#ifdef DEBUG_MSG
    std::cout << "[Sensor] ~Sensor()" << std::endl;
#endif
    mIO.close();
    delete &mIO;
}

std::ostream& operator<<( std::ostream& os, const Sensor& stream ) {
    //        os << stream.mSensorName << stream.mFormat << stream.mDims << stream.mAcquisitionSize;
    os << stream.m_header.getSensorName();
    return os;
}

///////////////////////////////////////////////////////////////////////////////

// size_t Sensor::getAcquisitionSize() const {
//     return m_header->getAcquisitionSize();
// }

const Header& Sensor::getHeader() const {
    return m_header;
}

IO& Sensor::getIO() const {
    return mIO;
}

///////////////////////////////////////////////////////////////////////////////

InputSensor::InputSensor( const std::string& sensorName, const std::string& syncSensorName ) :
    Sensor( {}, *std::move( new ClientSocket( sensorName, syncSensorName ) ) ) {

    m_header.read( mIO );
}

InputSensor::~InputSensor() {
#ifdef DEBUG_MSG
    std::cout << "[InputSensor] ~InputSensor()" << std::endl;
#endif
}

Acquisition InputSensor::getAcquisition() const {
    long long start, end;
    unsigned char* data = new unsigned char[m_header.getAcquisitionSize()];

    mIO.read( start );
    mIO.read( end );
    mIO.read( data, m_header.getAcquisitionSize() );

#ifdef DEBUG_STREAM
    std::cout << "[InputSensor] read acq :  " << acquisition << std::endl;
#endif

    return Acquisition( start, end, data, m_header.getAcquisitionSize() );
    delete[] data;
}

std::vector<Acquisition> InputSensor::getAllAcquisitions() {
    std::vector<Acquisition> acqs;

    try {
        int nReadAcqs = 0;
        while ( true ) {
            acqs.emplace_back( getAcquisition() );
            ++nReadAcqs;
        }
    }
    catch ( Sensor::exception& e ) {
        std::cout << "[stream] catch sensor exception : " << e.what() << std::endl;
        throw;
    }
    catch ( std::exception& e ) {
        std::cout << "[stream] catch exception : " << e.what() << std::endl;
    }

    return acqs;
}

std::ostream& operator<<( std::ostream& os, const InputSensor& inputSensor ) {
    os << "metadata:" << Header::metaData2string( inputSensor.m_header.getMetaData() );
    return os;
}

////////////////////////////////// OutputSensor /////////////////////////////////////////////

OutputSensor::OutputSensor( const Header& header, ClientSocket&& ioStream ) :
    Sensor( header, *std::move( new ClientSocket( std::move( ioStream ) ) ) ) {

    mIO.setupOutput( m_header.getSensorName() );

    m_header.write( mIO );
}

void OutputSensor::operator<<( const Acquisition& acquisition ) const {
    assert( acquisition.mData != nullptr );
    // assert( acquisition.mSize == mAcquisitionSize );

    assert( acquisition.mBackendTimestamp <= acquisition.mBackendTimeOfArrival );

    mIO.write( acquisition.mBackendTimestamp );
    mIO.write( acquisition.mBackendTimeOfArrival );
    mIO.write( acquisition.mData, m_header.getAcquisitionSize() );
}

} // namespace hub
