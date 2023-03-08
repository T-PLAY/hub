#pragma once

#include <any>
#include <cassert>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "Macros.hpp"
#include "Acquisition.hpp"
#include "SensorSpec.hpp"

 #define DEBUG_IOSTREAM

//namespace hub {
////class Acquisition;
//class Measure;
////class UserData;
////class SensorSpec;
//}

namespace hub {
namespace io {


///
/// \brief The Interface class
/// describes the features that must be implemented to define a communication bus.
/// This class allows to abstract these functionalities in order to use different
/// communication buses allowing the transfer of sensor data (file, socket, RAM, etc).
/// This class serializes any type of data and is architecture independent (32/64 bits).
/// \note This class is an interface.
/// \warning Unable to perform write/read operations if communication is ended or closed.
/// TODO: split Interface -> Input and Output classes
///
class SRC_API Interface
{
  public:
    Interface() = default;

    ///
    /// \param ioStream
    ///
    Interface( Interface&& ioStream )                 = default;
    Interface( const Interface& ioStream )            = delete;
    Interface& operator=( const Interface& ioStream ) = delete;
    Interface&& operator=( Interface&& ioStream )     = delete;

    virtual ~Interface() = default;

  protected:
    ///
    /// \brief write
    /// function describes how to write data through the communication bus.
    /// \param data
    /// [in] array of bytes in memory
    /// \param len
    /// [in] size of the data array to write
    ///
    virtual void write( const unsigned char* data, size_t len ) const = 0;

    ///
    /// \brief read
    /// function describes how to read data through from the communication bus.
    /// \param data
    /// [out] array of bytes in memory
    /// \param len
    /// [in] size of the data array to read
    ///
    virtual void read( unsigned char* data, size_t len ) const = 0;

  public:
    ///
    /// \brief isEnd
    /// allows to know if the communication bus is no longer active,
    /// no memory bandwidth, no more acquisition coming.
    /// \return
    /// true if the bus is inactive.\n
    /// false otherwise.
    ///
    virtual bool isEnd() const = 0;

    ///
    /// \brief close
    /// is the capability to terminate communication at both ends if possible.
    ///
    virtual void close() const = 0;

    ///
    /// \brief isOpen
    /// allows to verify the still alive communication of the bus.
    /// \return
    /// true if the bus is active.\n
    /// false otherwise.
    ///
    virtual bool isOpen() const = 0;

    ///
    /// \brief write
    /// \param any
    ///
    void write( const std::any& any ) const;

    ///
    /// \brief write
    /// \param str
    ///
    void write( const char* str ) const;

    ///
    /// \brief write
    /// \param t
    ///
    template <class T>
    void write( const T& t ) const;

    ///
    /// \brief write
    /// \param list
    ///
    template <class T>
    void write( const std::list<T>& list ) const;

    ///
    /// \brief write
    /// \param vector
    ///
    template <class T>
    void write( const std::vector<T>& vector ) const;

    ///
    /// \brief write
    /// \param map
    ///
    template <class T, class U>
    void write( const std::map<T, U>& map ) const;

    ///
    /// \brief write
    /// \param pair
    ///
    template <class T, class U>
    void write( const std::pair<T, U>& pair ) const;

  public:
    ///
    /// \brief write
    /// \param str
    ///
    void write( const std::string& str ) const;

    ///
    /// \brief write
    /// \param sensorSpec
    ///
    void write( const SensorSpec& sensorSpec ) const;


    ///
    /// \brief write
    /// \param measure
    ///
    void write( const Measure& measure ) const;

    void write( const UserData& userData) const;

    ///
    /// \brief write
    /// \param acq
    ///
    virtual void write( const Acquisition& acq ) const;

    ////////////////////////////////////////////////////////////////////////////

    ///
    /// \brief read
    /// \param any
    ///
    void read( std::any& any ) const;

    ///
    /// \brief read
    /// \param str
    ///
    void read( char* str ) const;

    ///
    /// \brief read
    /// \param t
    ///
    template <class T>
    void read( T& t ) const;

    ///
    /// \brief read
    /// \param list
    ///
    template <class T>
    void read( std::list<T>& list ) const;

    ///
    /// \brief read
    /// \param vector
    ///
    template <class T>
    void read( std::vector<T>& vector ) const;

    ///
    /// \brief read
    /// \param map
    ///
    template <class T, class U>
    void read( std::map<T, U>& map ) const;

