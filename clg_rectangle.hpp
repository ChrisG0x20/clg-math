//
// Copyright (c) 2021 Christopher Gassib
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CLGRECTANGLE_HPP
#define CLGRECTANGLE_HPP

#ifndef CLGVECTOR_HPP
  #include "clg_vector.hpp"
#endif

namespace clg
{
    // rect polices
    ///////////////
    struct StandardYAxis {};        // Lower-left corner is the origin (common in vector graphics).
    struct InvertedYAxis {};        // Upper-left corner is the origin (common in raster graphics, and text cursor coordinates).
    struct ClosedIntervals {};      // Range comparisons include the last row and column (common in vector graphics).
    struct RightOpenIntervals {};   // Range comparisons exclude the last row and column (common in raster graphics).

namespace impl
{
    // rect
    ///////
    template<
        typename ScalarT,
        typename YAxesPolicy        = StandardYAxis,
        typename BoundsCheckPolicy  = ClosedIntervals
        >
    class rect
    {
    public:
        using scalar_type           = ScalarT;
        using y_axes_policy         = YAxesPolicy;
        using bounds_check_policy   = BoundsCheckPolicy;
        using point_type            = point<scalar_type>;
        using size_type             = size<scalar_type>;

        constexpr rect() { }

        constexpr rect(const rect& original)
            : _location(original._location)
            , _size(original._size) { }

        explicit constexpr rect(const scalar_type width, const scalar_type height)
            : _size(width, height) { }

        explicit constexpr rect(const scalar_type x, const scalar_type y, const scalar_type width, const scalar_type height)
            : _location(x, y)
            , _size(width, height) { }

        explicit constexpr rect(const point_type& location)
            : _location(location) { }

        explicit constexpr rect(const size_type& size)
            : _size(size) { }

        explicit constexpr rect(const point_type& location, const size_type& size)
            : _location(location)
            , _size(size) { }

        template<typename src_scalar_type>
        explicit constexpr rect(const impl::rect<src_scalar_type, y_axes_policy, bounds_check_policy>& original)
            : _location(original.location())
            , _size(original.size()) { }

        constexpr rect& operator =(const rect& rhs)
        {
            if (&rhs == this)
            {
                return *this;
            }

            _location   = rhs._location;
            _size       = rhs._size;

            return *this;
        }

        constexpr const point_type& location() const
        {
            return _location;
        }

        constexpr void location(const point_type& value)
        {
            _location = value;
        }

        constexpr const size_type& size() const
        {
            return _size;
        }

        constexpr void size(const size_type& value)
        {
            _size = value;
        }

        constexpr scalar_type x() const
        {
            return _location.x;
        }

        constexpr void x(const scalar_type value)
        {
            _location.x = value;
        }

        constexpr scalar_type y() const
        {
            return _location.y;
        }

        constexpr void y(const scalar_type value)
        {
            _location.y = value;
        }

        constexpr scalar_type width() const
        {
            return _size.width;
        }

        constexpr void width(const scalar_type value)
        {
            _size.width = value;
        }

        constexpr scalar_type height() const
        {
            return _size.height;
        }

        constexpr void height(const scalar_type value)
        {
            _size.height = value;
        }

        constexpr scalar_type left() const
        {
            return _location.x;
        }

        constexpr void left(const scalar_type value)
        {
            _size.width = _size.width + (_location.x - value);
            _location.x = value;
        }

        constexpr scalar_type top() const
        {
            return impl_get_top<y_axes_policy>();
        }

        constexpr void top(const scalar_type value)
        {
            impl_set_top<y_axes_policy>(value);
        }

        constexpr scalar_type right() const
        {
            return _location.x + _size.width;
        }

        constexpr void right(const scalar_type value)
        {
            _size.width = value - _location.x;
        }

        constexpr scalar_type bottom() const
        {
            return impl_get_bottom<y_axes_policy>();
        }

        constexpr void bottom(const scalar_type value)
        {
            impl_set_bottom<y_axes_policy>(value);
        }

        constexpr bool operator ==(const rect& rhs) const
        {
            return _location == rhs._location && _size == rhs._size;
        }

        constexpr bool operator !=(const rect& rhs) const
        {
            return !(*this == rhs);
        }

        constexpr bool contains(const point_type& point) const
        {
            return impl_contains<bounds_check_policy>(point);
        }

        constexpr bool overlaps(const rect& rectangle) const
        {
            return impl_overlaps<bounds_check_policy>(rectangle);
        }

        constexpr void swap(rect& rhs) noexcept
        {
            _location.swap(rhs._location);
            _size.swap(rhs._size);
        }

    private:
        template<typename YAxesPolicy>
        constexpr scalar_type impl_get_top() const;

        template<>
        constexpr scalar_type impl_get_top<StandardYAxis>() const
        {
            return _location.y + _size.height;
        }

        template<>
        constexpr scalar_type impl_get_top<InvertedYAxis>() const
        {
            return _location.y;
        }

        template<typename YAxesPolicy>
        constexpr void impl_set_top(const scalar_type value);

        template<>
        constexpr void impl_set_top<StandardYAxis>(const scalar_type value)
        {
            _size.height = value - _location.y;
        }

        template<>
        constexpr void impl_set_top<InvertedYAxis>(const scalar_type value)
        {
            _size.height = _size.height + (_location.y - value);
            _location.y = value;
        }

