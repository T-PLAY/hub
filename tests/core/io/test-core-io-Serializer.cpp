
// #define HUB_DEBUG_INPUT
// #define HUB_DEBUG_OUTPUT

#include "core/Traits.hpp"
#include "test_common.hpp"
#include <core/Macros.hpp>

// #include <cstring>
// #include <iostream>
// #include <set>

// #include "core/test_core_common.hpp"
// #include "core/io/test_core_io_common.hpp"

#include <core/io/Archive.hpp>
// #include <core/io/InputOutput.hpp>
#include <core/Serializer.hpp>

// auto serialize(const Node &) -> zpp::bits::pb_protocol;

class SensorSpec
{
  public:
    auto toString() const { return "sensorSpec: '" + m_sensorName + "'" + " " + std::to_string(m_value); }

    void write( hub::Serializer& serializer ) const {
        std::cout << "[SensorSpec] write(Serializer&) : " << *this << std::endl;
        serializer.write( m_sensorName );
        // serializer.writeAll( m_sensorName, m_value );
    }
    void read( hub::Serializer& serializer ) {
        serializer.read( m_sensorName );
        // serializer.readAll( m_sensorName, m_value );
        std::cout << "[SensorSpec] read(Serializer&) : " << *this << std::endl;
    }
    bool operator==( const SensorSpec& other ) const {
        return m_sensorName == other.m_sensorName && m_value == other.m_value;
    }

    std::string m_sensorName;
    int m_value;
};

class InputOutputStream : public hub::Output, public hub::Input
{
  public:
    using hub::Input::read;
    using hub::Output::write;

    InputOutputStream( hub::Output& output ) : m_output( output ) {};

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        if ( m_retained ) { m_retainedData.insert( m_retainedData.end(), data, data + size ); }
        m_output.write( data, size );
    }
    void close() override {}
    bool isOpen() const override { return true; }
    void setRetain( bool retain ) override { m_retained = retain; }
    void read( hub::Data_t* data, hub::Size_t size ) override {
        assert( m_readPosition + size <= m_retainedData.size() );
        std::copy( m_retainedData.data() + m_readPosition,
                   m_retainedData.data() + m_readPosition + size,
                   data );
        m_readPosition += size;
    }
    bool isEnd() const override { return false; }
    void clear() override {}

    int m_readPosition = 0;
    std::vector<hub::Data_t> m_retainedData;
    bool m_retained = false;
    hub::Output& m_output;
};

TEST_CASE( "Serializer test" ) {
    hub::io::Archive archive;

    SensorSpec sensorSpec;
    sensorSpec.m_sensorName = "gauthier";
    sensorSpec.m_value      = 93;

    InputOutputStream inputOutputStream( archive );
    inputOutputStream.setRetain( true );
    inputOutputStream.write( sensorSpec );
    inputOutputStream.setRetain( false );

    // SensorSpec sensorSpec_read;
    // archive.read(sensorSpec_read);
    // assert(sensorSpec == sensorSpec_read);

    SensorSpec sensorSpec_read2;
    inputOutputStream.read( sensorSpec_read2 );
    // assert( sensorSpec == sensorSpec_read2 );

    // hub::Serializer serializer;
    // serializer.pack( archive, myName );

    return;
}
