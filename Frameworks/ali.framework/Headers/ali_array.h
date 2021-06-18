#pragma once
#include "ali/ali_array_forward.h"
#include "ali/ali_addressof.h"
#include "ali/ali_array_utils.h"
#include "ali/ali_attribute.h"
#include "ali/ali_auto_ptr.h"
#include "ali/ali_debug.h"
#include "ali/ali_exception_if.h"
#include "ali/ali_in_place_buffer.h"
#include "ali/ali_initializer_list.h"
#include "ali/ali_integer.h"
#include "ali/ali_range.h"

// Debug buffer overflow.

#if defined(ALI_DEBUG_OVERFLOW)

#   define ALI_ARRAY_DEBUG_OVERFLOW
    //  Globally controlled define.

#else

//#   define ALI_ARRAY_DEBUG_OVERFLOW
    //  Locally controlled define (comment/uncomment).

#endif  //  ALI_DEBUG_OVERFLOW

// ******************************************************************

#if defined(ALI_ARRAY_DEBUG_OVERFLOW)
#   define ALI_ARRAY_IF_DEBUG_OVERFLOW(...) __VA_ARGS__
#else
#   define ALI_ARRAY_IF_DEBUG_OVERFLOW(...)
#endif

// ******************************************************************

namespace ali
{

namespace hidden
{

// ******************************************************************
template <typename T, int _capacity>
class array_common;
// ******************************************************************

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
inline void array_storage_wipe_content( array_ref<T> arr ) noexcept
// ******************************************************************
//  Assumes all the objects of type T have already been destroyed.
// ******************************************************************
{
    arr.as_mutable_blob().fill();
}

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T, int _capacity>
class array_storage
// ******************************************************************
//  Storage for array with fixed capacity.
// ******************************************************************
{
public:     //  Methods
        /// \brief Returns the maximum number of
        /// elements this array can potentially hold.
        ///
        /// \returns The maximum number of elements
        /// this array can potentially hold.
        ///
        /// \throws noexcept
        ///
    static constexpr int max_size( void ) noexcept
    {
        return _capacity;
    }

        /// \brief  Returns the number of elements this
        /// array can accommodate without triggering
        /// memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee is
        /// valid only for the memory managed by the array
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// The capacity of an array object can be
        /// increased using the \ref reserve method.
        ///
        /// \returns The number of elements this array
        /// can accommodate without triggering memory
        /// allocation.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref reserve
        ///
    int capacity( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return _capacity;
    }

private:    //  Methods
    array_storage( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
    }

    array_storage( array_storage const& ) = delete;
    array_storage( array_storage&& b ) = delete;

    ~array_storage( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(this->_size == 0);

        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->uninitialize_overflow_guards());

        array_storage_wipe_content(array_ref<T>{
            this->_begin, _capacity});

        this->_size = meta::max_value(this->_size);
    }

    array_storage& operator=( array_storage const& ) = delete;
    array_storage& operator=( array_storage&& b ) = delete;

#ifdef  ALI_ARRAY_DEBUG_OVERFLOW

    struct check_overflow_guards_now_and_on_exit
    {
        array_storage const&  a;

        explicit check_overflow_guards_now_and_on_exit(
            array_storage const&  a )
        :   a{a}
        {
            a.check_overflow_guards();
        }

        ~check_overflow_guards_now_and_on_exit( void )
        {
            a.check_overflow_guards();
        }
    };

    void uninitialize_overflow_guards( void )
    {
        this->_guard1 = 0U;
        this->_guard2 = 0U;
        this->_guard3 = 0U;
    }

    bool check_overflow_guards( void ) const
    {
        bool const success{
                this->_guard1 == this->_correct_guard
            &&  this->_guard2 == this->_correct_guard
            &&  this->_guard3 == this->_correct_guard};

        if ( !success )
            debug::abort();

        return success;
    }

#endif  //  ALI_ARRAY_DEBUG_OVERFLOW

private:    //  Data members
    ALI_ARRAY_IF_DEBUG_OVERFLOW(
        static ali::uint32 const        _correct_guard{0xaaaaaaaaU};
        ali::uint32                     _guard1{_correct_guard};
    );

    int                                 _size{};

    ALI_ARRAY_IF_DEBUG_OVERFLOW(
        ali::uint32                     _guard2{_correct_guard};
    );

    class in_place_storage
    {
    public:
        operator T*( void ) noexcept
        {
            return _buf.template begin<T>();
        }

        operator T const*( void ) const noexcept
        {
            return _buf.template begin<T>();
        }

    private:    //  Data members
        in_place_buffer<
            sizeof(T) * _capacity,
            alignof(T)>                 _buf{};

    }                                   _begin{};

    ALI_ARRAY_IF_DEBUG_OVERFLOW(
        ali::uint32                     _guard3{_correct_guard};
    );

    template <typename, int>
    friend class array_common;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <typename T>
class array_storage<T, 0>
// ******************************************************************
//  Storage for array with dynamic capacity.
// ******************************************************************
{
public:     //  Enum
    enum unit : ali::uint8 {};
        //  A strong name for byte.
    
    ali_static_assert(sizeof(unit) == sizeof(ali::uint8));

    ali_static_assert(meta::is_trivially_copyable<unit>::result);

public:     //  Methods
        /// \brief Returns the maximum number of
        /// elements this array can potentially hold.
        ///
        /// \returns The maximum number of elements
        /// this array can potentially hold.
        ///
        /// \throws noexcept
        ///
    static constexpr int max_size( void ) noexcept
    {
        return (meta::integer::max_value<int>::result / sizeof(T))
            ALI_ARRAY_IF_DEBUG_OVERFLOW(- 2 * guard_element_count());
    }

        /// \brief  Returns the number of elements this
        /// array can accommodate without triggering
        /// memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee is
        /// valid only for the memory managed by the array
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// The capacity of an array object can be
        /// increased using the \ref reserve method.
        ///
        /// \returns The number of elements this array
        /// can accommodate without triggering memory
        /// allocation.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref reserve
        ///
    int capacity( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_capacity;
    }

private:    //  Methods
    array_storage( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->initialize_overflow_guards();
            this->check_overflow_guards());
    }

    explicit array_storage( int capacity )
    :   _begin{reinterpret_cast<T*>(ali::new_auto_ptr<unit[]>(
            (capacity ALI_ARRAY_IF_DEBUG_OVERFLOW(+ 2 * this->guard_element_count())) * sizeof(T)).release())
                ALI_ARRAY_IF_DEBUG_OVERFLOW(+ this->guard_element_count())},
        _capacity{capacity}
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->initialize_overflow_guards();
            this->check_overflow_guards());
    }

    array_storage( array_storage const& ) = delete;
    array_storage( array_storage&& b ) = delete;

    ~array_storage( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(this->_size == 0);

        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->uninitialize_overflow_guards());

        array_storage_wipe_content(array_ref<T>{
            this->_begin, this->_capacity});

        ali::auto_ptr<unit[]>{
            reinterpret_cast<unit*>(this->_begin
                ALI_ARRAY_IF_DEBUG_OVERFLOW(
                    - (this->_begin == nullptr
                        ? 0 : this->guard_element_count())))};

        this->_begin = invalid_pointer_value;

        this->_size = meta::max_value(this->_size);

        this->_capacity = meta::max_value(this->_capacity) / 2;
    }

    array_storage& operator=( array_storage const& ) = delete;
    array_storage& operator=( array_storage&& b ) = delete;

#ifdef  ALI_ARRAY_DEBUG_OVERFLOW

    struct check_overflow_guards_now_and_on_exit
    {
        array_storage const&  a;

        explicit check_overflow_guards_now_and_on_exit(
            array_storage const&  a )
        :   a{a}
        {
            a.check_overflow_guards();
        }

        ~check_overflow_guards_now_and_on_exit( void )
        {
            a.check_overflow_guards();
        }
    };

    void initialize_overflow_guards( void )
    {
        if ( _begin != nullptr )
        {
            make_mutable_ref(
                this->_begin - this->guard_element_count(),
                this->guard_element_count()).as_mutable_blob()
                    .mutable_ref_back(sizeof(this->_correct_guard))
                    .set_int32_ne_at(0, this->_correct_guard);
            make_mutable_ref(
                this->_begin + this->_capacity,
                this->guard_element_count()).as_mutable_blob()
                    .mutable_ref_front(sizeof(this->_correct_guard))
                    .set_int32_ne_at(0, this->_correct_guard);
        }
    }

    void uninitialize_overflow_guards( void )
    {
        this->_guard1 = 0U;
        this->_guard2 = 0U;
        if ( this->_begin != nullptr )
        {
            make_mutable_ref(
                this->_begin - this->guard_element_count(),
                this->guard_element_count())
                    .as_mutable_blob().fill();
            make_mutable_ref(
                this->_begin + this->_capacity,
                this->guard_element_count())
                    .as_mutable_blob().fill();
        }
    }

    bool check_overflow_guards( void ) const
    {
        bool const success{
                this->_guard1 == this->_correct_guard
            &&  this->_guard2 == this->_correct_guard
            &&  (   this->_begin == nullptr
                ||  (   make_ref(
                            this->_begin - this->guard_element_count(),
                            this->guard_element_count()).as_blob()
                                .ref_back(sizeof(this->_correct_guard))
                                .int32_ne_at(0) == this->_correct_guard
                    &&  make_ref(
                            this->_begin + this->_capacity,
                            this->guard_element_count()).as_blob()
                                .ref_front(sizeof(this->_correct_guard))
                                .int32_ne_at(0) == this->_correct_guard))};

        if ( !success )
            debug::abort();

        return success;
    }

    static int guard_element_count( void )
    {
        return 1 + (sizeof(_correct_guard) - 1) / sizeof(T);
    }

#endif  //  ALI_ARRAY_DEBUG_OVERFLOW

