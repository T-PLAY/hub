
#pragma once

#if CPLUSPLUS_VERSION == 20

#    include <algorithm>
#    include <stdexcept>
#    include <vector>

namespace hub {

template <typename Key, typename Value>
struct Map {
    std::vector<std::pair<Key, Value>> data;

    using Iterator = decltype( data.cbegin() );

    [[nodiscard]] constexpr Iterator find( const Key& key ) const {
        return std::find_if(
            data.begin(), data.end(), [&key]( const auto& v ) { return v.first == key; } );
    }

    [[nodiscard]] constexpr Value at( const Key& key ) const {
        const auto itr = find( key );
        if ( itr != std::end( data ) ) { return itr->second; }
        else {
            throw std::range_error( "Not Found" );
        }
    }

    [[nodiscard]] Value& operator[]( const Key& key ) {
        auto itr = std::find_if(
            data.begin(), data.end(), [&key]( const auto& v ) { return v.first == key; } );
        if ( itr != std::end( data ) ) { return itr->second; }
        else {
            std::pair<Key, Value> pair = std::make_pair( std::move( key ), Value() );
            auto itrNew                = data.insert( data.end(), pair );
            return itrNew->second;
        }
    }

    void erase( const Key& key ) {
        const auto itr = find( key );
        if ( itr != std::end( data ) ) { data.erase( itr ); }
        else {
            throw std::range_error( "Not Found" );
        }
    }

    [[nodiscard]] constexpr auto begin() const { return data.begin(); }

    [[nodiscard]] constexpr auto end() const { return data.end(); }

    [[nodiscard]] constexpr auto size() const { return data.size(); }
    [[nodiscard]] constexpr auto empty() const { return data.empty(); }

    void insert( Iterator first, Iterator last ) { data.insert( data.end(), first, last ); }
};

} // namespace hub

#endif
