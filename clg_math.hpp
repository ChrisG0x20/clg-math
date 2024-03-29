//
// Copyright (c) 2021 Christopher Gassib
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CLGMATH_HPP
#define CLGMATH_HPP

namespace clg
{
    template<typename T = float>
    class trig
    {
    public:
        static constexpr T pi = static_cast<T>(3.141592653589793);
        static constexpr T two_pi = static_cast<T>(2.0) * pi;
        static constexpr T half_pi = pi / static_cast<T>(2.0);
        static constexpr T one_and_half_pi = pi + half_pi;
        static constexpr T third_pi = pi / static_cast<T>(3.0);
        static constexpr T quarter_pi = pi / static_cast<T>(4.0);
    };

    template<typename T>
    inline constexpr T to_degrees(T radians)
    {
        return (radians * static_cast<T>(180.0)) / trig<T>::pi;
    }

    template<typename T>
    inline constexpr T to_radians(T degrees)
    {
        return degrees * (trig<T>::pi / static_cast<T>(180.0));
    }

    template<typename T>
    inline constexpr T clockwise_distance(const T fromRadians, const T toRadians)
    {
        if (fromRadians >= toRadians)
        {
            return fromRadians - toRadians;
        }
        return trig<T>::two_pi - (toRadians - fromRadians);
    }

    template<typename T = float>
    inline constexpr const T min(const T lhs, const T rhs)
    {
        return rhs < lhs ? rhs : lhs;
    }

    template<typename T = float>
    inline constexpr const T max(const T lhs, const T rhs)
    {
        return rhs < lhs ? lhs : rhs;
    }

    // TODO: C++?? newer version finally adds this
    template<typename T = float>
    inline constexpr const T clamp(const T value, const T min = 0.0, const T max = 1.0)
    {
        return value < min ? min : value < max ? value : max;
    }

    // Clamp radians to the range of [0, 2 * pi]
    template<typename T>
    inline constexpr T clamp_radians(const T radians)
    {
        if (radians >= trig<T>::two_pi)
        {
            return fmod(radians, trig<T>::two_pi);
        }
        if (radians < static_cast<T>(0.0))
        {
            return trig<T>::two_pi + fmod(radians, trig<T>::two_pi);
        }
        return radians;
    }

    // Gets the shortest distance between two angles in radians.
    // Returns a number in the range [0, pi]
    template<typename T>
    inline constexpr T abs_radial_distance(const T lhs, const T rhs)
    {
        const T distance = std::abs(clamp_radians(lhs) - clamp_radians(rhs));
        return (distance > trig<T>::pi) ? trig<T>::two_pi - distance : distance;
    }

    // Should be called with either <long, float> or <long long, double>.
    // TODO: This could be improved since C++11
    template<typename T, typename U>
    inline constexpr T round_to_integer(const U number)
    {
        return static_cast<T>((number >= static_cast<U>(0.0)) ?
            number + static_cast<U>(0.5) : number - static_cast<U>(0.5));
    }

    // Rounds a number up to the nearest power of 2
    template<typename T>
    inline constexpr T round_up_to_nearest_power_of_two(T number)
    {
        number--;
        for (int i = 1; i < sizeof(T) * 8; i *= 2)
        {
            number |= number >> i;
        }
        return ++number;
    }

    // Rounds a number up to the nearest power of 2
    template<>
    inline constexpr unsigned int round_up_to_nearest_power_of_two<unsigned int>(unsigned int number)
    {
        static_assert(sizeof(unsigned int) == 4u, "unsigned int is NOT 32-bit on this platform! comment out this template specialization to compile, or change to std::uint32_t");
        number--;
        number |= number >> 1;
        number |= number >> 2;
        number |= number >> 4;
        number |= number >> 8;
        number |= number >> 16;
        return ++number;
    }

    template<typename T>
    inline constexpr T byte_swap(const T value)
    {
        static_assert(sizeof(T) >= 2 && sizeof(T) <= 8, "attempting to byte_swap an unsupported integer size");
        if constexpr (2 == sizeof(T))
        {
            return value >> 8 | value << 8;
        }
        else if constexpr (4 == sizeof(T))
        {
            return value >> 24 | (value >> 8 & 0x0000ff00) | (value << 8 & 0x00ff0000) | value << 24;
        }
        else if constexpr (8 == sizeof(T))
        {
            return value >> 56 | (value >> 40 & 0x000000000000ff00) |
                (value >> 24 & 0x0000000000ff0000) | (value >> 8 & 0x00000000ff000000) |
                (value << 8 & 0x000000ff00000000) | (value << 24 & 0x0000ff0000000000) |
                (value << 40 & 0x00ff000000000000) | value << 56;
        }
    }

