
#include <iostream>

#include <core/Macros.hpp>
#include <core/Traits.hpp>

//#include "zpp/serializer.h"
//#include "core/serializer/zpp/serializer.h"

#include <Eigen/Core>
#include <client/viewer/ViewerStream.hpp>

#include <io/input/InputStream.hpp>
#include <core/matrix/MatrixXD.hpp>
#include <core/Info.hpp>

int main() {
    hub::client::ViewerHandler viewerHandler;
    hub::io::Header header;
    hub::client::ViewerStream<hub::input::InputStream> viewerStream(0, "ouuou", 5, "ououou", std::move(header), viewerHandler);

    hub::s_contributionEnd;

    hub::MatrixTs<int, double> mat;

    Eigen::Matrix4<int> mat2;

    // std::cout << "void: " << TYPE_NAME(void) << std::endl;
    // std::cout << "void: " << hub::typeName((void)) << std::endl;

//    std::vector<unsigned char> data(100);
//    zpp::serializer::memory_output_archive out(data);
//    zpp::serializer::memory_input_archive in(data);

//    data.clear();
//    data.reserve(1000);
//    out.reset(0);
//    in.reset(0);

//    assert(out.offset() == 0);
//    assert(in.offset() == 0);
//    assert(data.size() == 0);

//    std::string str = "gauthier";
//    int a = 5;

//    out(str);
//    assert(data.size() == str.size() + 4);
//    assert(out.offset() == str.size() + 4);
//    out(a);
//    assert(in.offset() == 0);

//    std::string str_read;
//    in(str_read);

//    assert(data.size() == 4);
//    assert(in.offset() == 0);

//    assert(str == str_read);
    return 0;
}
