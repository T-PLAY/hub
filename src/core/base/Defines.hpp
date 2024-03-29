
#pragma once

#include <vector>
//#include <stdio.h>
//#include <cstdint>
//#include <iostream>

namespace hub {

///
/// \brief Data_t
///
using Data_t  = unsigned char;

///
/// \brief Datas_t
///
using Datas_t = std::vector<Data_t>;

///
/// \brief Size_t
///
using Size_t  = uint64_t; // max = 18'446'744'073'709'551'616 ~= 18 exa

}