    // Nudges an integer count up until it is a multiple of the specified power of two in the template parameter.
    template<unsigned int PowerOfTwo>
    inline constexpr unsigned int round_up_to_alignment(const unsigned int odd_value)
    {
        const unsigned int alignment = PowerOfTwo - 1;
        return (odd_value + alignment) & ~alignment;
    }

    // Nudges the pointer forward until it is aligned at byte granularity specified in the template parameter.
    template<unsigned int BytesOfAlignment, typename pointerT>
    inline constexpr pointerT align_pointer(const pointerT pUnaligned)
    {
        const char* const pByte = reinterpret_cast<const char* const>(pUnaligned);
        const uintptr_t alignment = BytesOfAlignment - 1;
        return reinterpret_cast<pointerT>(reinterpret_cast<uintptr_t>(pByte + alignment) & ~alignment);
    }

    template<typename T, size_t S>
    inline constexpr size_t array_count(const T(&arr)[S])
    {
        return sizeof(arr) / sizeof(arr[0]);
    }

    inline constexpr float fmod(const float numerator, const float denominator)
    {
        const auto iq = round_to_integer<long>(numerator / denominator);
        const auto result = numerator - iq * denominator;
        return result;
    }

    inline constexpr double fmod(const double numerator, const double denominator)
    {
        const auto iq = round_to_integer<long long>(numerator / denominator);
        const auto result = numerator - iq * denominator;
        return result;
    }

    // TODO: this is out-of-date with C++11
    //inline float Random(const float max)
    //{
    //    return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * max;
    //}

    /// <summary>
    /// interpolation function objects
    /// </summary>
    namespace interp
    {
        struct linear
        {
            constexpr float operator ()(const float x) const
            {
                return x;
            }
        };

        struct smoothstep
        {
            constexpr float operator ()(const float x) const
            {
                return x * x * (3.0f - 2.0f * x);
            }
        };

        struct smoothstep2
        {
            constexpr float operator ()(const float x) const
            {
                const smoothstep f;
                return f(f(x));
            }
        };

        struct smoothstep3
        {
            constexpr float operator ()(const float x) const
            {
                const smoothstep f;
                return f(f(f(x)));
            }
        };

        // Slowly accelerates.
        struct quadratic
        {
            constexpr float operator ()(const float x) const
            {
                return x * x;
            }
        };

        // Slowly decelerates.
        struct inverse_quadratic
        {
            constexpr float operator ()(const float x) const
            {
                const auto invX = 1.0f - x;
                return 1.0f - invX * invX;
            }
        };

        // Slowly accelerates.
        struct cubic
        {
            constexpr float operator ()(const float x) const
            {
                return x * x * x;
            }
        };

        // Slowly decelerates.
        struct inverse_cubic
        {
            constexpr float operator ()(const float x) const
            {
                const auto invX = 1.0f - x;
                return 1.0f - invX * invX * invX;
            }
        };

        struct sin
        {
            float operator ()(const float x) const
            {
                return std::sin(x * clg::trig<>::pi / 2.0f);
            }
        };

        struct inverse_sin
        {
            float operator ()(const float x) const
            {
                const auto invX = 1.0f - x;
                return 1.0f - std::sin(invX * clg::trig<>::pi / 2.0f);
            }
        };

        // Low-pass filter.
        struct weighted_average
        {
            weighted_average(float slowdownFactor)
                : _slowdownFactor(slowdownFactor) { }
            weighted_average(const weighted_average&) = default;
            weighted_average& operator =(const weighted_average&) = default;

            constexpr float operator ()(const float x) const
            {
                return ((x * (_slowdownFactor - 1.0f)) + 1.0f) / _slowdownFactor;
            }

            float _slowdownFactor;
        };
    } // namespace interp

    /// <summary>
    /// Performs interpolation between 0 and 1 for any x between start and end, where start < end.
    /// WARNING: The results are undefined if x is outside the bounds of start and end.
    /// </summary>
    /// <typeparam name="interpolate_func">The interpolation function to use.</typeparam>
    /// <param name="interp">The interpolation function to use.</param>
    /// <param name="start">The starting edge of the function.</param>
    /// <param name="end">The ending edge of the function.</param>
    /// <param name="x">The source value for interpolation.</param>
    /// <returns>An interpolated value in the range [0.0f, 1.0f].</returns>
    template<typename interpolate_func = interp::linear>
    inline constexpr float interpolate(interpolate_func interp, const float start, const float end, const float x)
    {
        if (start == end)
        {
            return 1.0f;
        }

        const auto t = clg::clamp((x - start) / (end - start));
        return interp(t);
    }

