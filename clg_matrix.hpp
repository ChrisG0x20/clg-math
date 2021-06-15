//
// Copyright (c) 2021 Christopher Gassib
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CLGMATRIX_HPP
#define CLGMATRIX_HPP

#ifndef CLGVECTOR_HPP
  #include "clg_vector.hpp"
#endif

#undef far
#undef near

/*
 *
 * Matrix implementation compatible with OpenGL and DirectX APIs.
 * NOTE: OpenGL and DirectX matrices use the same memory layout.
 * 
 * Creates a m*n matrix of floats via template expansion like mat<float, n, m> where:
 *      m is row count, and n is column count
 *
 * NOTE: The memory layout is a C array, ex. scalars[9], read top-to-bottom, left-to-right
 * [|  /|  /|] [ 0 3 6 ]
 * [| / | / |] [ 1 4 7 ]
 * [|/  |/  |] [ 2 5 8 ]
 *
 * NOTE: The memory layout is NOT this:
 * Ex. Translation matrix in a math-style visualization, a row-major format m[i][j] where i is row and j is column:
 *  [  m00  m01  m02  m03  ]  [  1  0  0  x  ]
 *  [  m10  m11  m12  m13  ]  [  0  1  0  y  ]
 *  [  m20  m21  m22  m23  ]  [  0  0  1  z  ]
 *  [  m30  m31  m32  m33  ]  [  0  0  0  1  ]
 *
 * NOTE: The memory layout IS like this:
 * Ex. Translation matrix stored in column-major format, in a flat array (visualized as column vectors):
 *  float scalars[] =
 *  {
 *    m00, m10, m20, m30, //  {  1,  0,  0,  0  }, // column vector 1
 *    m01, m11, m21, m31, //  {  0,  1,  0,  0  }, // column vector 2
 *    m02, m12, m22, m32, //  {  0,  0,  1,  0  }, // ...
 *    m03, m13, m23, m33  //  {  x,  y,  z,  1  }
 *  };
 *
 * NOTE: Because OpenGL uses _column vector_ matrices stored in _column-major_
 * format and DirectX uses _row vector_ matrices stored in _row-major_ format, this
 * class's internal memory layout is compatible with both APIs.
 *
 * WARNING: Multidimensional C++ arrays are row-major format by default. Be
 * careful mixing mulitdimensional C++ arrays with this class.
 *   Example: int array[4][4]; // Would index like this: array[column][row]
 *           // indexing the matrix class like this: matrix(column, row) // not the math style matrix(row, column); m(i,j) style
 *           // to index the math style use the m.row_col(row, column)
 *
 * Example of indexing an n-by-m matrix as stored in flat memory:
 *  for (auto column = 0u; column < n; column++)
 *  {
 *    for (auto row = 0u; row < m; row++)
 *    {
 *      cout << "[" << row << "][" << column << "] = " << _scalars[m * column + row] << "    ";
 *      // -- OR --
 *      cout << "[" << row << "][" << column << "] = " << _scalars[column][row] << "    ";
 *    }
 *    cout << '\n';
 *  }
 *
 */

namespace clg { namespace impl
{
    template<
        typename ScalarT = float,
        unsigned int Columns = 4u,
        unsigned int Rows = 4u
    >
    class mat
    {
    public:
        using scalar_type = ScalarT;
        static constexpr unsigned int column_count = Columns;
        static constexpr unsigned int row_count = Rows;
        static constexpr unsigned int element_count = column_count * row_count;
        using array_type = scalar_type[element_count];
        static_assert(row_count > 0 && column_count > 0, "Matrix template mat<> must have at least one column and one row!");

        constexpr mat()
        {
            clg::mat::assign_identity<row_count, column_count>(_scalars);
        }

        constexpr mat(const mat& original)
        {
            clg::vec::assign(_scalars, original._scalars);
        }

        explicit constexpr mat(const array_type& scalars)
        {
            clg::vec::assign(_scalars, scalars);
        }

