
#include "Acquisition.hpp"

namespace hub {

template <class... Args>
void Acquisition::emplaceMeasure( Args&&... args ) {
    static_assert( 3 <= sizeof...( args ) && sizeof...( args ) <= 4 );
    m_measures.emplace_back( std::forward<Args>( args )... );
    m_size += m_measures.back().m_size;
}

} // namespace hub
