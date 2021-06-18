//
//  ali_objc.h
//  ali-new
//
//  Created by Stanislav Kutil on 17/02/14.
//
//

#pragma once

namespace ali
{
namespace mac
{
    
// ******************************************************************
template <typename T>
struct weak_reference
// ******************************************************************
{
    __weak T value;
    weak_reference(T t) : value(t) {};
    
    T strong() const
    {
        __strong T strong = value;
        return strong;
    }
};

// ******************************************************************
template<typename T>
inline weak_reference<T> make_weak( T t )
// ******************************************************************
{
    return t;
}

} // namespace mac
} // namespace ali

#define weakify(VAR) \
autoreleasepool {} \
__weak __typeof__(VAR) _weak ## VAR = VAR;

#define strongify(VAR) \
autoreleasepool {} \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Wshadow\"") \
__strong __typeof__(VAR) VAR = _weak ## VAR; \
_Pragma("clang diagnostic pop")

#define NonLeaking(CODE) \
do { \
_Pragma("clang diagnostic push") \
_Pragma("clang diagnostic ignored \"-Warc-performSelector-leaks\"") \
CODE; \
_Pragma("clang diagnostic pop") \
} while (0)


