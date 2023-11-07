

#define HUB_DEBUG_INPUT
#define HUB_DEBUG_OUTPUT

#include <catch2/catch_test_macros.hpp>

#include <cmath>
#include <iostream>

#include <core/Buffer.hpp>

// #include <sensor/Resolution.hpp>
// #include <sensor/Resolutions.hpp>
// #include <sensor/resolution/Format.hpp>

// #include <core/io/Archive.hpp>

#include <algorithm>
#include <iostream>
#include <type_traits>

enum class eType : uint64_t {
    None   = 0,
    VOID   = 1 << 0,
    INT    = 1 << 1,
    DOUBLE = 1 << 2,
    RGB8   = 1 << 3,
    BGR8   = 1 << 4,
    DOF6   = 1 << 5,
    //        Count
    Image = RGB8 | BGR8,
};
constexpr auto eTypeCount = 6;
//static_assert(std::pow(2, eTypeCount) == (uint64_t)eType::Last);

// constexpr auto eTypeCount = std::log2((uint64_t)eType::Count);
//  static_assert((uint64_t)eType::VOID == 1);
//  static_assert((uint64_t)eType::INT == 2);
//  static_assert((uint64_t)eType::DOUBLE == 4);
static_assert( sizeof( eType ) == 8 );

// uint64_t bitMask( TypType type ) {
//     return 1 << (uint64_t)type;
// }

static constexpr eType operator|( eType left, eType right ) {
    return (eType)( (uint64_t)left | (uint64_t)right );
}
static constexpr eType operator&( eType left, eType right ) {
    return (eType)( (uint64_t)left & (uint64_t)right );
}
static constexpr bool operator&&( eType left, eType right ) {
    return (uint64_t)left & (uint64_t)right;
}
static constexpr bool operator||( eType left, eType right ) {
    return (uint64_t)left | (uint64_t)right;
}

static constexpr int s_nByte[eTypeCount] {
    0,  // VOID
    4,  // INT
    8,  // DOUBLE
    3,  // RGB8
    3,  // BGR8
    28, // 6DOF
};

static constexpr size_t typeIndex( eType type ) {
    switch ( type ) {
    case eType::VOID:
        return 0;
    case eType::INT:
        return 1;
    case eType::DOUBLE:
        return 2;
    case eType::RGB8:
        return 3;
    case eType::BGR8:
        return 4;
    case eType::DOF6:
        return 5;
    default:
        assert(false);
        return -1;
    }
    //    int n = 0;
    //    uint64_t t = (uint64_t)type;
    //    while (t >> 1) ++n;
    //    return n;
    //    return std::log2((uint64_t)type);
}

static constexpr size_t Type2nByte( const eType& type ) {
    return s_nByte[typeIndex( type )];
}

static constexpr std::string s_toString[eTypeCount] { "VOID", "INT", "DOUBLE", "RGB8", "BGR8", "6DOF" };

static constexpr std::string toString( eType type ) {
    //    return s_toString[(int)type];
    std::vector<std::string> strs;
    for ( int i = 0; i < eTypeCount; ++i ) {
        auto idx = std::pow( 2, i );
        if ( type && (eType)idx ) { strs.push_back( s_toString[typeIndex( (eType)idx )] ); }
    }
    std::string str;
    for ( int i = 0; i < strs.size(); ++i ) {
        str += strs.at( i );
        if ( i != strs.size() - 1 ) str += "_";
    }
    return str;
    //    return s_toString[typeIndex( type )];
}
// bool operator int(eType type) const {
//     return true;
// }

std::ostream& operator<<( std::ostream& os, eType type ) {
    //    os << toString(type);
    //    int n = 0;
    os << toString( type );
    //    for ( int i = 0; i < eTypeCount; ++i ) {
    //        auto idx = std::pow( 2, i );
    //        if ( type && (eType)idx ) { os << toString( (eType)idx ) << " "; }
    //    }
    //    os << "= " << (uint64_t)type;
    return os;
}

// class eType {
//   public:
//     template <class... eTypes>
//     eType(eType... types) {

//    }

//    std::array<
//};

namespace type {
struct INT {
    int value;
    static constexpr auto eType() { return eType::INT; }
};
static_assert( sizeof( INT ) == 4 );

struct DOUBLE {
    double value;
    static constexpr auto eType() { return eType::DOUBLE; }
};
static_assert( sizeof( DOUBLE ) == 8 );


struct RGB8 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    static constexpr auto eType() { return eType::RGB8; }
};
static_assert( sizeof( RGB8 ) == 3 );