    //    ///
    //    /// \brief read
    //    /// \param pair
    //    /// \return
    //    ///
    //    template <class T, class U>
    //    typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value ), void>::type
    //    read( std::pair<T, U>& pair ) const;

    //    ///
    //    /// \brief read
    //    /// \param pair
    //    /// \return
    //    ///
    //    template <class T, class U>
    //    typename std::enable_if<( std::is_same<U, hub::SensorSpec>::value ), void>::type
    //    read( std::pair<T, U>& pair ) const;

    //    template <class T>
    //    T get() const {
    //        T t;
    //        read(t);
    //        return t;
    //    }

    ////    template<class T>
    ////    T get<T, std::enable_if_t<std::is_same_v<SensorSpec, T>>>() const {
    ////    }
    //    template <>
    //    SensorSpec get<SensorSpec>() const {
    //        SensorSpec sensorSpec;
    //        return sensorSpec;
    //    }

    template <class T>
    void put( const T& t ) const {
        write(t);
    }

//#ifdef OS_LINUX && OS_WINDOWS
#if defined( OS_LINUX ) || defined ( OS_WINDOWS )

//    template <class T>
//    typename std::enable_if<( not std::is_same<T, SensorSpec>::value and
//                              not std::is_same<T, UserData>::value ),
//                            T>::type
//    get() const {
//#ifdef DEBUG_IOSTREAM
//        std::cout << "[Interface] get()" << std::endl;
//#endif
//        T t;
//        read( t );
//        return std::move(t);
//    };

//    template <class T>
//    typename std::enable_if<( std::is_same<T, SensorSpec>::value ), T>::type get() const {
//#ifdef DEBUG_IOSTREAM
//        std::cout << "[Interface] get<SensorSpec>()" << std::endl;
//#endif
//        return getSensorSpec();
//    };


//    template <class T,
//    typename = typename std::enable_if<( not std::is_same<T, SensorSpec>::value
//                                                ), T>::type>
//    T get() const {
//#ifdef DEBUG_IOSTREAM
//        std::cout << "[Interface] get()" << std::endl;
//#endif
//        T t;
//        read( t );
//        return t;
//    };

//    template <class T>
//        typename std::enable_if<( std::is_same<T, SensorSpec>::value ), T>::type
//                 get() const {
//#ifdef DEBUG_IOSTREAM
//        std::cout << "[Interface] get<SensorSpec>()" << std::endl;
//#endif
//        return getSensorSpec();
//    };


//    template <class T>
//    typename std::enable_if<( std::is_same<T, UserData>::value ), T>::type get() const {
//#ifdef DEBUG_IOSTREAM
//        std::cout << "[Interface] get<UserData>()" << std::endl;
//#endif
//        return getUserData();
//    };

        //    template <class T, class = void>
        //    T get() const
        //    {
        //        T t;
        //        read(t);
        //        return t;
        //    };

        //    template <class T>
        //    typename std::enable_if_t<std::is_same_v<int, T>>::type
        //            get() const {
        //        return getSensorSpec();
        //    };

#else

#    if CPLUSPLUS_VERSION == 20
    template <class T>
    T get() const {
        T t;
        read( t );
        return t;
    };

    template <std::same_as<SensorSpec> T>
    T get() const {
        return getSensorSpec();
    };

#    elif CPLUSPLUS_VERSION == 17

//    template <class T>
//    T get() const {
//        T t;
//        read( t );
//        return t;
//    };

//    template <class T>
//    T get() const {
//        write(t);
//    }

//    template <>
//    SensorSpec get<SensorSpec>() const {
//        return getSensorSpec();
//    }

//    template <class T>
//    T get<T, std::enable_if_t<std::is_same_v<int, T>>>() const {
//        return getSensorSpec();
//    };

#else

#endif

#endif

    //    template <class T>
    //        typename std::enable_if<(std::is_same_v<SensorSpec, T>>)
    //        get<T> const {
    //    template <class T>
    //    typename std::enable_if<( not std::is_same<T, hub::SensorSpec>::value ), void>::type
    //    get<T>() const;
    //    read( std::pair<T, U>& pair ) const;

    //                                      };
    //    template <class T = SensorSpec>
    //    SensorSpec get<SensorSpec>() const {
    ////    T get<T, std::enable_if<std::is_same_v<SensorSpec, T>>> () const {
    //        SensorSpec sensorSpec;
    //        return sensorSpec;
    //    }