    /// <summary>
    /// Performs interpolation between 0 and 1 for any x between start and end, where start < end.
    /// WARNING: The results are undefined if x is outside the bounds of start and end.
    /// </summary>
    /// <typeparam name="interpolate_func">The interpolation function to use.</typeparam>
    /// <param name="start">The starting edge of the function.</param>
    /// <param name="end">The ending edge of the function.</param>
    /// <param name="x">The source value for interpolation.</param>
    /// <returns>An interpolated value in the range [0.0f, 1.0f].</returns>
    template<typename interpolate_func = interp::linear>
    inline constexpr float interpolate(const float start, const float end, const float x)
    {
        return interpolate(interpolate_func(), start, end, x);
    }

    /// <summary>
    /// When one interpolation function just isn't enough...
    /// Performs interpolation between 0 and 1 for any x between start and end, where start < end.
    /// WARNING: The results are undefined if x is outside the bounds of start and end.
    /// </summary>
    /// <typeparam name="interpolate_in_func">The interpolation function to use in the first half.</typeparam>
    /// <typeparam name="interpolate_out_func">The interpolation function to use in the second half.</typeparam>
    /// <param name="interp_in">The interpolation function to use for the first half of the range.</param>
    /// <param name="interp_out">The interpolation function to use for the second half of the range.</param>
    /// <param name="start">The starting edge of the function.</param>
    /// <param name="end">The ending edge of the function.</param>
    /// <param name="x">The source value for interpolation.</param>
    /// <returns>An interpolated value in the range [0.0f, 1.0f].</returns>
    template<typename interpolate_in_func = interp::linear, typename interpolate_out_func = interp::linear>
    inline constexpr float interpolate_inout(interpolate_in_func interp_in, interpolate_out_func interp_out, const float start, const float end, const float x)
    {
        if (start == end)
        {
            return 1.0f;
        }

        const auto t = clg::clamp((x - start) / (end - start));
        if (t < 0.5f)
        {
            return interp_in(t);
        }

        return interp_out(t);
    }

    /// <summary>
    /// When one interpolation function just isn't enough...
    /// Performs interpolation between 0 and 1 for any x between start and end, where start < end.
    /// WARNING: The results are undefined if x is outside the bounds of start and end.
    /// </summary>
    /// <typeparam name="interpolate_in_func">The interpolation function to use in the first half.</typeparam>
    /// <typeparam name="interpolate_out_func">The interpolation function to use in the second half.</typeparam>
    /// <param name="start">The starting edge of the function.</param>
    /// <param name="end">The ending edge of the function.</param>
    /// <param name="x">The source value for interpolation.</param>
    /// <returns>An interpolated value in the range [0.0f, 1.0f].</returns>
    template<typename interpolate_in_func = interp::linear, typename interpolate_out_func = interp::linear>
    inline constexpr float interpolate_inout(const float start, const float end, const float x)
    {
        return interpolate_inout(interpolate_in_func(), interpolate_out_func(), start, end, x);
    }