        template<typename YAxesPolicy>
        constexpr scalar_type impl_get_bottom() const;

        template<>
        constexpr scalar_type impl_get_bottom<StandardYAxis>() const
        {
            return _location.y;
        }

        template<>
        constexpr scalar_type impl_get_bottom<InvertedYAxis>() const
        {
            return _location.y + _size.height;
        }

        template<typename YAxesPolicy>
        constexpr void impl_set_bottom(const scalar_type value);

        template<>
        constexpr void impl_set_bottom<StandardYAxis>(const scalar_type value)
        {
            _size.height = _size.height + (_location.y - value);
            _location.y = value;
        }

        template<>
        constexpr void impl_set_bottom<InvertedYAxis>(const scalar_type value)
        {
            _size.height = value - _location.y;
        }

        template<typename BoundsCheckPolicy>
        constexpr bool impl_contains(const point_type& point) const;

        template<>
        constexpr bool impl_contains<ClosedIntervals>(const point_type& point) const
        {
            if (point.x < _location.x || point.y < _location.y)
            {
                return false;
            }

            if ( right() < point.x || top() < point.y) // NOTE: Inclusive coordinates.
            {
                return false;
            }

            return true;
        }

        template<>
        constexpr bool impl_contains<RightOpenIntervals>(const point_type& point) const
        {
            if (point.x < _location.x || point.y < _location.y)
            {
                return false;
            }

            if ( right() <= point.x || bottom() <= point.y) // NOTE: Exclusive coordinates.
            {
                return false;
            }

            return true;
        }

        template<typename BoundsCheckPolicy>
        constexpr bool impl_overlaps(const rect& rectangle) const;

        template<>
        constexpr bool impl_overlaps<ClosedIntervals>(const rect& rectangle) const
        {
             // NOTE: Inclusive coordinate checks.
            if (right() < rectangle.left() || rectangle.right() < left())
            {
                return false;
            }
            if (top() < rectangle.bottom() || rectangle.top() < bottom())
            {
                return false;
            }
            return true;
        }

        template<>
        constexpr bool impl_overlaps<RightOpenIntervals>(const rect& rectangle) const
        {
             // NOTE: Exclusive coordinate checks.
            if (right() <= rectangle.left() || rectangle.right() <= left())
            {
                return false;
            }
            if (bottom() <= rectangle.top() || rectangle.bottom() <= top())
            {
                return false;
            }
            return true;
        }

        point_type  _location;
        size_type   _size;
    };

} // namespace impl

    // Typedefs of some basic rectangle types.
    //////////////////////////////////////////
    using rect          = impl::rect<float>;
    using recti         = impl::rect<int_fast32_t>;

    using raster_rect   = impl::rect<int_fast32_t, InvertedYAxis, RightOpenIntervals>;
    using raster_rectui = impl::rect<uint_fast32_t, InvertedYAxis, RightOpenIntervals>;
    using raster_rectf  = impl::rect<float, InvertedYAxis, RightOpenIntervals>;


#ifdef _WINDEF_
    template<typename ScalarT>
    inline constexpr RECT ConvertToWindowsRECT(const impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals>& rectangle)
    {
        RECT result;
        result.left     = rectangle.left();
        result.top      = rectangle.top();
        result.right    = rectangle.right();
        result.bottom   = rectangle.bottom();
        return result;
    }

    template<typename ScalarT>
    inline constexpr impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals> ConvertToRectangle(const RECT& rect)
    {
        return impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals>(
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top
            );
    }
#endif

#ifdef __OBJC__
    template<typename ScalarT>
    inline constexpr CGRect ConvertToCGRect(const impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals>& rectangle)
    {
        CGRect result;
        result.origin.x     = rectangle.x;
        result.origin.y     = rectangle.y;
        result.size.width   = rectangle.width;
        result.size.height  = rectangle.height;
        return result;
    }

    template<typename ScalarT>
    inline constexpr impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals> ConvertToRectangle(const CGRect& rect)
    {
        return impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals>(
            rect.origin.x,
            rect.origin.y,
            rect.size.width,
            rect.size.height
            );
    }
#endif
} // namespace clg

namespace std
{
    // std:: swap() support
    template<typename ScalarT, typename YAxesPolicy, typename RightOpenIntervals>
    inline constexpr void swap(clg::impl::rect<ScalarT, YAxesPolicy, RightOpenIntervals>& lhs, clg::impl::rect<ScalarT, YAxesPolicy, RightOpenIntervals>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

#ifdef _OSTREAM_
#ifdef _IOMANIP_

    // std:: stream support
    template<typename ScalarT, typename YAxesPolicy, typename RightOpenIntervals>
    inline ostream& operator <<(ostream& stream, const clg::impl::rect<ScalarT, YAxesPolicy, RightOpenIntervals>& rhs)
    {
        stream << rhs.location() << ", " << rhs.size();
        return stream;
    }

    template<typename ScalarT, typename YAxesPolicy, typename RightOpenIntervals>
    inline wostream& operator <<(wostream& stream, const clg::impl::rect<ScalarT, YAxesPolicy, RightOpenIntervals>& rhs)
    {
        stream << rhs.location() << L", " << rhs.size();
        return stream;
    }
} // namespace std
#endif _IOMANIP_
#endif _OSTREAM_

#endif
