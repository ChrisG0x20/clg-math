clg-math
========

This is a C++ header file library for graphics-related math. If you're looking for a linear algebra library to work with OpenGL/Vulkan, DirectX, or Metal you should stop reading this and go look at:

- [GLM](https://github.com/g-truc/glm)
- [DirectXMath](https://github.com/Microsoft/DirectXMath)
- [Accelerate](https://developer.apple.com/documentation/accelerate)

If your needs are really hardcore:

- [OpenCL](https://www.khronos.org/opencl)
- [Intel's oneAPI](https://software.intel.com/content/www/us/en/develop/tools/oneapi.html)
- [Nvidia's CUDA](https://developer.nvidia.com/cuda-zone)

Background
----------
Many years ago, I needed some vector math functions for a hobby game engine. GLM didn't exist yet, or I was unaware of it. This started as a vector class in a single header file with very simple functions. It keeps getting used for personal projects because I know how it works, it's easy to extend and modify, and GLM is typically more than I need. At some point, the math library grew larger than initially intended, and a little too big to copy-paste around. This is me parking it somewhere for easy access.

So... you really should go check out [GLM](https://github.com/g-truc/glm) now.

About
-----
This library is broken into three header files. Almost everything in the `clg::` namespace.

1. **clg_math.hpp**
    - some things that are probably handled by newer `std::` C++ header files now
    - several trigonometry constants and functions
    - numeric clamping and rounding
    - fmod
    - nearest power of two
    - pointer alignment
    - array counting template
    - basic vector and matrix operations that operate on plain-old-arrays
2. **clg_vector.hpp** (includes _clg_math.hpp_)
    - base vector class template `clg::impl::vec<ScalarT, Dimensions>`
        - `ScalarT` - type of the scalar components
        - `Dimensions` - number of scalar components
    - macros to help create specialized derived vector types with more specific accessors
        - `clg::impl::rgb<T>` and `clg::rgba<T>` for colors
        - `clg::impl::point<T>` for 2D points
        - `clg::impl::size<T>` for 2D sizes
        - `clg::impl::vec2<T>` for 2D vectors
        - `clg::impl::vec3<T>` and `clg::vec4<T>` for 3D and homogenous cooridinates
    - common typedefs: `rgb, rgba, point, pointi, sizev, sizevi, vec2, vec2i, vec2ui`, etc...
    - `clg::cast_scalars<T>(impl::vec<U,V>)` copies a vector using `static_cast<T>()` on each vector component
        - ex. `auto v2 = cast_scalars<float>(vec2i(1, 2)); // v2's type will be clg::impl::vec<float, 2>`
    - `clg::cast_dimensions<D>(impl::vec<U,V>)` copies a vector producing a new vector of a different size
        - ex. `auto v3 = cast_dimensions<3>(vec2(1.0f, 2.0f)); // v3's type will be clg::impl::vec<float, 3>`
    - `std::swap()` support
    - `std::ostream` overloads for quickly writing vector contents to a C++ stream
    - many common vector operations...
3. **clg_rectanble.hpp** (includes _clg_vector.hpp_)
    - rectangle class template `clg::impl::rect<ScalarT, YAxesPolicy, BoundsCheckPolicy>`
        - `ScalarT` - type of the scalar components
        - `YAxesPolicy` - template policy for how to represent the y-axis:
            - `StandardYAxis` - positive y direction points up; useful for vector graphics and bitmap files
            - `InvertedYAxis` - positive y direction points down; useful for raster graphics and text
        - `BoundsCheckPolicy` - template policy for how to handle right-side bounds checks:
            - `ClosedIntervals` - furthest coordinates are included in rectangle; useful in vector graphics
            - `RightOpenIntervals` - furthest coordinates are excluded from rectangle; useful in raster graphics
    - internally the `rect` class maintains a starting point `location` and a `size`
    - the `location` may represent the top-left or bottom-left position of the rectangle depending on the `YAxesPolicy`
    - various accessors for bounds checking
    - methods to check for `rect` overlap or containment
    - common typedefs: `rect, recti, raster_rect, raster_rectui`, and `raster_rectf`
    - `clg::cast_scalars<T>(impl::rect<>)`, works the same as the vector version
    - a couple utility functions for interoperating with GUI rects Win32 `RECT` and Cocoa `CGRect`
    - `std::swap()` support
    - `std::ostream` overloads for quickly writing contents to a C++ stream
3. **clg_matrix.hpp** (includes _clg_vector.hpp_)
    - matrix class template `clg::impl::mat<ScalarT, Columns, Rows>`
        - `ScalarT` - type of the scalar components
        - `Columns` - number of matrix columns
        - `Rows` - number of matrix columns
    - primarily designed with OpenGL in mind, so many non-math and non-DirectX matrix conventions
    - common typedefs: `mat2, mat3, mat4, mat2x3, mat2x4, mat3x2`, etc...
    - many utility functions to quickly create common matricies (for 2D and 3D):
        - `translation_matrix(t)`
        - `rotatation_matrix(rad)`
        - `rotatation_matrix_x(rad), rotatation_matrix_y(rad), rotatation_matrix_z(rad)`
        - `scaling_matrix(s)`
        - `reflection_about_x_matrix(), reflection_about_y_matrix(), reflection_about_xy_line_matrix()`, etc...
        - `shearing_matrix_x(s), shearing_matrix_y(s), shearing_matrix_xy(s), shearing_matrix_xz(s)`, etc...
        - `euler_transfor_mmatrix(heading, pitch, roll)`
        - `arbitrary_axis_rotation_matrix(axis, rad)`
        - `orthographic_projectio_nmatrix_gl(left, right, bottom, top, near, far), orthographic_projection_matrix_dx()`
        - `perspective_projection_matrix_gl(left, right, bottom, top, near, far),  perspective_projection_matrix_dx()`
        - `perspective_projection_matrix_gl(vertical_fov, aspect_ratio, near, far)`
    - `clg::cast_scalars<T>(impl::mat<>)`, works the same as the vector version
    - `clg::cast_dimensions<C,R>(impl::mat<>)`, works similarly to the vector version
    - `clg::cast_column_matrix(impl::vec<>)` casts a vector to a column matrix type
    - `clg::cast_row_matrix(impl::vec<>)` casts a vector to a row matrix type
    - `std::swap()` support
    - `std::ostream` overloads for formatting matrix to a C++ stream
    - many common matrix operations...

Design
------
The primary design principle of this library was that the memory layout of the vector, rectangle, and matrix, would be equivalent to an array of scalars as submitted to an OpenGL, or other, API. That is, you can freely `reinterpret_cast<float*>(vec3())` without worrying about copying each of the individual scalars out of the vector into an array.

Second, the math operations were simple enough for a modern compiler to easily convert to SIMD instructions. This does work. Though, I'm sure someone could do better by hand or preferably using something like [ISPC](https://github.com/ispc/ispc).

Notes
-----
Once upon a time, this compiled for win32, macOS, iOS, and Linux; using, VC, Clang, or GCC. I'm not sure of the status of that at this time.

TODOs
-----
Testing. All testing has been by hand. Would be easy to add asserts to my manual tests. Also, I haven't tested at all, or in a very long time at least, with DirectX APIs. Also, haven't tested outside of Windows and VC in a while. Fuzz testing.

May be nice to add a texture coordinate vector type, with accessors for `t, s, p, q`.

Would like additional common color functions. Something that would convert between floating-point and byte color channels. Also, maybe add some output style options to support the different color channel formats [0.0f, 1.0f], [0, 255], and [00, FF]. ex. `0.5, 0.75, 1.0`, `100, 180, 255`, or `#7FA0FF`.

Cleanup and import my interpolation and matrix stack code.

ISPC functions for operating on arrays of vectors.

Better docs.

License
=======

### Boost Software License - Version 1.0
- See accompanying file LICENSE or copy at <http://www.boost.org/LICENSE_1_0.txt>