        explicit constexpr mat(const scalar_type* const scalars, const size_t count)
        {
            clg::vec::assign(_scalars, scalars, count);
        }

        explicit constexpr mat(const scalar_type value)
        {
            clg::vec::assign(_scalars, value);
        }

        constexpr mat& operator =(const mat& rhs)
        {
            if (this != &rhs)
            {
                clg::vec::assign(_scalars, rhs._scalars);
            }
            return *this;
        }

        constexpr mat& operator =(const scalar_type rhs)
        {
            clg::vec::assign(_scalars, rhs);
            return *this;
        }

        constexpr const array_type& data() const
        {
            return _scalars;
        }

        constexpr const scalar_type& operator ()(const unsigned int column, const unsigned int row) const
        {
            assert(row < row_count&& column < column_count);
            return _scalars[row_count * column + row];
        }

        constexpr scalar_type& operator ()(const unsigned int column, const unsigned int row)
        {
            assert(row < row_count&& column < column_count);
            return _scalars[row_count * column + row];
        }

        // for math-style indexing m(i,j) m(row, column)
        constexpr const scalar_type& row_col(const unsigned int row, const unsigned int column) const
        {
            assert(row < row_count && column < column_count);
            return _scalars[row_count * column + row];
        }

        constexpr scalar_type& row_col(const unsigned int row, const unsigned int column)
        {
            assert(row < row_count && column < column_count);
            return _scalars[row_count * column + row];
        }

        // index the matrix by column vector (column vector accessor)
        constexpr const impl::vec<scalar_type, row_count>& operator [](const unsigned int column) const
        {
            assert(column < column_count);
            return *reinterpret_cast<const impl::vec<scalar_type, row_count>*>(&_scalars[row_count * column]);
        }

        // index the matrix by column vector (column vector accessor)
        constexpr impl::vec<scalar_type, row_count>& operator [](const unsigned int column)
        {
            assert(column < column_count);
            return *reinterpret_cast<impl::vec<scalar_type, row_count>*>(&_scalars[row_count * column]);
        }

        // row vector getter
        constexpr impl::vec<scalar_type, column_count> row(const unsigned int row) const
        {
            assert(row < row_count);
            scalar_type result[column_count];
            clg::mat::slice_row<row_count>(result, _scalars, row);
            return impl::vec<scalar_type, column_count>(result);
        }

        // row vector setter
        constexpr void row(const unsigned int row, const impl::vec<scalar_type, column_count>& value)
        {
            assert(row < row_count);
            auto dstIndex = row;
            for (auto j = 0u; j < column_count; j++)
            {
                _scalars[dstIndex] = value[j];
                dstIndex += row_count;
            }
        }

        constexpr bool operator ==(const mat& rhs) const
        {
            return clg::vec::equal(_scalars, rhs._scalars);
        }

        constexpr bool operator !=(const mat& rhs) const
        {
            return !clg::vec::equal(_scalars, rhs._scalars);
        }

        constexpr bool operator <(const mat& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) < 0;
        }

        constexpr bool operator <=(const mat& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) <= 0;
        }