private:    //  Data members
    ALI_ARRAY_IF_DEBUG_OVERFLOW(
        static ali::uint32 const    _correct_guard{0xaaaaaaaaU};
        ali::uint32                 _guard1{_correct_guard};
    );

    T*                              _begin{};
    int                             _size{};
    int                             _capacity{};

    ALI_ARRAY_IF_DEBUG_OVERFLOW(
        ali::uint32                 _guard2{_correct_guard};
    );

    template <typename, int>
    friend class array_common;
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
/// \brief Manages a contiguous sequence of elements
/// of type `T`.
///
/// \tparam T Type of contained elements.
///
/// \pre    `T` supports copying using copy constructor
/// and assignment operator and the copy is equivalent
/// to the original source object.
///
/// \invariant
///
///             0 <= this->size()
///
///         &&  this->size() <= this->capacity()
///
///         &&  this->capacity() <= this->max_size()
///
///         &&  this->is_empty() == (this->size() == 0)
///
// ******************************************************************
template <typename T, int capacity_>
class ALI_ATTRIBUTE_EMPTY_BASES array_common
// ******************************************************************
     :  public array_storage<T, capacity_>,
        public exposes_array_ref<T, array_common<T, capacity_>>
{
    using this_array_storage
        = array_storage<T, capacity_>;
    
    using this_array
        = array<T, capacity_>;

    ALI_ARRAY_IF_DEBUG_OVERFLOW(
        using check_overflow_guards_now_and_on_exit
            = typename this_array_storage
                ::check_overflow_guards_now_and_on_exit);

    using trivially_copyable_version
        = meta::define_bool_result<true>;

    using not_trivially_copyable_version
        = meta::define_bool_result<false>;
    
    using is_trivially_copyable
        = meta::is_trivially_copyable<T>;

    using fixed_capacity_version
        = meta::define_bool_result<true>;

    using dynamic_capacity_version
        = meta::define_bool_result<false>;

    using has_fixed_capacity
        = meta::define_bool_result<capacity_ != 0>;

public:     //  Typedefs
    using read_only_T
        = typename array_const_ref<T>::read_only_T;

    using read_only_T_is_const_ref
        = meta::is_same_type<read_only_T, T const&>;

    using value_type = T;
        ///<        The type of contained elements.

    using iterator = T*;
        ///<\brief  Provides pointer-like read-write
        /// access to individual elements contained
        /// in an `array` object.

    using const_iterator = T const*;
        ///<\brief  Provides pointer-like read-only
        /// access to individual elements contained
        /// in an `array` object.

    using range_type
        = ali::range<T*>;

    using const_range_type
        = ali::range<T const*>;

public:     //  Methods
        /// \brief  Determines whether this array contains any
        /// elements.
        ///
        /// \retval true
        ///         This array contains no elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept
        ///
    bool is_empty( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_size == 0;
    }

        /// \brief  Returns the number of elements
        /// currently stored in this array.
        ///
        /// \returns The number of elements currently
        /// stored in this array.
        ///
        /// \throws noexcept
        ///
    int size( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_size;
    }

    int back_capacity( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->capacity() - this->_size;
    }

    //  reserve

        /// \brief  Increases capacity of this array.
        ///
        /// Ensures that this array will be able to
        /// accommodate at least `cap` elements without
        /// triggering memory allocation.
        ///
        /// \note   The _no memory allocation_ guarantee
        /// is valid only for the memory managed by the array
        /// object itself. Memory might still be allocated
        /// internally by the contained elements, though.
        ///
        /// \note   This method affects only the \ref capacity
        /// of this array, not its \ref size.
        ///
        /// \note   If `cap <= this->capacity()` then this
        /// method has no effect.
        ///
        /// \param  cap
        ///         The desired number of elements this
        ///         array will reserve space for.
        ///
        /// \pre    <tt>0 <= cap</tt>
        ///
        /// \post   <tt>cap <= this->capacity()
        /// && this->capacity() <= this->max_size()</tt>
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& reserve( int min_capacity )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->reserve(min_capacity, has_fixed_capacity{});
    }

    this_array& reserve_back( int min_back_capacity )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->reserve_back(min_back_capacity, has_fixed_capacity{});
    }

    T const* data( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }

    T* data( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }

        /// \brief  Provides read-only access
        /// to the first element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Const reference to the
        /// first element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T const& front( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(!this->is_empty());

        return this->_begin[0];
    }

        /// \brief  Provides read-write access
        /// to the first element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Reference to the first
        /// element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T& front( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(!this->is_empty());

        return this->_begin[0];
    }

        /// \brief  Provides read-only access
        /// to the last element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Const reference to the
        /// last element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T const& back( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(!this->is_empty());

        return this->_begin[this->_size - 1];
    }

        /// \brief  Provides read-write access
        /// to the last element of this array.
        ///
        /// \pre    <tt>!this->is_empty()</tt>
        ///
        /// \returns Reference to the last
        /// element of this array.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    T& back( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(!this->is_empty());

        return this->_begin[this->_size - 1];
    }

        /// \brief  Provides read-only access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Constant reference to
        /// the element at the given index.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref at
        ///
    T const& operator[]( int pos1 ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

        /// \brief  Provides read-write access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Reference to the element at
        /// the given index.
        ///
        /// \throws noexcept
        ///
        /// \see    \ref at
        ///
    T& operator[]( int pos1 ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

        /// \brief  Provides read-only access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Constant reference to
        /// the element at the given index.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref operator[]
        ///
    T const& at( int pos1 ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

        /// \brief  Provides read-write access to
        /// individual elements of this array.
        ///
        /// \param  i
        ///         Index of the desired element.
        ///
        /// \pre    <tt>0 <= i && i < this->size()</tt>
        ///
        /// \returns Reference to the element at
        /// the given index.
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
        /// \see    \ref operator[]
        ///
    T& at( int pos1 ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ali_assert(0 <= pos1);
        ali_assert(pos1 < this->_size);

        return this->_begin[pos1];
    }

    //  push_back

        /// \brief  Appends copies of all elements in
        /// another array to the end of this array.
        ///
        /// \param  b
        ///         Array of objects to initialize the
        ///         new elements with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + b.size()
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall i in set {0, ..., pre(this)->size() - 1} &
        ///                     this->at(i) is not changed in any way
        ///                 ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///         &&  forall i in set {pre(this)->size(), ..., this->size() - 1} &
        ///                 this->at(i) is copy of b.at(i - pre(this)->size())
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_back( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(
            if ( b.weak_reference() == _weak_rc )
                b.weak_reference().reset();
                    //  We can handle aliasing.
        );

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->back_capacity() >= b.size() )
        {
            this->uninitialized_copy_back_n(ali::move(b));
        }
        else
        {
            this->reallocating_copy_back_n(ali::move(b));
        }

        return this->self();
    }

    this_array& append( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return this->push_back(ali::move(b));
    }

        /// \brief  Appends copies of all elements in
        /// an initializer list to the end of this array.
        ///
        /// \param  b
        ///         Initializer list of objects to append.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + b.size()
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_back( ali::initializer_list<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->push_back(make_ref(b));
    }

    this_array& append( ali::initializer_list<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->push_back(b);
    }

    this_array& push_back( read_only_T value, int n2 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});
        ali_assert(0 <= n2);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->back_capacity() >= n2 )
        {
            this->uninitialized_fill_back(value, n2);
        }
        else
        {
            this->reallocating_replace_1(this->_size, 0, value, n2);
        }

        return this->self();
    }

    this_array& append( read_only_T value, int n2 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->push_back(value, n2);
    }

        /// \brief  Appends a copy of the given
        /// element to the end of this array.
        ///
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 2} &
        ///                     this->at(i) is not changed in any way
        ///                 ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///         &&  this->back() is copy of value
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_back( read_only_T value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->_size != this->capacity() )
        {
            this->uninitialized_copy_back_1(value);
        }
        else
        {
            this->reallocating_replace_1(this->_size, 0, value, 1);
        }

        return this->self();
    }

    this_array& append( read_only_T value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->push_back(value);
    }

        /// \brief  Appends the given element to the end of this array
        /// using move semantic.
        ///
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 2} &
        ///                     this->at(i) is not changed in any way
        ///                 ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///         &&  this->back() is moved from value
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type  //  :-/
            push_back( T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->move_back(value);
    }

    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type  //  :-/
            append( T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->move_back(value);
    }

    //  move_back

        /// \brief  Appends all elements in another array
        /// to the end of this array using move semantic.
        ///
        /// \param  b
        ///         Array of objects to initialize the
        ///         new elements with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + b.size()
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall i in set {0, ..., pre(this)->size() - 1} &
        ///                     this->at(i) is not changed in any way
        ///                 ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///         &&  forall i in set {pre(this)->size(), ..., this->size() - 1} &
        ///                 this->at(i) is moved from b.at(i - pre(this)->size())
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& move_back( array_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(
            if ( b.weak_reference() == _weak_rc )
                b.weak_reference().reset();
                    //  We can handle aliasing.
        );

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->back_capacity() >= b.size() )
        {
            this->uninitialized_move_back_n(b);
        }
        else
        {
            this->reallocating_move_back_n(b);
        }

        return this->self();
    }

    this_array& move_back( T& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->_size != this->capacity() )
        {
            this->uninitialized_move_back_1(value);
        }
        else
        {
            this->reallocating_replace_1(this->_size, 0, ali::move(value));
        }

        return this->self();
    }

    //  replace

    this_array& replace( int pos1, int n1, array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);

        ALI_IF_DEBUG_WEAK_REF(
            if ( b.weak_reference() == _weak_rc )
                b.weak_reference().reset();
                    //  We can handle aliasing.
        );

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( n1 >= b.size() )
        {
            this->shrinking_replace_n(pos1, n1, ali::move(b));
        }
        else if ( this->back_capacity() < b.size() - n1 )
        {
            this->reallocating_replace_n(pos1, n1, ali::move(b));
        }
        else
        {
            this->growing_replace_n(pos1, n1, ali::move(b));
        }

        return this->self();
    }

    this_array& replace( int pos1, int n1, ali::initializer_list<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->replace(pos1, n1, make_ref(b));
    }

    this_array& replace( int pos1, int n1, read_only_T value, int n2 = 1 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(0 <= n2);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( n1 >= n2 )
        {
            this->shrinking_replace_1(pos1, n1, value, n2);
        }
        else if ( this->back_capacity() < n2 - n1 )
        {
            this->reallocating_replace_1(pos1, n1, value, n2);
        }
        else
        {
            this->growing_replace_1(pos1, n1, value, n2);
        }

        return this->self();
    }

    //  :-/
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type
            replace( int pos1, int n1, T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( n1 >= 1 )
        {
            this->shrinking_replace_1(pos1, n1, ali::move(value));
        }
        else if ( this->back_capacity() < 1 - n1 )
        {
            this->reallocating_replace_1(pos1, n1, ali::move(value));
        }
        else
        {
            this->growing_replace_1(pos1, n1, ali::move(value));
        }

        return this->self();
    }

    //  erase

        /// \brief  Erases elements in the given range from this array.
        ///
        /// \param  i
        ///         Index of the first element to erase.
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre
        ///
        ///             0 <= i && i <= this->size()
        ///         &&  0 <= n && n <= this->size() - i
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                 this->at(j) is not changed in any way
        ///
        ///         &&  forall j in set {i, ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j + n)
        ///
        ///         &&  forall j in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(j) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    this_array& erase( int pos1, int n1 = 1 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( n1 != 0 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1, this->mutable_ref_right(pos1 + n1));

            this->private_erase_back(n1);
        }

        return this->self();
    }

        /// \brief  Erases all elements contained in this array.
        ///
        /// \note   This method does not necessarily deallocate
        /// the internal buffer. If you need to ensure that the
        /// internal buffer of `arr` is deallocated, then write:
        /// `ali::array<T>().swap(arr)`.
        ///
        /// \post
        ///
        ///             this->is_empty()
        ///
        ///         &&  forall i in set {0, ..., pre(this)->size() - 1}
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    this_array& erase( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        this->private_erase_back(this->_size);

        return this->self();
    }

        /// \brief  Erases elements from the end of this array.
        ///
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre    <tt>0 <= n && n <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is not changed in any way
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    this_array& erase_back( int n1 = 1 ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        this->private_erase_back(n1);

        return this->self();
    }

        /// \brief  Erases elements from the beginning of this array.
        ///
        /// \param  n
        ///         Number of elements to erase.
        ///
        /// \pre    <tt>0 <= n && n <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() - n
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is moved from pre(this)->at(i + n)
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    this_array& erase_front( int n1 = 1 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->erase(0, n1);
    }

    this_array& erase_left( int pos1 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->erase(0, pos1);
    }

    this_array& erase_right( int pos1 ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        this->private_erase_back(this->_size - pos1);

        return this->self();
    }

        /// \brief  Erases elements matching the specified predicate.
        ///
        /// \tparam predicate
        ///         The predicate's type.
        /// \param  p
        ///         The predicate; an element `t` is erased
        ///         if `p(t)` evaluates to `true`.
        ///
        /// \pre    For an object `t` of type `T`, `p(t)` is valid
        ///         expression convertible to `bool`.
        ///
        /// \post
        ///
        ///             result == pre(this)->size() - this->size()
        ///
        ///         &&  let original_indexes_of_retained_elements
        ///                     = [i | 0 <= i && i < pre(this)->size() && !p(pre(this)->at(i))]
        ///                         //  This is a sequence, not a set -- order is important.
        ///
        ///             in      this->size() == len original_indexes_of_retained_elements
        ///
        ///                 &&  forall i in set {0, ..., this->size() - 1} &
        ///                             i == original_indexes_of_retained_elements(i)
        ///                         ||  this->at(i) is moved from pre(this)->at(
        ///                                 original_indexes_of_retained_elements(i))
        ///
        ///         &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                 pre(this)->at(i) is destroyed
        ///
        /// \returns The number of erased elements.
        ///
        /// \throws noexcept <tt>(noexcept(p(T) && noexcept(T = T) && noexcept(T::~T()))</tt>
        ///
    template <typename predicate>
    int erase_if( predicate p )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        int const result{
            this->_size - this->mutable_ref().stable_partition_left_only(
                [p] ( T& value ) { return !p(value); })};

        this->private_erase_back(result);

        return result;
    }

    //  resize

        /// \brief  Appends/erases elements to/from this
        /// array such that its \ref size is eual to the
        /// specified value.
        ///
        /// Appended elements are initialized with `value`.
        ///
        /// \param  size
        ///         The desired size of this array.
        /// \param  value
        ///         Object to initialize appended elements with.
        ///
        /// \pre    <tt>0 <= size</tt>
        ///
        /// \post
        ///
        ///                 this->size() == size
        ///
        ///             &&  this->size() <= this->max_size()
        ///
        ///             &&  (    this->size() == pre(this)->size()
        ///
        ///                 ||  (   this->size() < pre(this)->size()
        ///
        ///                     &&  forall i in set {0, ..., this->size() - 1} &
        ///                             this->at(i) is not changed in any way
        ///
        ///                     &&  forall i in set {this->size(), ..., pre(this)->size() - 1} &
        ///                             pre(this)->at(i) is destroyed)
        ///
        ///                 ||  (   this->size() > pre(this)->size()
        ///
        ///                     &&  forall i in set {0, ..., pre(this)->size() - 1} &
        ///                                 this->at(i) is not changed in any way
        ///                             ||  this->at(i) is moved from pre(this)->at(i)
        ///
        ///                     &&  forall i in set {pre(this)->size(), ..., this->size() - 1} &
        ///                             this->at(i) is copy of value))
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& resize( int size, read_only_T value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});
        ali_assert(0 <= size);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( size < this->_size )
        {
            this->private_erase_back(this->_size - size);
        }
        else if ( size > this->_size )
        {
            this->push_back(value, size - this->_size);
        }

        return this->self();
    }

    this_array& resize( int size )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});
        ali_assert(0 <= size);
        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( size < this->_size )
        {
            this->private_erase_back(this->_size - size);
        }
        else if ( size > this->_size )
        {
            this->reserve(size);

            this->default_construct_back(size - this->_size);
        }

        return this->self();
    }

    //  assign

    this_array& assign( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(
            if ( b.weak_reference() == _weak_rc )
                b.weak_reference().reset();
                    //  We can handle aliasing.
        );

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this->_size >= b.size() )
        {
            this->shrinking_assign(ali::move(b));
        }
        else if ( this->back_capacity() < b.size() - this->_size )
        {
            this->reallocating_assign(ali::move(b));
        }
        else
        {
            this->growing_assign(ali::move(b));
        }

        return this->self();
    }

    this_array& assign( ali::initializer_list<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->assign(make_ref(b));
    }

    this_array& assign( read_only_T value, int n2 = 1 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->replace(0, this->_size, value, n2);
    }

    //  :-/
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type
            assign( T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->replace(0, this->_size, ali::move(value));
    }

    //  push_front

    this_array& push_front( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return this->replace(0, 0, ali::move(b));
    }

    this_array& push_front( ali::initializer_list<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->push_front(make_ref(b));
    }

        /// \brief  Inserts a copy of the given element at the
        /// beginning of this array.
        ///
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  this->front() is copy of value
        ///
        ///         &&  forall i in set {1, ..., this->size() - 1} &
        ///                 this->at(i) is moved from pre(this)->at(i - 1)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& push_front( read_only_T value, int n2 = 1 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->replace(0, 0, value, n2);
    }

    //  :-/
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type
            push_front( T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->replace(0, 0, ali::move(value));
    }

    //  insert

        /// \brief  Inserts a copy of all elements in another
        /// array into this array at the specified index.
        ///
        /// \param  i
        ///         Index where to place the first new element.
        /// \param  b
        ///         Array of objects to initialize the new elements with.
        ///
        /// \pre    <tt>0 <= i && i <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + b.size()
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                     this->at(j) is not changed in any way
        ///                 ||  this->at(j) is moved from pre(this)->at(j)
        ///
        ///         &&  forall j in set {i, ..., i + b.size() - 1} &
        ///                 this->at(j) is copy of b.at(j - i)
        ///
        ///         &&  forall j in set {i + b.size(), ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j - b.size())
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& insert( int pos1, array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return this->replace(pos1, 0, ali::move(b));
    }

    this_array& insert( int pos1, ali::initializer_list<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->insert(pos1, make_ref(b));
    }

        /// \brief  Inserts a copy of the given element into
        /// this array at the specified index.
        ///
        /// \param  i
        ///         Index where to place the new element.
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \pre    <tt>0 <= i && i <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                     this->at(j) is not changed in any way
        ///                 ||  this->at(j) is moved from pre(this)->at(j)
        ///
        ///         &&  this->at(i) is copy of value
        ///
        ///         &&  forall j in set {i + 1, ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j - 1)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    this_array& insert( int pos1, read_only_T value, int n2 = 1 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->replace(pos1, 0, value, n2);
    }

        /// \brief  Inserts the given element into this array at the
        //  specified index using move semantic.
        ///
        /// \param  i
        ///         Index where to place the new element.
        /// \param  value
        ///         Object to initialize the new element with.
        ///
        /// \pre    <tt>0 <= i && i <= this->size()</tt>
        ///
        /// \post
        ///
        ///             this->size() == pre(this)->size() + 1
        ///
        ///         &&  this->size() <= this->max_size()
        ///
        ///         &&  forall j in set {0, ..., i - 1} &
        ///                     this->at(j) is not changed in any way
        ///                 ||  this->at(j) is moved from pre(this)->at(j)
        ///
        ///         &&  this->at(i) is moved from value
        ///
        ///         &&  forall j in set {i + 1, ..., this->size() - 1} &
        ///                 this->at(j) is moved from pre(this)->at(j - 1)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    template <typename return_type = this_array&>
    typename meta::enable_if<
        read_only_T_is_const_ref::result,
        return_type>::type  //  :-/
            insert( int pos1, T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->replace(pos1, 0, ali::move(value));
    }

    //  ref

    array_const_ref<T> ref( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_const_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(_weak_rc,)
            this->_begin, this->_size};
    }

    array_const_ref<T> ref( int pos, int n ) const noexcept
    {
        return this->ref().ref(pos, n);
    }

    //  mutable_ref

    array_ref<T> mutable_ref( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        return array_ref<T>{
            ALI_IF_DEBUG_WEAK_REF(_weak_rc,)
            this->_begin, this->_size};
    }

    array_ref<T> mutable_ref( int pos, int n ) noexcept
    {
        return this->mutable_ref().mutable_ref(pos, n);
    }

    void assert_invariant( void ) const noexcept
    {
        ali_assert(0 <= this->size());
        ali_assert(this->size() <= this->capacity());
        ali_assert(this->capacity() <= this->max_size());
        ali_assert(this->is_empty() == (this->size() == 0));
    }

        /// \brief  Determines whether the given object resides
        /// inside this array.
        ///
        /// \param  value
        ///         Object to test.
        ///
        /// \returns Index into this array where the given object
        /// is located, if it indeed resides in this array.
        /// Value less than zero otherwise.
        ///
        /// \throws noexcept
        ///
    is_internal_element_result is_internal_element( T const& value ) const noexcept
    {
        return this->ref().is_internal_element(value);
    }

    is_internal_pointer_result is_internal_pointer( T const* b ) const noexcept
    {
        return this->ref().is_internal_pointer(b);
    }

    is_internal_pointer_result is_internal_pointer( array_const_ref<T> b ) const noexcept
    {
        return this->ref().is_internal_pointer(ali::move(b));
    }

    is_internal_pointer_result is_internal_pointer( array_const_ptr<T> b ) const noexcept
    {
        return this->ref().is_internal_pointer(ali::move(b));
    }

    //  iterators

        /// \brief  Provides read-only iterator
        /// pointing to the beginning of this array.
        ///
        /// \returns \ref const_iterator object
        /// pointing to the beginning of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    const_iterator begin( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }

    friend const_iterator begin( array_common const& a ) noexcept
    {
        return a.begin();
    }

        /// \brief  Provides read-write iterator
        /// pointing to the beginning of this array.
        ///
        /// \returns \ref iterator object pointing
        /// to the beginning of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    iterator begin( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin;
    }

    friend iterator begin( array_common& a ) noexcept
    {
        return a.begin();
    }

        /// \brief  Provides read-only iterator pointing to
        /// the one past the last element of this array.
        ///
        /// \returns \ref const_iterator object pointing to
        /// the one past the last element of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    const_iterator end( void ) const noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin + this->_size;
    }

    friend const_iterator end( array_common const& a ) noexcept
    {
        return a.end();
    }

        /// \brief  Provides read-write iterator pointing to
        /// the one past the last element of this array.
        ///
        /// \returns \ref iterator object pointing to
        /// the one past the last element of this array.
        ///
        /// \throws noexcept
        ///
        /// \deprecated Use indexes instead.
        ///
    iterator end( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        return this->_begin + this->_size;
    }

    friend iterator end( array_common& a ) noexcept
    {
        return a.end();
    }

    //  range

    const_range_type range( void ) const noexcept
    {
        return const_range_type(this->begin(), this->end());
    }

    range_type range( void ) noexcept
    {
        return range_type(this->begin(), this->end());
    }

        /// \brief  Determines whether both arrays are of
        /// the same size and contain equal elements.
        ///
        /// \param  b
        ///         Another array to compare this array with.
        ///
        /// \pre    There are `operator==` and `operator!=`
        /// that accept objects of type `T` and
        /// `(a != b) == !(a == b)` for all `a`, `b` of type `T`.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->size() == b.size()
        ///                 &&  forall i in set {0, ..., this->size() - 1} &
        ///                         this->at(i) == b.at(i)
        ///
        /// \retval true
        ///         Both arrays are of the same size
        ///         and contain equal elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(T == T) && noexcept(T != T))</tt>
        ///
    //bool operator==( array_const_ref<T> b ) const
    //{
    //    ALI_ARRAY_IF_DEBUG_OVERFLOW(
    //        this->check_overflow_guards());
    //
    //    return this->ref() == b;
    //}

        /// \brief  Determines whether both arrays are of
        /// different size or contain non-equal elements.
        ///
        /// \param  b
        ///         Another array to compare this array with.
        ///
        /// \pre    There are `operator==` and `operator!=`
        /// that accept objects of type `T` and
        /// `(a != b) == !(a == b)` for all `a`, `b` of type `T`.
        ///
        /// \post
        ///
        ///             result
        ///                 ==  this->size() != b.size()
        ///                 ||  exists i in set {0, ..., this->size() - 1} &
        ///                         this->at(i) != b.at(i)
        ///
        /// \retval true
        ///         The arrays differ in size or
        ///         in the contained elements.
        /// \retval false
        ///         Otherwise.
        ///
        /// \throws noexcept <tt>(noexcept(T == T) && noexcept(T != T))</tt>
        ///
    //bool operator!=( array_const_ref<T> b ) const
    //{
    //    return !this->operator==(b);
    //}

