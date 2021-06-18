#pragma once

namespace ali
{

// ******************************************************************
struct default_comparator;
// ******************************************************************

//// ******************************************************************
//struct less;
//// ******************************************************************
//
//// ******************************************************************
//struct deref_less;
//// ******************************************************************
//
//// ******************************************************************
//template <typename comparator = default_comparator>
//struct less_from_comparator;
//// ******************************************************************
//
//// ******************************************************************
//template <typename T>
//struct less_from_compare;
//// ******************************************************************
//
//// ******************************************************************
//template <typename T>
//struct less_from_compare_ptr;
//// ******************************************************************

// ******************************************************************
template <typename RefT>
class ref_holder;
// ******************************************************************

// ******************************************************************
template <typename T1, typename T2>
struct pair;
// ******************************************************************

// ******************************************************************
template <typename derived, typename intT = unsigned>
class flags_common;
// ******************************************************************

// ******************************************************************
struct yes_no;
// ******************************************************************

}   //  namespace ali