    namespace vec_util
    {
        // assign the elements of the source vector to the elements of the destination vector
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void assign(scalar_type(&dst)[dimension_count], const scalar_type(&src)[dimension_count]) noexcept
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                dst[i] = src[i];
            }
        }

        // assign count elements of a scalar array the elements of the destination vector
        // where count < dimension_count, fill remaining elements with zeros
        // where count > dimension_count, src is truncated to fit in dst
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void assign(scalar_type(&dst)[dimension_count], const scalar_type* const src, const size_t count)
        {
            assert(nullptr != src && count <= static_cast<size_t>(dimension_count));
            auto i = 0u;
            for (; i < min(static_cast<unsigned int>(count), dimension_count); i++)
            {
                dst[i] = src[i];
            }
            for (; i < dimension_count; i++)
            {
                dst[i] = scalar_type(0);
            }
        }

        // assign a single scalar to each of the elements of the destination vector; i.e. fill()
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void assign(scalar_type(&dst)[dimension_count], const scalar_type value) noexcept
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                dst[i] = value;
            }
        }

        // compares two vectors, returning true if all elements are equal
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr bool equal(const scalar_type(&lhs)[dimension_count], const scalar_type(&rhs)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                if (lhs[i] != rhs[i])
                {
                    return false;
                }
            }

            return true;
        }

        // compares two vectors, returning
        // -1 : lhs  < rhs
        //  0 : lhs == rhs
        //  1 : lhs  > rhs
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr int compare(const scalar_type(&lhs)[dimension_count], const scalar_type(&rhs)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                if (lhs[i] < rhs[i])
                {
                    return -1;
                }

                if (lhs[i] > rhs[i])
                {
                    return 1;
                }
            }

            return 0;
        }

        // add and assign the right-hand side vector elements to the left-hand side elements
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void add(scalar_type(&lhs)[dimension_count], const scalar_type(&rhs)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] += rhs[i];
            }
        }

        // add and assign the right-hand side scalar to each of the left-hand side elements
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void add(scalar_type(&lhs)[dimension_count], const scalar_type rhs)
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] += rhs;
            }
        }

        // subtract and assign the right-hand side vector elements from the left-hand side elements
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void subtract(scalar_type(&lhs)[dimension_count], const scalar_type(&rhs)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] -= rhs[i];
            }
        }

        // subtract and assign the right-hand side scalar from each of the left-hand side elements
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void subtract(scalar_type(&lhs)[dimension_count], const scalar_type rhs)
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] -= rhs;
            }
        }

        // Hadamard product
        // multiply and assign the right-hand side vector elements to the left-hand side elements
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void multiply(scalar_type(&lhs)[dimension_count], const scalar_type(&rhs)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] *= rhs[i];
            }
        }

        // multiply and assign the right-hand side scalar to each of the left-hand side elements
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void multiply(scalar_type(&lhs)[dimension_count], const scalar_type rhs)
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] *= rhs;
            }
        }

#pragma warning( push )
#pragma warning( disable : 4723 ) // possible divide by zero

        // divide and assign the left-hand side vector elements by the right-hand side elements
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void divide(scalar_type(&lhs)[dimension_count], const scalar_type(&rhs)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] /= rhs[i];
            }
        }

        // divide and assign each of the left-hand side vector elements by the right-hand side scalar
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void divide(scalar_type(&lhs)[dimension_count], const scalar_type rhs)
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                lhs[i] /= rhs;
            }
        }

