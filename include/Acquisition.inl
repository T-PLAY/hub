
#include "Acquisition.hpp"

namespace hub {

template <class... Args>
void Acquisition::emplaceMeasure( Args&&... args ) {
    static_assert( sizeof...( args ) == 3 );
    m_measures.emplace_back( std::forward<Args>( args )... );
    m_size += m_measures.back().m_size;
}

} // namespace hub