        constexpr bool operator >(const mat& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) > 0;
        }

        constexpr bool operator >=(const mat& rhs) const
        {
            return clg::vec::compare(_scalars, rhs._scalars) >= 0;
        }

        constexpr bool is_square() const
        {
            return row_count == column_count;
        }

        constexpr bool is_diagonal() const
        {
            if (!is_square())
            {
                return false;
            }
            for (auto j = 0u; j < column_count; j++)
            {
                for (auto i = 0u; i < row_count; i++)
                {
                    if (i == j)
                    {
                        continue;
                    }
                    if (scalar_type(0) != _scalars[row_count * j + i])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        constexpr bool is_triangular() const
        {
            return is_upper_triangular() || is_lower_triangular();
        }

        constexpr bool is_upper_triangular() const
        {
            if (!is_square())
            {
                return false;
            }
            for (auto j = 0u; j < column_count - 1; j++)
            {
                for (auto i = j + 1u; i < row_count; i++)
                {
                    if (scalar_type(0) != _scalars[row_count * j + i])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        constexpr bool is_lower_triangular() const
        {
            if (!is_square())
            {
                return false;
            }
            for (auto j = 1u; j < column_count; j++)
            {
                for (auto i = 0u; i < j; i++)
                {
                    if (scalar_type(0) != _scalars[row_count * j + i])
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        constexpr bool is_row_matrix() const
        {
            return 1 == row_count;
        }

        constexpr bool is_column_matrix() const
        {
            return 1 == column_count;
        }

        constexpr mat operator +(const mat& rhs) const
        {
            mat lhs(_scalars);
            clg::vec::add(lhs._scalars, rhs._scalars);
            return lhs;
        }

        constexpr mat operator +(const scalar_type rhs) const
        {
            mat lhs(_scalars);
            clg::vec::add(lhs._scalars, rhs);
            return lhs;
        }

        constexpr mat& operator +=(const mat& rhs)
        {
            clg::vec::add(_scalars, rhs._scalars);
            return *this;
        }

        constexpr mat& operator +=(const scalar_type rhs)
        {
            clg::vec::add(_scalars, rhs);
            return *this;
        }

        constexpr mat operator -() const
        {
            mat result(_scalars);
            clg::vec::negate(result._scalars);
            return result;
        }

        constexpr mat operator -(const mat& rhs) const
        {
            mat lhs(_scalars);
            clg::vec::subtract(lhs._scalars, rhs._scalars);
            return lhs;
        }

        constexpr mat operator -(const scalar_type rhs) const
        {
            mat lhs(_scalars);
            clg::vec::subtract(lhs._scalars, rhs);
            return lhs;
        }

        constexpr mat& operator -=(const mat& rhs)
        {
            clg::vec::subtract(_scalars, rhs._scalars);
            return *this;
        }

        constexpr mat& operator -=(const scalar_type rhs)
        {
            clg::vec::subtract(_scalars, rhs);
            return *this;
        }

        template<unsigned int rhsColumns>
        constexpr mat<scalar_type, rhsColumns, row_count> operator *(const mat<scalar_type, rhsColumns, column_count>& rhs) const
        {
            scalar_type result[row_count * rhsColumns];
            clg::mat::multiply<row_count, column_count, rhsColumns>(result, _scalars, rhs.data());
            return mat<scalar_type, rhsColumns, row_count>(result);
        }

        constexpr mat operator *(const scalar_type rhs) const
        {
            mat lhs(_scalars);
            clg::vec::multiply(lhs._scalars, rhs);
            return lhs;
        }

        constexpr mat& operator *=(const mat& rhs)
        {
            array_type result;
            clg::mat::multiply<row_count, column_count, column_count>(result, _scalars, rhs._scalars);
            clg::vec::assign(_scalars, result);
            return *this;
        }

        constexpr mat& operator *=(const scalar_type rhs)
        {
            clg::vec::multiply(_scalars, rhs);
            return *this;
        }

        constexpr mat operator /(const scalar_type rhs) const
        {
            mat lhs(_scalars);
            clg::vec::divide(lhs._scalars, rhs);
            return lhs;
        }

        constexpr mat& operator /=(const scalar_type rhs)
        {
            clg::vec::divide(_scalars, rhs);
            return *this;
        }

        constexpr mat<scalar_type, row_count, column_count> transpose() const
        {
            scalar_type result[column_count * row_count];
            for (auto j = 0u; j < column_count; j++)
            {
                for (auto i = 0u; i < row_count; i++)
                {
                    result[column_count * i + j] = _scalars[row_count * j + i];
                }
            }
            return mat<scalar_type, row_count, column_count>(result);
        }

        constexpr void swap(mat& rhs) noexcept
        {
            for (auto i = 0u; i < element_count; i++)
            {
                const scalar_type temp = _scalars[i];
                _scalars[i] = rhs._scalars[i];
                rhs._scalars[i] = temp;
            }
        }

        constexpr const scalar_type* const begin() const
        {
            return &_scalars[0];
        }

        constexpr const scalar_type* const end() const
        {
            return &_scalars[row_count * column_count];
        }

        constexpr scalar_type* const begin()
        {
            return &_scalars[0];
        }

        constexpr scalar_type* const end()
        {
            return &_scalars[row_count * column_count];
        }

        static constexpr mat zero()
        {
            return mat(0);
        }

        static constexpr mat identity()
        {
            return mat();
        }

    private:
        scalar_type _scalars[column_count * row_count];
    };

    // Derived Types
    ////////////////

    //template<typename ScalarT, unsigned int Columns, unsigned int Rows>
    //class col_major_mat
    //    : public mat<ScalarT, Columns, Rows>
    //{
    //public:
    //    // column-major index accessors
    //};
} // namespace impl

    // Most common matrix types
    ///////////////////////////

    using mat2 = impl::mat<float, 2, 2>;
    using mat3 = impl::mat<float, 3, 3>;
    using mat4 = impl::mat<float, 4, 4>;

    // NOTE: These names are backwards from the math perspective; so they match shader languages.
    //       They should be read like as Matrix(n-by-m) (where n is column count, and m is row count)
    using mat2x3 = impl::mat<float, 2, 3>;
    using mat2x4 = impl::mat<float, 2, 4>;
    using mat3x2 = impl::mat<float, 3, 2>;
    using mat3x4 = impl::mat<float, 3, 4>;
    using mat4x2 = impl::mat<float, 4, 2>;
    using mat4x3 = impl::mat<float, 4, 3>;


    // Translation Matrix
    /////////////////////

    // 2D translation
    inline constexpr mat3 translation_matrix(const impl::vec<float, 2>& translation)
    {
        mat3 result;
        for (auto i = 0u; i < std::remove_reference_t<decltype(translation)>::dimension_count; i++)
        {
            result(1, i) = translation[i];
        }
        return result;
    }

    // 3D translation
    inline constexpr mat4 translation_matrix(const impl::vec<float, 3>& translation)
    {
        mat4 result;
        for (auto i = 0u; i < std::remove_reference_t<decltype(translation)>::dimension_count; i++)
        {
            result(3, i) = translation[i];
        }
        return result;
    }

    // Rotation Matrix
    //////////////////

    // 2D rotation (about the z-axis
    inline mat3 rotation_matrix(const float radians)
    {
        mat3 result;
        result(0, 0) = std::cos(radians); result(1, 0) = -std::sin(radians);
        result(0, 1) = std::sin(radians); result(1, 1) = std::cos(radians);
        return result;
    }

    // 3D rotation about x-axis
    inline mat4 rotation_matrix_x(const float radians)
    {
        mat4 result;
        result(1, 1) = std::cos(radians); result(2, 1) = -std::sin(radians);
        result(1, 2) = std::sin(radians); result(2, 2) =  std::cos(radians);
        return result;
    }

    // 3D rotation about y-axis
    inline mat4 rotation_matrix_y(const float radians)
    {
        mat4 result;
        result(0, 0) =  std::cos(radians); result(2, 0) = std::sin(radians);
        result(0, 2) = -std::sin(radians); result(2, 2) = std::cos(radians);
        return result;
    }

    // 3D rotation about z-axis
    inline mat4 rotation_matrix_z(const float radians)
    {
        mat4 result;
        result(0, 0) = std::cos(radians); result(1, 0) = -std::sin(radians);
        result(0, 1) = std::sin(radians); result(1, 1) =  std::cos(radians);
        return result;
    }

    // Scaling Matrix
    /////////////////

    // 2D scaling
    inline constexpr mat3 scaling_matrix(const impl::vec<float, 2>& scale)
    {
        mat3 result;
        for (auto i = 0u; i < std::remove_reference_t<decltype(scale)>::dimension_count; i++)
        {
            result(i, i) = scale[i];
        }
        return result;
    }

    // 3D scaling
    inline constexpr mat4 scaling_matrix(const impl::vec<float, 3>& scale)
    {
        mat4 result;
        for (auto i = 0u; i < std::remove_reference_t<decltype(scale)>::dimension_count; i++)
        {
            result(i, i) = scale[i];
        }
        return result;
    }

    // Reflection Matrix
    ////////////////////

    // 2D reflection about x-axis
    inline constexpr mat3 reflection_about_x_matrix()
    {
        mat3 result;
        result(1, 1) = -1.0f;
        return result;
    }

    // 2D reflection about y-axis
    inline constexpr mat3 reflection_about_y_matrix()
    {
        mat3 result;
        result(0, 0) = -1.0f;
        return result;
    }

    // 2D reflection about the line y=x
    inline constexpr mat3 reflection_about_xy_line_matrix()
    {
        mat3 result(0.0f);
        result(0, 1) = 1.0f;
        result(1, 0) = 1.0f;
        return result;
    }

    // 2D reflection about the line y=-x
    inline constexpr mat3 reflection_about_negative_xy_line_matrix()
    {
        mat3 result(0.0f);
        result(0, 1) = -1.0f;
        result(1, 0) = -1.0f;
        return result;
    }

    // 2D reflection about origin
    inline constexpr mat3 reflection_about_origin_matrix()
    {
        mat3 result;
        result(0, 0) = -1.0f;
        result(1, 1) = -1.0f;
        return result;
    }

    // Shear Matrix
    ///////////////

    // 2D x-shear preserves the Y coordinate and changes the X coordinates
    inline constexpr mat3 shearing_matrix_x(const float shear)
    {
        mat3 result;
        result(1, 0) = shear;
        return result;
    }

    // 2D y-shear preserves the X coordinate and changes the Y coordinates
    inline constexpr mat3 shearing_matrix_y(const float shear)
    {
        mat3 result;
        result(0, 1) = shear;
        return result;
    }

    // 3D xy-shear
    inline constexpr mat4 shearing_matrix_xy(const float shear)
    {
        mat4 result;
        result(1, 0) = shear;
        return result;
    }

    // 3D xz-shear
    inline constexpr mat4 shearing_matrix_xz(const float shear)
    {
        mat4 result;
        result(2, 0) = shear;
        return result;
    }

    // 3D yx-shear
    inline constexpr mat4 shearing_matrix_yx(const float shear)
    {
        mat4 result;
        result(0, 1) = shear;
        return result;
    }

    // 3D yz-shear
    inline constexpr mat4 shearing_matrix_yz(const float shear)
    {
        mat4 result;
        result(2, 1) = shear;
        return result;
    }

    // 3D zx-shear
    inline constexpr mat4 shearing_matrix_zx(const float shear)
    {
        mat4 result;
        result(0, 2) = shear;
        return result;
    }

    // 3D zy-shear
    inline constexpr mat4 shearing_matrix_zy(const float shear)
    {
        mat4 result;
        result(1, 2) = shear;
        return result;
    }

    // Euler Transform Matrix
    /////////////////////////
    inline mat4 euler_transfor_mmatrix(const float heading, const float pitch, const float roll)
    {
        // E(h, p, r) heading, pitch, roll = Rz(r)Rx(p)Ry(h)
        return rotation_matrix_z(roll) *=
            rotation_matrix_x(pitch) *=
            rotation_matrix_y(heading);
    }

    // Rotation about an Arbitrary Axis Matrix
    //////////////////////////////////////////
    inline mat4 arbitrary_axis_rotation_matrix(const impl::vec<float, 3>& axisOfRotation, const float radians)
    {
        const auto& r = axisOfRotation;
        const auto c = std::cos(radians);
        const auto s = std::sin(radians);
        const auto ic = 1.0f - c;
        mat4 result;
        result(0,0) = c + ic * r[0] * r[0];
        result(1,0) = ic * r[0] * r[1] - r[2] * s;
        result(2,0) = ic * r[0] * r[2] + r[1] * s;
        result(0,1) = ic * r[0] * r[1] + r[2] * s;
        result(1,1) = c + ic * r[1] * r[1];
        result(2,1) = ic * r[1] * r[2] - r[0] * s;
        result(0,2) = ic * r[0] * r[2] - r[1] * s;
        result(1,2) = ic * r[1] * r[2] + r[0] * s;
        result(2,2) = c + ic * r[2] * r[2];
        return result;
    }

    // Orthographic Projection Matrix
    /////////////////////////////////
    inline constexpr mat4 orthographic_projectio_nmatrix_gl(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float near,
        const float far
        )
    {
        // NOTE: AABB looks like min(l, b, n) -- max(r, t, f) -- n > f
        // OpenGL negates near and far which gives an AABB of: -1, -1, 1 and
        // 1, 1, -1 for its cononical view volume.
        mat4 result;
        result(0, 0) = 2.0f / (right - left);
        result(3, 0) = -((right + left) / (right - left));
        result(1, 1) = 2.0f / (top - bottom);
        result(3, 1) = -((top + bottom) / (top - bottom));
        result(2, 2) = 2.0f / (far - near);
        result(3, 2) = -((far + near) / (far - near));
        return result;
    }

    inline constexpr mat4 orthographic_projection_matrix_dx(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float near,
        const float far
        )
    {
        // NOTE: AABB looks like min(l, b, n) -- max(r, t, f) -- n > f
        // DirectX uses the range [0, 1] for z-depths.
        mat4 result;
        result(0, 0) = 2.0f / (right - left);
        result(3, 0) = -((right + left) / (right - left));
        result(1, 1) = 2.0f / (top - bottom);
        result(3, 1) = -((top + bottom) / (top - bottom));
        result(2, 2) = 1.0f / (far - near);
        result(3, 2) = -(near / (far - near));
        return result;
    }

    // Perspective Projection Matrix
    ////////////////////////////////
    inline constexpr mat4 perspective_projection_matrix_gl(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float near,
        const float far
        )
    {
        mat4 result;
        result(0, 0) = (2.0f * near) / (right - left);
        result(2, 0) = (right + left) / (right - left);
        result(1, 1) = (2.0f * near) / (top - bottom);
        result(2, 1) = (top + bottom) / (top - bottom);
        result(2, 2) = -((far + near) / (far - near));
        result(3, 2) = -((2.0f * far * near) / (far - near));
        result(2, 3) = -1.0f;
        result(3, 3) = 0.0f;
        return result;
    }

    // Landscape perspective projection.
    inline mat4 perspective_projection_matrix_gl(
        const float verticalFieldOfView,          // in radians
        const float aspectRatioWidthOverHeight,   // width / height
        const float near,                         // always positive
        const float far                           // always positive
        )
    {
        assert(verticalFieldOfView >= 0.0f && verticalFieldOfView < trig<>::two_pi);
        assert(near > 0.0f);
        assert(far > 0.0f);

        const auto halfHeight = std::tan(verticalFieldOfView * 0.5f) * near;
        const auto halfWidth = halfHeight * aspectRatioWidthOverHeight;

        return perspective_projection_matrix_gl(
            -halfWidth,
            halfWidth,
            -halfHeight,
            halfHeight,
            near,
            far
            );
    }

    // TODO: figure out what I was doing with this function. Looks like the near and far clip plans
    // TODO: were normalized. I'm not sure how that would be useful looking at it right this second.
        //GLfloat nearPlane = 0.0f;
        //auto projection = NormalizedPerspectiveProjectionMatrixGl(
        //    60.0f / 360.0f * clg::twoPif,   // vertical FOV
        //    640.0f / 480.0f,                // aspect ratio (width / height)
        //    2.0f,                           // frustum depth
        //    nearPlane                       // distance to resulting near plane
        //    );

    // Landscape perspective projection.
    inline mat4 normalized_perspective_projection_matrix_gl(
        const float verticalFieldOfView,          // in radians
        const float aspectRatioWidthOverHeight,   // width / height
        const float depth,                        // distance from near to far clip planes (always positive)
        float& near                               // [out] distance to the near plane
        )
    {
        assert(verticalFieldOfView > 0.0f && verticalFieldOfView < trig<>::pi);
        assert(depth > 0.0f);

        near = 0.5f / std::tan(verticalFieldOfView * 0.5f);
        const auto halfWidth = 0.5f * aspectRatioWidthOverHeight;

        return perspective_projection_matrix_gl(
            -halfWidth,
            halfWidth,
            -0.5f,
            0.5f,
            near,
            near + depth
            );
    }

    inline constexpr mat4 perspective_projection_matrix_dx(
        const float left,
        const float right,
        const float bottom,
        const float top,
        const float near,
        const float far
        )
    {
        mat4 result;
        result(0, 0) = (2.0f * near) / (right - left);
        result(2, 0) = (right + left) / (right - left);
        result(1, 1) = (2.0f * near) / (top - bottom);
        result(2, 1) = (top + bottom) / (top - bottom);
        result(2, 2) = far / (far - near);
        result(3, 2) = -((far * near) / (far - near));
        result(2, 3) = 1.0f;
        result(3, 3) = 0.0f;
        return result;
    }

    // Creates a new matrix by static-casting all elements of the original.
    template<typename dst_scalar_type, unsigned int column_count, unsigned int row_count, typename src_scalar_type>
    inline constexpr impl::mat<dst_scalar_type, column_count, row_count> cast_scalars(const impl::mat<src_scalar_type, column_count, row_count>& original)
    {
        dst_scalar_type result[row_count * column_count];
        vec::cast_scalars(original.data(), result);
        return impl::mat<dst_scalar_type, column_count, row_count>(result);
    }

    // Changes the dimensions of a matrix.
    template<unsigned int column_count, unsigned int row_count = column_count, typename scalar_type, unsigned int src_column_count, unsigned int src_row_count>
    inline constexpr impl::mat<scalar_type, column_count, row_count> cast_dimensions(const impl::mat<scalar_type, src_column_count, src_row_count>& original)
    {
        using dst_mat_type = impl::mat<scalar_type, column_count, row_count>;
        dst_mat_type result;
        for (auto j = 0u; j < std::min(src_column_count, column_count); j++)
        {
            for (auto i = 0u; i < std::min(src_row_count, row_count); i++)
            {
                result(j, i) = original(j, i);
            }
        }
        return result;
    }

    // Casts a vector to a single column matrix.
    template<typename scalar_type, unsigned int row_count>
    inline constexpr const impl::mat<scalar_type, 1u, row_count>& cast_column_matrix(const impl::vec<scalar_type, row_count>& column_vector)
    {
        return *reinterpret_cast<const impl::mat<scalar_type, 1u, row_count>*>(column_vector.data());
    }

    // Casts a vector to a single row matrix.
    template<typename scalar_type, unsigned int column_count>
    inline constexpr const impl::mat<scalar_type, column_count, 1u>& cast_row_matrix(const impl::vec<scalar_type, column_count>& row_vector)
    {
        return *reinterpret_cast<const impl::mat<scalar_type, column_count, 1u>*>(row_vector.data());
    }
} // namespace clg

namespace std
{
    // std:: swap() support
    template<typename ScalarT, unsigned int Columns, unsigned int Rows>
    inline constexpr void swap(clg::impl::mat<ScalarT, Columns, Rows>& lhs, clg::impl::mat<ScalarT, Columns, Rows>& rhs) noexcept
    {
        lhs.swap(rhs);
    }

#ifdef _OSTREAM_
#ifdef _IOMANIP_
    // std:: stream support
    template<typename ScalarT, unsigned int Columns, unsigned int Rows>
    inline std::ostream& operator <<(std::ostream& stream, const clg::impl::mat<ScalarT, Columns, Rows>& rhs)
    {
        const std::streamsize floatingPointDigits = 3;

        ScalarT largestElement = ScalarT(0);
        for (auto i = 0u; i < Rows * Columns; i++)
        {
            largestElement = std::max(largestElement, std::abs(rhs.data()[i]));
        }

        std::streamsize padding = floatingPointDigits + 3; // +3 for these characters "-0."
        if (largestElement >= ScalarT(1000))
        {
            padding += 3;
        }
        else if (largestElement >= ScalarT(100))
        {
            padding += 2;
        }
        else if (largestElement >= ScalarT(10))
        {
            padding += 1;
        }

        const auto orgFlags = stream.setf(std::ios::right | std::ios::fixed, std::ios::adjustfield | std::ios::floatfield);
        const auto orgPrecision = stream.precision(floatingPointDigits);
        const auto orgWidth = stream.width();

#pragma warning( push )
#pragma warning( disable : 6294 ) // for loops always skipped when Columns or Rows == 1
        stream << "[ " << std::setw(padding) << rhs(0, 0) << std::setw(0);
        for (auto j = 1u; j < Columns; j++)
        {
            stream << ", " << std::setw(padding) << rhs(j, 0) << std::setw(0);
        }
        stream << " ]";
        for (auto i = 1u; i < Rows; i++)
        {
            stream << "\n[ " << std::setw(padding) << rhs(0, i) << std::setw(0);
            for (auto j = 1u; j < Columns; j++)
            {
                stream << ", " << std::setw(padding) << rhs(j, i) << std::setw(0);
            }
            stream << " ]";
        }
#pragma warning( pop )

        stream.width(orgWidth);
        stream.precision(orgPrecision);
        stream.setf(orgFlags, std::ios::adjustfield | std::ios::floatfield);

        return stream;
    }

    template<typename ScalarT, unsigned int Columns, unsigned int Rows>
    inline std::wostream& operator <<(std::wostream& stream, const clg::impl::mat<ScalarT, Columns, Rows>& rhs)
    {
        const std::streamsize floatingPointDigits = 3;

        ScalarT largestElement = ScalarT(0);
        for (auto i = 0u; i < Rows * Columns; i++)
        {
            largestElement = std::max(largestElement, std::abs(rhs.data()[i]));
        }

        std::streamsize padding = floatingPointDigits + 3; // +3 for these characters "-0."
        if (largestElement >= ScalarT(1000))
        {
            padding += 3;
        }
        else if (largestElement >= ScalarT(100))
        {
            padding += 2;
        }
        else if (largestElement >= ScalarT(10))
        {
            padding += 1;
        }

        const auto orgFlags = stream.setf(std::ios::right | std::ios::fixed, std::ios::adjustfield | std::ios::floatfield);
        const auto orgPrecision = stream.precision(floatingPointDigits);
        const auto orgWidth = stream.width();

#pragma warning( push )
#pragma warning( disable : 6294 ) // for loops always skipped when Columns or Rows == 1
        stream << L"[ " << std::setw(padding) << rhs(0, 0) << std::setw(0);
        for (auto j = 1u; j < Columns; j++)
        {
            stream << L", " << std::setw(padding) << rhs(j, 0) << std::setw(0);
        }
        stream << L" ]";
        for (auto i = 1u; i < Rows; i++)
        {
            stream << L"\n[ " << std::setw(padding) << rhs(0, i) << std::setw(0);
            for (auto j = 1u; j < Columns; j++)
            {
                stream << L", " << std::setw(padding) << rhs(j, i) << std::setw(0);
            }
            stream << L" ]";
        }
#pragma warning( pop )        

        stream.width(orgWidth);
        stream.precision(orgPrecision);
        stream.setf(orgFlags, std::ios::adjustfield | std::ios::floatfield);

        return stream;
    }
#endif _IOMANIP_
#endif _OSTREAM_
} // namespace std

#endif
