/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

#define BOOST_TEST_MODULE test_eckit_la_linalg

#include "ecbuild/boost_test_framework.h"

#include "eckit/config/Resource.h"
#include "eckit/exception/Exceptions.h"
#include "eckit/linalg/LinearAlgebra.h"
#include "eckit/linalg/Matrix.h"
#include "eckit/linalg/SparseMatrix.h"
#include "eckit/linalg/Vector.h"

#include "util.h"

#include "eckit/testing/Setup.h"



using namespace eckit::linalg;

namespace eckit {
namespace test {

//----------------------------------------------------------------------------------------------------------------------

SparseMatrix S(Size rows, Size cols, Size nnz, ...) {
    va_list args;
    va_start(args, nnz);
    std::vector<Triplet> triplets;
    for (Size n = 0; n < nnz; ++n) {
        Size row = Size( va_arg(args, int) );
        Size col = Size( va_arg(args, int) );
        Scalar v = va_arg(args, Scalar);
        triplets.push_back(Triplet(row, col, v));
    }
    va_end(args);

    SparseMatrix mat(rows, cols, triplets);

//    ECKIT_DEBUG_VAR(mat.nonZeros());
//    ECKIT_DEBUG_VAR(mat.rows());
//    ECKIT_DEBUG_VAR(mat.cols());

    return mat;
}



// Set linear algebra backend
struct Setup : testing::Setup {
    Setup() : testing::Setup() {
        LinearAlgebra::backend(Resource<std::string>("-linearAlgebraBackend", "generic"));
    }
};


//----------------------------------------------------------------------------------------------------------------------

BOOST_GLOBAL_FIXTURE(Setup);



struct Fixture {

    Fixture() : A(S(3, 3, 4,
                    0, 0, 2.,
                    0, 2, -3.,
                    1, 1, 2.,
                    2, 2, 2.)),
                A2(S(2, 3, 4,
                     0, 0, 1.,
                     0, 2, 2.,
                     1, 0, 3.,
                     1, 1, 4.)),
                x(V(3, 1., 2., 3.)),
                linalg(LinearAlgebra::backend()) {}

