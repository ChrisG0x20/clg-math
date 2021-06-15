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
#define IMPLEMENT_COMMON_METHODS(derivedName, dimensions)\
        using scalar_type = ScalarT;\
        using base_vec = vec<scalar_type, dimensions>;\
\
        constexpr derivedName() : base_vec() { }\
        constexpr derivedName(const derivedName& original) : base_vec(original) { }\
        explicit constexpr derivedName(const base_vec& original) : base_vec(original) { }\
        explicit constexpr derivedName(const scalar_type value) : base_vec(value) { }\
        explicit constexpr derivedName(const typename base_vec::array_type& scalars) : base_vec(scalars) { }\
        explicit constexpr derivedName(const scalar_type* const scalars, const unsigned int count) : base_vec(scalars, count) { }\
\
        constexpr derivedName& operator =(const derivedName& rhs) { return static_cast<derivedName&>(this->base_vec::operator =(rhs)); }\
        constexpr derivedName& operator =(const base_vec& rhs) { return static_cast<derivedName&>(this->base_vec::operator =(rhs)); }\
        constexpr derivedName& operator =(const scalar_type rhs) { return static_cast<derivedName&>(this->base_vec::operator =(rhs)); }\
        constexpr derivedName& operator =(const typename base_vec::array_type& rhs) { return static_cast<derivedName&>(this->base_vec::operator =(rhs)); }\
\
        constexpr derivedName unit() const { return static_cast<derivedName>(this->base_vec::unit()); }\
        constexpr derivedName cross(const base_vec& rhs) const { return static_cast<derivedName>(this->base_vec::cross(rhs)); }\
        constexpr derivedName rotate_clockwise(const scalar_type radians) const { return static_cast<derivedName>(this->base_vec::rotate_clockwise(radians)); }\
        constexpr derivedName rotate_counter_clockwise(const scalar_type radians) const { return static_cast<derivedName>(this->base_vec::rotate_counter_clockwise(radians)); }\
        constexpr derivedName operator +(const base_vec& rhs) const { return static_cast<derivedName>(this->base_vec::operator +(rhs)); }\
        constexpr derivedName operator +(const scalar_type rhs) const { return static_cast<derivedName>(this->base_vec::operator +(rhs)); }\
        constexpr derivedName& operator +=(const base_vec& rhs) { return static_cast<derivedName&>(this->base_vec::operator +=(rhs)); }\
        constexpr derivedName& operator +=(const scalar_type rhs) { return static_cast<derivedName&>(this->base_vec::operator +=(rhs)); }\
        constexpr derivedName operator -() const { return static_cast<derivedName>(this->base_vec::operator -()); }\
        constexpr derivedName operator -(const base_vec& rhs) const { return static_cast<derivedName>(this->base_vec::operator -(rhs)); }\
        constexpr derivedName operator -(const scalar_type rhs) const { return static_cast<derivedName>(this->base_vec::operator -(rhs)); }\
        constexpr derivedName& operator -=(const base_vec& rhs) { return static_cast<derivedName&>(this->base_vec::operator -=(rhs)); }\
        constexpr derivedName& operator -=(const scalar_type rhs) { return static_cast<derivedName&>(this->base_vec::operator -=(rhs)); }\
        constexpr derivedName operator *(const base_vec& rhs) const { return static_cast<derivedName>(this->base_vec::operator *(rhs)); }\
        constexpr derivedName operator *(const scalar_type rhs) const { return static_cast<derivedName>(this->base_vec::operator *(rhs)); }\
        constexpr derivedName& operator *=(const base_vec& rhs) { return static_cast<derivedName&>(this->base_vec::operator *=(rhs)); }\
        constexpr derivedName& operator *=(const scalar_type rhs) { return static_cast<derivedName&>(this->base_vec::operator *=(rhs)); }\
        constexpr derivedName operator /(const base_vec& rhs) const { return static_cast<derivedName>(this->base_vec::operator /(rhs)); }\
        constexpr derivedName operator /(const scalar_type rhs) const { return static_cast<derivedName>(this->base_vec::operator /(rhs)); }\
        constexpr derivedName& operator /=(const base_vec& rhs) { return static_cast<derivedName&>(this->base_vec::operator /=(rhs)); }\
        constexpr derivedName& operator /=(const scalar_type rhs) { return static_cast<derivedName&>(this->base_vec::operator /=(rhs)); }\

