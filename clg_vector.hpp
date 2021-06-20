//
// Copyright (c) 2021 Christopher Gassib
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CLGVECTOR_HPP
#define CLGVECTOR_HPP

#ifndef CLGMATH_HPP
  #include "clg_math.hpp"
#endif

// These macros are used for creating handy constructors and accessor methods, on derived types.
// Creating the constructors and assignment operators is repetitive, this macro handles that.
#define IMPLEMENT_COMMON_METHODS(derivedName)\
        using scalar_type = typename base::scalar_type;\
        static constexpr unsigned int dimension_count = base::dimension_count;\
        using array_type = typename base::array_type;\
        using vec_type = typename base::vec_type;\
\
        constexpr derivedName() : base() { }\
        constexpr derivedName(const derivedName& original) : base(original) { }\
        explicit constexpr derivedName(const base& original) : base(original) { }\
        explicit constexpr derivedName(const scalar_type value) : base(value) { }\
        explicit constexpr derivedName(const typename base::array_type& scalars) : base(scalars) { }\
        explicit constexpr derivedName(const scalar_type* const scalars, const size_t count) : base(scalars, count) { }\
        template<typename src_scalar_type, unsigned int src_dimension_count, typename src_vec_type>\
            explicit constexpr derivedName(const base_vec<src_scalar_type, src_dimension_count, src_vec_type>& original) : base(original) { }\
        template<typename... Args>\
            explicit constexpr derivedName(const scalar_type first, const Args&... args) : base(first, args...) { }\
        template<unsigned int arg_dimension_count, typename arg_vec_type, typename... Args>\
            explicit constexpr derivedName(const base_vec<scalar_type, arg_dimension_count, arg_vec_type>& first, const Args&... args) : base(first, args...) { }\
\
        constexpr derivedName& operator =(const derivedName& rhs) { return static_cast<derivedName&>(this->base::operator =(rhs)); }\
        constexpr derivedName& operator =(const base& rhs) { return static_cast<derivedName&>(this->base::operator =(rhs)); }\
        constexpr derivedName& operator =(const scalar_type rhs) { return static_cast<derivedName&>(this->base::operator =(rhs)); }\
        constexpr derivedName& operator =(const typename base::array_type& rhs) { return static_cast<derivedName&>(this->base::operator =(rhs)); }\

namespace clg { namespace impl
{
    template<typename ScalarT, unsigned int Dimensions> class vec;

    template<
        typename        ScalarT,
        unsigned int    Dimensions,
        typename        VecT = vec<ScalarT, Dimensions>
    >
    class base_vec
    {
    public:
        using scalar_type                               = ScalarT;
        static constexpr unsigned int dimension_count   = Dimensions;
        using array_type                                = ScalarT[Dimensions];
        using vec_type                                  = VecT;
        using ref_type                                  = vec_type&;
        using ref_const_type                            = vec_type const&;

        constexpr vec_type& operator =(const base_vec& rhs)
        {
            if (this != &rhs)
            {
                clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
            }
            return static_cast<ref_type>(*this);
        }

        constexpr vec_type& operator =(const scalar_type rhs)
        {
            clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, rhs);
            return static_cast<ref_type>(*this);
        }

        constexpr vec_type& operator =(const array_type& scalars)
        {
            clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, scalars);
            return static_cast<ref_type>(*this);
        }

        constexpr const array_type& data() const
        {
            return static_cast<ref_const_type>(*this)._scalars;
        }

        constexpr const scalar_type& operator [](const int index) const
        {
            return static_cast<ref_const_type>(*this)._scalars[index];
        }

        constexpr scalar_type& operator [](const int index)
        {
            return static_cast<ref_type>(*this)._scalars[index];
        }