protected:  //  Methods

        /// \brief  Default constructor. Constructs empty array.
        /// \post   <tt>this->is_empty()</tt>
        /// \throws noexcept
        ///
    array_common( void ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
    }

        /// \brief  Copy constructor.
        /// Initializes this array by copying all elements
        /// from another array.
        ///
        /// \param  b
        ///         Another instance of the `array` class.
        ///
        /// \post
        ///
        ///             this->size() == b.size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is copy of b.at(i)
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_common( array_common const& b )
    :   array_common{b.ref()}
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            b.check_overflow_guards());

        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
    }

        /// \brief  Move constructor.
        /// Initializes this array by moving all elements
        /// from another array.
        ///
        /// \param  b
        ///         Another instance of the `array` class.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 &this->at(i) == &pre(b)->at(i)
        ///                     //  Note the address-of operators (&) above.
        ///
        /// \throws noexcept
        ///
    array_common( array_common&& b ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        this->swap(b);
    }

    array_common( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->push_back(ali::move(b));
    }

        /// \brief Initializes this array with the specified
        /// number of elements.
        ///
        /// \param  size
        ///         The desired number of elements in the array.
        ///
        /// \pre    <tt>0 <= size</tt>
        ///
        /// \post
        ///
        ///             this->size() == size
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is default constructed element
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    explicit array_common( int size )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->resize(size);
    }

        /// \brief Initializes this array with the specified
        /// number of elements.
        ///
        /// \param  size
        ///         The desired number of elements in the array.
        /// \param  value
        ///         Object to initialize elements of this array with.
        ///
        /// \pre    <tt>0 <= size</tt>
        ///
        /// \post
        ///
        ///             this->size() == size
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is copy of value
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_common( int size, read_only_T value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->resize(size, value);
    }

    //  array_common( ali::initializer_list<T> b );
            //  Intentionally postponed to derived classes as
            //  this constructor is a nasty little game changer.

        /// \brief  Destructor.
        /// Destroys all contained elements and
        /// frees all memory used by this array.
        ///
        /// \post
        ///
        ///         forall i in set {0, ..., pre(this)->size() - 1} &
        ///             pre(this)->at(i) is destroyed
        ///
        /// \throws noexcept <tt>(noexcept(T::~T()))</tt>
        ///
    ~array_common( void ) noexcept
    {
        ALI_IF_DEBUG(this->assert_invariant());

        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        this->erase();
    }

        /// \brief  Assignment operator.
        /// Copies all elements from another array into this array.
        ///
        /// \param  b
        ///         Array of elements to copy into this array.
        ///
        /// \post
        ///
        ///             this->size() == b.size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 this->at(i) is copy of b.at(i)
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array_common& operator=( array_common const& b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->operator=(b.ref());
    }

        /// \brief  Move assignment operator.
        /// Moves all elements from another array into this array.
        ///
        /// \param  b
        ///         Array of elements to move into this array.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        ///         &&  forall i in set {0, ..., this->size() - 1} &
        ///                 &this->at(i) == &pre(b)->at(i)
        ///                     //  Note the address-of operators (&) above.
        ///
        /// \returns `*this`
        ///
        /// \throws noexcept
        ///
    array_common& operator=( array_common&& b ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _b{b});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        if ( this != &b )
        {
            this->swap(b);

            array_common{}.swap(b);
        }

        return *this;
    }

    array_common& operator=( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(
            if ( b.weak_reference() == _weak_rc )
                b.weak_reference().reset();
                    //  We can handle aliasing.
        );

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->assign(ali::move(b));
    }

    array_common& operator=( ali::initializer_list<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ALI_IF_DEBUG_WEAK_REF(this->check_no_weak_ref());

        return this->operator=(make_ref(b));
    }

    int calculate_auto_capacity( int min_back_capacity ) const
        //  pre:    0 < n
        //  post:       n <= result - this->_size
        //          &&  result <= this->max_size()
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 < min_back_capacity);

        int const max_size{this->max_size()};

        this->raise_requested_capacity_too_large_if(
            min_back_capacity > max_size - this->_size,
            ALI_HERE);

        int const capacity{ali::maxi(4, this->_size + ali::maxi(
            ali::mini(max_size - this->_size, this->_size / 2),
            min_back_capacity))};

        ali_assert(min_back_capacity <= capacity - this->_size);
        ali_assert(capacity <= max_size);

        return capacity;
    }

    this_array& reserve( int min_capacity, fixed_capacity_version )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= min_capacity);

        this->raise_requested_capacity_too_large_if(
            min_capacity > this->max_size(), ALI_HERE);

        return this->self();
    }

    this_array& reserve( int min_capacity, dynamic_capacity_version )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= min_capacity);

        if ( this->capacity() < min_capacity )
        {
            this->raise_requested_capacity_too_large_if(
                min_capacity > this->max_size(), ALI_HERE);

            this->private_reserve(min_capacity);
        }

        ali_assert(min_capacity <= this->capacity());

        return this->self();
    }

    this_array& reserve_back( int min_back_capacity, fixed_capacity_version )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= min_back_capacity);

        this->raise_requested_capacity_too_large_if(
            min_back_capacity > this->max_size() - this->_size,
            ALI_HERE);

        return this->self();
    }

    this_array& reserve_back( int min_back_capacity, dynamic_capacity_version )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= min_back_capacity);

        if ( this->back_capacity() < min_back_capacity )
        {
            this->raise_requested_capacity_too_large_if(
                min_back_capacity > this->max_size() - this->_size,
                ALI_HERE);

            this->private_reserve(this->_size + min_back_capacity);
        }

        ali_assert(min_back_capacity <= this->back_capacity());

        return this->self();
    }

    void private_reserve( int capacity )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(this->capacity() < capacity);
        ali_assert(capacity <= this->max_size());

        array_common temp{private_version{}, capacity};

        ali_assert(temp._size == 0);
        ali_assert(temp.capacity() == capacity);
        ali_assert(temp._begin != nullptr);

        temp.uninitialized_move_back_n(*this);

        temp.swap(*this);

        ali_assert(this->capacity() == capacity);
    }

    void default_construct_back( int const n )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->default_construct_back(n, is_trivially_copyable{});
    }

        void default_construct_back( int const n, trivially_copyable_version )
        {
            ali_assert(0 <= n);
            ali_assert(n <= this->back_capacity());

            this->_size += n;

            this->mutable_ref_back(n).fill(T());
        }

        void default_construct_back( int const n, not_trivially_copyable_version )
        {
            ali_assert(0 <= n);
            ali_assert(n <= this->back_capacity());

            for ( int i{}; i != n; ++i, ++this->_size )
                new (this->_begin + this->_size) T();
        }

    template <typename constructor>
    void construct_back(
        int const n,
        constructor construct )
            //  construct( T* storage, int i )
            //  i goes from 0 upto n - 1, inclusive.
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});
        ali_assert(0 <= n);
        ali_assert(n <= this->back_capacity());

        for ( int i{}; i != n; ++i, ++this->_size )
            construct(this->_begin + this->_size, i);
    }

    void uninitialized_copy_back_n( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->uninitialized_copy_back_n(ali::move(b), is_trivially_copyable{});
    }

        void uninitialized_copy_back_n( array_const_ref<T> b, trivially_copyable_version )
        {
            ali_assert(b.size() <= this->back_capacity());

            this->_size += b.size();

            this->mutable_ref().copy_back(ali::move(b));
        }

        void uninitialized_copy_back_n( array_const_ref<T> b, not_trivially_copyable_version )
        {
            this->construct_back(
                b.size(),
                [b]( T* storage, int i )
                { new (storage) T(b[i]); });
        }

    void uninitialized_copy_back_1( read_only_T value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->uninitialized_copy_back_1(value, is_trivially_copyable{});
    }

        void uninitialized_copy_back_1( read_only_T value, trivially_copyable_version )
        {
            ali_assert(1 <= this->back_capacity());

            this->_begin[this->_size] = value;

            ++this->_size;
        }

        void uninitialized_copy_back_1( read_only_T value, not_trivially_copyable_version )
        {
            ali_assert(1 <= this->back_capacity());

            new (this->_begin + this->_size) T(value);

            ++this->_size;
        }

    void uninitialized_move_back_n( array_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->uninitialized_move_back_n(b, is_trivially_copyable{});
    }

        void uninitialized_move_back_n( array_ref<T> b, trivially_copyable_version version )
        {
            this->uninitialized_copy_back_n(b, version);
        }

        void uninitialized_move_back_n( array_ref<T> b, not_trivially_copyable_version )
        {
            this->construct_back(
                b.size(),
                [&b]( T* storage, int i )
                { new (storage) T(ali::move_if_noexcept(b.at(i))); });
        }

    void uninitialized_move_back_1( T& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->uninitialized_move_back_1(value, is_trivially_copyable{});
    }

        void uninitialized_move_back_1( T& value, trivially_copyable_version version )
        {
            this->uninitialized_copy_back_1(value, version);
        }

        void uninitialized_move_back_1( T& value, not_trivially_copyable_version )
        {
            ali_assert(1 <= this->back_capacity());

            new (this->_begin + this->_size) T(ali::move_if_noexcept(value));

            ++this->_size;
        }

    void uninitialized_fill_back( read_only_T value, int n2 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->uninitialized_fill_back(value, n2, is_trivially_copyable{});
    }

        void uninitialized_fill_back( read_only_T value, int n2, trivially_copyable_version )
        {
            ali_assert(0 <= n2);
            ali_assert(n2 <= this->back_capacity());

            this->_size += n2;

            this->mutable_ref_back(n2).fill(value);
        }

        void uninitialized_fill_back( read_only_T value, int n2, not_trivially_copyable_version )
        {
            this->construct_back(
                n2,
                [&value]( T* storage, int )
                { new (storage) T(value); });
        }

    void reallocating_copy_back_n( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->reallocating_copy_back_n(ali::move(b), has_fixed_capacity{});
    }

        void reallocating_copy_back_n(
            array_const_ref<T>,
            fixed_capacity_version )
        {
            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_copy_back_n(
            array_const_ref<T> b,
            dynamic_capacity_version )
        {
            array_common temp{
                private_version{},
                this->calculate_auto_capacity(b.size())};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(b.front());

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative >= 0);

                int const pos2{internal_element.index_or_negative};

                int const n2{b.size()};
                    //  b's elements were moved to temp along
                    //  with ours and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "b" from this.

                this->uninitialized_copy_back_n(this->ref(pos2, n2));
            }
            else
            {
                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative < 0);

                this->uninitialized_copy_back_n(ali::move(b));
            }
        }

    void reallocating_move_back_n( array_ref<T> b )
    {
        this->reallocating_move_back_n(b, has_fixed_capacity{});
    }

        void reallocating_move_back_n(
            array_ref<T> b,
            fixed_capacity_version )
        {
            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_move_back_n(
            array_ref<T> b,
            dynamic_capacity_version )
        {
            array_common temp{
                private_version{},
                this->calculate_auto_capacity(b.size())};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(b.front());

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative >= 0);

                int const pos2{internal_element.index_or_negative};

                int const n2{b.size()};
                    //  b's elements were moved to temp along
                    //  with ours and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "b" from this.

                this->uninitialized_move_back_n(this->mutable_ref(pos2, n2));
            }
            else
            {
                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative < 0);

                this->uninitialized_move_back_n(b);
            }
        }

    void reallocating_replace_n( int pos1, int n1, array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->reallocating_replace_n(pos1, n1, ali::move(b), has_fixed_capacity{});
    }

        void reallocating_replace_n(
            int ALI_IF_DEBUG(pos1),
            int ALI_IF_DEBUG(n1),
            array_const_ref<T> ALI_IF_DEBUG(b),
            fixed_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < b.size());

            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_replace_n(
            int pos1, int n1, array_const_ref<T> b,
            dynamic_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < b.size());

            array_common temp{
                private_version{},
                this->calculate_auto_capacity(b.size() - n1)};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(b.front());

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative >= 0);

                int const pos2{internal_element.index_or_negative};

                int const n2{b.size()};
                    //  b's elements were moved to temp along
                    //  with ours and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "b" from this.

                this->growing_replace_n(pos1, n1, this->ref(pos2, n2));
            }
            else
            {
                ali_assert(temp.is_internal_element(
                    b.back()).index_or_negative < 0);

                this->growing_replace_n(pos1, n1, ali::move(b));
            }
        }

    void reallocating_replace_1( int pos1, int n1, read_only_T value, int n2 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->reallocating_replace_1(
            pos1, n1, value, n2,
            has_fixed_capacity{});
    }

        void reallocating_replace_1(
            int ALI_IF_DEBUG(pos1),
            int ALI_IF_DEBUG(n1),
            read_only_T /*value*/,
            int ALI_IF_DEBUG(n2),
            fixed_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < n2);

            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_replace_1(
            int pos1, int n1, read_only_T value, int n2,
            dynamic_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size - pos1);
            ali_assert(n1 < n2);

            array_common temp{
                private_version{},
                this->calculate_auto_capacity(n2 - n1)};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(value);

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                int const pos2{internal_element.index_or_negative};
                    //  value was moved to temp along with our
                    //  elements and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "value" from this.

                this->growing_replace_1(pos1, n1, this->_begin[pos2], n2);
            }
            else
            {
                this->growing_replace_1(pos1, n1, value, n2);
            }
        }

    void reallocating_replace_1( int pos1, int n1, T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->reallocating_replace_1(
            pos1, n1, ali::move(value),
            has_fixed_capacity{});
    }

        void reallocating_replace_1(
            int ALI_IF_DEBUG(pos1),
            int ALI_IF_DEBUG(n1),
            T&& /*value*/,
            fixed_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(n1 == 0);

            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_replace_1(
            int pos1, int n1, T&& value,
            dynamic_capacity_version )
        {
            ali_assert(0 <= pos1);
            ali_assert(pos1 <= this->_size);
            ali_assert(n1 == 0);

            array_common temp{
                private_version{},
                this->calculate_auto_capacity(1)};

            {
                temp.uninitialized_move_back_n(*this);

                temp.swap(*this);
            }

            //  Now temp holds the original vector.
            //  Thus, regarding aliasing, we need
            //  to check against temp, not this.

            auto const internal_element
                = temp.is_internal_element(value);

            if ( internal_element.index_or_negative >= 0 )
            {
                //  Aliasing.

                int const pos2{internal_element.index_or_negative};
                    //  value was moved to temp along with our
                    //  elements and we then swapped this and temp.
                    //  Here, we calculate the coordinates we
                    //  will use to obtain a new "value" from this.

                this->growing_replace_1(
                    pos1, n1, ali::move(this->_begin[pos2]));
            }
            else
            {
                this->growing_replace_1(
                    pos1, n1, ali::move(value));
            }
        }

    void growing_replace_n( int pos1, int n1, array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(n1 < b.size());

        int const delta{b.size() - n1};

        ali_assert(this->back_capacity() >= delta);

        int const end1{pos1 + n1};

        if ( pos1 == this->_size )
        {
            //  push_back

            this->uninitialized_copy_back_n(ali::move(b));
        }
        else if ( end1 == this->_size )
        {
            //  replace_right

            this->uninitialized_copy_back_n(b.ref_right(n1));

            this->mutable_ref().copy(pos1, b.ref_left(n1));
        }
        else
        {
            auto const internal_element
                = this->is_internal_element(b.front());

            if ( internal_element.index_or_negative >= 0 )
            {
                ali_assert(this->is_internal_element(
                    b.back()).index_or_negative >= 0);

                int const pos2{internal_element.index_or_negative};

                this->uninitialized_copy_back_n(this->ref(pos2 + n1, delta));

                this->mutable_ref().copy(pos1, this->ref(pos2, n1));

                this->mutable_ref_right(pos1 + n1).rotate_right(delta);
            }
            else
            {
                ali_assert(this->is_internal_element(
                    b.back()).index_or_negative < 0);

                if ( pos1 + delta > this->_size )
                {
                    int const n_tail{this->_size - end1};

                    int const pos2{this->_size - pos1};

                    this->uninitialized_copy_back_n(b.ref_right(pos2));
                        //  also: b.ref_back(delta - (this->_size - end1)));

                    this->uninitialized_move_back_n(
                        this->mutable_ref(end1, n_tail));

                    this->mutable_ref().copy(pos1, b.ref_left(pos2));
                }
                else
                {
                    this->uninitialized_move_back_n(
                        this->mutable_ref_back(delta));

                    if ( this->_size - end1 > 2 * delta )
                    {
                        this->mutable_ref().move_if_noexcept(
                            pos1 + b.size(),
                            this->mutable_ref(
                                end1,
                                this->_size - 2 * delta - end1));
                    }

                    this->mutable_ref().copy(pos1, ali::move(b));
                }
            }
        }
    }

    void growing_replace_1( int pos1, int n1, read_only_T value, int n2 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(n1 < n2);

        int const delta{n2 - n1};

        ali_assert(this->back_capacity() >= delta);

        int const end1{pos1 + n1};

        if ( end1 == this->_size )
        {
            this->uninitialized_fill_back(value, delta);

            this->mutable_ref(pos1, n1).fill(value);
        }
        else
        {
            this->uninitialized_copy_back_n(this->ref_back(delta));

            if ( this->_size - end1 > 2 * delta )
            {
                this->mutable_ref().copy(
                    pos1 + n2,
                    this->ref(
                        end1,
                        this->_size - 2 * delta - end1));

                auto const internal_element
                    = this->is_internal_element(value);

                if ( internal_element.index_or_negative >= 0 )
                {
                    int pos2 = internal_element.index_or_negative;

                    if ( pos2 >= pos1 + n2 )
                        pos2 += delta;

                    this->mutable_ref(pos1, n2).fill(this->_begin[pos2]);
                }
                else
                {
                    this->mutable_ref(pos1, n2).fill(value);
                }
            }
            else
            {
                this->mutable_ref(pos1, n2).fill(value);
            }
        }
    }

    void growing_replace_1(
        int pos1, int ALI_IF_DEBUG(n1), T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(n1 == 0);

        ali_assert(this->back_capacity() >= 1);

        if ( pos1 == this->_size )
        {
            this->uninitialized_move_back_1(value);
        }
        else
        {
            T temp(ali::move_if_noexcept(value));
                //  Move the value to a temporary first,
                //  because the following call may change
                //  the value if &value == &this->back().

            this->uninitialized_move_back_1(this->back());
                //  This may change the value
                //  if &value == &this->back().

            if ( this->_size - pos1 > 2 )
            {
                this->mutable_ref().move_if_noexcept(
                    pos1 + 1,
                    this->mutable_ref(
                        pos1, this->_size - 2 - pos1));
            }

            this->_begin[pos1] = ali::move_if_noexcept(temp);
        }
    }

    void shrinking_replace_n( int pos1, int n1, array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(b.size() <= n1);

        this->mutable_ref().copy(pos1, b);

        if ( b.size() < n1 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1 + b.size(),
                this->mutable_ref_right(
                    pos1 + n1));

            this->private_erase_back(n1 - b.size());
        }
    }

    void shrinking_replace_1( int pos1, int n1, read_only_T value, int n2 )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(0 <= n1);
        ali_assert(n1 <= this->_size - pos1);
        ali_assert(n2 <= n1);

        this->mutable_ref(pos1, n2).fill(value);

        if ( n2 < n1 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1 + n2,
                this->mutable_ref_right(
                    pos1 + n1));

            this->private_erase_back(n1 - n2);
        }
    }

    void shrinking_replace_1( int pos1, int n1, T&& value )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(0 <= pos1);
        ali_assert(pos1 <= this->_size);
        ali_assert(1 <= n1);
        ali_assert(n1 <= this->_size - pos1);

        this->_begin[pos1] = ali::move_if_noexcept(value);

        if ( 1 < n1 )
        {
            this->mutable_ref().move_if_noexcept(
                pos1 + 1,
                this->mutable_ref_right(
                    pos1 + n1));

            this->private_erase_back(n1 - 1);
        }
    }

    //  The *_assign methods are specializations of *_replace methods
    //  with the first two parameters fixed at 0 and this->_size.
    //  The motivation is to eliminate unnecessary call to rotate_right
    //  which requires T to define swap function.

    void reallocating_assign( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->reallocating_assign(ali::move(b), has_fixed_capacity{});
    }

        void reallocating_assign(
            array_const_ref<T> /*b*/,
            fixed_capacity_version )
        {
            this->raise_requested_capacity_too_large(ALI_HERE);
        }

        void reallocating_assign(
            array_const_ref<T> b,
            dynamic_capacity_version )
        {
            ali_assert(this->_size < b.size());

            array_common temp{private_version{}, b.size()};

            temp.uninitialized_copy_back_n(ali::move(b));

            temp.swap(*this);
        }

    void growing_assign( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(this->_size < b.size());
        ali_assert(this->back_capacity() >= b.size() - this->_size);

        this->uninitialized_copy_back_n(b.ref_right(this->_size));

        this->mutable_ref().copy(0, b.ref_left(this->_size));
    }

    void shrinking_assign( array_const_ref<T> b )
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        ali_assert(b.size() <= this->_size);

        this->mutable_ref().copy(0, b);

        this->private_erase_back(this->_size - b.size());
    }

    void private_erase_back( int n1 ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->private_erase_back(n1, is_trivially_copyable{});
    }

        void private_erase_back( int n1, trivially_copyable_version ) noexcept
        {
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size);

            this->_size -= n1;
        }

        void private_erase_back( int n1, not_trivially_copyable_version ) noexcept
        {
            ali_assert(0 <= n1);
            ali_assert(n1 <= this->_size);

            for ( ; n1 != 0; --n1 )
            {
                --this->_size;

                this->_begin[this->_size].~T();
            }
        }

        /// \brief  Swaps contents of this array with another array.
        ///
        /// \param  b
        ///         Another array to swap content with.
        ///
        /// \post
        ///
        ///                 this->size() == pre(b).size()
        ///
        ///             &&  forall i in set {0, ..., this->size() - 1} &
        ///                     &this->at(i) == &pre(b).at(i)
        ///
        ///             &&  b.size() == pre(this)->size()
        ///
        ///             &&  forall i in set {0, ..., b.size() - 1} &
        ///                     &b.at(i) == &pre(this)->at(i)
        ///
        /// \throws noexcept
        ///
    void swap( array_common& b ) noexcept
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            check_overflow_guards_now_and_on_exit _{*this});

        this->swap(b, has_fixed_capacity{});
    }

        void swap( array_common& b, fixed_capacity_version ) noexcept
        {
            //  Swap elements.

            int const m{ali::maxi(this->_size, b._size)};
            int const this_delta{m - this->_size};
            int const b_delta{m - b._size};

            this->default_construct_back(this_delta);
            b.default_construct_back(b_delta);
                //  Make the arrays the same size.

            ali_assert(this->_size == b._size);

            this->mutable_ref()
                .swap_block_front(
                    b.mutable_ref());
                        //  Swap individual elements.

            this->private_erase_back(b_delta);
                //  Intentionally erasing b_delta elements from this.
            b.private_erase_back(this_delta);
                //  Intentionally erasing this_delta elements from b.
        }

        void swap( array_common& b, dynamic_capacity_version ) noexcept
        {
            //  Swap pointers.

            using ali::swap;
            swap(this->_begin, b._begin);
            swap(this->_size, b._size);
            swap(this->_capacity, b._capacity);
            ALI_IF_DEBUG_WEAK_REF(
                swap(this->_weak_rc, b._weak_rc));
        }

    this_array& self( void ) noexcept
    {
        return static_cast<this_array&>(*this);
    }

    this_array const& self( void ) const noexcept
    {
        return static_cast<this_array const&>(*this);
    }

    static void raise_requested_capacity_too_large(
        ali::location const& location )
    {
        general_error("Requested capacity too large"_s, location);
    }

    static void raise_requested_capacity_too_large_if(
        bool condition, ali::location const& location )
    {
        if ( condition )
            raise_requested_capacity_too_large(location);
    }

