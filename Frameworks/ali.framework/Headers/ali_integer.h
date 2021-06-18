#pragma once

#include "ali/ali_meta.h"

namespace ali
{

using int8 = meta::integer::signed_of_exact_size_in_bits<8>::type;
using int16 = meta::integer::signed_of_exact_size_in_bits<16>::type;
using int32 = meta::integer::signed_of_exact_size_in_bits<32>::type;
using int64 = meta::integer::signed_of_exact_size_in_bits<64>::type;

using uint8 = meta::integer::unsigned_of_exact_size_in_bits<8>::type;
using uint16 = meta::integer::unsigned_of_exact_size_in_bits<16>::type;
using uint32 = meta::integer::unsigned_of_exact_size_in_bits<32>::type;
using uint64 = meta::integer::unsigned_of_exact_size_in_bits<64>::type;

using uint_of_pointer_size = meta::integer::unsigned_of_exact_size_in_bits<
        meta::size_in_bits<void*>::result>::type;

}   //  namespace ali
