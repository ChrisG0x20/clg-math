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
            if constexpr (std::is_same_v<YAxesPolicy, StandardYAxis>)
            {
                return _location.y + _size.height;
            }
            else
            {
                return _location.y;
            }
        }

        constexpr void top(const scalar_type value)
        {
            if constexpr (std::is_same_v<YAxesPolicy, StandardYAxis>)
            {
                _size.height = value - _location.y;
            }
            else
            {
                _size.height = _size.height + (_location.y - value);
                _location.y = value;
            }
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
            if constexpr (std::is_same_v<YAxesPolicy, StandardYAxis>)
            {
                return _location.y;
            }
            else
            {
                return _location.y + _size.height;
            }
        }

        constexpr void bottom(const scalar_type value)
        {
            if constexpr (std::is_same_v<YAxesPolicy, StandardYAxis>)
            {
                _size.height = _size.height + (_location.y - value);
                _location.y = value;
            }
            else
            {
                _size.height = value - _location.y;
            }
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
            if constexpr (std::is_same_v<BoundsCheckPolicy, ClosedIntervals>)
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
            else
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
        }

        constexpr bool overlaps(const rect& rectangle) const
        {
            if constexpr (std::is_same_v<BoundsCheckPolicy, ClosedIntervals>)
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
            else
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
        }

        constexpr rect make_union(const rect& rectangle) const
        {
            const auto x = min(_location.x, rectangle._location.x);
            const auto y = min(_location.y, rectangle._location.y);
            const auto w = max(_location.x + _size.width,  rectangle._location.x + rectangle._size.width ) - x;
            const auto h = max(_location.y + _size.height, rectangle._location.y + rectangle._size.height) - y;
            return rect(x, y, w, h);
        }

        constexpr void swap(rect& rhs) noexcept
        {
            _location.swap(rhs._location);
            _size.swap(rhs._size);
        }

    private:
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

    template<typename dst_scalar_type, typename src_scalar_type>
    inline constexpr impl::rect<dst_scalar_type, InvertedYAxis, RightOpenIntervals> to_raster_rect(const impl::rect<src_scalar_type>& rectangle, const int_fast32_t client_height)
    {
        const impl::rect<dst_scalar_type, InvertedYAxis, RightOpenIntervals> result(rectangle.x(), client_height - rectangle.top(), rectangle.width(), rectangle.height());
        return result;
    }

    template<typename dst_scalar_type, typename src_scalar_type>
    inline constexpr impl::rect<dst_scalar_type> from_raster_rect(const impl::rect<src_scalar_type, InvertedYAxis, RightOpenIntervals>& rectangle, const int_fast32_t client_height)
    {
        const impl::rect<dst_scalar_type> result(rectangle.x(), client_height - rectangle.bottom(), rectangle.width(), rectangle.height());
        return result;
    }

#ifdef _WINDEF_
    using winRECT_scalar_t = decltype(RECT::left);
#ifdef _TYPE_TRAITS_
    template<typename ScalarT = int_fast32_t, std::enable_if_t<std::is_integral<ScalarT>::value, bool> = true>
    inline constexpr RECT to_windows_RECT(const impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals>& rectangle)
    {
        RECT result
        {
            static_cast<winRECT_scalar_t>(rectangle.left()),
            static_cast<winRECT_scalar_t>(rectangle.top()),
            static_cast<winRECT_scalar_t>(rectangle.right()),
            static_cast<winRECT_scalar_t>(rectangle.bottom())
        };
        return result;
    }
#endif
    inline constexpr impl::rect<winRECT_scalar_t, InvertedYAxis, RightOpenIntervals> from_windows_RECT(const RECT& rect)
    {
        const impl::rect<winRECT_scalar_t, InvertedYAxis, RightOpenIntervals> result(
            rect.left,
            rect.top,
            rect.right - rect.left,
            rect.bottom - rect.top
            );
        return result;
    }
#endif

#ifdef __OBJC__
    template<typename ScalarT>
    inline constexpr CGRect to_CGRect(const impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals>& rectangle)
    {
        CGRect result;
        result.origin.x     = rectangle.x;
        result.origin.y     = rectangle.y;
        result.size.width   = rectangle.width;
        result.size.height  = rectangle.height;
        return result;
    }

    template<typename ScalarT>
    inline constexpr impl::rect<ScalarT, InvertedYAxis, RightOpenIntervals> from_CGRect(const CGRect& rect)
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
#endif // _IOMANIP_
#endif // _OSTREAM_
} // namespace std

#endif