// This creates custom accessors for the internal vector elements so you add things like:
//  r(), x(), width() without too much work.
#define IMPLEMENT_ACCESSORS(arrayIndex, symbolicName)\
        scalar_type symbolicName() const { return (*this)[arrayIndex]; }\
        void symbolicName(const scalar_type value) { (*this)[arrayIndex] = value; }\

namespace clg { namespace impl
{
    template<
        typename        ScalarT,
        unsigned int    Dimensions
    >
    class vec
    {
    public:
        using scalar_type                               = ScalarT;
        static constexpr unsigned int dimension_count   = Dimensions;
        using array_type                                = ScalarT[Dimensions];

        constexpr vec()
        {
            clg::vec::assign(_scalars, static_cast<scalar_type>(0));
        }

        constexpr vec(const vec& original)
        {
            clg::vec::assign(_scalars, original._scalars);
        }

        /// <summary>
        /// Single scalar parameter used to initialize all components of the constructed vector to that scalar’s value.
        /// </summary>
        explicit constexpr vec(const scalar_type value)
        {
            clg::vec::assign(_scalars, value);
        }

        explicit constexpr vec(const array_type& scalars)
        {
            clg::vec::assign(_scalars, scalars);
        }

        explicit constexpr vec(const scalar_type* const scalars, const unsigned int count)
        {
            clg::vec::assign(_scalars, scalars, count);
        }

        // TODO: If a vector is constructed from multiple scalars, one or more vectors, or one or more matrices, or a
        // mixtureof these, the vector's components will be constructed in order from the components of the arguments.
        // The arguments will be consumed left to right, and each argument will have all its components consumed, in
        // order, before any components from the next argument are consumed.

        constexpr vec& operator =(const vec& rhs)
        {
            if (this != &rhs)
            {
                clg::vec::assign(_scalars, rhs._scalars);
            }
            return *this;
        }

        constexpr vec& operator =(const scalar_type rhs)
        {
            clg::vec::assign(_scalars, rhs);
            return *this;
        }

        constexpr vec& operator =(const array_type& scalars)
        {
            clg::vec::assign(_scalars, scalars);
            return *this;
        }

        constexpr const array_type& data() const
        {
            return _scalars;
        }

        constexpr const scalar_type& operator [](const int index) const
        {
            return _scalars[index];
        }

        constexpr scalar_type& operator [](const int index)
        {
            return _scalars[index];
        }

        constexpr bool operator ==(const vec& rhs) const
        {
            return clg::vec::equal(_scalars, rhs._scalars);
        }

        constexpr bool operator !=(const vec& rhs) const
        {
            return !clg::vec::equal(_scalars, rhs._scalars);
        }

        constexpr bool operator <(const vec& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) < 0;
        }

        constexpr bool operator <=(const vec& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) <= 0;
        }