#pragma warning( pop )

        // negates and assigns each of the vector components
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void negate(scalar_type(&vec)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                vec[i] = -vec[i];
            }
        }

        // computes the magnitude of the vector^2 (squared)
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr scalar_type length_squared(const scalar_type(&vec)[dimension_count])
        {
            scalar_type length2 = 0;
            for (auto i = 0u; i < dimension_count; i++)
            {
                length2 += vec[i] * vec[i];
            }

            return length2;
        }

        // computes the magnitude (length) of the vector
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr scalar_type length(const scalar_type(&vec)[dimension_count])
        {
            return std::sqrt(length_squared(vec));
        }

        // normalizes a vector (converts to a unit vector)
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr scalar_type normalize(scalar_type(&vec)[dimension_count])
        {
            const scalar_type len = length(vec);
            if (0 != len)
            {
                const scalar_type inverseLength = static_cast<scalar_type>(1) / len;
                multiply(vec, inverseLength);
            }

            return len;
        }

        // computes a dot product of two vectors
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr scalar_type dot_product(const scalar_type(&lhs)[dimension_count], const scalar_type(&rhs)[dimension_count])
        {
            scalar_type dotProduct = 0;
            for (auto i = 0u; i < dimension_count; i++)
            {
                dotProduct += lhs[i] * rhs[i];
            }

            return dotProduct;
        }

        // computes a cross product of two vectors, and stores the result
        template<typename scalar_type>
        inline constexpr void cross_product(const scalar_type(&lhs)[3], const scalar_type(&rhs)[3], scalar_type(&product)[3])
        {
            product[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
            product[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
            product[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
        }

        // rotates a point clockwise around the origin, and stores the result
        // x' = x  cos(Theta) + y sin(Theta)
        // y' = x -sin(Theta) + y cos(Theta)
        template<typename scalar_type>
        inline constexpr void rotate_clockwise(const scalar_type(&vec)[2], const scalar_type radians, scalar_type(&result)[2])
        {
            const auto cosTheta = std::cos(radians);
            const auto sinTheta = std::sin(radians);
            result[0] = vec[0] * cosTheta + vec[1] * sinTheta;
            result[1] = vec[0] * -sinTheta + vec[1] * cosTheta;
        }

        // rotates a point counter-clockwise around the origin, and stores the result
        // x' = x cos(Theta) - y sin(Theta)
        // y' = x sin(Theta) + y cos(Theta)
        template<typename scalar_type>
        inline constexpr void rotate_counter_clockwise(const scalar_type(&vec)[2], const scalar_type radians, scalar_type(&result)[2])
        {
            const auto cosTheta = std::cos(radians);
            const auto sinTheta = std::sin(radians);
            result[0] = vec[0] * cosTheta - vec[1] * sinTheta;
            result[1] = vec[0] * sinTheta + vec[1] * cosTheta;
        }

        // computes the absolute value of a vector
        template<typename scalar_type, unsigned int dimension_count>
        inline constexpr void abs(const scalar_type(&vec)[dimension_count], scalar_type(&result)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                result[i] = std::abs(vec[i]);
            }
        }

        // copies a vector using static_cast on each vector element
        template<typename input_scalar_type, typename output_scalar_type, unsigned int dimension_count>
        inline constexpr void cast_scalars(const input_scalar_type(&input_vec)[dimension_count], output_scalar_type(&output_vec)[dimension_count])
        {
            for (auto i = 0u; i < dimension_count; i++)
            {
                output_vec[i] = static_cast<output_scalar_type>(input_vec[i]);
            }
        }

        // Copies a vector of one length to a vector of a differnt length. If the
        // original vector is bigger the result is a truncated vector. If the target
        // vector is bigger the additional elements are intitalized to zero.
        template<typename src_scalar_type, unsigned int src_count, typename dst_scalar_type, unsigned int dst_count>
        inline constexpr void cast_dimensions(const src_scalar_type(&src_vec)[src_count], dst_scalar_type(&dst_vec)[dst_count])
        {
            auto i = 0u;
            for (; i < min(src_count, dst_count); i++)
            {
                dst_vec[i] = static_cast<dst_scalar_type>(src_vec[i]);
            }

            while (i < dst_count)
            {
                dst_vec[i++] = static_cast<dst_scalar_type>(0.0);
            }
        }

    } // namespace vec

    namespace mat_util
    {
        // assign a scalar value to the diagonal of a column-major matrix; everything else is zeros
        template<unsigned int column_count, unsigned int row_count, typename scalar_type>
        inline constexpr void assign_diagonal(scalar_type(&dst)[column_count * row_count], scalar_type value) noexcept
        {
            for (auto j = 0u, k = 0u; j < column_count; j++)
            {
                auto i = 0u;
                for (; i < min(j, row_count); i++)
                {
                    dst[k++] = static_cast<scalar_type>(0.0);
                }
                if (j < row_count)
                {
                    dst[k++] = value;
                    i++;
                }
                for (; i < row_count; i++)
                {
                    dst[k++] = static_cast<scalar_type>(0.0);
                }
            }
        }

        // slices a row vector out of a matrix
        template<unsigned int src_row_count, typename src_scalar_type, unsigned int src_column_count>
        inline constexpr void slice_row(src_scalar_type(&dst)[src_column_count], const src_scalar_type(&src)[src_row_count * src_column_count], const unsigned int row)
        {
            assert(row < src_row_count);
            auto srcIndex = row;
            for (auto j = 0u; j < src_column_count; j++)
            {
                dst[j] = src[srcIndex];
                srcIndex += src_row_count;
            }
        }

        // multiples two matrices
        template<unsigned int lhs_row_count, unsigned int lhs_column_rhs_row_count, unsigned int rhs_column_count, typename scalar_type>
        inline constexpr void multiply(
            scalar_type(&result)[lhs_row_count * rhs_column_count],
            const scalar_type(&lhs)[lhs_row_count * lhs_column_rhs_row_count],
            const scalar_type(&rhs)[lhs_column_rhs_row_count * rhs_column_count]
        )
        {
            constexpr auto lhs_column_count = lhs_column_rhs_row_count;
            constexpr auto rhs_row_count = lhs_column_rhs_row_count;

            for (auto i = 0u; i < lhs_row_count; i++)
            {
                scalar_type lhs_row[lhs_column_count];
                clg::mat_util::slice_row<lhs_row_count>(lhs_row, lhs, i);
                for (auto j = 0u; j < rhs_column_count; j++)
                {
                    const auto& rhs_col = reinterpret_cast<const scalar_type(&)[rhs_row_count]>(rhs[rhs_row_count * j]);
                    result[lhs_row_count * j + i] = clg::vec_util::dot_product(lhs_row, rhs_col);
                }
            }
        }
    } // namespace mat
} // namespace clg

#endif