    // A = 2 0 -3
    //     0 2  0
    //     0 0  2
    SparseMatrix A;
    // A2 = 1. 0. 2.
    //      3. 4. 0.
    SparseMatrix A2;
    Vector x;
    const LinearAlgebra& linalg;
};



template <class T>
void test(const T& v, const T& r) {
    const size_t s = std::min(v.size(), r.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(v.data(), v.data() + s, r.data(), r.data() + s);
}

template <typename T>
void test(T* v, T* r, size_t s) {
    BOOST_CHECK_EQUAL_COLLECTIONS(v, v + s, r, r + s);
}

void test(const SparseMatrix& A, const Index* outer, const Index* inner, const Scalar* data) {
    test(A.outer(), outer, A.rows()+1);
    test(A.inner(), inner, A.nonZeros());
    test(A.data(), data, A.nonZeros());
}


/// Test linear algebra interface

BOOST_FIXTURE_TEST_SUITE(test_eckit_la_sparse, Fixture)

BOOST_AUTO_TEST_CASE(test_set_from_triplets) {

    {
        BOOST_CHECK_EQUAL(A.nonZeros(), 4);

        Index outer[4] = {0, 2, 3, 4};
        Index inner[4] = {0, 2, 1, 2};
        Scalar data[4] = {2., -3., 2., 2.};
        test(A, outer, inner, data);
    }

    // Pathological case with empty rows
    {
        Index outer[7] = {0, 0, 1, 1, 2, 2, 2};
        Index inner[2] = {0, 3};
        Scalar data[2] = {1., 2.};
        test(S(6, 6, 2, 1, 0, 1., 3, 3, 2.), outer, inner, data);
    }

    // Rows in wrong order (not triggering right now since triplets are sorted)
    //BOOST_CHECK_THROW(S(2, 2, 2, 1, 1, 1., 0, 0, 1.), AssertionFailed);
}

BOOST_AUTO_TEST_CASE(test_identity) {

    {
        Vector y1(3);

        SparseMatrix B;
        B.setIdentity(3, 3);

        linalg.spmv(B, x, y1);
        test(y1, x);
    }

    {
        SparseMatrix C;
        C.setIdentity(6, 3);

        Vector y2(6);
        linalg.spmv(C, x, y2);
        test(y2, x);
        test(y2.data()+3, V(3, 0., 0., 0.).data(), 3);
    }

    {
        SparseMatrix D;
        D.setIdentity(2, 3);

        Vector y3(2);

        linalg.spmv(D, x, y3);
        test(y3, x);
    }
}

BOOST_AUTO_TEST_CASE(test_prune) {

    SparseMatrix A(S(3, 3, 5,
                     0, 0, 0.,
                     0, 2, 1.,
                     1, 0, 0.,
                     1, 1, 2.,
                     2, 2, 0.));

    A.prune();
    BOOST_CHECK_EQUAL(A.nonZeros(), 2);
    Index outer[4] = {0, 1, 2, 2};
    Index inner[2] = {2, 1};
    Scalar data[2] = {1., 2.};
    test(A, outer, inner, data);
}

BOOST_AUTO_TEST_CASE(test_iterator) {

    SparseMatrix A(S(3, 3, 5,
                     0, 0, 0.,
                     1, 0, 0.,
                     1, 1, 0.,
                     1, 2, 1.,
                     2, 2, 2.));

    A.prune();
    BOOST_CHECK_EQUAL(A.nonZeros(), 2);

    //  data    [ 1 2 ]
    //  outer   [ 0 0 1 2 ]
    //  inner   [ 2 2 ]

    Scalar data[2] = {1., 2.};
    Index outer[4] = {0, 0, 1, 2};
    Index inner[2] = {2, 2};
    test(A, outer, inner, data);

    SparseMatrix::const_iterator it = A.begin();

    // check entry #1
    BOOST_CHECK_EQUAL(it.row(), 1);
    BOOST_CHECK_EQUAL(it.col(), 2);
    BOOST_CHECK(*it == 1.);

    // check entry #2
    ++it;

    BOOST_CHECK_EQUAL(it.row(), 2);
    BOOST_CHECK_EQUAL(it.col(), 2);
    BOOST_CHECK(*it == 2.);

    // go past the end
    BOOST_CHECK(it != A.end());

    ++it;

    BOOST_CHECK(it == A.end());
    BOOST_CHECK(!it);

    // go back and re-check entry #1
    // (row 0 is empty, should relocate to row 1)
    it = A.begin();
    BOOST_CHECK(it);

    BOOST_CHECK_EQUAL(it.row(), 1);
    BOOST_CHECK_EQUAL(it.col(), 2);
    BOOST_CHECK(*it == 1.);

    // go way past the end
    it = A.begin(42);
    BOOST_CHECK(!it);

}


BOOST_AUTO_TEST_CASE(test_transpose_square) {
    Index outer[4] = {0, 1, 2, 4};
    Index inner[4] = {0, 1, 0, 2};
    Scalar data[4] = {2., 2., -3., 2.};
    test(A.transpose(), outer, inner, data);
}


BOOST_AUTO_TEST_CASE(test_transpose_nonsquare) {
    Index outer[4] = {0, 2, 3, 4};
    Index inner[4] = {0, 1, 1, 0};
    Scalar data[4] = {1., 3., 4., 2.};
    test(A2.transpose(), outer, inner, data);
}

BOOST_AUTO_TEST_CASE(test_spmv) {
    Vector y(3);
    linalg.spmv(A, x, y);
    test(y, V(3, -7., 4., 6.));
    BOOST_TEST_MESSAGE("spmv of sparse matrix and vector of nonmatching sizes should fail");
    BOOST_CHECK_THROW(linalg.spmv(A, Vector(2), y), AssertionFailed);
}

BOOST_AUTO_TEST_CASE(test_spmm) {
    Matrix C(3, 2);
    linalg.spmm(A, M(3, 2, 1., 2., 3., 4., 5., 6.), C);
    test(C, M(3, 2, -13., -14., 6., 8., 10., 12.));
    BOOST_TEST_MESSAGE("spmm of sparse matrix and matrix of nonmatching sizes should fail");
    BOOST_CHECK_THROW(linalg.spmm(A, Matrix(2, 2), C), AssertionFailed);
}

BOOST_AUTO_TEST_CASE(test_dsptd_square) {
    SparseMatrix B;
    linalg.dsptd(x, A, x, B);
    Index outer[4] = {0, 1, 2, 4};
    Index inner[4] = {0, 1, 0, 2};
    Scalar data[4] = {2., 8., -9., 18.};
    test(B, outer, inner, data);
    BOOST_TEST_MESSAGE("dsptd with vectors of nonmatching sizes should fail");
    BOOST_CHECK_THROW(linalg.dsptd(x, A, Vector(2), B), AssertionFailed);
}

BOOST_AUTO_TEST_CASE(test_dsptd_nonsquare) {
    SparseMatrix B;
    linalg.dsptd(x, A2, V(2, 1., 2.), B);
    Index outer[4] = {0, 2, 3, 4};
    Index inner[4] = {0, 1, 1, 0};
    Scalar data[4] = {1., 6., 16., 6.};
    test(B, outer, inner, data);
    BOOST_TEST_MESSAGE("dsptd with vectors of nonmatching sizes should fail");
    BOOST_CHECK_THROW(linalg.dsptd(x, A2, x, B), AssertionFailed);
}

BOOST_AUTO_TEST_SUITE_END()

//----------------------------------------------------------------------------------------------------------------------

} // namespace test
} // namespace eckit
