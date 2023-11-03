
#include <iostream>
#include <core/Macros.hpp>

template <class Item, hub::Size_t... NDim>
class ResolutionT
{

};

template <class... Resolution>
class ResolutionsT
{

};


int main() {

    struct RGB8 {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct Z16 {
        uint16_t z;
    };

    using MyResolution = ResolutionT<RGB8, 640, 480>;
    using MyResolution2 = ResolutionT<Z16, 640, 480>;
    using MyResolutions = ResolutionsT<MyResolution, MyResolution2>;
//    ResolutionT<RGB8, 640, 480> resolution;

    return 0;
}
