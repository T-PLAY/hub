
//#define HUB_DEBUG_INPUT
//#define HUB_DEBUG_OUTPUT

#include "test_common.hpp"
// #include <catch2/catch_test_macros.hpp>
// #include <iostream>

#include <core/Traits.hpp>
#include <core/Any.hpp>
// #include <core/io/InputOutput.hpp>
#include <core/io/Archive.hpp>
#include <map>

// #include <data/Mat4.hpp>

class Lambda
{
  public:
    int a    = 0;
    float b  = 0.0f;
    double c = 0.0;
    std::vector<int> ds;
    std::string e;

    template <class Serial>
    void serialize( Serial& serial ) {
        serial( a, b, c, ds, e );
    }

    friend std::ostream& operator<<( std::ostream& os, const Lambda& lambda ) {
//        os << lambda.a << " " << lambda.b << " " << lambda.c << " " << lambda.ds << " " << lambda.e;

//        for ( const auto& d : lambda.ds ) {
//            os << d << " ";
//        }
//        os << "]";
        return os;
    }
    bool operator==( const Lambda& lambda ) const {
        return a == lambda.a && b == lambda.b && c == lambda.c && ds == lambda.ds && e == lambda.e;
    }
    //    Lambda() = default;
};

//#define CHECK assert

#include <core/Serializer.hpp>

 TEST_CASE( "Any test" ) {
//int main() {
    // int main() {
    //     hub::Any::printTable();

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;

    //    hub::Anyable::registerTypes<Lambda>();
    //    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;


    //    hub::io::InputOutput inputOutput;
    hub::io::Archive archive;
    std::any std_any;
    assert(! std_any.has_value());

    hub::Any any;
    assert(any.type() == typeid(void));
    hub::Any any_read;
    assert( !any.hasValue() );
    assert( !any_read.hasValue() );
    assert( any.type() == typeid( void ) );
    //    std::cout << any.type().name() << std::endl;
    std::cout << any << std::endl;

    // void
    {
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( void ) << " ---------------------\n";
#endif
        archive.write( any );
        archive.read( any_read );
        assert(! any_read.hasValue());
        CHECK( any == any_read );
    }

    // int
    {
        any      = 1;
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( int ) << " ---------------------\n";
#endif
        any_read = 1;
        CHECK( any == any_read );
        any_read = 2;
        CHECK( any != any_read );
        std::cout << any << std::endl;
        //    return;
        std::cout << any.type().name() << std::endl;
        std::cout << any.getValueStr() << std::endl;
        assert( any.hasValue() );
        //    std::cout << any.to_string<int>() << std::endl;
        CHECK( any.is<int>() );
        //    CHECK( any.typeId() == typeid(int));
        CHECK( !any.is<bool>() );
        CHECK( !any.is<double>() );
        CHECK( !any.is<const char*>() );
        CHECK( !any.is<std::string>() );
        CHECK( !any.is<Lambda>() );
        int a = any.get<int>();
        //    int a2 = any.get();
        CHECK( a == 1 );

        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
    }

    // double
    {
        any      = 1.4;
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( double ) << " ---------------------\n";
#endif
        any_read = 1.4;
        CHECK( any == any_read );
        any_read = 2.1;
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<double>() );
        CHECK( !any.is<int>() );
        double b = any.get<double>();
        //    auto b2 = any.get<int>();
        CHECK( b == 1.4 );

        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
    }

    // std::string
    {
        any      = std::string( "hello" );
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( std::string ) << " ---------------------\n";
#endif
        any_read = std::string( "hello" );
        CHECK( any == any_read );
        any_read = std::string( "bye" );
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<std::string>() );
        std::string c = any.get<std::string>();
        CHECK( c == "hello" );

        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
    }

    // const char *
    {
        const char* str = "hello";
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( const char * ) << " ---------------------\n";
#endif
        any             = str;
        any_read        = str;
        CHECK( any == any_read );
        any_read = "bye";
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<const char*>() );
        const char* d = any.get<const char*>();
        CHECK( !strcmp( d, "hello" ) );

        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
    }

    //    any = hub::data::Mat4();
    //    std::cout << any << std::endl;
    //    const auto& i = any.get<hub::data::Mat4>();
    //    CHECK( i == hub::data::Mat4() );

    {
        const Lambda lambda { 5, 0.0, 1.0f, { 0, 1, 2 }, "gauthier" };
#ifdef HUB_DEBUG_OUTPUT
        std::cout << "\n------------------ " << TYPE_NAME( Lambda ) << " ---------------------\n";
#endif
        //    archive.write(lambda);
        //    Lambda lambda{5, 0.0, 1.0f, {0, 1, 2}};
        any      = lambda;
        any_read = lambda;
        CHECK( any == any_read );
        any_read = Lambda { 2, 0.0, 1.0f, { 0 }, "bouyjou" };
        CHECK( any != any_read );
        std::cout << any << std::endl;
        CHECK( any.is<Lambda>() );
        CHECK( any.get<Lambda>() == lambda );

        archive.write( any );
        archive.read( any_read );
        CHECK( any == any_read );
    }

    //    std::cout << "any type: " << any.type() << std::endl;
    //    std::cout << "any value: " << any.getValueAsString() << std::endl;
    //    std::cout << "any typeid: " << any.typeId().name() << std::endl;
    //    std::cout << any << std::endl;

    std::cout << "any supported types : " << hub::Anyable::supportedTypes() << std::endl;
}
