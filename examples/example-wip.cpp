
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

#include <type_traits>



template <class Type2>
class Test {
public:

    Test(Type2 type2) : m_type(type2) {};

  template <class Type, int i = 0, class RawType = std::remove_pointer_t<Type>>
    REQUIRES(, std::is_pointer_v<Type>, Type)
    get()
    {
      std::cout << "1 get ptr " << TYPE_NAME(Type()) << " is pointer" << std::endl;
        return &m_type;
    }

    template <class Type, int i = 0, class RawType = std::remove_cvref_t<Type>>
    REQUIRES(, !std::is_pointer_v<Type>, Type)
    get()
    {
        std::cout << "2 get ref " << TYPE_NAME(RawType()) << " is not pointer" << std::endl;
        return m_type;
    }

//    template <class Type>
//    REQUIRES(, std::is_pointer_v<Type>, Type)
//    get() const
//    {
//        std::cout << "3 get ptr " << TYPE_NAME(Type()) << " is pointer" << std::endl;
//        return &const_cast<Type2&>(m_type);
////        return &m_type;
//    }

//    template <class Type, class RawType = std::remove_cvref_t<Type>>
//    REQUIRES(, !std::is_pointer_v<Type>, Type)
//    get() const
//    {
//        std::cout << "4 get ref " << TYPE_NAME(RawType()) << " is not pointer" << std::endl;
//        return const_cast<Type2&>(m_type);
////        return m_type;
//    }

    Type2 m_type;
};

int main()
{

    static_assert(std::is_pointer_v<int*>);
    //    static_assert(! std::is_pointer_v<int *>);

    Test<int> test(5);
    test.get<int>();
    test.get<int*>();
    test.get<int&>();
    test.get<const int*>();
    test.get<const int&>();

//    const Test<int> test2(3);
//    test2.get<int>();
//    test2.get<int*>();
//    test2.get<int&>();
//    test2.get<const int*>();
//    test2.get<const int&>();

    return 0;

    hub::Node node = hub::make_node<int, 640, 480>();

    auto typeId = TYPE_ID(hub::format::BGR8);
    static_assert(hub::Types::BGR8 == TYPE_ID(hub::format::BGR8));

    switch (typeId) {
    case hub::Types::BGR8:
        break;
    default:
        assert(false);
    }

    int b = 5;

    switch (node.getTypeId()) {
    default:
        std::cout << "id: " << node.getTypeId() << std::endl;
    }

    // Eigen::Matrix4<int> mat2;

    return 0;
}
