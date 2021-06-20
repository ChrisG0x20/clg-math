//
// Copyright (c) 2021 Christopher Gassib
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include "clg_vector.hpp"
#include "clg_rectangle.hpp"
#include "clg_matrix.hpp"

using namespace std;
using namespace clg;

void TakesSize(sizev s)
{
    cout << "size is: " << s << '\n';
}

int main()
{
    impl::vec<int, 2> t1;
    t1[0] = 3;
    t1[1] = 6;
    cout << t1.x << ' ' << t1[0] << '\n';

    cout << impl::vec<int, 2>(t1) << '\n';
    cout << impl::vec<int, 3>(8) << '\n';
    int a1[2]{ 4, 5 };
    cout << impl::vec<int, 2>(a1) << '\n';
    cout << impl::vec<int, 2>(&a1[0], array_count(a1)) << '\n';

    cout << impl::vec<int, 2>(impl::vec<int, 1>(7), 8) << '\n';
    cout << impl::vec<int, 2>(8, impl::vec<int, 1>(7)) << '\n';
    cout << impl::vec<int, 2>(vec2i(9, 9)) << '\n';
    cout << impl::vec<int, 2>(10, 11) << '\n';

    cout << impl::vec<int, 3>(vec3(234.234, 327.12, 827.23)) << '\n';
    cout << impl::vec<float, 3>(vec3(234, 327, 827)) << '\n';
    cout << impl::vec<int, 3>(vec2(234.234, 327.12)) << '\n';

    cout << impl::vec<int, 2>(vec3i(12, 13, 14)) << '\n';
    cout << impl::vec<int, 3>(vec2i(57, 19)) << '\n';

    impl::vec<int, 2> t2(9, 12);
    t1 = t2;
    cout << t1 << '\n';
    impl::vec<unsigned int, 2> t3(15, 18);
    impl::vec<int, 3> t4(9, 12, 15);
    impl::rgb<int> c1(1, 2, 3);
    //t4 = c1; // fails - types are too different
    t4 = impl::vec<int, 3>(c1); // TODO: <- this should work
    //t3 = c1; // fails - types are too different
    //c1 = t3; // fails - types are too different
    cout << t2 << '\n';
    t3 = 8;
    cout << t3 << '\n';
    t2 = a1;
    cout << t2 << '\n';

    auto z1 = { 1, 4, 5 };
    cout << typeid(z1).name() << '\n';
    impl::vec<int, 2> z2{ 6, 4 };
    impl::vec<int, 2> z4(6, 4);
    //impl::vec<int, 2> z3 { 6, 4, 8 }; // fails - too many args

    impl::vec<int, 2> v5(-9);
    const auto v6 = clg::abs(v5);
    cout << typeid(v6).name() << v6 << '\n';

    vec2i v7(2, 3);
    vec2i v8(5, 7);
    wcout << typeid(v7).name() << v7 << '\n';
    cout << typeid(v5 - v7).name() << v5 - v7 << '\n';
    cout << typeid(v7 - v5).name() << v7 - v5 << '\n';
    cout << typeid(v8 - v7).name() << v8 - v7 << '\n';

    rgb c2(3, 4, 6);
    rgb c3(1);
    cout << typeid(c2 - c3).name() << c2 - c3 << '\n';

    //newvec_test();

    vec2 p; //vec()
    vec2 v6o;
    v6o = p = 1.0f; //vec& operator =(const scalar_type rhs)
    v6o = vec2(p); //vec(const vec & original)
    const float a[] = { 1.0f, 2.0f };
    v6o = vec2(a); //explicit vec(const array_type & scalars)
    const float a2[] = { 1.0f, 2.0f, 3.0f };
    wcout << a2 << L'\n';
    v6o = vec2(&a[0], 2); //explicit vec(const scalar_type * const scalars, const unsigned int count)

    p.x = 1.0f;
    auto x = p.x;
    cout << x << '\n';

    //TakesSize(p); // should fail
    TakesSize(static_cast<sizev>(p));

    //size s = p; // should fail
    auto s = static_cast<sizev>(p);
    // const sizev& s2 = p; // conversion fails to compile
    const sizev& s2 = static_cast<sizev>(p);
    cout << s2.width << '\n';
    //const sizev s3 = p; // fails to call explicit constructor
    const sizev s3 = sizev(p); // explicit constructor
    wcout << s3 << L'\n';
    //sizei si = 1.0; // should fail
    sizevi si = sizevi(static_cast<int>(1.0));
    wcout << si << L'\n';

    vec2 v1(1.0f); //explicit vec(const scalar_type value)
    v1 = static_cast<decltype(v1)>(s);
    v1 = p; //vec& operator =(const vec & rhs)
    //point p2; v1 = p2; // should fail

    const vec2::array_type& a3 = v1.data(); //const array_type& Data() const
    cout << a3 << '\n';
    const float f1 = v1[0]; //const scalar_type& operator [](const int index) const
    const float f2 = v1[1];
    wcout << f1 << ' ' << f2 << L'\n';
    //const float f3 = v1[2]; // shouldn't work TODO: debug bounds checking
    v1[0] = 8.0f; //scalar_type& operator [](const int index)

    vec2i vi1(1, 2);
    vec2i vi2{ 1, 2 };
    const bool equal = vi1 == vi2; //bool operator ==(const vec & rhs) const
    const bool not_equal = vi1 != vi2; //bool operator !=(const vec & rhs) const
    wcout << equal << ' ' << not_equal << L'\n';

    //bool operator <(const vec & rhs) const
    //bool operator <=(const vec & rhs) const
    //bool operator >(const vec & rhs) const
    //bool operator >=(const vec & rhs) const
    auto cr = vi1 < vi2;
    cout << cr << ' ';
    cr = vi1 <= vi2;
    cout << cr << ' ';
    cr = vi1 > vi2;
    cout << cr << ' ';
    cr = vi1 >= vi2;
    cout << cr << '\n';

    const auto vils = vi1.length_squared(); //scalar_type LengthSquared() const
    const float v6ls = v6o.length_squared();
    const float v6l = v6o.length(); //scalar_type Length() const
    cout << vils << ' ' << v6ls << ' ' << v6l << '\n';

    const vec2 u1 = v6o.unit(); //vec unit() const
    const vec2 ut = v6o.unit();
    cout << u1 << ' ' << ut << '\n';

    vec2 u2;
    u2 = v6o.unit();

    vec2 v2(1.0f, 0.0f);
    vec2 v3 = v2.rotate_clockwise(trig<>::half_pi); //derived_type rotate_clockwise(const scalar_type radians) const
    v3.normalize(); //scalar_type normalize()
    cout << "rotated: " << v3 << '\n';

    v3 = v2.rotate_counter_clockwise(trig<>::half_pi); //derived_type rotate_counter_clockwise(const scalar_type radians) const
    v3.normalize();
    cout << "rotated: " << v3 << '\n';

    const auto d1 = v2.dot(v3); // scalar_type dot(const vec & rhs) const
    cout << d1 << '\n';

    vec3 v3a{ 1.0f, 0.0f, 0.0f };
    vec3 v3b{ 0.0f, 1.0f, 0.0f };
    const vec3 c1o = v3a.cross(v3b); // derived_type cross(const vec& rhs) const
    cout << c1o << '\n';

    pointi pi;
    cout << "point: " << pi << '\n';

    vec3 v3c = v3a + v3b; //derived_type operator +(const vec & rhs) const
    vec3 v3d = v3a + 2.0f; //derived_type operator +(const scalar_type rhs) const
    v3c += v3a; //vec& operator +=(const vec & rhs)
    v3c += 2.0f; //vec& operator +=(const scalar_type rhs)
    cout << v3d << '\n';

    vec3 neg = -v3a; //derived_type operator -() const
    cout << neg << '\n';

    v3c = v3a - v3b; //derived_type operator -(const vec & rhs) const
    v3c = v3a - 2.0f; //derived_type operator -(const scalar_type rhs) const
    v3c += v3a; //vec& operator -=(const vec & rhs)
    v3c += 2.0f; //vec& operator -=(const scalar_type rhs)

    v3c = v3a * v3b; //derived_type operator *(const vec & rhs) const
    v3c = v3a * 2.0f; //derived_type operator *(const scalar_type rhs) const
    v3c *= v3a; //vec& operator *=(const vec & rhs)
    v3c *= 2.0f; //vec& operator *=(const scalar_type rhs)

    v3c = v3a / v3b; //derived_type operator /(const vec & rhs) const
    v3c = v3a / 2.0f; //derived_type operator /(const scalar_type rhs) const
    v3c /= v3a; //vec& operator /=(const vec & rhs)
    v3c /= 2.0f; //vec& operator /=(const scalar_type rhs)

    //vec3i v4a = static_cast<vec3i>(cast_scalars<int>(v3)); // should fail
    //vec2i v4 = static_cast<vec2i>(cast_scalars<int_fast32_t>(v3));
    vec2i v4(v3);
    cout << v4 << '\n';

    vec3i v5o(1, 2, 3);
    //v4 = cast_dimensions<2>(v5o);
    //v5o = cast_dimensions<3>(v4);
    v4 = vec2i(v5o);
    v5o = vec3i(v4);
    vec4i v7o(vec3(1.2f, 3.4f, 7.3f));
    //auto v6t = cast_dimensions<vec3>(v4); // should fail
    cout << v4 << ' ' << v5o << ' ' << v7o << '\n';

    v3a.swap(v3b);  //void swap(vec& rhs)

    const vec3 av3 = abs(v3a); // T abs(const T& value)
    cout << av3 << '\n';
    const auto av4 = abs(impl::vec<float, 4>({ 1, 3, 5, 6 }));
    cout << av4 << '\n';

    //const auto av5 = abs(impl::mat<float, 2, 2>({ 1, 3, 5, 6 }));
    const float fff = abs(3.95f);

    const vec3 sn = get_surface_normal(v3a, v3b, v3c); // T get_surface_normal(const T& v0, const T& v1, const T& v2)

    cout << av3 << ' ' << av4 << ' ' << fff << ' ' << sn << '\n';

    // 'Dst clg::get_surface_normal(
    // const clg::impl::vec<SrcT,SrcL,SrcU> &,
    // const clg::impl::vec<SrcT,SrcL,SrcU> &,
    // const clg::impl::vec<SrcT,SrcL,SrcU> &)
    //' : could not deduce template argument for 'Dst'

    ////////////////////////////////////////////////////////////////////////////////////////////////

    rect r1(3.5f, 54.2f, 2.3f, 6.9f);
    cout << "rect: " << r1 << '\n';

    rect r2(r1); //rect(const rect& original) : _location(original._location), _size(original._size) { }
    recti r3(r1); // constructor casts scalars
    cout << "rect: " << r3 << '\n';

    rect r4(10.0f, 10.0f); //explicit rect(const scalar_type width, const scalar_type height) : _size(width, height) { }
    rect r5(5.0f, 5.0f, 10.0f, 10.0f); //explicit rect(const scalar_type x, const scalar_type y, const scalar_type width, const scalar_type height) : _location(x, y), _size(width, height) { }

    rect r6(point(1.0f, 2.0f)); //explicit rect(const point_type& location) : _location(location) { }
    //rect r6t(vec2(1.0f, 2.0f)); // should fail
    rect r7(sizev(3.0f, 4.0f)); //explicit rect(const size_type& size) : _size(size) { }

    rect r8(point(1.0f, 2.0f), sizev(3.0f, 4.0f)); //rect(const point_type& location, const size_type& size) : _location(location), _size(size) { }
    //rect r8t(size(3.0f, 4.0f), point(1.0f, 2.0f)); // should fail

    r6 = r7; //rect& operator =(const rect& rhs)
    wcout << r3 << L' ' << r4 << L' ' << r5 << L' ' << r6 << L' ' << r7 << L'\n';


    const point pa = r6.location(); //const point_type& location() const
    r6.location(point(5.0f, 6.0f)); //point_type& location()

    const sizev sa = r7.size(); //size_type GetSize() const
    r7.size(sizev(7.0f, 8.0f)); //void SetSize(const size_type& size)
    float x1 = r6.x(); //scalar_type GetX() const
    r6.x(x1); //void SetX(const scalar_type x)
    x1 = r6.y(); //scalar_type GetY() const
    r6.y(x1); //void SetY(const scalar_type y)
    x1 = r6.width(); //scalar_type GetWidth() const
    r6.width(x1); //void SetWidth(const scalar_type width)
    x1 = r6.height(); //scalar_type GetHeight() const
    r6.height(x1); //void SetHeight(const scalar_type height)
    x1 = r6.left(); //scalar_type GetLeft() const
    r6.left(x1); //void SetLeft(const scalar_type left)
    x1 = r6.top(); //scalar_type GetTop() const
    r6.top(x1); //void SetTop(const scalar_type top)
    x1 = r6.right(); //scalar_type GetRight() const
    r6.right(x1); //void SetRight(const scalar_type right)
    x1 = r6.bottom(); //scalar_type GetBottom() const
    r6.bottom(x1); //void SetBottom(const scalar_type bottom)

    bool b2 = r6 == r7; //bool operator ==(const rect& rhs) const
    cout << b2 << ' ';
    //bool b2t = r6 == r3; // should fail
    b2 = r6 != r7; //bool operator !=(const rect& rhs) const
    cout << b2 << ' ';
    b2 = r7.contains(point(5.0f, 5.0f)); //bool contains(const point_type& point) const
    cout << b2 << ' ';
    b2 = r7.overlaps(r8); //bool overlaps(const rect& rectangle) const
    cout << b2 << '\n';

    impl::vec<float, 4> v4va(1.5f, vec2(2.0f, 3.0f), 4.0f);
    impl::vec<float, 4> v4vb(vec2(2.0f, 3.0f), 1, 4);
    impl::vec<float, 4> v4vd(1, 4, vec2(2.0f, 3.0f));
    impl::vec<float, 4> v4bc(1, 2, 3, 4.0f);
    cout << v4va << '\n';
    cout << v4vb << '\n';
    cout << v4vd << '\n';
    cout << v4bc << '\n';

    vec4 v4var(4, 3, 2, 1);
    cout << v4var << '\n';
    v4var = vec4(4, 3, vec2(5, 6));
    cout << v4var << '\n';

    ////////////////////////////////////////////////////////////////////////////////////////////////

    int flat_arr[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << flat_arr[3 * j + i] << ' ';
        }
        cout << '\n';
    }

    int multi_arr[3][3] = { { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 } };
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            cout << multi_arr[j][i] << ' ';
        }
        cout << '\n';
    }

    //mat<int, 3, 3> mat1;
    //auto trans1 = translation_matrix(vec3i(1, 2, 3));
    //cout << trans1 << '\n';

    auto ident1 = impl::mat<float, 4, 4>();
    cout << ident1 << '\n';
    auto ident2 = impl::mat<float, 3, 4>();
    cout << ident2 << '\n';
    auto ident3 = impl::mat<float, 4, 3>();
    cout << ident3 << '\n';

    mat2 id21; // <- default constructor
    cout << id21 << '\n';
    wcout << id21 << L'\n';

    mat2x3 id231;
    cout << id231 << '\n' << '\n';

    id21 = mat2({ 1, 2, 3, 4 }); // assignment operator
    cout << id21 << '\n';
    for (auto& e : id21)
    {
        e += 1;
        cout << e << '\t';
    }
    cout << '\n';

    const mat2 cid22({ 5, 6, 7, 8 }); // <- scalar constructor
    for (auto& e : cid22)
    {
        //e += 1;
        cout << e << '\t';
    }
    cout << '\n';

    cout << cid22.data()[0] << '\t' << cid22.data()[1] << '\n' << cid22.data()[2] << '\t' << cid22.data()[3] << '\n';
    cout << cid22 << '\n';

    id21 = mat2(cid22); // <- copy constructor
    cout << id21 << '\n';

    cout << cid22[0] << '\n';
    //cid22[0] = vec2(8, 8); // assignment to const vec2 fails to compile

    // scalar pointer constructor tests
    float scalars[] = { 1, 2, 3, 4 };
    id21 = mat2(scalars, array_count(scalars));
    cout << id21 << '\n';
    float scalars2[] = { 5, 6, 7, 8 };
    id21 = mat2(scalars2, 0);
    cout << id21 << '\n';
    id21 = mat2(scalars2, 1);
    cout << id21 << '\n';
    id21 = mat2(scalars2, 3);
    cout << id21 << '\n';
    // id21 = mat2(scalars2, 5); // fails with asserts enabled
    cout << id21 << '\n';

    // single scalar constructor
    id21 = mat2(-1);
    cout << id21 << '\n';
    id21 = -2; // scalar assignment
    cout << id21 << '\n';
    cout << mat2::identity() << '\n';
    cout << mat2::zero() << '\n';
    // id21.data()[1] = -3; // assignment to const fails to compile
    cout << id21 << '\n';
    cout << "is NOT identity matrix: " << (id21 != mat2::identity()) << '\n';
    cout << "is NOT identity matrix: " << (mat2({ 1, 0, 0, 1 }) != mat2::identity()) << '\n';
    cout << "is identity matrix: " << (mat2({ 1, 0, 0, 1 }) == mat2::identity()) << '\n';
    cout << "is identity matrix: " << (id21 == mat2::identity()) << '\n';
    cout << "is 0, 0, 0, 0 < 1, 0, 0, 0:" << (mat2({ 0, 0, 0, 0 }) < mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 1 < 0, 0, 0, 0:" << (mat2({ 0, 0, 0, 1 }) < mat2({ 0, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 0 <= 1, 0, 0, 0:" << (mat2({ 0, 0, 0, 0 }) <= mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 1, 0, 0, 0 <= 1, 0, 0, 0:" << (mat2({ 1, 0, 0, 0 }) <= mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 1 <= 1, 0, 0, 0:" << (mat2({ 0, 0, 0, 1 }) <= mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 1 <= 0, 0, 0, 0:" << (mat2({ 0, 0, 0, 1 }) <= mat2({ 0, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 0 > 1, 0, 0, 0:" << (mat2({ 0, 0, 0, 0 }) > mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 1 > 0, 0, 0, 0:" << (mat2({ 0, 0, 0, 1 }) > mat2({ 0, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 0 >= 1, 0, 0, 0:" << (mat2({ 0, 0, 0, 0 }) >= mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 1, 0, 0, 0 >= 1, 0, 0, 0:" << (mat2({ 1, 0, 0, 0 }) >= mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 1 >= 1, 0, 0, 0:" << (mat2({ 0, 0, 0, 1 }) >= mat2({ 1, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is 0, 0, 0, 1 >= 0, 0, 0, 0:" << (mat2({ 0, 0, 0, 1 }) >= mat2({ 0, 0, 0, 0 }) ? "true" : "false") << '\n';
    cout << "is square mat2: " << (mat2::zero().is_square() ? "true" : "false") << '\n';
    cout << "is square mat2x3: " << (mat2x3::zero().is_square() ? "true" : "false") << '\n';
    cout << "is square mat3x2: " << (mat3x2::zero().is_square() ? "true" : "false") << '\n';
    cout << "is diagonal zero: " << (mat2::zero().is_diagonal() ? "true" : "false") << '\n';
    cout << "is diagonal identity: " << (mat2().is_diagonal() ? "true" : "false") << '\n';
    cout << "is diagonal 0, 0, 1, 0: " << (mat2({ 0, 0, 1, 0 }).is_diagonal() ? "true" : "false") << '\n';
    cout << "is diagonal 0, 1, 0, 0: " << (mat2({ 0, 1, 0, 0 }).is_diagonal() ? "true" : "false") << '\n';
    cout << "is diagonal 1, 0, 0, 0: " << (mat2({ 1, 0, 0, 0 }).is_diagonal() ? "true" : "false") << '\n';
    cout << "is diagonal 0, 0, 0, 1: " << (mat2({ 0, 0, 0, 1 }).is_diagonal() ? "true" : "false") << '\n';
    cout << "is upper triangular 0, 0, 1, 0: " << (mat2({ 0, 0, 1, 0 }).is_upper_triangular() ? "true" : "false") << '\n';
    cout << "is upper triangular 0, 1, 0, 0: " << (mat2({ 0, 1, 0, 0 }).is_upper_triangular() ? "true" : "false") << '\n';
    cout << "is upper triangular 1, 0, 0, 0: " << (mat2({ 1, 0, 0, 0 }).is_upper_triangular() ? "true" : "false") << '\n';
    cout << "is upper triangular 0, 0, 0, 1: " << (mat2({ 0, 0, 0, 1 }).is_upper_triangular() ? "true" : "false") << '\n';
    cout << "is lower triangular 0, 0, 1, 0: " << (mat2({ 0, 0, 1, 0 }).is_lower_triangular() ? "true" : "false") << '\n';
    cout << "is lower triangular 0, 1, 0, 0: " << (mat2({ 0, 1, 0, 0 }).is_lower_triangular() ? "true" : "false") << '\n';
    cout << "is lower triangular 1, 0, 0, 0: " << (mat2({ 1, 0, 0, 0 }).is_lower_triangular() ? "true" : "false") << '\n';
    cout << "is lower triangular 0, 0, 0, 1: " << (mat2({ 0, 0, 0, 1 }).is_lower_triangular() ? "true" : "false") << '\n';
    cout << "is row matrix identity: " << (mat2().is_row_matrix() ? "true" : "false") << '\n';
    cout << "is row matrix mat<2,1>: " << (impl::mat<float, 2, 1>().is_row_matrix() ? "true" : "false") << '\n';
    cout << "is row matrix mat<1,2>: " << (impl::mat<float, 1, 2>().is_row_matrix() ? "true" : "false") << '\n';
    cout << "is row matrix mat<1,1>: " << (impl::mat<float, 1, 1>().is_row_matrix() ? "true" : "false") << '\n';
    cout << "is column matrix mat<2,1>: " << (impl::mat<float, 2, 1>().is_column_matrix() ? "true" : "false") << '\n';
    cout << "is column matrix mat<1,2>: " << (impl::mat<float, 1, 2>().is_column_matrix() ? "true" : "false") << '\n';
    cout << "is column matrix mat<1,1>: " << (impl::mat<float, 1, 1>().is_column_matrix() ? "true" : "false") << '\n';
    cout << (mat2() + mat2()) << '\n';
    cout << (mat2() + 4) << '\n';
    cout << (mat2() - mat2()) << '\n';
    cout << (mat2() - 4) << '\n';
    cout << (mat2() * 3) << '\n';

    // 1 3 = 1*1+3*2, 1*3+3*4 =  7 15
    // 2 4 = 2*1+4*2, 2*3+4*4 = 10 22
    cout << (mat2({ 1, 2, 3, 4 }) * mat2({ 1, 2, 3, 4 })) << '\n';

    cout << (mat2({ 1, 2, 3, 4 }) / 2) << '\n';
    cout << (mat2({ 1, 2, 3, 4 }) /= 2) << '\n';
    cout << (mat2() += mat2()) << '\n';
    cout << (mat2() += 4) << '\n';
    cout << (mat2() -= mat2()) << '\n';
    cout << (mat2() -= 4) << '\n';
    cout << (mat2({ 1, 2, 3, 4 }) *= mat2({ 1, 2, 3, 4 })) << '\n';
    cout << (mat2({ 1, 2, 3, 4 }).row(0)) << '\n' << (mat2({ 1, 2, 3, 4 }).row(1)) << '\n';
    // cout << (mat2({ 1, 2, 3, 4 }).row(3)) << '\n'; // asserts
    cout << (mat2({ 1, 2, 3, 4 })[0]) << '\n' << (mat2({ 1, 2, 3, 4 })[1]) << '\n';
    // cout << (mat2({ 1, 2, 3, 4 }).column(3)) << '\n'; // asserts
    mat2 rct;
    rct[0] = vec2(5, 5);
    rct[1] = vec2(6, 6);
    cout << rct << '\n';
    rct.row(0, vec2(5, 5));
    rct.row(1, vec2(6, 6));
    cout << rct << '\n';
    rct = mat2({ 1, 2, 3, 4 });
    rct = rct.transpose();
    cout << rct << '\n';
    auto m23 = mat2x3({ 1, 2, 3, 4, 5, 6 });
    auto m32 = mat3x2({ 1, 2, 3, 4, 5, 6 });
    cout << m23 << '\n';
    cout << m32 << '\n';
    auto rct2 = mat2({ 1, 2, 3, 4 });

    cout << rct << '\n';
    cout << rct2 << '\n';
    swap(rct2, rct);
    cout << rct << '\n';
    cout << rct2 << '\n';


    cout << cid22(1, 1) << '\n';
    rct(1, 1) = 50;
    cout << rct << '\n';

    cout << translation_matrix(vec2({ 4, 5 })) << '\n';
    cout << translation_matrix(vec3({ 1.2f, 2.3f, 3.4f })) << '\n' << '\n';

    cout << rotation_matrix(trig<>::half_pi) << '\n';
    cout << rotation_matrix_z(trig<>::half_pi) << '\n';
    cout << rotation_matrix_y(trig<>::half_pi) << '\n';
    cout << rotation_matrix_x(trig<>::half_pi) << '\n' << '\n';

    cout << scaling_matrix(vec2({ 1.2f, 2.3f })) << '\n';
    cout << scaling_matrix(vec3({ 1.2f, 2.3f, 3.4f })) << '\n' << '\n';

    cout << reflection_about_x_matrix() << '\n' << '\n';
    cout << reflection_about_y_matrix() << '\n' << '\n';
    cout << reflection_about_xy_line_matrix() << '\n' << '\n';
    cout << reflection_about_negative_xy_line_matrix() << '\n' << '\n';
    cout << reflection_about_origin_matrix() << '\n' << '\n';

    cout << shearing_matrix_x(1.4f) << '\n' << '\n';
    cout << shearing_matrix_y(4.1f) << '\n' << '\n';
    cout << shearing_matrix_xy(2.3f) << '\n' << '\n';
    cout << shearing_matrix_xz(3.4f) << '\n' << '\n';
    cout << shearing_matrix_yx(4.5f) << '\n' << '\n';
    cout << shearing_matrix_yz(5.6f) << '\n' << '\n';
    cout << shearing_matrix_zx(6.7f) << '\n' << '\n';
    cout << shearing_matrix_zy(7.8f) << '\n' << '\n';

    cout << euler_transfor_mmatrix(trig<>::pi, trig<>::pi / 2.0f, trig<>::pi / 3.0f) << '\n' << '\n';
    cout << arbitrary_axis_rotation_matrix(vec3(1, 2, 3), trig<>::pi) << '\n' << '\n';

    cout << orthographic_projectio_nmatrix_gl(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f) << '\n' << '\n';
    cout << orthographic_projection_matrix_dx(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f) << '\n' << '\n';
    cout << perspective_projection_matrix_gl(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1.0f) << '\n' << '\n';
    cout << perspective_projection_matrix_gl(to_radians(90.0f), 19.0f / 9.0f, 0.1f, 1.0f) << '\n' << '\n';
    float distanceToNearClipPlane = -99.0f;
    cout << normalized_perspective_projection_matrix_gl(to_radians(90.0f), 21.0f / 9.0f, 1.0f, distanceToNearClipPlane) << '\n' << distanceToNearClipPlane << '\n' << '\n';
    cout << perspective_projection_matrix_dx(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1.0f) << '\n' << '\n';

    //cout << cast_scalars<int>(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';
    cout << impl::mat<int, 2, 2>(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';

    //cout << cast_dimensions<2>(mat3({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })) << '\n' << '\n';
    //cout << cast_dimensions<2, 3>(mat3({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })) << '\n' << '\n';
    //cout << cast_dimensions<3, 2>(mat3({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })) << '\n' << '\n';
    //cout << cast_dimensions<3>(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';
    //cout << cast_dimensions<2, 3>(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';
    //cout << cast_dimensions<3, 2>(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';
    cout << mat2(impl::mat<int, 3, 3>({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })) << '\n' << '\n';
    cout << mat2x3(mat3({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })) << '\n' << '\n';
    cout << mat3x2(mat3({ 1, 2, 3, 4, 5, 6, 7, 8, 9 })) << '\n' << '\n';
    cout << mat3(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';
    cout << mat2x3(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';
    cout << mat3x2(mat2({ 1, 2, 3, 4 })) << '\n' << '\n';

    cout << cast_column_matrix(vec4(1, 2, 3, 4)) << '\n' << '\n';
    cout << cast_row_matrix(vec4(1, 2, 3, 4)) << '\n' << '\n';

    //mat3 colinit0(8.0f, 7.0f);
    //cout << colinit0 << '\n' << '\n';
    mat3 colinit01(8.0f, 7.0f, 6.0f, 5.0f, 1.0f, 2.0f, 3.0f, 4.0f, 9.0f);
    cout << colinit01 << '\n' << '\n';
    mat3 colinit1(vec3(1, 2, 3), 5.0f, 1.0f, 2.0f, 3.0f, vec2(4, 5));
    cout << colinit1 << '\n' << '\n';
    //mat3 colinit2(vec3(1, 2, 3), vec3(4, 5, 6));
    //cout << colinit2 << '\n' << '\n';
    mat3 colinit3(vec3(1, 2, 3), vec3(4, 5, 6), vec3(7, 8, 9));
    cout << colinit3 << '\n' << '\n';
    //mat3 colinit4(vec3(1, 2, 3), vec3(4, 5, 6), vec3(7, 8, 9), vec3(10, 11, 12)); // fails with static assert
    //mat3 colinit4(vec3(1, 2, 3), vec3(4, 5, 6), vec3(7, 8, 9), 10.0f); // fails with static assert

    //mat3 colinit4(vec2(1, 2));
    //cout << colinit4 << '\n' << '\n';
    //mat3 colinit5(vec2(1, 2), vec2(4, 5));
    //cout << colinit5 << '\n' << '\n';
    //mat3 colinit6(vec3(1, 2, 3), vec2(4, 5), vec3(7, 8, 9));
    //cout << colinit6 << '\n' << '\n';
    mat3 colinit7(vec3(1, 2, 3), vec2(4, 5), vec4(7, 8, 9, 10));
    cout << colinit7 << '\n' << '\n';

    mat3 colinit8(vec2(1, 3), vec2(4, 5), 8.0f, vec4(11, 12, 13, 14));
    cout << colinit8 << '\n' << '\n';

    mat2 b2st(colinit8);
    cout << b2st << '\n' << '\n';
    mat3 s2bt(b2st);
    cout << s2bt << '\n' << '\n';

    mat2 colint9(1, 2.0f, 3.0f, 4);
    cout << colint9 << '\n' << '\n';
    mat2 colint10(1.0f, 2, 3.0f, 4.0f);
    cout << colint10 << '\n' << '\n';
}
