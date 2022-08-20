#include "Acquisition.hpp"

#include <cassert>
#include <iomanip>

#include <cstring>

namespace hub {
Acquisition::Acquisition( long long backendTimestamp,
                          long long backendTimeOfArrival,
                          const unsigned char* const data,
                          size_t size ) :
    mBackendTimestamp( backendTimestamp ),
    mBackendTimeOfArrival( backendTimeOfArrival ),
    mData( new unsigned char[size] ),
    mSize( size ) {

    assert( data != nullptr );
    memcpy( (unsigned char*)mData, data, size );
    assert( size > 0 );
    assert( backendTimestamp <= backendTimeOfArrival );
}

Acquisition::~Acquisition() {
    assert( mData != nullptr );

    if ( !mIsMoved ) { delete[] mData; }
}

Acquisition::Acquisition( Acquisition&& acq ) noexcept :
    mBackendTimestamp( acq.mBackendTimestamp ),
    mBackendTimeOfArrival( acq.mBackendTimeOfArrival ),
    mData( acq.mData ),
    mSize( acq.mSize ) {
    acq.mIsMoved = true;
}

bool Acquisition::operator==( const Acquisition& acq ) const {
    if ( mBackendTimestamp == acq.mBackendTimestamp &&
         mBackendTimeOfArrival == acq.mBackendTimeOfArrival && mSize == acq.mSize ) {

        return memcmp( mData, acq.mData, mSize ) == 0;
    }
    return false;
}

Acquisition Acquisition::clone() const {
    assert( mData != nullptr );

    return Acquisition( mBackendTimestamp, mBackendTimeOfArrival, mData, mSize );
}

std::ostream& operator<<( std::ostream& os, const Acquisition& acq ) {
    os << "start:" << acq.mBackendTimestamp / 1000 << ", end:" << acq.mBackendTimeOfArrival / 1000;
    os << ", data:[";
    for ( auto i = 0; i < std::min( (int)acq.mSize, 10 ); ++i ) {
        os << std::setw( 3 ) << (int)acq.mData[i] << " ";
    }
    os << "], ";
    os << 1'000'000.0 / ( acq.mBackendTimeOfArrival - acq.mBackendTimestamp ) << " fps";
    return os;
}

} // namespace hub
