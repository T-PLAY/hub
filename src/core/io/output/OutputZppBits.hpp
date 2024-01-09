/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/10/25
	
#pragma once

//#include "BasicOutputI.hpp"

//#include "zpp_bits.h"
//#include <iostream>

//namespace hub {
//namespace io {
//namespace output {

// template <Size_t BuffSize = 1'000'000>
//class OutputZppBits : public BasicOutputI
//{
//  public:
//    //    template <typename T>
//    //    using serializable_t = decltype( std::declval<T>().serialize(
//    //    std::declval<OutputZppBits&>() ) );

//    //    template <typename T, typename = std::void_t<>>
//    //    struct serializable : std::false_type {};

//    //    template <typename T>
//    //    struct serializable<T, std::void_t<serializable_t<T>>> : std::true_type {};

//    //    template <typename T>
//    //    static constexpr bool serializable_v = serializable<T>::value;

//    static constexpr Size_t BuffSize = 1'000'000;

//    virtual void write( const Data_t* data, Size_t len ) = 0;

//    template <class T>
//    //    requires (std::cout << declval<T>() )
//    typename std::enable_if<writable_v<T>>::type write( const T& t ) {
////        static_assert()
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t ) << ") ..." << std::endl;
//#endif
//        assert( isOpen() );
//        //        t.serialize(*this);
//        t.write( *this );
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(writable: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//    }

//    template <class T>
//    typename std::enable_if<serializable_v<T>>::type write( const T& t ) {
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(serial: " << TYPE_NAME( t ) << ") ..." << std::endl;
//#endif
//        assert( isOpen() );
//        const_cast<T&>(t).serialize( *this );
//        //        t.write(*this);
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "\033[1mwrite\033[0m(serial: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//    }

//    template <class T>
//    //    typename std::enable_if_t<isPackable_v<T> || isSimpleType_v<T>, void>
//    typename std::enable_if<packable_v<T>>::type write( const T& t ) {
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(packable: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//        assert( isOpen() );

////        zpp::bits::out output( m_serialBuff );
////        output( t ).or_throw();
////        Size_t size = output.position();
////        assert( size < BuffSize );
////        assert(size == sizeof(T));
//////        write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
////        write( m_serialBuff.data(), output.position() );

//        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
//    }


//    template <class T>
//    //    typename std::enable_if_t<isPackable_v<T> || isSimpleType_v<T>, void>
//    typename std::enable_if<!writable_v<T> && !serializable_v<T> && !packable_v<T>>::type write( const T& t ) {
////    typename std::enable_if<!writable_v<T> && !serializable_v<T>>::type write( const T& t ) {
//#ifdef HUB_DEBUG_OUTPUT
//        std::cout << HEADER << "write(raw: " << TYPE_NAME( t ) << ") = " << t << std::endl;
//#endif
//        assert( isOpen() );

//        zpp::bits::out output( m_serialBuff );
//        output( t ).or_throw();
//        Size_t size = output.position();
//        assert( size < BuffSize );
////        write(size);
//        write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );
//        write( m_serialBuff.data(), output.position() );

//        //        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
//    }

//    //    template <class... Ts>
//    //    void

////    template <class T>
////    void operator()( const T& t ) {
////        write( t );
////    }

//    template <class... Ts>
//    void operator()( const Ts&... ts ) {
//        writeAll_( ts... );
//        //        write(t);
//    }

//  private:
//    template <class... Ts>
//    void writeAll_( const Ts&... ts ) {
//#ifdef HUB_DEBUG_OUTPUT
////        std::cout << HEADER << "write(raw: " << TYPE_NAME( ts ) << ") = " << t << std::endl;
//#endif
//        assert( isOpen() );

//        zpp::bits::out output( m_serialBuff );
//        output( ts... ).or_throw();

////        assert( ( sizeof( Ts ) + ... ) == output.position() );

//                Size_t size = output.position();
//                assert(size < BuffSize);
////                write(size);
//                write( reinterpret_cast<const Data_t*>( &size ), sizeof( Size_t ) );

//        write( m_serialBuff.data(), output.position() );
//        //        write( reinterpret_cast<const Data_t*>( &t ), sizeof( T ) );
//    }

//  private:
//    std::array<unsigned char, BuffSize> m_serialBuff;
//};

//} // namespace output
//} // namespace io
//} // namespace hub