        constexpr bool operator >(const vec& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) > 0;
        }

        constexpr bool operator >=(const vec& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) >= 0;
        }

        // gets the magnitude of the vector^2 (squared)
        constexpr scalar_type length_squared() const
        {
            return clg::vec::length_squared(_scalars);
        }

        // gets the magnitude (length) of the vector
        constexpr scalar_type length() const
        {
            return clg::vec::length(_scalars);
        }

        // gets normalized (unit vector) copy of the vector
        constexpr vec unit() const
        {
            vec result(_scalars);
            clg::vec::normalize(result._scalars);
            return result;
        }

        // normalizes the current vector (converts to a unit vector)
        // returns the original magnitude (length)
        constexpr scalar_type normalize()
        {   
            return clg::vec::normalize(_scalars);
        }

        // gets the dot product of two vectors
        constexpr scalar_type dot(const vec& rhs) const
        {
            return clg::vec::dot_product(_scalars, rhs._scalars);
        }

        // gets the cross product of two vectors
        constexpr vec cross(const vec& rhs) const
        {
            static_assert(dimension_count == 3, "cross product only works with 3 dimensional vectors");
            vec product;
            clg::vec::cross_product(_scalars, rhs._scalars, product._scalars);
            return product;
        }

        constexpr vec rotate_clockwise(const scalar_type radians) const
        {
            static_assert(dimension_count == 2, "rotation only works with 2 dimensional vectors");
            vec result;
            clg::vec::rotate_clockwise(_scalars, radians, result._scalars);
            return result;
        }

        constexpr vec rotate_counter_clockwise(const scalar_type radians) const
        {
            static_assert(dimension_count == 2, "rotation only works with 2 dimensional vectors");
            vec result;
            clg::vec::rotate_counter_clockwise(_scalars, radians, result._scalars);
            return result;
        }

        constexpr vec operator +(const vec& rhs) const
        {
            vec lhs(_scalars);
            clg::vec::add(lhs._scalars, rhs._scalars);
            return lhs;
        }

        constexpr vec operator +(const scalar_type rhs) const
        {
            vec lhs(_scalars);
            clg::vec::add(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec& operator +=(const vec& rhs)
        {
            clg::vec::add(_scalars, rhs._scalars);
            return *this;
        }

        constexpr vec& operator +=(const scalar_type rhs)
        {
            clg::vec::add(_scalars, rhs);
            return *this;
        }

        // negate operator
        constexpr vec operator -() const
        {
            vec result(_scalars);
            clg::vec::negate(result._scalars);
            return result;
        }

        constexpr vec operator -(const vec& rhs) const
        {
            vec lhs(_scalars);
            clg::vec::subtract(lhs._scalars, rhs._scalars);
            return lhs;
        }

        constexpr vec operator -(const scalar_type rhs) const
        {
            vec lhs(_scalars);
            clg::vec::subtract(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec& operator -=(const vec& rhs)
        {
            clg::vec::subtract(_scalars, rhs._scalars);
            return *this;
        }

        constexpr vec& operator -=(const scalar_type rhs)
        {
            clg::vec::subtract(_scalars, rhs);
            return *this;
        }

        // Hadamard product
        constexpr vec operator *(const vec& rhs) const
        {
            vec lhs(_scalars);
            clg::vec::multiply(lhs._scalars, rhs._scalars);
            return lhs;
        }

        constexpr vec operator *(const scalar_type rhs) const
        {
            vec lhs(_scalars);
            clg::vec::multiply(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec& operator *=(const vec& rhs)
        {
            clg::vec::multiply(_scalars, rhs._scalars);
            return *this;
        }

        constexpr vec& operator *=(const scalar_type rhs)
        {
            clg::vec::multiply(_scalars, rhs);
            return *this;
        }

        constexpr vec operator /(const vec& rhs) const
        {
            vec lhs(_scalars);
            clg::vec::divide(lhs._scalars, rhs._scalars);
            return lhs;
        }

        constexpr vec operator /(const scalar_type rhs) const
        {
            vec lhs(_scalars);
            clg::vec::divide(lhs._scalars, rhs);
            return lhs;
        }

        constexpr vec& operator /=(const vec& rhs)
        {
            clg::vec::divide(_scalars, rhs._scalars);
            return *this;
        }

        constexpr vec& operator /=(const scalar_type rhs)
        {
            clg::vec::divide(_scalars, rhs);
            return *this;
        }

        constexpr void swap(vec& rhs) noexcept
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                const scalar_type temp = _scalars[i];
                _scalars[i] = rhs._scalars[i];
                rhs._scalars[i] = temp;
            }
        }

    private:
        scalar_type _scalars[dimension_count];
    };

    template<typename scalar_type, unsigned int dimension_count>
    inline constexpr vec<scalar_type, dimension_count> abs(const vec<scalar_type, dimension_count>& value)
    {
        typename vec<scalar_type, dimension_count>::array_type result;
        clg::vec::abs(value.data(), result);
        return vec<scalar_type, dimension_count>(result);
    }

    template<typename dst_scalar_type, unsigned int dst_dimension_count, typename src_scalar_type, unsigned int src_dimension_count>
    inline constexpr vec<dst_scalar_type, dst_dimension_count> cast_scalars(const vec<src_scalar_type, src_dimension_count>& value)
    {
        static_assert(dst_dimension_count == src_dimension_count, "bad cast -- vector dimensions do not match");
        typename vec<dst_scalar_type, dst_dimension_count>::array_type result;
        clg::vec::cast_scalars(value.data(), result);
        return vec<dst_scalar_type, dst_dimension_count>(result);
    }

    template<typename dst_scalar_type, unsigned int dst_dimension_count, typename src_scalar_type, unsigned int src_dimension_count>
    inline constexpr vec<dst_scalar_type, dst_dimension_count> cast_dimensions(const vec<src_scalar_type, src_dimension_count>& value)
    {
        static_assert(std::is_same<dst_scalar_type, src_scalar_type>::value, "bad cast -- vector scalar types do not match");
        typename vec<dst_scalar_type, dst_dimension_count>::array_type result;
        clg::vec::cast_dimensions(value.data(), result);
        return vec<dst_scalar_type, dst_dimension_count>(result);
    }

    template<typename scalar_type, unsigned int dimension_count>
    inline constexpr vec<scalar_type, dimension_count> get_surface_normal(const vec<scalar_type, dimension_count>& v0,
        const vec<scalar_type, dimension_count>& v1, const vec<scalar_type, dimension_count>& v2)
    {
        static_assert(3 == dimension_count, "can only calculate surface normal from 3 dimensional vectors");
        return (v0 - v1).cross(v1 - v2).unit();
    }

    ////////////////////////////////////////////////////////////////////////////
    /// Some derived types.
    ////////////////////////////////////////////////////////////////////////////

    template<typename ScalarT>
    class rgb
        : public vec<ScalarT, 3>
    {
    public:
        IMPLEMENT_COMMON_METHODS(rgb, 3);

        constexpr rgb(const ScalarT red, const ScalarT green, const ScalarT blue)
        {
            (*this)[0] = red;
            (*this)[1] = green;
            (*this)[2] = blue;
        }

        IMPLEMENT_ACCESSORS(0, r);
        IMPLEMENT_ACCESSORS(1, g);
        IMPLEMENT_ACCESSORS(2, b);
    };

    template<typename ScalarT>
    class rgba
        : public vec<ScalarT, 4>
    {
    public:
        IMPLEMENT_COMMON_METHODS(rgba, 4);

        constexpr rgba(const ScalarT red, const ScalarT green, const ScalarT blue, const ScalarT alpha)
        {
            (*this)[0] = red;
            (*this)[1] = green;
            (*this)[2] = blue;
            (*this)[3] = alpha;
        }

        IMPLEMENT_ACCESSORS(0, r);
        IMPLEMENT_ACCESSORS(1, g);
        IMPLEMENT_ACCESSORS(2, b);
        IMPLEMENT_ACCESSORS(3, a);
    };

    template<typename ScalarT>
    class point
        : public vec<ScalarT, 2>
    {
    public:
        IMPLEMENT_COMMON_METHODS(point, 2);

        constexpr point(const ScalarT x, const ScalarT y)
        {
            (*this)[0] = x;
            (*this)[1] = y;
        }

        IMPLEMENT_ACCESSORS(0, x);
        IMPLEMENT_ACCESSORS(1, y);
    };

    template<typename ScalarT>
    class size
        : public vec<ScalarT, 2>
    {
    public:
        IMPLEMENT_COMMON_METHODS(size, 2);

        constexpr size(const ScalarT width, const ScalarT height)
        {
            (*this)[0] = width;
            (*this)[1] = height;
        }

        IMPLEMENT_ACCESSORS(0, width);
        IMPLEMENT_ACCESSORS(1, height);
    };

    template<typename ScalarT>
    class vec2
        : public vec<ScalarT, 2>
    {
    public:
        IMPLEMENT_COMMON_METHODS(vec2, 2);

        constexpr vec2(const ScalarT x, const ScalarT y)
        {
            (*this)[0] = x;
            (*this)[1] = y;
        }

        IMPLEMENT_ACCESSORS(0, x);
        IMPLEMENT_ACCESSORS(1, y);
    };

    template<typename ScalarT>
    class vec3
        : public vec<ScalarT, 3>
    {
    public:
        IMPLEMENT_COMMON_METHODS(vec3, 3);

        constexpr vec3(const ScalarT x, const ScalarT y, const ScalarT z)
        {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
        }

        IMPLEMENT_ACCESSORS(0, x);
        IMPLEMENT_ACCESSORS(1, y);
        IMPLEMENT_ACCESSORS(2, z);
    };

    template<typename ScalarT>
    class vec4
        : public vec<ScalarT, 4>
    {
    public:
        IMPLEMENT_COMMON_METHODS(vec4, 4);

        constexpr vec4(const ScalarT x, const ScalarT y, const ScalarT z, const ScalarT w)
        {
            (*this)[0] = x;
            (*this)[1] = y;
            (*this)[2] = z;
            (*this)[3] = w;
        }

        IMPLEMENT_ACCESSORS(0, x);
        IMPLEMENT_ACCESSORS(1, y);
        IMPLEMENT_ACCESSORS(2, z);
        IMPLEMENT_ACCESSORS(3, w);
    };
} // namespace impl

    ////////////////////////////////////////////////
    // Typedefs for the most common vector types. //
    ////////////////////////////////////////////////

    using rgb       = impl::rgb<float>;
    using rgba      = impl::rgba<float>;

    using point     = impl::point<float>;
    using pointi    = impl::point<int_fast32_t>;
    using pointui   = impl::point<uint_fast32_t>;

    using sizev     = impl::size<float>;
    using sizevi    = impl::size<int_fast32_t>;
    using sizevui   = impl::size<uint_fast32_t>;

    using vec2      = impl::vec2<float>;
    using vec2i     = impl::vec2<int_fast32_t>;
    using vec2ui    = impl::vec2<uint_fast32_t>;

    using vec3      = impl::vec3<float>;
    using vec3i     = impl::vec3<int_fast32_t>;
    using vec3ui    = impl::vec3<uint_fast32_t>;

    using vec4      = impl::vec4<float>;
    using vec4i     = impl::vec4<int_fast32_t>;
    using vec4ui    = impl::vec4<uint_fast32_t>;


    // Absolute value function over each vector element.
    template<typename vec_type, typename vec_t = impl::vec<typename vec_type::scalar_type, vec_type::dimension_count>>
    inline constexpr vec_type abs(const vec_type& value)
    {
        return static_cast<vec_type>(impl::abs<typename vec_t::scalar_type, vec_t::dimension_count>(value));
    }

    // Creates a new vector of a different type via a static_cast of each
    // vector element.
    template<typename scalar_type, typename src_scalar_type, unsigned int dimension_count>
    inline constexpr impl::vec<scalar_type, dimension_count> cast_scalars(const impl::vec<src_scalar_type, dimension_count>& value)
    {
        return impl::cast_scalars<scalar_type, dimension_count>(value);
    }

    // Creates a new vector of a different dimensionality based on the size of
    // the target template.  If the original vector is bigger the result is a
    // truncated vector.  If the target vector is bigger the additional elements
    // are intitalized to zero.
    template<unsigned int dimension_count, typename scalar_type, unsigned int src_dimension_count>
    inline constexpr impl::vec<scalar_type, dimension_count> cast_dimensions(const impl::vec<scalar_type, src_dimension_count>& value)
    {
        return impl::cast_dimensions<scalar_type, dimension_count>(value);
    }

    // Gets the surface normal of three counter-clockwise points in space.
    template<typename vec_type, typename vec_t = impl::vec<typename vec_type::scalar_type, vec_type::dimension_count>>
    inline constexpr vec_type get_surface_normal(const vec_type& v0, const vec_type& v1, const vec_type& v2)
    {
        return static_cast<vec_type>(impl::get_surface_normal(v0, v1, v2));
    }
} // namespace clg

namespace std
{
    // std:: swap() support
    template<typename ScalarT, unsigned int Dimensions>
    inline constexpr void swap(clg::impl::vec<ScalarT, Dimensions>& lhs, clg::impl::vec<ScalarT, Dimensions>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

#ifdef _OSTREAM_
    // std:: stream support
    template<typename ScalarT, unsigned int Dimensions>
    inline std::ostream& operator <<(std::ostream& stream, const clg::impl::vec<ScalarT, Dimensions>& rhs)
    {
        stream << "( " << rhs[0];
        for (auto i = 1u; i < Dimensions; i++)
        {
            stream << ", " << rhs[i];
        }
        stream << " )";
        return stream;
    }

    template<typename ScalarT, unsigned int Dimensions>
    inline std::wostream& operator <<(std::wostream& stream, const clg::impl::vec<ScalarT, Dimensions>& rhs)
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