struct BGR8 {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    static constexpr auto eType() { return eType::BGR8; }
};
static_assert( sizeof( RGB8 ) == 3 );

struct DOF6 {
    float x;
    float y;
    float z;
    float w1;
    float w2;
    float w3;
    float w4;
    static constexpr auto eType() { return eType::DOF6; }
};
static_assert( sizeof( DOF6 ) == 28 );
} // namespace type

template <class T>
concept isMatrix = requires( T a ) { a.isMatrix; };

class MatrixLeaf
{
  public:
    static constexpr auto s_maxDim = 5;
//    static struct {} isMatrix;
    //    template <class FormatT>
    //    constexpr Matrix(enum Format format)
    //        : m_format(format)
    //    {
    //    }
    //    template <Matrix... Matrices>
    //    constexpr Matrix()
    ////        : m_type{Matrices::type(), ...}
    ////        , m_size{sizeof...(dims)}
    ////        , m_dims{}
    ////        , m_dims{std::forward<int>(Ns)...}
    //    {
    //    }

//    template <MatrixLeaf... matrices>
//        requires (sizeof...(matrices) > 0)
//    constexpr MatrixLeaf()
//        : type { },
//        nDim{ 0},
//        dims{0}
//    {
//    }

    constexpr MatrixLeaf(eType type_, std::vector<MatrixLeaf*> sons)
        : m_sons(std::move(sons))
    {
    }

//    template <class Type, class... Dims>
//    template <size_t... dims_>
    template <class... Dims>
    constexpr MatrixLeaf( eType type_, Dims... dims_ ) :
        type { type_ },
        nDim { sizeof...( dims_ ) },
        dims { dims_... } //        , m_dims{std::forward<int>(Ns)...}
    {
        //        static_assert(sizeof(eType) == Type2nByte(m_type));
    }

    std::vector<MatrixLeaf*> m_sons;

    constexpr auto nByte() const {
        auto ret = Type2nByte( type );
        for ( int i = 0; i < nDim; ++i ) {
            ret *= dims.at( i );
        }
        return ret;
    }

    template <eType type>
    constexpr auto get() const {
//        return 5;
        return type;
    }

//    template <class Type>
    template <eType type_>
    constexpr auto has() const {
        return type_ && type;
    }

//    constexpr auto getType() const { return m_type; }

    constexpr auto toString() const {
        std::string str;
        str += ::toString( type );
        if ( nDim > 0 ) {
            str += ":";
            for ( int i = 0; i < nDim; ++i ) {
                str += std::to_string( dims.at( i ) );
                if ( i != nDim - 1 ) str += "x";
            }
        }
        return str;
    }

    friend std::ostream& operator<<( std::ostream& os, const MatrixLeaf& mat );

    void setData(unsigned char * data, size_t size) const {
//        if (m_parent == nullptr) {
//            if (data == nullptr) {
//                m_data = new unsigned char[size];
//            }
//            memcpy(m_data, data, size);
//        }
    }
//    template <hub::Size_t Size>
//    void setData(const hub::ConstantBuffer<Size> & buffer) const {
//    }

//    template <hub::Size_t Size>
//    void setData(const hub::MutableBuffer<Size> & buffer) const {
//    }

    MatrixLeaf * m_parent = nullptr;
    unsigned char * m_data = nullptr;
//    constexpr auto nDim() const {
//        return m_nDim;
//    }

//  private:
    eType type;
    size_t nDim;
    //    size_t m_nByte;
    //    size_t m_capacity;
    //    std::vector<int> m_dims;
    std::array<size_t, s_maxDim> dims;
};

//class MatrixNode : public MatrixLeaf {
//  public:
//    template <class... Matrices>
//    MatrixNode(Matrices... matrices) {

//    }
//};

std::ostream& operator<<( std::ostream& os, const MatrixLeaf& mat ) {
    os << mat.toString();
    return os;
}

template <class Type, size_t... Dims>
constexpr auto Matrix() {
    static_assert( sizeof( Type ) == Type2nByte( Type::eType() ) );
    return MatrixLeaf(Type::eType(), Dims... );
//    return MatrixLeaf(Type(), Dims... );
}