private:    //  Methods
    struct private_version{};

    array_common( private_version, int capacity )
    :   this_array_storage{capacity}
    {
        ALI_ARRAY_IF_DEBUG_OVERFLOW(
            this->check_overflow_guards());
    }

    template <typename, int>
    friend class hidden::array_storage;

    ALI_IF_DEBUG_WEAK_REF(
        bool check_no_weak_ref( void ) const noexcept
        {
            bool const success{_weak_rc.get() == 0};

            if ( !success )
                debug::abort();

            return success;
        }

    private:    //  Data members
        mutable debug::weak_reference::counter  _weak_rc{};
    );
};

// ******************************************************************
// ******************************************************************

}   //  namespace hidden


// ******************************************************************
template <typename T, int capacity_>
class array : public hidden::array_common<T, capacity_>
// ******************************************************************
{
    using array_common
        = hidden::array_common<T, capacity_>;

public:
    using read_only_T
        = typename array_common::read_only_T;

public:
    array( void ) noexcept = default;

    array( array const& b )
    :   array_common{b}
    {}

    array( array&& b ) noexcept
    :   array_common{ali::move(b)}
    {}

    array( array_const_ref<T> b )
    :   array_common{ali::move(b)}
    {}

    explicit array( int size )
    :   array_common(size)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

    array( int size, read_only_T value )
    :   array_common(size, value)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

        /// \brief  Initializer list constructor.
        /// Initializes this array from an initializer list.
        ///
        /// \param  b
        ///         The initializer list.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array( ali::initializer_list<T> b )
    :   array_common{make_ref(b)}
    {}

    array& operator=( array const& b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( array&& b ) noexcept
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( array_const_ref<T> b )
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( ali::initializer_list<T> b )
    {
        array_common::operator=(b);

        return *this;
    }

    template <typename funcT>
    auto map( funcT func ) const
    {
        ali::array<decltype(func(this->front()))> ret;
        ret.reserve(this->size());
        for (auto const& item : *this)
            ret.push_back(func(item));
        return ret;
    }

    void swap( array& b ) noexcept
    {
        array_common::swap(b);
    }

    friend void swap( array& a, array& b ) noexcept
    {
        a.swap(b);
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <int capacity_>
class array<ali::uint8, capacity_>
// ******************************************************************
    : public hidden::array_common<ali::uint8, capacity_>
{
    using T = ali::uint8;
    using array_common
        = hidden::array_common<T, capacity_>;

public:
    using read_only_T
        = typename array_common::read_only_T;

public:
    array( void ) noexcept = default;

    array( array const& b )
    :   array_common{b}
    {}

    array( array&& b ) noexcept
    :   array_common{ali::move(b)}
    {}

    array( array_const_ref<T> b )
    :   array_common{ali::move(b)}
    {}

    array( void const* data, int size )
    :   array_common{blob_const_ref{reinterpret_cast<
            ali::uint8 const*>(data), size}}
    {}

    array( string_const_ref b )
    :   array_common{b.as_blob()}
    {}

    explicit array( int size )
    :   array_common(size)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

    array( int size, read_only_T value )
    :   array_common(size, value)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

        /// \brief  Initializer list constructor.
        /// Initializes this array from an initializer list.
        ///
        /// \param  b
        ///         The initializer list.
        ///
        /// \post
        ///
        ///             this->size() == pre(b)->size()
        ///
        /// \throws noexcept <tt>(false)</tt>
        ///
    array( ali::initializer_list<T> b )
    :   array_common{make_ref(b)}
    {}

    array& operator=( array const& b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( array&& b ) noexcept
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( array_const_ref<T> b )
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( ali::initializer_list<T> b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( string_const_ref b )
    {
        array_common::operator=(b.as_blob());

        return *this;
    }

    //  replace

    using array_common::replace;

    array& replace( int pos1, int n1, void const* data, int size )
    {
        return this->replace(pos1, n1, blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& replace( int pos1, int n1, string_const_ref b )
    {
        return this->replace(pos1, n1, b.as_blob());
    }

    //  assign

    using array_common::assign;

    array& assign( void const* data, int size )
    {
        return this->assign(blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& assign( string_const_ref value )
    {
        return this->assign(value.as_blob());
    }

    //  append

    using array_common::append;

    array& append( void const* data, int size )
    {
        return this->append(blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& append( string_const_ref value )
    {
        return this->append(value.as_blob());
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& append_int_be( ali::uint32 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_int_be_at(begin, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& append_int_le( ali::uint32 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_int_le_at(begin, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& append_long_be( ali::uint64 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_long_be_at(begin, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& append_long_le( ali::uint64 value, int byte_count )
    {
        int const begin{this->size()};
        return this->append(ali::uint8{}, byte_count)
            .set_long_le_at(begin, value, byte_count);
    }

    array& append_int8( ali::uint8 value )
    {
        return this->append(value);
    }

    array& append_int8( ali::uint8 value, int n2 )
    {
        return this->append(value, n2);
    }

    array& append_int16_be( ali::uint16 value )
    {
        return this->append_int_be(value, 2);
    }

    array& append_int16_le( ali::uint16 value )
    {
        return this->append_int_le(value, 2);
    }

    array& append_int16_ne( ali::uint16 value )
    {
        return this->append_int_le(to_little_endian(value), 2);
    }

    array& append_int32_be( ali::uint32 value )
    {
        return this->append_int_be(value, 4);
    }

    array& append_int32_le( ali::uint32 value )
    {
        return this->append_int_le(value, 4);
    }

    array& append_int32_ne( ali::uint32 value )
    {
        return this->append_int_le(to_little_endian(value), 4);
    }

    array& append_int64_be( ali::uint64 value )
    {
        return this->append_long_be(value, 8);
    }

    array& append_int64_le( ali::uint64 value )
    {
        return this->append_long_le(value, 8);
    }

    array& append_int64_ne( ali::uint64 value )
    {
        return this->append_long_le(to_little_endian(value), 8);
    }

    //  insert

    using array_common::insert;

    array& insert( int pos1, void const* data, int size )
    {
        return this->insert(pos1, blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
    }

    array& insert( int pos1, string_const_ref b )
    {
        return this->insert(pos1, b.as_blob());
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& insert_int_be( int pos1, ali::uint32 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_int_be_at(pos1, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& insert_int_le( int pos1, ali::uint32 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_int_le_at(pos1, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& insert_long_be( int pos1, ali::uint64 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_long_be_at(pos1, value, byte_count);
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& insert_long_le_at( int pos1, ali::uint64 value, int byte_count )
    {
        return this->insert(pos1, ali::uint8{}, byte_count)
            .set_long_le_at(pos1, value, byte_count);
    }

    array& insert_int8( int pos1, ali::uint8 value, int n2 = 1 )
    {
        return this->insert(pos1, value, n2);
    }

    array& insert_int16_be( int pos1, ali::uint16 value )
    {
        return this->insert_int_be(pos1, value, 2);
    }

    array& insert_int16_le( int pos1, ali::uint16 value )
    {
        return this->insert_int_le(pos1, value, 2);
    }

    array& insert_int16_ne( int pos1, ali::uint16 value )
    {
        return this->insert_int_le(pos1, to_little_endian(value), 2);
    }

    array& insert_int32_be( int pos1, ali::uint32 value )
    {
        return this->insert_int_be(pos1, value, 4);
    }

    array& insert_int32_le( int pos1, ali::uint32 value )
    {
        return this->insert_int_le(pos1, value, 4);
    }

    array& insert_int32_ne( int pos1, ali::uint32 value )
    {
        return this->insert_int_le(pos1, to_little_endian(value), 4);
    }

    array& insert_int64_be( int pos1, ali::uint64 value )
    {
        return this->insert_long_be(pos1, value, 8);
    }

    array& insert_int64_le( int pos1, ali::uint64 value )
    {
        return this->insert_long_le(pos1, value, 8);
    }

    array& insert_int64_ne( int pos1, ali::uint64 value )
    {
        return this->insert_long_le(pos1, to_little_endian(value), 8);
    }

    //  int

    template <typename integer_type>
    integer_type int_be_at_( int pos, int byte_count ) const noexcept
        // pre: byte_count in set {1 ... sizeof(integer_type)}
    {
        return this->ref().template int_be_at_<integer_type>(pos, byte_count);
    }

    template <typename integer_type>
    integer_type int_le_at_( int pos, int byte_count ) const noexcept
        // pre: byte_count in set {1 ... sizeof(integer_type)}
    {
        return this->ref().template int_le_at_<integer_type>(pos, byte_count);
    }

    ali::uint32 int_be_at( int pos, int byte_count ) const noexcept
        // pre: byte_count in set {1, 2, 3, 4}
    {
        return this->ref().int_be_at(pos, byte_count);
    }

    ali::uint32 int_le_at( int pos, int byte_count ) const noexcept
        // pre: byte_count in set {1, 2, 3, 4}
    {
        return this->ref().int_le_at(pos, byte_count);
    }

    ali::uint8 int8_at( int pos ) const noexcept
    {
        return this->at(pos);
    }

    ali::uint16 int16_be_at( int pos ) const noexcept
    {
        return this->ref().int16_be_at(pos);
    }

    ali::uint16 int16_le_at( int pos ) const noexcept
    {
        return this->ref().int16_le_at(pos);
    }

    ali::uint16 int16_ne_at( int pos ) const noexcept
        //  Native endian.
    {
        return this->ref().int16_ne_at(pos);
    }

    ali::uint32 int32_be_at( int pos ) const noexcept
    {
        return this->ref().int32_be_at(pos);
    }

    ali::uint32 int32_le_at( int pos ) const noexcept
    {
        return this->ref().int32_le_at(pos);
    }

    ali::uint32 int32_ne_at( int pos ) const noexcept
        //  Native endian.
    {
        return this->ref().int32_ne_at(pos);
    }

    ali::uint64 int64_be_at( int pos ) const noexcept
    {
        return this->ref().int64_be_at(pos);
    }

    ali::uint64 int64_le_at( int pos ) const noexcept
    {
        return this->ref().int64_le_at(pos);
    }

    ali::uint64 int64_ne_at( int pos ) const noexcept
        //  Native endian.
    {
        return this->ref().int64_ne_at(pos);
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& set_int_be_at(
        int pos, ali::uint32 value, int byte_count ) noexcept
    {
        this->mutable_ref().set_int_be_at(pos, value, byte_count);
        return *this;
    }

    // pre: byte_count in set {1, 2, 3, 4}
    array& set_int_le_at(
        int pos, ali::uint32 value, int byte_count ) noexcept
    {
        this->mutable_ref().set_int_le_at(pos, value, byte_count);
        return *this;
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& set_long_be_at(
        int pos, ali::uint64 value, int byte_count ) noexcept
    {
        this->mutable_ref().set_long_be_at(pos, value, byte_count);
        return *this;
    }

    // pre: byte_count in set {1, 2, 3, 4, 5, 6, 7, 8}
    array& set_long_le_at(
        int pos, ali::uint64 value, int byte_count ) noexcept
    {
        this->mutable_ref().set_long_le_at(pos, value, byte_count);
        return *this;
    }

    array& set_int8_at( int pos, ali::uint8 value ) noexcept
    {
        this->at(pos) = value;
        return *this;
    }

    array& set_int16_be_at( int pos, ali::uint16 value ) noexcept
    {
        return this->set_int_be_at(pos, value, 2);
    }

    array& set_int16_le_at( int pos, ali::uint16 value ) noexcept
    {
        return this->set_int_le_at(pos, value, 2);
    }

    array& set_int16_ne_at( int pos, ali::uint16 value ) noexcept
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 2);
    }

    array& set_int32_be_at( int pos, ali::uint32 value ) noexcept
    {
        return this->set_int_be_at(pos, value, 4);
    }

    array& set_int32_le_at( int pos, ali::uint32 value ) noexcept
    {
        return this->set_int_le_at(pos, value, 4);
    }

    array& set_int32_ne_at( int pos, ali::uint32 value ) noexcept
        //  Native endian.
    {
        return this->set_int_le_at(pos, to_little_endian(value), 4);
    }

    array& set_int64_be_at( int pos, ali::uint64 value ) noexcept
    {
        return this->set_long_be_at(pos, value, 8);
    }

    array& set_int64_le_at( int pos, ali::uint64 value ) noexcept
    {
        return this->set_long_le_at(pos, value, 8);
    }

    array& set_int64_ne_at( int pos, ali::uint64 value ) noexcept
        //  Native endian.
    {
        return this->set_long_le_at(pos, to_little_endian(value), 8);
    }

    //  set_bin_at

    array& set_bin_at( int pos, void const* data, int size ) noexcept
    {
        this->mutable_ref().copy(pos, blob_const_ref{
            reinterpret_cast<ali::uint8 const*>(data), size});
        return *this;
    }

    array& set_bin_at( int pos, blob_const_ref value ) noexcept
    {
        this->mutable_ref().copy(pos, value);
        return *this;
    }

    // map

    template <typename funcT>
    auto map( funcT func ) const
    {
        ali::array<decltype(func(this->front()))> ret;
        ret.reserve(this->size());
        for (auto const& item : *this)
            ret.push_back(func(item));
        return ret;
    }

    //  swap

    void swap( array& b ) noexcept
    {
        array_common::swap(b);
    }

    friend void swap( array& a, array& b ) noexcept
    {
        a.swap(b);
    }

    string_const_ref as_string( void ) const noexcept
    {
        return this->ref().as_string();
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <int capacity_>
class array<char, capacity_>
// ******************************************************************
    : public hidden::array_common<char, capacity_>
{
    using T = char;
    using array_common
        = hidden::array_common<T, capacity_>;

public:
    using read_only_T
        = typename array_common::read_only_T;

public:
    array( void ) noexcept = default;

    array( array const& b )
    :   array_common{b}
    {}

    array( array&& b ) noexcept
    :   array_common{ali::move(b)}
    {}

    array( array_const_ref<T> b )
    :   array_common{ali::move(b)}
    {}

    explicit array( int size )
    :   array_common(size)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

    array( int size, read_only_T value )
    :   array_common(size, value)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

    array& operator=( array const& b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( array&& b ) noexcept
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( array_const_ref<T> b )
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    //  operator+=

    array& operator+=( array_const_ref<T> b )
    {
        array_common::push_back(ali::move(b));

        return *this;
    }

    array& operator+=( read_only_T c )
    {
        array_common::push_back(c);

        return *this;
    }

    //  swap

    void swap( array& b ) noexcept
    {
        array_common::swap(b);
    }

    friend void swap( array& a, array& b ) noexcept
    {
        a.swap(b);
    }
};

// ******************************************************************
// ******************************************************************

// ******************************************************************
template <int capacity_>
class array<ali::wchar, capacity_>
// ******************************************************************
    : public hidden::array_common<ali::wchar, capacity_>
{
    using T = ali::wchar;
    using array_common
        = hidden::array_common<T, capacity_>;

public:
    using read_only_T
        = typename array_common::read_only_T;

public:
    array( void ) noexcept = default;

    array( array const& b )
    :   array_common{b}
    {}

    array( array&& b ) noexcept
    :   array_common{ali::move(b)}
    {}

    array( array_const_ref<T> b )
    :   array_common{ali::move(b)}
    {}

    explicit array( int size )
    :   array_common(size)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

    array( int size, read_only_T value )
    :   array_common(size, value)
            //  Intentional use of ( ) initializer to
            //  disambiguate from the initializer_list version.
    {}

    array& operator=( array const& b )
    {
        array_common::operator=(b);

        return *this;
    }

    array& operator=( array&& b ) noexcept
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    array& operator=( array_const_ref<T> b )
    {
        array_common::operator=(ali::move(b));

        return *this;
    }

    //  operator+=

    array& operator+=( array_const_ref<T> b )
    {
        array_common::push_back(ali::move(b));

        return *this;
    }

    array& operator+=( read_only_T c )
    {
        array_common::push_back(c);

        return *this;
    }

    //  swap

    void swap( array& b ) noexcept
    {
        array_common::swap(b);
    }

    friend void swap( array& a, array& b ) noexcept
    {
        a.swap(b);
    }
};

// ******************************************************************
// ******************************************************************

array<bool> test_array( void );

}   //  namespace ali

// ******************************************************************
//  THE FOLLOWING CLASSES AND FUNCTIONS ARE TO BE GRADUALLY REMOVED.
// ******************************************************************

#include "ali/ali_auto_ptr.h"
#include "ali/ali_buffer.h"

namespace ali
{

// ******************************************************************
template <typename T, int size>
using fixed_size_array = buffer<T, size>;
// ******************************************************************

// ******************************************************************
template <typename T, int capacity>
using fixed_capacity_array = array<T, capacity>;
// ******************************************************************

// ******************************************************************
template <typename T>
using ptr_array = array<T*>;
// ******************************************************************

/** @brief Dynamic pointer array with automatic deletion
  * @tparam T Type of the pointer stored in the array */
// ******************************************************************
template <typename T>
class ALI_ATTRIBUTE_EMPTY_BASES auto_ptr_array
// ******************************************************************
    :   public exposes_array_const_ref<T*, auto_ptr_array<T>>
{
public:
    /** @brief Pointer type stored in the array */
    using value_type = T*;

    /** @brief Reference type */
    using  reference
        = ali::auto_ptr_lvalue<T>;

    /** @brief Constant reference type */
    using const_reference
        = T* const&;

    /** @brief Constant iterator type */
    using const_iterator
        = T* const*;


    /** @brief Iterator class */
    class iterator
        : public ali::iterator_base<ali::random_access_iterator_tag, T*>
    {
    public:
        /** @brief Default constructor */
        iterator()
            : _p(nullptr)
        {}

        /** @brief Constructor from a pointer
          * @param p The pointer */
        iterator(T** p)
            : _p(p)
        {}

        /** @brief Dereference operator
          * @return Value reference */
        reference operator *() const
        {
            return *_p;
        }

        /** @brief Prefix increment operator
          * @return Self */
        iterator& operator ++()
        {
            ++_p;
            return *this;
        }

        /** @brief Postfix increment operator
          * @return Original (before increment) iterator */
        iterator operator++(int)
        {
            iterator t(*this);
            ++*this;
            return t;
        }

        /** @brief Prefix decrement operator
          * @return Self */
        iterator& operator--( void )
        {
            --_p;
            return *this;
        }

        /** @brief Postfix decrement operator
          * @return Original (before decrement) iterator */
        iterator operator--( int )
        {
            iterator t(*this);
            --*this;
            return t;
        }

        /** @brief Addition-augmented assignment operator
          * @param d Value to add
          * @return Self */
        iterator& operator +=(int d)
        {
            _p += d;
            return *this;
        }

        /** @brief Addition operator
          * @param d Value to add
          * @return Iterator with the specified value added */
        iterator operator +(int d) const
        {
            return iterator(*this) += d;
        }

        /** @brief Subtraction-augmented assignment operator
          * @param d Value to subtract
          * @return Self */
        iterator& operator -=(int d)
        {
            _p -= d;
            return *this;
        }

        /** @brief Subtraction operator
          * @param d Value to subtract
          * @return Iterator with the specified value subtracted */
        iterator operator -(int d) const
        {
            return iterator(*this) -= d;
        }

        /** @brief Difference operator
          * @param b Iterator to compute difference from
          * @return The difference between this and the specified iterator */
        int operator -(iterator const& b) const
        {
            return _p - b._p;
        }

        /** @brief Comparison operator
          * @param b Iterator to compare with
          * @retval true This iterator is equal to the specified iterator
          * @retval false This iterator is not equal to the specified iterator */
        bool operator ==(iterator const& b) const
        {
            return _p == b._p;
        }

        /** @brief Comparison operator
          * @param b Iterator to compare with
          * @retval false This iterator is equal to the specified iterator
          * @retval true This iterator is not equal to the specified iterator */
        bool operator !=(iterator const& b) const
        {
            return !(*this == b);
        }

        /** @brief Operator for conversion to constant iterator
          * @return The constant iterator */
        operator const_iterator() const
        {
            return _p;
        }

    private:
        T** _p; ///< Value
    };


    /** @brief Default constructor */
    auto_ptr_array()
    {}

    /** @brief Constructor of a pre-filled array
      * @param size Size of the newly created array
      * @param val Initializer for each new element
      *
      * The array is preffiled with <code>nullptr</code>s */
    explicit auto_ptr_array(int size)
    :   _arr(size)
    {}

    /** @brief Destructor */
    ~auto_ptr_array()
    {
        erase();
    }

    /** @brief Move constructor
      * @param b Array to move  */
    auto_ptr_array( auto_ptr_array&& b )
    {
        _arr.swap(b._arr);
    }

    /** @brief Move assignment operator
      * @param b Array to move from
      * @return Self */
    auto_ptr_array& operator=( auto_ptr_array&& b )
    {
        if ( this != &b )
        {
            swap(b);
            auto_ptr_array{}.swap(b);
        }
        return *this;
    }

    /** @brief Get maximal size
      * @return Maximal size */
    static int max_size()
    {
        return ptr_array<T>::max_size();
    }

    /** @brief Get current size
      * @return Current size */
    int size() const
    {
        return _arr.size();
    }

    /** @brief Get capacity
     * @return Capacity of the array that it can grow to without reallocating */
    int capacity() const
    {
        return _arr.capacity();
    }

    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements */
    bool is_empty() const
    {
        return _arr.is_empty();
    }

ALI_DEPRECATED(
    /** @brief Check if the array is empty
      * @retval true The array is empty
      * @retval false The array contains some elements
      * @deprecated Use @ref is_empty instead */
    bool empty() const
    {
        return is_empty();
    }
)

    /** @brief Reserve capacity
      * @param cap Capacity to reserve; @c 0 ? @p cap ? @ref max_size
      * @return Self
      *
      * If @p cap ? @ref capacity, nothing happens. */
    auto_ptr_array& reserve(int cap)
    {
        _arr.reserve(cap);
        return *this;
    }

    /** @brief Resize the array
      * @param size New array size; @c 0 ? @p size ? @ref max_size
      * @param val Constructor for newly created elements
      * @return Self */
    auto_ptr_array& resize(int size)
    {
        ali_assert(0 <= size && size <= max_size());
        for (int i = size; i < _arr.size(); ++i)
        {
            delete_scalar(_arr[i]);
            _arr[i] = nullptr;
        }
        _arr.resize(size, nullptr);
        return *this;
    }

    /** @brief Erase elements from the array's back
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size
      * @return Self */
    auto_ptr_array& erase_back(int n = 1)
    {
        ali_assert(0 <= n && n <= _arr.size());

        return erase(_arr.size() - n, n);
    }

    /** @brief Erase elements from the array
      * @param i Index of the first element to erase; @c 0 ? @p i ? @ref size
      * @param n Number of elements to erase; @c 0 ? @p n ? @ref size - @p i
      * @return Self */
    auto_ptr_array& erase(int i, int n = 1)
    {
        ali_assert(0 <= i && i <= _arr.size());
        ali_assert(0 <= n && n <= _arr.size() - i);

        for (int j = i + n; j > i;)
        {
            delete_scalar(_arr[--j]);
            _arr[j] = nullptr;
        }

        _arr.erase(i, n);

        return *this;
    }

    /** @brief Erase an element from the array
      * @param p Iterator to the element to erase; @ref begin ? @p p < @ref end
      * @return Iterator to the next element; @ref end if the erased element
      *         was at the back */
    iterator erase(iterator p)
    {
        erase(p - begin());
        return p;
    }

    /** @brief Erase elements matching a predicate
      * @tparam predicate The predicate's type
      * @param p The predicate; the element is erased if @p p returns @c true
      * @return Number of erased elements */
    template <typename predicate>
    int erase_if(predicate p)
    {
        int iw = 0;

        for (int ir = 0; ir < _arr.size(); ++ir)
            if (!p(_arr[ir]))
            {
                if (iw != ir)
                {
                    ali_assert(_arr[iw] == nullptr);
                    _arr[iw] = _arr[ir];
                    _arr[ir] = nullptr;
                }
                ++iw;
            }
            else
            {
                delete_scalar(_arr[ir]);
                _arr[ir] = nullptr;
            }

        const int result = _arr.size() - iw;
        _arr.erase_back(result);
        return result;
    }

    /** @brief Erase the array
      * @return Self */
    auto_ptr_array& erase()
    {
        return erase_back(_arr.size());
    }

    /** @brief Insert an element into the array
      * @param i Index on which to insert the new element; @c 0 ? @p i ? @ref size
      * @param val The element to insert
      * @return Self */
    auto_ptr_array& insert(int i,
                           ali::auto_ptr<T> val)
    {
        ali_assert(0 <= i && i <= _arr.size());

        _arr.insert(i, val.get());
        ali_assert(i != _arr.size() && _arr[i] == val.get());

        val.release();
        return *this;
    }

    /** @brief Insert an array into the array
      * @param i Index on which to insert the array; @c 0 ? @p i ? @ref size
      * @param vals The array to insert
      * @return Self */
    auto_ptr_array& insert(int i,
                           auto_ptr_array& vals)
    {
        ali_assert(0 <= i && i <= _arr.size());
        _arr.insert(i, vals._arr);

        ali_assert(vals.is_empty()
            || (i != _arr.size() && _arr[i] == vals.front()));

        vals._arr.erase();
        return *this;
    }

    /** @brief Reset an element
      * @param i Index of the element to reset; @c 0 ? @p i < @ref size
      * @param val The value to set
      * @return Self */
    auto_ptr_array& reset(int i,
                          T* val = nullptr)
    {
        return reset(i, ali::auto_ptr<T>(val));
    }

    /** @brief Reset an element
      * @param i Index of the element to reset; @c 0 ? @p i < @ref size
      * @param val The value to set
      * @return Self */
    auto_ptr_array& reset(int i,
                          ali::auto_ptr<T> val)
    {
        ali_assert(0 <= i && i < _arr.size());
        delete_scalar(_arr[i]);
        _arr[i] = val.release();
        return *this;
    }

    /** @brief Release an element
      * @param i Index of the element to release; @c 0 ? @p i < @ref size
      * @return The released element
      *
      * The released element's position is erased */
    ali::auto_ptr<T> release(int i)
    {
        ali_assert(0 <= i && i < _arr.size());
        ali::auto_ptr<T> temp(_arr[i]);
        _arr.erase(i);
        return temp;
    }

    /** @brief Release an element without erasing its place
      * @param i Index of the element to release; @c 0 ? @p i < @ref size
      * @return The released element
      *
      * The released element's position is reset to @c nullptr */
    ali::auto_ptr<T> release_no_erase(int i)
    {
        ali_assert(0 <= i && i < _arr.size());
        ali::auto_ptr<T> temp(_arr[i]);
        _arr[i] = nullptr;
        return temp;
    }

ALI_DEPRECATED(
    /** @brief Clear the array
      * @return Self
      * @deprecated Use @ref erase instread */
    auto_ptr_array& clear()
    {
        return erase();
    }
)

    /** @brief Push an item at the back
      * @param val The item to push
      * @return Self */
    auto_ptr_array& push_back(ali::auto_ptr<T> val)
    {
        _arr.push_back(val.get());

        ali_assert(!_arr.is_empty() && _arr.back() == val.get());

        val.release();
        return *this;
    }

    /** @brief Push an array at the back
      * @param vals Array to push; the array is cleared
      * @return Self */
    auto_ptr_array& push_back(auto_ptr_array& vals)
    {
        _arr.push_back(vals._arr);

        ali_assert(vals.is_empty()
            || (!_arr.is_empty() && _arr.back() == vals.back()));

        vals._arr.erase();
        return *this;
    }

    template <typename clone_function>
    auto_ptr_array& push_back_cloning(
        array_const_ref<T const*> vals,
        clone_function clone )
    {
        if ( vals.is_empty() )
            return *this;

        this->reserve(this->size() + vals.size());

        for ( int i = 0; i < vals.size(); ++i )
            if ( vals[i] == nullptr )
                this->push_back(ali::auto_ptr<T>());
            else
                this->push_back(clone(*vals[i]));

        return *this;
    }

    template <typename clone_function>
    auto_ptr_array& push_back_cloning(
        array_const_ref<T*> vals,
        clone_function clone )
    {
        return this->push_back_cloning(
            array_const_ref<T const*>{
                vals.data(), vals.size()},
            clone);
    }

    auto_ptr_array& push_back_cloning(
        array_const_ref<T const*> vals )
    {
        return this->push_back_cloning(vals,
            []( T const& b )
            { return new_auto_ptr<T>(b); });
    }

    auto_ptr_array& push_back_cloning(
        array_const_ref<T*> vals )
    {
        return this->push_back_cloning(
            array_const_ref<T const*>{
                vals.data(), vals.size()});
    }

    /** @brief Pop an item from the back
      * @return The popped element */
    ali::auto_ptr<T> pop_back()
    {
        ali_assert(!_arr.is_empty());

        auto_ptr<T> p{_arr.back()};
        _arr.erase_back();
        return p;
    }

    /** @brief Pop an item from the front
      * @return The popped element */
    ali::auto_ptr<T> pop_front()
    {
        ali_assert(!_arr.is_empty());

        auto_ptr<T> p{_arr.front()};
        _arr.erase_front();
        return p;
    }

    /** @brief Get constant element at the front
      * @return Constant element at the front */
    const_reference front() const
    {
        return _arr.front();
    }

    /** @brief Get element at the front
      * @return Element at the front */
    reference front()
    {
        return _arr.front();
    }

    /** @brief Get constant element at the back
      * @return Constant element at back */
    const_reference back() const
    {
        return _arr.back();
    }

    /** @brief Get element at the back
      * @return Element at back */
    reference back()
    {
        return _arr.back();
    }

    /** @brief Get constant iterator at the begin
      * @return Constant iterator at the begin */
    const_iterator begin() const
    {
        return _arr.begin();
    }

    /** @brief Get iterator at the begin
      * @return Iterator at the begin */
    iterator begin()
    {
        return _arr.begin();
    }

    /** @brief Get constant iterator at the end
      * @return Constant iterator at the end */
    const_iterator end() const
    {
        return _arr.end();
    }

    /** @brief Get iterator at the end
      * @return Iterator at the end */
    iterator end()
    {
        return _arr.end();
    }

    /** @brief Constant index operator
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference operator [](int i) const
    {
        return _arr[i];
    }

    /** @brief Index operator
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference operator [](int i)
    {
        return _arr[i];
    }

    /** @brief Get a constant element at an index
      * @param i Index of the requested constant element; @c 0 ? @p i < @ref size
      * @return The constant element at the specified index */
    const_reference at(int i) const
    {
        return operator [](i);
    }

    /** @brief Get an element at an index
      * @param i Index of the requested element; @c 0 ? @p i < @ref size
      * @return The element at the specified index */
    reference at(int i)
    {
        return operator [](i);
    }

    /** @brief Swap content with another array
      * @param b Another array to swap content with
      * @return Self */
    auto_ptr_array& swap(auto_ptr_array& b)
    {
        _arr.swap(b._arr);
        return *this;
    }

    /** @brief Swap content of two arrays
      * @param a Array to swap content with @p b
      * @param b Array to swap content with @p a */
    friend void swap(auto_ptr_array& a,
                     auto_ptr_array& b)
    {
        a.swap(b);
    }

    /** @brief Swap two items
      * @param idx1 Index of the first item to swap; @c 0 ? @p idx1 < @ref size
      * @param idx2 Index of the second item to swap; @c 0 ? @p idx2 < @ref size
      * @return Self */
    auto_ptr_array<T>& swap_items(int idx1,
                                  int idx2)
    {
        _arr.swap_items(idx1, idx2);
        return *this;
    }

    /** @brief Sort the array
      * @return Self */
    auto_ptr_array& sort()
    {
        return sort(0, size());
    }

    /** @brief Sort a region of the array
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    auto_ptr_array& sort(int from,
                         int n)
    {
        _arr.mutable_ref(from, n).sort();
        return *this;
    }

    /** @brief Sort the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @return Self */
    auto_ptr_array& sort(int (*cmp)(T const*,
                                    T const*))
    {
        _arr.mutable_ref().sort(cmp);
        return *this;
    }

    /** @brief Sort the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @return Self */
    template <typename comparator>
    auto_ptr_array& sort( comparator cmp )
    {
        _arr.mutable_ref().sort(cmp);
        return *this;
    }

    /** @brief Sort a region of the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    auto_ptr_array& sort(int (*cmp)(T const*, T const*),
                         int from,
                         int n)
    {
        _arr.mutable_ref(from, n).sort(cmp);
        return *this;
    }

    /** @brief Sort a region of the array using a comparator
      * @param cmp The comparator; returns:
      *            - < @c 0 if the first element is smaller than the second
      *            - @c 0 if both elements are equal
      *            - > @c 0 if the first element is greater than the second
      * @param from Index of the first element to sort; @c 0 ? @p from ? @ref size
      * @param n Number of elements to sort; @c 0 ? @p n ? @ref size - @p from
      * @return Self */
    template <typename comparator>
    auto_ptr_array& sort( comparator cmp, int from, int n )
    {
        _arr.mutable_ref(from, n).sort(cmp);
        return *this;
    }

    array_const_ref<T*> ref( void ) const
    {
        return _arr.ref();
    }

    array_const_ref<T*> ref( int pos, int n ) const
    {
        return _arr.ref(pos, n);
    }

    void assert_invariant( void ) const
    {
        _arr.assert_invariant();
    }

private:    //  Methods
    /** @brief Copy constructor
      * @param b Array to copy
      * @note This makes a deep copy, i.e. each element of the provided array is
      *       copy-constructed */
    auto_ptr_array( auto_ptr_array const& b );

    /** @brief Assignment operator
      * @param b Array to copy from
      * @return Self */
    auto_ptr_array& operator=( auto_ptr_array const& );

private:
    ali::ptr_array<T>   _arr; ///< Storage
};

// ******************************************************************
template <typename T>
using pod_array = array<T>;
// ******************************************************************

}   //  namespace ali