        constexpr bool operator ==(const base_vec& rhs) const
        {
            return clg::vec_util::equal(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
        }

        constexpr bool operator !=(const base_vec& rhs) const
        {
            return !clg::vec_util::equal(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
        }

        constexpr bool operator <(const base_vec& rhs) const
        {
            return clg::vec_util::compare(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars) < 0;
        }

        constexpr bool operator <=(const base_vec& rhs) const
        {
            return clg::vec_util::compare(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars) <= 0;
        }

        constexpr bool operator >(const base_vec& rhs) const
        {
            return clg::vec_util::compare(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars) > 0;
        }

        constexpr bool operator >=(const base_vec& rhs) const
        {
            return clg::vec_util::compare(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars) >= 0;
        }

        // gets the magnitude of the vector^2 (squared)
        constexpr scalar_type length_squared() const
        {
            return clg::vec_util::length_squared(static_cast<ref_const_type>(*this)._scalars);
        }

        // gets the magnitude (length) of the vector
        constexpr scalar_type length() const
        {
            return clg::vec_util::length(static_cast<ref_const_type>(*this)._scalars);
        }

        // gets normalized (unit vector) copy of the vector
        constexpr vec_type unit() const
        {
            vec_type result(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::normalize(result._scalars);
            return result;
        }

        // normalizes the current vector (converts to a unit vector)
        // returns the original magnitude (length)
        constexpr scalar_type normalize()
        {   
            return clg::vec_util::normalize(static_cast<ref_type>(*this)._scalars);
        }

        // gets the dot product of two vectors
        constexpr scalar_type dot(const base_vec& rhs) const
        {
            return clg::vec_util::dot_product(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
        }

        // gets the cross product of two vectors
        constexpr vec_type cross(const base_vec& rhs) const
        {
            static_assert(dimension_count == 3, "cross product only works with 3 dimensional vectors");
            vec_type product;
            clg::vec_util::cross_product(static_cast<ref_const_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars, product._scalars);
            return product;
        }

        constexpr vec_type rotate_clockwise(const scalar_type radians) const
        {
            static_assert(dimension_count == 2, "rotation only works with 2 dimensional vectors");
            vec_type result;
            clg::vec_util::rotate_clockwise(static_cast<ref_const_type>(*this)._scalars, radians, result._scalars);
            return result;
        }

        constexpr vec_type rotate_counter_clockwise(const scalar_type radians) const
        {
            static_assert(dimension_count == 2, "rotation only works with 2 dimensional vectors");
            vec_type result;
            clg::vec_util::rotate_counter_clockwise(static_cast<ref_const_type>(*this)._scalars, radians, result._scalars);
            return result;
        }

        constexpr vec_type operator +(const base_vec& rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::add(lhs._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return lhs;
        }

        constexpr vec_type operator +(const scalar_type rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::add(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec_type& operator +=(const base_vec& rhs)
        {
            clg::vec_util::add(static_cast<ref_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return static_cast<ref_type>(*this);
        }

        constexpr vec_type& operator +=(const scalar_type rhs)
        {
            clg::vec_util::add(static_cast<ref_type>(*this)._scalars, rhs);
            return static_cast<ref_type>(*this);
        }

        // negate operator
        constexpr vec_type operator -() const
        {
            vec_type result(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::negate(result._scalars);
            return result;
        }

        constexpr vec_type operator -(const base_vec& rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::subtract(static_cast<ref_type>(lhs)._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return lhs;
        }

        constexpr vec_type operator -(const scalar_type rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::subtract(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec_type& operator -=(const base_vec& rhs)
        {
            clg::vec_util::subtract(static_cast<ref_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return static_cast<ref_type>(*this);
        }

        constexpr vec_type& operator -=(const scalar_type rhs)
        {
            clg::vec_util::subtract(static_cast<ref_type>(*this)._scalars, rhs);
            return static_cast<ref_type>(*this);
        }

        // Hadamard product
        constexpr vec_type operator *(const base_vec& rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::multiply(lhs._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return lhs;
        }

        constexpr vec_type operator *(const scalar_type rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::multiply(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec_type& operator *=(const base_vec& rhs)
        {
            clg::vec_util::multiply(static_cast<ref_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return static_cast<ref_type>(*this);
        }

        constexpr vec_type& operator *=(const scalar_type rhs)
        {
            clg::vec_util::multiply(static_cast<ref_type>(*this)._scalars, rhs);
            return static_cast<ref_type>(*this);
        }

        constexpr vec_type operator /(const base_vec& rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::divide(lhs._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return lhs;
        }

        constexpr vec_type operator /(const scalar_type rhs) const
        {
            vec_type lhs(static_cast<ref_const_type>(*this)._scalars);
            clg::vec_util::divide(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec_type& operator /=(const base_vec& rhs)
        {
            clg::vec_util::divide(static_cast<ref_type>(*this)._scalars, static_cast<ref_const_type>(rhs)._scalars);
            return static_cast<ref_type>(*this);
        }

        constexpr vec_type& operator /=(const scalar_type rhs)
        {
            clg::vec_util::divide(static_cast<ref_type>(*this)._scalars, rhs);
            return static_cast<ref_type>(*this);
        }

        constexpr void swap(base_vec& rhs) noexcept
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                const scalar_type temp = static_cast<ref_type>(*this)._scalars[i];
                static_cast<ref_type>(*this)._scalars[i] = static_cast<ref_type>(rhs)._scalars[i];
                static_cast<ref_type>(rhs)._scalars[i] = temp;
            }
        }

    protected:
        constexpr base_vec()
        {
            clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, static_cast<scalar_type>(0));
        }

        constexpr base_vec(const base_vec& original)
        {
            clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, static_cast<ref_const_type>(original)._scalars);
        }

        /// <summary>
        /// Single scalar parameter used to initialize all components of the constructed vector to that scalar’s value.
        /// </summary>
        explicit constexpr base_vec(const scalar_type value)
        {
            clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, value);
        }

        explicit constexpr base_vec(const array_type& scalars)
        {
            static_assert(sizeof(array_type) == sizeof(scalar_type) * dimension_count, "failed size check");
            clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, scalars);
        }

        explicit constexpr base_vec(const scalar_type* const scalars, const size_t count)
        {
            clg::vec_util::assign(static_cast<ref_type>(*this)._scalars, scalars, count);
        }

        /// <summary>
        /// Called with a single vector of a different size, it will copy and resize it. If the original vector is
        /// bigger the result is a truncated vector. If the target vector is bigger the additional elements are
        /// intitalized to zero. Will also, static_cast<>() each of the source elements to the new vector.
        /// </summary>
        template<typename src_scalar_type, unsigned int src_dimension_count, typename src_vec_type>
        explicit constexpr base_vec(const base_vec<src_scalar_type, src_dimension_count, src_vec_type>& original)
        {
            // NOTE: If the scalar_types of the two vectors are different this is called.
            clg::vec_util::cast_dimensions(original.data(), static_cast<ref_type>(*this)._scalars);
        }

        /// <summary>
        /// Vector constructed from multiple scalars, one or more vectors, or a mixture of these. The vector's
        /// components will be constructed in order from the components of the arguments. The arguments will be consumed
        /// left to right, and each argument will have all its components consumed, in order, before any components from
        /// the next argument are consumed.
        /// </summary>
        template<typename... Args>
        explicit constexpr base_vec(const scalar_type first, const Args&... args)
        {
            mixture_constructor_unpack<0>(first, args...);
        }

        /// <summary>
        /// Called with a single vector of a different size, it will copy and resize it. If the original vector is
        /// bigger the result is a truncated vector. If the target vector is bigger the additional elements are
        /// intitalized to zero. When called with more than one vector...
        /// 
        /// Vector constructed from multiple scalars, one or more vectors, or a mixture of these. The vector's
        /// components will be constructed in order from the components of the arguments. The arguments will be consumed
        /// left to right, and each argument will have all its components consumed, in order, before any components from
        /// the next argument are consumed.
        /// </summary>
        template<unsigned int arg_dimension_count, typename arg_vec_type, typename... Args>
        explicit constexpr base_vec(const base_vec<scalar_type, arg_dimension_count, arg_vec_type>& first, const Args&... args)
        {
            constexpr auto additional_args{ sizeof...(args) };
            if constexpr (0 == additional_args)
            {
                // NOTE: If the scalar_types of the two vectors are the same this is called to cast dimensions.
                clg::vec_util::cast_dimensions(first.data(), static_cast<ref_type>(*this)._scalars);
            }
            else
            {
                mixture_constructor_unpack<0>(first, args...);
            }
        }

    private:
        template<unsigned int next_index>
        constexpr void mixture_constructor_unpack(const scalar_type last_arg)
        {
            constexpr auto provided_element_count = next_index + 1u;
            static_assert(provided_element_count <= dimension_count, "attempting to construct a vector from too many elements");
            static_assert(provided_element_count >= dimension_count, "attempting to construct a vector from too few elements");
            static_cast<ref_type>(*this)._scalars[next_index] = last_arg;
        }

        template<unsigned int next_index, unsigned int arg_dimension_count, typename arg_vec_type>
        constexpr void mixture_constructor_unpack(const base_vec<scalar_type, arg_dimension_count, arg_vec_type>& last_arg)
        {
            constexpr auto provided_element_count = next_index + arg_dimension_count;
            static_assert(provided_element_count <= dimension_count, "attempting to construct a vector from too many elements");
            static_assert(provided_element_count >= dimension_count, "attempting to construct a vector from too few elements");
            clg::vec_util::assign(reinterpret_cast<scalar_type(&)[arg_dimension_count]>(static_cast<ref_type>(*this)._scalars[next_index]), last_arg.data());
        }

        template<unsigned int next_index, typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        constexpr void mixture_constructor_unpack(const T last_arg)
        {
            constexpr auto provided_element_count = next_index + 1u;
            static_assert(provided_element_count <= dimension_count, "attempting to construct a vector from too many elements");
            static_assert(provided_element_count >= dimension_count, "attempting to construct a vector from too few elements");
            static_cast<ref_type>(*this)._scalars[next_index] = static_cast<scalar_type>(last_arg);
        }

        template<unsigned int next_index, typename... Args>
        constexpr void mixture_constructor_unpack(const scalar_type next_arg, const Args&... args)
        {
            static_cast<ref_type>(*this)._scalars[next_index] = next_arg;
            mixture_constructor_unpack<next_index + 1u>(args...);
        }

        template<unsigned int next_index, unsigned int arg_dimension_count, typename arg_vec_type, typename... Args>
        constexpr void mixture_constructor_unpack(const base_vec<scalar_type, arg_dimension_count, arg_vec_type>& next_arg, const Args&... args)
        {
            clg::vec_util::assign(reinterpret_cast<scalar_type(&)[arg_dimension_count]>(static_cast<ref_type>(*this)._scalars[next_index]), next_arg.data());
            mixture_constructor_unpack<next_index + arg_dimension_count>(args...);
        }

        template<unsigned int next_index, typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true, typename... Args>
        constexpr void mixture_constructor_unpack(const T next_arg, const Args&... args)
        {
            static_cast<ref_type>(*this)._scalars[next_index] = static_cast<scalar_type>(next_arg);
            mixture_constructor_unpack<next_index + 1u>(args...);
        }
    };

    ////////////////////////////////////////////////////////////////////////////
    /// Some derived types.
    ////////////////////////////////////////////////////////////////////////////

#pragma warning( push )
#pragma warning( disable : 4201 26495 ) // nameless structs and unions, uninitialized struct members

    template<typename ScalarT, unsigned int Dimensions>
    class vec
        : public base_vec<ScalarT, Dimensions>
    {
    public:
        using base = base_vec<ScalarT, Dimensions>;
        IMPLEMENT_COMMON_METHODS(vec);
        array_type _scalars;
    };

    template<typename ScalarT>
    class vec<ScalarT, 1>
        : public base_vec<ScalarT, 1>
    {
    public:
        using base = base_vec<ScalarT, 1>;
        IMPLEMENT_COMMON_METHODS(vec);
        union
        {
            struct { scalar_type x; };
            array_type _scalars;
        };
    };

    template<typename ScalarT>
    class vec<ScalarT, 2>
        : public base_vec<ScalarT, 2>
    {
    public:
        using base = base_vec<ScalarT, 2>;
        IMPLEMENT_COMMON_METHODS(vec);
        union
        {
            struct { scalar_type x, y; };
            array_type _scalars;
        };
    };

    template<typename ScalarT>
    class vec<ScalarT, 3>
        : public base_vec<ScalarT, 3>
    {
    public:
        using base = base_vec<ScalarT, 3>;
        IMPLEMENT_COMMON_METHODS(vec);
        union
        {
            struct { scalar_type x, y, z; };
            array_type _scalars;
        };
    };

    template<typename ScalarT>
    class vec<ScalarT, 4>
        : public base_vec<ScalarT, 4>
    {
    public:
        using base = base_vec<ScalarT, 4>;
        IMPLEMENT_COMMON_METHODS(vec);
        union
        {
            struct { scalar_type x, y, z, w; };
            array_type _scalars;
        };
    };

    template<typename ScalarT>
    class rgb
        : public base_vec<ScalarT, 3, rgb<ScalarT>>
    {
    public:
        using base = base_vec<ScalarT, 3, rgb<ScalarT>>;
        IMPLEMENT_COMMON_METHODS(rgb);

        // 32-bit (888) color accessors for rgb<byte>
        /////////////////////////////////////////////

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type rgba32(const uint_fast32_t value)
        {
            return rgb(value >> 24 & 0xff, value >> 16 & 0xff, value >> 8 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type argb32(const uint_fast32_t value)
        {
            return rgb(value >> 16 & 0xff, value >> 8 & 0xff, value >> 0 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type abgr32(const uint_fast32_t value)
        {
            return rgb(value >> 0 & 0xff, value >> 8 & 0xff, value >> 16 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type bgra32(const uint_fast32_t value)
        {
            return rgb(value >> 8 & 0xff, value >> 16 & 0xff, value >> 24 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t rgba32() const
        {
            return uint_fast32_t(r) << 24 | uint_fast32_t(g) << 16 | uint_fast32_t(b) << 8 | 0xff;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t argb32() const
        {
            return 0xff000000 | uint_fast32_t(r) << 16 | uint_fast32_t(g) << 8 | uint_fast32_t(b) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t abgr32() const
        {
            return 0xff000000 | uint_fast32_t(b) << 16 | uint_fast32_t(g) << 8 | uint_fast32_t(r) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t bgra32() const
        {
            return uint_fast32_t(b) << 24 | uint_fast32_t(g) << 16 | uint_fast32_t(r) << 8 | 0xff;
        }

        // 32-bit (888) color accessors for rgb<float>
        //////////////////////////////////////////////

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type rgba32(const uint_fast32_t value)
        {
            return rgb((value >> 24 & 0xff) / 255.0f, (value >> 16 & 0xff) / 255.0f, (value >> 8 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type argb32(const uint_fast32_t value)
        {
            return rgb((value >> 16 & 0xff) / 255.0f, (value >> 8 & 0xff) / 255.0f, (value >> 0 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type abgr32(const uint_fast32_t value)
        {
            return rgb((value >> 0 & 0xff) / 255.0f, (value >> 8 & 0xff) / 255.0f, (value >> 16 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type bgra32(const uint_fast32_t value)
        {
            return rgb((value >> 8 & 0xff) / 255.0f, (value >> 16 & 0xff) / 255.0f, (value >> 24 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t rgba32() const
        {
            return static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 24 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 8 |
                0xff;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t argb32() const
        {
            return 0xff000000 |
                static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 8 |
                static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t abgr32() const
        {
            return 0xff000000 |
                static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 8 |
                static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t bgra32() const
        {
            return static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 24 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 8 |
                0xff;
        }

        union
        {
            struct { scalar_type r, g, b; };
            array_type _scalars;
        };
    };

    template<typename ScalarT>
    class rgba
        : public base_vec<ScalarT, 4, rgba<ScalarT>>
    {
    public:
        using base = base_vec<ScalarT, 4, rgba<ScalarT>>;
        IMPLEMENT_COMMON_METHODS(rgba);

        // 32-bit (8888) color accessors for rgba<byte>
        ///////////////////////////////////////////////

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type rgba32(const uint_fast32_t value)
        {
            return rgba(value >> 24 & 0xff, value >> 16 & 0xff, value >> 8 & 0xff, value >> 0 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type argb32(const uint_fast32_t value)
        {
            return rgba(value >> 16 & 0xff, value >> 8 & 0xff, value >> 0 & 0xff, value >> 24 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type abgr32(const uint_fast32_t value)
        {
            return rgba(value >> 0 & 0xff, value >> 8 & 0xff, value >> 16 & 0xff, value >> 24 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        static constexpr vec_type bgra32(const uint_fast32_t value)
        {
            return rgba(value >> 8 & 0xff, value >> 16 & 0xff, value >> 24 & 0xff, value >> 0 & 0xff);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t rgba32() const
        {
            return uint_fast32_t(r) << 24 | uint_fast32_t(g) << 16 | uint_fast32_t(b) << 8 | uint_fast32_t(a) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t argb32() const
        {
            return uint_fast32_t(a) << 24 | uint_fast32_t(r) << 16 | uint_fast32_t(g) << 8 | uint_fast32_t(b) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t abgr32() const
        {
            return uint_fast32_t(a) << 24 | uint_fast32_t(b) << 16 | uint_fast32_t(g) << 8 | uint_fast32_t(r) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        constexpr uint_fast32_t bgra32() const
        {
            return uint_fast32_t(b) << 24 | uint_fast32_t(g) << 16 | uint_fast32_t(r) << 8 | uint_fast32_t(a) << 0;
        }

        // 32-bit (8888) color accessors for rgba<float>
        ////////////////////////////////////////////////

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type rgba32(const uint_fast32_t value)
        {
            return rgba((value >> 24 & 0xff) / 255.0f, (value >> 16 & 0xff) / 255.0f, (value >> 8 & 0xff) / 255.0f, (value >> 0 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type argb32(const uint_fast32_t value)
        {
            return rgba((value >> 16 & 0xff) / 255.0f, (value >> 8 & 0xff) / 255.0f, (value >> 0 & 0xff) / 255.0f, (value >> 24 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type abgr32(const uint_fast32_t value)
        {
            return rgba((value >> 0 & 0xff) / 255.0f, (value >> 8 & 0xff) / 255.0f, (value >> 16 & 0xff) / 255.0f, (value >> 24 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        static constexpr vec_type bgra32(const uint_fast32_t value)
        {
            return rgba((value >> 8 & 0xff) / 255.0f, (value >> 16 & 0xff) / 255.0f, (value >> 24 & 0xff) / 255.0f, (value >> 0 & 0xff) / 255.0f);
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t rgba32() const
        {
            return static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 24 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 8 |
                static_cast<uint_fast32_t>(clg::clamp(a) * 255.0f) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t argb32() const
        {
            return static_cast<uint_fast32_t>(clg::clamp(a) * 255.0f) << 24 |
                static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 8 |
                static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t abgr32() const
        {
            return static_cast<uint_fast32_t>(clg::clamp(a) * 255.0f) << 24 |
                static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 8 |
                static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 0;
        }

        template<typename T = scalar_type, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        constexpr uint_fast32_t bgra32() const
        {
            return static_cast<uint_fast32_t>(clg::clamp(b) * 255.0f) << 24 |
                static_cast<uint_fast32_t>(clg::clamp(g) * 255.0f) << 16 |
                static_cast<uint_fast32_t>(clg::clamp(r) * 255.0f) << 8 |
                static_cast<uint_fast32_t>(clg::clamp(a) * 255.0f) << 0;
        }

        union
        {
            struct { scalar_type r, g, b, a; };
            array_type _scalars;
        };
    };

    template<typename ScalarT>
    class point
        : public base_vec<ScalarT, 2, point<ScalarT>>
    {
    public:
        using base = base_vec<ScalarT, 2, point<ScalarT>>;
        IMPLEMENT_COMMON_METHODS(point);
        union
        {
            struct { scalar_type x, y; };
            array_type _scalars;
        };
    };

    template<typename ScalarT>
    class size
        : public base_vec<ScalarT, 2, size<ScalarT>>
    {
    public:
        using base = base_vec<ScalarT, 2, size<ScalarT>>;
        IMPLEMENT_COMMON_METHODS(size);
        union
        {
            struct { scalar_type width, height; };
            array_type _scalars;
        };
    };

#pragma warning( pop )
} // namespace impl

    ////////////////////////////////////////////////
    // Typedefs for the most common vector types. //
    ////////////////////////////////////////////////

    using vec2      = impl::vec<float, 2>;
    using vec2i     = impl::vec<int_fast32_t, 2>;
    using vec2ui    = impl::vec<uint_fast32_t, 2>;

    using vec3      = impl::vec<float, 3>;
    using vec3i     = impl::vec<int_fast32_t, 3>;
    using vec3ui    = impl::vec<uint_fast32_t, 3>;

    using vec4      = impl::vec<float, 4>;
    using vec4i     = impl::vec<int_fast32_t, 4>;
    using vec4ui    = impl::vec<uint_fast32_t, 4>;

    using rgb       = impl::rgb<float>;
    using rgba      = impl::rgba<float>;
    using rgbui     = impl::rgb<uint_fast8_t>;
    using rgbaui    = impl::rgba<uint_fast8_t>;

    using point     = impl::point<float>;
    using pointi    = impl::point<int_fast32_t>;
    using pointui   = impl::point<uint_fast32_t>;

    using sizev     = impl::size<float>;
    using sizevi    = impl::size<int_fast32_t>;
    using sizevui   = impl::size<uint_fast32_t>;


    // Absolute value function over each vector element.
    template<typename vec_type, typename vec_t = impl::base_vec<typename vec_type::scalar_type, vec_type::dimension_count, vec_type>>
    inline constexpr vec_type abs(const vec_type& value)
    {
        typename vec_type::array_type result;
        clg::vec_util::abs(value.data(), result);
        return vec_type(result);
    }

    // Gets the surface normal of three counter-clockwise points in space.
    template<typename vec_type, typename vec_t = impl::base_vec<typename vec_type::scalar_type, vec_type::dimension_count, vec_type>>
    inline constexpr vec_type get_surface_normal(const vec_type& v0, const vec_type& v1, const vec_type& v2)
    {
        static_assert(3 == vec_type::dimension_count, "can only calculate surface normal from 3 dimensional vectors");
        return (v0 - v1).cross(v1 - v2).unit();
    }
} // namespace clg

namespace std
{
    // std:: swap() support
    template<typename ScalarT, unsigned int Dimensions, typename VecT>
    inline constexpr void swap(clg::impl::base_vec<ScalarT, Dimensions, VecT>& lhs, clg::impl::base_vec<ScalarT, Dimensions, VecT>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

#ifdef _OSTREAM_
    // std:: stream support
    template<typename ScalarT, unsigned int Dimensions, typename VecT>
    inline std::ostream& operator <<(std::ostream& stream, const clg::impl::base_vec<ScalarT, Dimensions, VecT>& rhs)
    {
        stream << "( " << rhs[0];
        for (auto i = 1u; i < Dimensions; i++)
        {
            stream << ", " << rhs[i];
        }
        stream << " )";
        return stream;
    }

    template<typename ScalarT, unsigned int Dimensions, typename VecT>
    inline std::wostream& operator <<(std::wostream& stream, const clg::impl::base_vec<ScalarT, Dimensions, VecT>& rhs)
    {
        stream << L"( " << rhs[0];
        for (auto i = 1u; i < Dimensions; i++)
        {
            stream << L", " << rhs[i];
        }
        stream << L" )";
        return stream;
    }
#endif _OSTREAM_
} // namespace std

#endif
