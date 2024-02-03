
#include <iostream>

#include <core/Macros.hpp>
#include <core/Traits.hpp>

#include <Eigen/Core>
#include <client/viewer/ViewerStream.hpp>

#include <core/Info.hpp>
#include <core/matrix/MatrixXD.hpp>
#include <io/input/InputStream.hpp>

#include <core/Macros.hpp>
#include <typeindex>

int main() {

    hub::Node node = hub::make_node<int, 640, 480>();

    auto typeId = TYPE_ID( hub::format::BGR8 );
    static_assert( hub::Types::BGR8 == TYPE_ID( hub::format::BGR8 ) );

    switch ( typeId ) {
    case hub::Types::BGR8:
        break;
    default:
        assert( false );
    }

    int b = 5;

    switch ( node.getTypeId() ) {
    default:
        std::cout << "id: " << node.getTypeId() << std::endl;
    }

    // Eigen::Matrix4<int> mat2;

    return 0;
}