//template <MatrixLeaf... matrices>
template <class... Matrices>
//    requires (sizeof...(Matrices) > 0)
//    requires(isMatrix<Matrices> && ...)
constexpr auto Matrix(Matrices... matrices) {
//constexpr auto Matrix() {
    //    eType type = ( eType::DOF6 | eType::VOID );
    //    eType type = ( matrices.type() |= ... );

    eType type = eType::None;
    for ( auto mat : { matrices... } ) {
        type = type | mat.type;
    }
    std::vector<MatrixLeaf*> sons;
//    sons.push_back({new MatrixLeaf(matrices) ...});

////    return MatrixLeaf( type );
//    return MatrixLeaf(type, matrices...);
//    return MatrixNode(matrices...);
//    return MatrixNode(std::forward<Matrices>(matrices)...);

    return true;
//        return type;
    //    static_assert(sizeof(eType) == Type2nByte(eType::eType()));
    //    return MatrixLeaf(eType::eType(), Dims...);
}

// Matrix::Matrix(Format format)
//     : m_format(format)
//{
// }

#include <eigen3/Eigen/Core>

TEST_CASE( "Resolution test" ) {
    //    using namespace hub::sensor;
    auto typeVoid = eType::VOID;
    std::cout << "type void: " << typeVoid << std::endl;

    auto typeInt = eType::INT;
    std::cout << "type int: " << typeInt << std::endl;

    auto typeDouble = eType::DOUBLE;
    std::cout << "type double: " << typeDouble << std::endl;

    auto typeRGB8 = eType::RGB8;
    std::cout << "type RGB8: " << typeRGB8 << std::endl;

    auto typeDOF6 = eType::DOF6;
    std::cout << "type DOF6: " << typeDOF6 << std::endl;

    auto typeIntDouble = eType::INT | eType::DOUBLE;
    std::cout << "type int/double: " << typeIntDouble << std::endl;

    //    auto type = (eType)((uint64_t)eType::INT || (uint64_t)eType::RGB8);

    //    constexpr Format format(Format::INT);
    //    RGB8_t rgb8;

    constexpr auto mat = Matrix<type::RGB8, 640, 480>();
    static_assert( mat.nByte() == 640 * 480 * 3 );
    std::cout << "mat: " << mat << std::endl;

    static_assert(mat.nDim == 2);
    static_assert(mat.type == eType::RGB8);
    static_assert(mat.type != eType::Image);
    static_assert(mat.dims.at(0) == 640);
    static_assert(mat.dims.at(1) == 480);
    static_assert(mat.dims.at(2) == 0);
    static_assert(mat.dims.at(3) == 0);
    static_assert(mat.dims.at(4) == 0);
//    static_assert(mat.dims.at(5) == 0);

    constexpr auto imageSize = 640 * 480 * 3;
    unsigned char dataImage[imageSize];
//    auto datasBuff  = hub::Buffer( dataImage, imageSize );
//    hub::Buffer(dataImage, imageSize);
//    mat.setData(datasBuff);
    mat.setData(dataImage, imageSize);
//    mat.setData(datasBuff);


//    constexpr auto mat2 = Matrix<eType::DOF6>();
    constexpr auto mat2 = Matrix<type::DOF6>();
    static_assert( mat2.nByte() == 28 );
    std::cout << "mat2: " << mat2 << std::endl;

//    MatrixLeaf mat3(mat, mat2);

    constexpr auto mat3 = Matrix(mat, mat2);
//    constexpr auto mat3 = MatrixLeaf<mat, mat2>();
//    constexpr auto mat3 = MatrixLeaf(mat, mat2);
//    std::cout << "mat3 type: " << mat3.type << std::endl;
//    std::cout << "mat3: " << mat3 << std::endl;


//    static_assert(mat3.has<eType::Image>());
//    static_assert(mat3.has<eType::RGB8>());
//    static_assert(! mat3.has<eType::BGR8>());

//    constexpr auto mat_get = mat3.get<eType::RGB8>();
//    std::cout << "mat_get: " << mat_get << std::endl;


//    mat3.get<decltype(eType::DOF6)>(eType::DOF6);

    //    constexpr auto mat = Matrix(RGB8_t, 640);
    //    static_assert(mat.nByte() == 640 * 480 * 3);

    //    constexpr Matrix mat2(eType::DOUBLE, 10);
    //    static_assert(mat2.nByte() == 8 * 10);

    //    constexpr eType type(mat, mat2);
    //    Matrix(mat, mat2);

    //    Matrix mat{Format::DOUBLE};
    //    constexpr Matrix mat(Format::DOUBLE);
    //    Matrix<Format::DOUBLE, 5>();
    //    Matrix(Format::DOF6, 10);
    //    Matrix()

    //    static_assert(mat.m_format == Format::DOUBLE);

    //    const auto& resolutions = sensorSpec.getResolutions();
    //    bool has2D              = false;
    //    for ( const auto& resolution : resolutions ) {
    //        const auto& dims   = resolution.first;
    //        const auto& format = resolution.second;
    //        if ( dims.size() == 2 ) {

    //            has2D = true;
    //            break;
    //        }
    //    }
    //    if ( !has2D ) return;

    //    StreamView streamView;
    //    streamView.m_sensorSpec = sensorSpec;
    //    const auto& metaData    = sensorSpec.getMetaData();
    //    streamView.m_isPacked   = metaData.find( "nAcq" ) != metaData.end();
    //    if ( streamView.m_isPacked ) { streamView.m_nAcq = metaData.at( "nAcq" ).getInt(); }

    //    Reso
    //    Resolution resolution;

    return;

    //    //    hub::sensor::Resolutions resolutions;
    //    //    resolutions.push_back( hub::sensor::Resolution {
    //    //    hub::sensor::format::DISPARITY16, 1  } ); std::cout << "resolutions: " <<
    //    //    resolutions << std::endl;

    ////    using MyResolution = hub::sensor::Resolution<unsigned char, width, height>;

    //    using Resolution = hub::sensor::ResolutionT<int, 640, 480>;
    //    static_assert(Resolution::nByte() == 640 * 480 * sizeof(int));
    //    static_assert(Resolution::nDim() == 2);
    //    static_assert(Resolution::nResolution == 1);
    //    static_assert(Resolution::getDim(0) == 640);
    //    static_assert(Resolution::getDim(1) == 480);
    //    static_assert(Resolution::getDim(2) == 0);
    //    static_assert(Resolution::width() == 640);
    //    static_assert(Resolution::height() == 480);

    ////    static_assert(Resolution::n<0>() == 640);
    ////    static_assert(Resolution::n<1>() == 480);
    ////    constexpr std::string_view str = "<int:640x480>";
    ////    static_assert(Resolution::typeName() == str);
    //    static_assert(std::is_same_v<Resolution::getFormatT, int>);
    //    std::cout << "resolution: " << Resolution() << std::endl;

    //    struct UserFormat {
    //        unsigned char r;
    //        unsigned char g;
    //        unsigned char b;
    ////    static constexpr auto nByte() {return 3;};
    //        static constexpr auto name() { return "userFormat"; };
    //        static constexpr auto interpolable() { return true; };
    //    };
    ////    std::cout << "RGB8 hash code: " << typeid(RGB8).hash_code() << std::endl;
    ////    struct BGR8 {
    ////        unsigned char b;
    ////        unsigned char g;
    ////        unsigned char r;
    ////    };
    ////    std::cout << "BGR8 hash code: " << typeid(BGR8).hash_code() << std::endl;

    //    constexpr auto size = 1;
    //    using Resolution1 = hub::sensor::ResolutionT<UserFormat>;
    //    static_assert(Resolution1::nByte() == sizeof(UserFormat) * size);
    //    static_assert(Resolution1::nDim() == 1);
    //    static_assert(Resolution1::nResolution == 1);
    //    static_assert(Resolution1::getDim(0) == size);
    ////    static_assert(Resolution1::length() == size);
    //    static_assert(std::is_same_v<Resolution1::getFormatT, UserFormat>);
    //    std::cout << "resolution1: " << Resolution1() << std::endl;

    ////    const auto format1 = Resolution1::getFormat();
    ////    assert(format1.interpolable == true);
    ////    assert(format1.nByte == 3);
    ////    assert(format1.name == "userFormat");

    ////    const auto nDim1 = Resolution1::getNDim();
    ////    assert(nDim1.capacity() == 1);
    ////    assert(nDim1.size() == 1);
    ////    assert(nDim1.at(0) == 1);

    //    using Resolution2 = hub::sensor::ResolutionT<hub::sensor::format::RGB8, 640, 480>;
    //    static_assert(Resolution2::width() == 640);
    //    static_assert(Resolution2::height() == 480);
    //    std::cout << "resolution2: " << Resolution2() << std::endl;

    //    static_assert(hub::notReadable_v<Resolution2>);

    //    using Resolution3 = hub::sensor::ResolutionT<hub::sensor::format::RGB8, 1, 2, 3>;
    //    static_assert(Resolution3::nx() == 1);
    //    static_assert(Resolution3::ny() == 2);
    //    static_assert(Resolution3::nz() == 3);
    //    std::cout << "resolution3: " << Resolution3() << std::endl;

    //    using Resolution4 = hub::sensor::ResolutionT<hub::sensor::format::RGB8, 1, 2, 3, 4>;
    //    static_assert(Resolution4::nx() == 1);
    //    static_assert(Resolution4::ny() == 2);
    //    static_assert(Resolution4::nz() == 3);
    //    static_assert(Resolution4::nt() == 4);
    //    std::cout << "resolution4: " << Resolution4() << std::endl;

    //    using Resolution5 = hub::sensor::ResolutionT<hub::sensor::format::RGB8, 1, 2, 3, 4, 5>;
    //    static_assert(Resolution5::n<0>() == 1);
    //    static_assert(Resolution5::n<1>() == 2);
    //    static_assert(Resolution5::n<2>() == 3);
    //    static_assert(Resolution5::n<3>() == 4);
    //    static_assert(Resolution5::n<4>() == 5);
    //    std::cout << "resolution5: " << Resolution5() << std::endl;
    ////    static_assert(Resolution4::n<4>() == 2);

    ////    using Resolutions = hub::sensor::ResolutionsT<Resolution>;
    ////    std::cout << "resolutions: " << Resolutions() << std::endl;
    //    using ResolutionRGB = hub::sensor::ResolutionT<hub::sensor::format::RGB8, 640, 480>;
    //    using ResolutionMat = hub::sensor::ResolutionT<hub::sensor::format::Mat4>;
    //    static_assert(ResolutionRGB::depth() == 0);
    //    static_assert(ResolutionMat::depth() == 0);

    //    using FormatRGBMat = hub::sensor::FormatT<ResolutionRGB, ResolutionMat>;
    //    std::cout << "FormatRGBMat hash: " << typeid(FormatRGBMat).hash_code() << std::endl;
    ////    using FormatRGBMat = hub::sensor::Resolution1DT<20, ResolutionRGB, ResolutionMat>;
    //    static_assert(FormatRGBMat::nByte() == 640 * 480 * 3 + 64);
    //    std::cout << "FormatRGBMat: " << FormatRGBMat() << std::endl;
    //    static_assert(FormatRGBMat::depth() == 0);

    //    using ResolutionsPack = hub::sensor::ResolutionT<FormatRGBMat, 20>;
    //    static_assert(ResolutionsPack::nByte() == (640 * 480 * 3 + 64) * 20);
    //    static_assert(ResolutionsPack::depth() == 1);
    //    std::cout << "ResolutionsPack: " << ResolutionsPack() << std::endl;

    //    hub::io::Archive archive;
    ////    archive.write(FormatRGBMat());
    //    archive.write(FormatRGBMat());

    ////    hub::sensor::Resolution

    //    return;

    ////    constexpr auto MyResolution = hub::sensor::make_Resolution<int, 640, 480>();
    ////    using MyResolution2 = decltype(hub::sensor::make_Resolution<int, 640, 480>());

    ////    using MyResolution3 = Res(int, 640, 480);
    ////    using MyResolution4 = Res(double, 640);

    //////    hub::sensor::ResolutionsT<MyResolution3, MyResolution4>;
    ////    using MyResolutions5 = Res(MyResolution3, MyResolution4);
    ////    using MyResolutions6 = Res(MyResolutions5, 10);
    //////    using MyResolutions5 = Resolutio(MyResolution3, MyResolution4);

    ////    using MyResolution3 = Res(int, 640, 480);

    ////    assert(MyResolution2::nByte() == 640 * 480 * 4);

    ////    using Resolutions2 = hub::sensor::ResolutionsT<Resolution, Resolution2, Resolution3>;
    ////    std::cout << "resolutions2: " << Resolutions2() << std::endl;

    //    hub::sensor::Format format3{5, TYPE_NAME(int), false};
    //    std::cout << "format: " << format3 << std::endl;
    ////    std::cout << "format: " << Resolution().getFormat() << std::endl;

    ////    archive.write(Resolution());
    ////    archive.write(Resolution().getFormat());
    ////    archive.write(Resolution().getNDim());

    ////    hub::sensor::Format format;
    ////    hub::sensor::resolution::NDim nDim_read;
    ////    double a;
    ////    archive.read(nDim_read);
    ////    archive.read(a);

    //    const hub::sensor::Format format2{3, UserFormat::name(), false};

    //    const hub::sensor::Format format4{3, "RGB8", false};
    //    const hub::sensor::NDim nDim{1, 2};
    ////    const hub::sensor::Resolution resolution{format4, nDim};
    //    const hub::sensor::Resolution resolution{format4, {1, 2}};

    ////    const hub::sensor::Resolution resolutionPack{resolution, {20}};

    ////    const std::vector<hub::Size_t> vints {1, 2};
    ////    archive.write(vints);
    ////    archive.write(resolution);
    //    archive.write(Resolution());

    //    hub::sensor::Resolution resolution_read;
    //    archive.read(resolution_read);
    //    assert(Resolution() == resolution_read);

    ////    return;
    //    using ResolutionsT = hub::sensor::FormatT<Resolution, Resolution2, Resolution3>;
    ////    using ResolutionsT = hub::sensor::ResolutionsT<Resolution>;
    ////    static_assert(ResolutionsT::nResolution() == 3);
    //    std::cout << "resolutions: " << ResolutionsT() << std::endl;

    ////    hub::sensor::Resolutions resolutions = FormatT::getResolutions();
    ////    std::cout << "resolutions: " << resolutions << std::endl;

    ////    archive.write(FormatT());
    ////    hub::sensor::Resolutions resolutions_read;
    ////    archive.read(resolutions_read);
    ////    assert(resolutions == resolutions_read);

    ////    const auto & format = resolutions_read.at(0).format;
    ////    assert(format.name == "int");

    ////    hub::sensor::Resolutions resolutions{resolution, resolution_read};
    ////    std::cout << resolutions << std::endl;

    ////    archive.write(resolutions);
    ////    archive.write(ResolutionsT());
    ////    hub::sensor::Resolutions resolutions_read;
    ////    archive.read(resolutions_read);
    ////    Resolution resolution;
    ////    archive.read(resolution);

    //    //    hub::sensor::Resolution resolution { hub::sensor::format::BGR8, 1 };
    //    //    CHECK(resolution.nByte() == 3);

    //    //    hub::sensor::Resolution resolution2 { hub::sensor::format::Y8, 6 };
    //    //    CHECK(resolution2.nByte() == 6);

    //    //    hub::sensor::Resolutions resolutions2 {resolution, resolution2};
    //    //    hub::Size_t size = 0;
    //    //    for (const auto & resolution : resolutions2) {
    //    //        size += resolution.nByte();
    //    //    }
    //    //    CHECK(size == 9);
    //    //    CHECK(hub::sensor::nByte(resolutions2) == 9);

    //    //    const hub::sensor::Resolution resolution3 { hub::sensor::format::Y8, 6};
    //    //    auto resolution3_copy = resolution3;
    //    //    assert(resolution3 == resolution3_copy);

    //    //    const hub::sensor::Resolutions resolutions3 {{ hub::sensor::format::Y8, 6}};
    //    //    auto resolutions3_copy = resolutions3;
    //    //    assert(resolutions3 == resolutions3_copy);

    //    //    hub::sensor::Resolution resolution4 { hub::sensor::format::Y8, 6};
    //    //    auto resolution4_copy = resolution4;
    //    //    assert(resolution4 == resolution4_copy);

    //    //    hub::sensor::Resolutions resolutions4 {{ hub::sensor::format::Y8, 6}};
    //    //    auto resolutions4_copy = resolutions4;
    //    //    assert(resolutions4 == resolutions4_copy);

    //    //    hub::sensor::Resolution resolution5 { hub::sensor::format::Y8, 6};
    //    //    auto resolution5_copy = std::move(resolution5);
    //    //    assert(resolution4 == resolution5_copy);

    //    //    hub::sensor::Resolutions resolutions5 {{ hub::sensor::format::Y8, 6}};
    //    //    auto resolutions5_copy = std::move(resolutions5);
    //    //    assert(resolutions4 == resolutions5_copy);

    //    //    std::cout << "resolutions2: " << resolutions2 << std::endl;

    //    ////    static_assert(hub::sensor::format::RGB8. == "RGB8");

    //    ////    auto & format = hub::sensor::format::RGB8;

    //    ////    hub::sensor::Format format = hub::sensor::format::NONE;
    //    ////    std::cout << format << std::endl;

    //    ////    hub::sensor::Resolution resolution { { 1 }, format };
    //    std::cout << resolution << std::endl;
}