    // template <>
    // SensorSpec read<SensorSpec>() const {

    //}

//    template <class T, class U>
//        void read( std::pair<T, U>& pair ) const {

////        template <class T, class U>
////        typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value ), void>::type
////        read( std::pair<T, U>& pair ) const {

//        assert( isOpen() );

//#ifdef DEBUG_IOSTREAM
//        std::cout << "[Interface] read(std::pair)" << std::endl;
//#endif
//        T first;
//        read( first );
//        //        hub::SensorSpec sensorSpec;
//        //        read(sensorSpec);
////        U second;
////        read( second );
//        U second = get<U>();
//        pair        = std::make_pair( first, std::move( second ) );
//    }

//    template <class T, class U>
//        typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value ), void>::type
    template <class T, class U,
              typename = typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value )>::type>
              void
//    void
        read( std::pair<T, U>& pair ) const {

//        template <class T, class U>
//        typename std::enable_if<( not std::is_same<U, hub::SensorSpec>::value ), void>::type
//        read( std::pair<T, U>& pair ) const {

        assert( isOpen() );

#ifdef DEBUG_IOSTREAM
        std::cout << "[Interface] read(std::pair)" << std::endl;
#endif
        T first;
        read( first );
        //        hub::SensorSpec sensorSpec;
        //        read(sensorSpec);
        U second;
        read( second );
//        auto second = get<U>();
        pair        = std::make_pair( first, std::move( second ) );
    }

        template <class T, class U>
        typename std::enable_if<( std::is_same<U, hub::SensorSpec>::value ), void>::type
        read( std::pair<T, U>& pair ) const {
            assert( isOpen() );

        #ifdef DEBUG_IOSTREAM
            std::cout << "[Interface] read(std::pair)" << std::endl;
        #endif
            T first;
            read( first );
            //    hub::SensorSpec sensorSpec;
            U second;
            second = getSensorSpec();
            //    read( second );
            pair = std::make_pair( first, second );
        }

    ///
    /// \brief read
    /// \param str
    ///
    void read( std::string& str ) const;

    ///
    /// \brief read
    /// \param sensorSpec
    ///
    void read( SensorSpec& sensorSpec ) const = delete;
    //    void read( SensorSpec& sensorSpec ) const;

    //    ///
    //    /// \brief read
    //    /// \return
    //    ///
    //    SensorSpec read() const { return getSensorSpec(); }

    ///
    /// \brief read
    /// \param acquisition
    ///
    void read( Acquisition& acquisition ) const = delete;

    ///
    /// \brief read
    /// \param measure
    ///
    void read( Measure& measure ) const = delete;

    void read( UserData& userData ) const = delete;

    ///
    /// \brief getSensorSpec
    /// \return
    ///
    SensorSpec getSensorSpec() const;

//    UserData getUserData() const;

    ///
    /// \brief getAcquisition
    /// \param sensorSpec
    /// \return
    ///
    virtual Acquisition getAcquisition( const SensorSpec& sensorSpec ) const;

    ///
    /// \brief anyValue2string
    /// \param any
    /// \return
    ///
    static std::string anyValue2string( const std::any& any );

    ///
    /// \brief anyType2string
    /// \param any
    /// \return
    ///
    static const std::string& anyType2string( const std::any& any );
};

// template <class T>
// T Interface::get() const
//{
//     T t;
//     read(t);
//     return t;
// }

// template <>
// SensorSpec Interface::get<SensorSpec>() const {
//     SensorSpec sensorSpec;
//     return sensorSpec;
// }

///
/// \brief The InputInterface class
/// is used to instantiate an InputSensor.
///
class SRC_API InputInterface : public virtual Interface
{
public:
//    ///
//    /// \brief read
//    /// \param t
//    ///
//    template <class T>
//    void read( T& t ) const {
//        Interface::read(t);
//    }

};

///
/// \brief The OutputInterface class
/// is used to instantiate an OutputSensor.
///
class SRC_API OutputInterface : public virtual Interface
{
  public:
//    ///
//    /// \brief write
//    /// \param t
//    ///
//    template <class T>
//    void write( const T& t ) const {
//        Interface::write(t);
//    }


};

///
/// \brief The InputOutputInterface class
/// is used to instantiate both InputSensor and OutputSensor.
///
class SRC_API InputOutputInterface : public InputInterface, public OutputInterface
{};

} // namespace io
} // namespace hub

#include "Interface.inl"
