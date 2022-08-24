#pragma once

#include <iostream>

#include "Macros.hpp"

namespace hub {

///
/// \brief The Acquisition class
/// represents the data with the start and the end of the acquire process.
///
class SRC_API Acquisition
{
  public:
    Acquisition( long long start,
                 long long end,
                 const unsigned char* const data,
                 size_t size );
    ~Acquisition();

    Acquisition( const Acquisition& acq ) = delete;
    Acquisition( Acquisition&& acq ) noexcept;

    Acquisition& operator=( const Acquisition& acq ) = delete;
    Acquisition& operator=( Acquisition&& acq )      = delete;

    bool operator==(const Acquisition & acq) const;
    bool operator!=(const Acquisition & acq) const;

  public:
    Acquisition clone() const;

  public:
    const long long m_start;     // microseconds
    const long long m_end; // microseconds
    const unsigned char* const m_data;
    const size_t m_size;

  private:
    bool m_isMoved = false;

  public:
    SRC_API friend std::ostream& operator<<( std::ostream& os, const Acquisition& acq );
};

} // namespace hub
