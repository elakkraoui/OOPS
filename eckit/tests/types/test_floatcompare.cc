/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <cmath>

#define BOOST_TEST_MODULE test_eckit_types

#include "ecbuild/boost_test_framework.h"

#include "eckit/types/FloatCompare.h"

#include "eckit/testing/Setup.h"

using namespace eckit;
using namespace eckit::testing;

namespace {

bool is_equal(float a, float b, float epsilon, int maxUlps) {
    return eckit::types::is_approximately_equal(a, b, epsilon, maxUlps);
}

bool is_equal(float a, float b, float epsilon) {
    return eckit::types::is_approximately_equal(a, b, epsilon);
}

bool is_equal(float a, float b) {
    return eckit::types::is_approximately_equal(a, b, 0.00001f);
}

const float dEps = std::numeric_limits<float>::epsilon();
const float dInf = std::numeric_limits<float>::infinity();
const float sMin = std::numeric_limits<float>::denorm_min();
const float dMin = std::numeric_limits<float>::min();
const float dMax = std::numeric_limits<float>::max();
const float qNaN = std::numeric_limits<float>::quiet_NaN();
const float sNaN = std::numeric_limits<float>::signaling_NaN();

};

//-----------------------------------------------------------------------------

BOOST_GLOBAL_FIXTURE(Setup);

BOOST_AUTO_TEST_SUITE( test_eckit_floatcompare )

BOOST_AUTO_TEST_CASE( test_large_numbers )
{
   BOOST_TEST_MESSAGE( "test_large_numbers" );

   BOOST_CHECK(  is_equal(1000000,       1000000      ));
   BOOST_CHECK(  is_equal(1000000,       1000000.00001));
   BOOST_CHECK(  is_equal(1000000.00001, 1000000      ));

   BOOST_CHECK(! is_equal(1000000.0,     1000001.0    ));
   BOOST_CHECK(! is_equal(1000001.0,     1000000.0    ));

   // -----------------------------------------------
   BOOST_CHECK(is_equal(dMax, dMax));
   BOOST_CHECK(is_equal(dMax, dMax, dEps));

   BOOST_CHECK(is_equal(dMin, dMin));
   BOOST_CHECK(is_equal(dMin, dMin, dEps));
}

BOOST_AUTO_TEST_CASE( test_negative_large_numbers )
{
   BOOST_TEST_MESSAGE( "test_negative_large_numbers " << dMin );

   BOOST_CHECK(  is_equal(-1000000,       -1000000      ));
   BOOST_CHECK(  is_equal(-1000000,       -1000000.00001));
   BOOST_CHECK(  is_equal(-1000000.00001, -1000000      ));

   BOOST_CHECK(! is_equal(-1000000.0,     -1000001.0    ));
   BOOST_CHECK(! is_equal(-1000001.0,     -1000000.0    ));

   // -----------------------------------------------
   BOOST_CHECK(is_equal(-dMax, -dMax      ));
   BOOST_CHECK(is_equal(-dMax, -dMax, dEps));

   BOOST_CHECK(is_equal(-dMin, -dMin      ));
   BOOST_CHECK(is_equal(-dMin, -dMin, dEps));
}

BOOST_AUTO_TEST_CASE( test_large_numbers_of_opposite_sign )
{
    BOOST_CHECK(! is_equal(-1000000,       1000000      ));
    BOOST_CHECK(! is_equal(-1000000,       1000000.00001));
    BOOST_CHECK(! is_equal(-1000000.00001, 1000000      ));

    BOOST_CHECK(! is_equal(-1000000.0,     1000001.0    ));
    BOOST_CHECK(! is_equal(-1000001.0,     1000000.0    ));

    // -----------------------------------------------
    BOOST_CHECK(! is_equal(-dMax, dMax      ));
    BOOST_CHECK(! is_equal(-dMax, dMax, dEps));
}

BOOST_AUTO_TEST_CASE( test_ulp_around_one )
{
   BOOST_TEST_MESSAGE( "test_ulp_around_one" );

   // ULP distances up to 10 are equal
   // Going right from 1 by eps increases distance by 1
   // Going left from 1 by eps increases distance by 2
   for (int i = 0; i <= 10; ++i) {
       BOOST_CHECK(is_equal(1.0 + i * dEps,   1.0,              dEps));
       BOOST_CHECK(is_equal(1.0,              1.0 + i * dEps,   dEps));
       BOOST_CHECK(is_equal(1.0 - i * dEps/2, 1.0,              dEps));
       BOOST_CHECK(is_equal(1.0,              1.0 - i * dEps/2, dEps));
   }
   // ULP distances greater 10 are not equal
   BOOST_CHECK(! is_equal(1.0 + 11 * dEps,   1.0,               dEps));
   BOOST_CHECK(! is_equal(1.0,               1.0 + 11 * dEps,   dEps));
   BOOST_CHECK(! is_equal(1.0 - 11 * dEps/2, 1.0,               dEps));
   BOOST_CHECK(! is_equal(1.0,               1.0 - 11 * dEps/2, dEps));
}

BOOST_AUTO_TEST_CASE( test_numbers_around_one )
{
   BOOST_TEST_MESSAGE( "test_numbers_around_one" );

   BOOST_CHECK(  is_equal(1.0000001, 1.0000002));
   BOOST_CHECK(  is_equal(1.0000002, 1.0000001));

   BOOST_CHECK(  is_equal(1.123456,  1.123457 ));
   BOOST_CHECK(  is_equal(1.12345,   1.12344, 0.001));

   BOOST_CHECK(! is_equal(1.0001,    1.0002   ));
   BOOST_CHECK(! is_equal(1.0002,    1.0001   ));
}

BOOST_AUTO_TEST_CASE( test_numbers_around_negative_one )
{
   BOOST_TEST_MESSAGE( "test_numbers_around_negative_one" );

   BOOST_CHECK(  is_equal(-1.0000001, -1.0000002));
   BOOST_CHECK(  is_equal(-1.0000002, -1.0000001));

   BOOST_CHECK(! is_equal(-1.0001,    -1.0002   ));
   BOOST_CHECK(! is_equal(-1.0002,    -1.0001   ));
}

BOOST_AUTO_TEST_CASE( test_numbers_between_one_and_zero )
{
   BOOST_TEST_MESSAGE( "test_numbers_between_one_and_zero" );

   BOOST_CHECK(  is_equal(0.000000001000001, 0.000000001000002));
   BOOST_CHECK(  is_equal(0.000000001000002, 0.000000001000001));

   BOOST_CHECK(! is_equal(0.0012,            0.0011           ));
   BOOST_CHECK(! is_equal(0.0011,            0.0012           ));

}

BOOST_AUTO_TEST_CASE( test_numbers_between_minusone_and_zero )
{
   BOOST_TEST_MESSAGE( "test_numbers_between_minusone_and_zero" );

   BOOST_CHECK(  is_equal(-0.000000001000001, -0.000000001000002));
   BOOST_CHECK(  is_equal(-0.000000001000002, -0.000000001000001));

   BOOST_CHECK(! is_equal(-0.0012,            -0.0011           ));
   BOOST_CHECK(! is_equal(-0.0011,            -0.0012           ));
}

BOOST_AUTO_TEST_CASE( test_comparisons_involving_zero )
{
   BOOST_TEST_MESSAGE( "test_comparisons_involving_zero" );

   BOOST_CHECK(  is_equal(0.0,     0.0   ));
   BOOST_CHECK(  is_equal(0.0,    -0.0   ));
   BOOST_CHECK(  is_equal(-0.0,   -0.0   ));

   BOOST_CHECK(! is_equal(0.0001,  0.0   ));
   BOOST_CHECK(! is_equal(0.0,     0.0001));
   BOOST_CHECK(! is_equal(-0.0001, 0.0   ));
   BOOST_CHECK(! is_equal(0.0,    -0.0001));

   BOOST_CHECK(  is_equal(0.0,     1e-30, 0.01 ));
   BOOST_CHECK(  is_equal(1e-30,   0.0,   0.01 ));
   BOOST_CHECK(! is_equal(1e-30,   0.0,   1e-31));
   BOOST_CHECK(! is_equal(0.0,     1e-30, 1e-31));

   BOOST_CHECK(  is_equal(0.0,    -1e-30, 0.1  ));
   BOOST_CHECK(  is_equal(-1e-30,  0.0,   0.1  ));
   BOOST_CHECK(! is_equal(-1e-30,  0.0,   1e-31));
   BOOST_CHECK(! is_equal(0.0,    -1e-30, 1e-31));
}

BOOST_AUTO_TEST_CASE( test_comparisons_involving_infinity )
{
   BOOST_TEST_MESSAGE( "test_comparisons_involving_infinity" );

   if (std::numeric_limits<float>::has_infinity) {
      BOOST_CHECK(  is_equal( dInf,  dInf));
      BOOST_CHECK(  is_equal(-dInf, -dInf));
      BOOST_CHECK(! is_equal( dInf,  dMax));
      BOOST_CHECK(! is_equal( dMax,  dInf));
      BOOST_CHECK(! is_equal(-dInf, -dMax));
      BOOST_CHECK(! is_equal(-dMax, -dInf));
   } else {
      BOOST_TEST_MESSAGE( "test_comparisons_involving_infinity NOT VALID on this platform" );
   }
}

BOOST_AUTO_TEST_CASE( test_comparisons_involving_nan )
{
    BOOST_TEST_MESSAGE( "test_comparisons_involving_nan" );

    // The value NaN (Not a Number) is used to represent a value that does not represent a real number.
    // NaN's are represented by a bit pattern with an exponent of all 1s and a non-zero fraction. T
    // there are two categories of NaN: QNaN (Quiet NaN) and SNaN (Signalling NaN).
    //
    // A QNaN is a NaN with the most significant fraction bit set.
    // QNaN's propagate freely through most arithmetic operations.
    // These values pop out of an operation when the result is not mathematically defined.

    // An SNaN is a NaN with the most significant fraction bit clear.
    // It is used to signal an exception when used in operations.
    // SNaN's can be handy to assign to uninitialized variables to trap premature usage.

    // Semantically, QNaN's denote indeterminate operations, while SNaN's denote invalid operations.

    BOOST_CHECK(! is_equal( qNaN,  qNaN));
    BOOST_CHECK(! is_equal( qNaN,  0.0 ));
    BOOST_CHECK(! is_equal(-0.0 ,  qNaN));
    BOOST_CHECK(! is_equal( qNaN, -0.0 ));
    BOOST_CHECK(! is_equal( 0.0 ,  qNaN));
    BOOST_CHECK(! is_equal( qNaN,  dInf));
    BOOST_CHECK(! is_equal( dInf,  qNaN));
    BOOST_CHECK(! is_equal( qNaN,  dMax));
    BOOST_CHECK(! is_equal( dMax,  qNaN));
    BOOST_CHECK(! is_equal( qNaN, -dMax));
    BOOST_CHECK(! is_equal(-dMax,  qNaN));
    BOOST_CHECK(! is_equal( qNaN,  dMin));
    BOOST_CHECK(! is_equal( dMin,  qNaN));
    BOOST_CHECK(! is_equal( qNaN, -dMin));
    BOOST_CHECK(! is_equal(-dMin,  qNaN));

    BOOST_CHECK(! is_equal( sNaN,  sNaN));
    BOOST_CHECK(! is_equal( sNaN,  0.0 ));
    BOOST_CHECK(! is_equal(-0.0 ,  sNaN));
    BOOST_CHECK(! is_equal( sNaN, -0.0 ));
    BOOST_CHECK(! is_equal( 0.0 ,  sNaN));
    BOOST_CHECK(! is_equal( sNaN,  dInf));
    BOOST_CHECK(! is_equal( dInf,  sNaN));
    BOOST_CHECK(! is_equal( sNaN,  dMax));
    BOOST_CHECK(! is_equal( dMax,  sNaN));
    BOOST_CHECK(! is_equal( sNaN, -dMax));
    BOOST_CHECK(! is_equal(-dMax,  sNaN));
    BOOST_CHECK(! is_equal( sNaN,  dMin));
    BOOST_CHECK(! is_equal( dMin,  sNaN));
    BOOST_CHECK(! is_equal( sNaN, -dMin));
    BOOST_CHECK(! is_equal(-dMin,  sNaN));
}

BOOST_AUTO_TEST_CASE( test_comparisons_opposite_side_of_zero )
{
   BOOST_TEST_MESSAGE( "test_comparisons_opposite_side_of_zero" );

   BOOST_CHECK(! is_equal( 1.0000001, -1.0      ));
   BOOST_CHECK(! is_equal(-1.0,        1.0000001));
   BOOST_CHECK(! is_equal(-1.0000001,  1.0      ));
   BOOST_CHECK(! is_equal( 1.0,       -1.0000001));

   BOOST_CHECK(  is_equal(   10.0 * dMin,    10.0 * -dMin));
   BOOST_CHECK(  is_equal(10000   * dMin, 10000   * -dMin));
}

BOOST_AUTO_TEST_CASE( test_comparisons_very_close_to_zero )
{
   BOOST_TEST_MESSAGE( "test_comparisons_very_close_to_zero" );

   BOOST_CHECK(  is_equal( dMin, -dMin, dEps));
   BOOST_CHECK(  is_equal(-dMin,  dMin, dEps));
   BOOST_CHECK(  is_equal( dMin,  0   , dEps));
   BOOST_CHECK(  is_equal( 0,     dMin, dEps));
   BOOST_CHECK(  is_equal(-dMin,  0   , dEps));
   BOOST_CHECK(  is_equal( 0,    -dMin, dEps));


   BOOST_CHECK(  is_equal( 0.000000001, -dMin       ));
   BOOST_CHECK(  is_equal( 0.000000001,  dMin       ));
   BOOST_CHECK(  is_equal( dMin,         0.000000001));
   BOOST_CHECK(  is_equal(-dMin,         0.000000001));


   BOOST_CHECK(! is_equal( 0.000000001, -dMin,        1e-10));
   BOOST_CHECK(! is_equal( 0.000000001,  dMin,        1e-10));
   BOOST_CHECK(! is_equal( dMin,         0.000000001, 1e-10));
   BOOST_CHECK(! is_equal(-dMin,         0.000000001, 1e-10));
}

BOOST_AUTO_TEST_CASE( test_comparisons_with_denormal_numbers )
{
   BOOST_TEST_MESSAGE( "test_comparisons_with_denormal_numbers" );

   BOOST_CHECK(  is_equal( sMin, -sMin, dEps));
   BOOST_CHECK(  is_equal(-sMin,  sMin, dEps));
   BOOST_CHECK(  is_equal( sMin,  0   , dEps));
   BOOST_CHECK(  is_equal( 0,     sMin, dEps));
   BOOST_CHECK(  is_equal(-sMin,  0   , dEps));
   BOOST_CHECK(  is_equal( 0,    -sMin, dEps));

   const float lMin = dMin - sMin;  // largest denormal number
   BOOST_CHECK(  is_equal( lMin, -lMin, dEps));
   BOOST_CHECK(  is_equal(-lMin,  lMin, dEps));
   BOOST_CHECK(  is_equal( lMin,  0   , dEps));
   BOOST_CHECK(  is_equal( 0,     lMin, dEps));
   BOOST_CHECK(  is_equal(-lMin,  0   , dEps));
   BOOST_CHECK(  is_equal( 0,    -lMin, dEps));
}

BOOST_AUTO_TEST_CASE( test_comparisons_ulps )
{
   BOOST_TEST_MESSAGE( "test_comparisons_ulps" );

   BOOST_CHECK(  is_equal( dMin, -dMin, 0, 2));
   BOOST_CHECK(  is_equal(-dMin,  dMin, 0, 2));
   BOOST_CHECK(  is_equal( dMin,  0   , 0, 1));
   BOOST_CHECK(  is_equal( 0,     dMin, 0, 1));
   BOOST_CHECK(  is_equal(-dMin,  0   , 0, 1));
   BOOST_CHECK(  is_equal( 0,    -dMin, 0, 1));

   BOOST_CHECK(! is_equal( dMin, -dMin, 0, 1));
   BOOST_CHECK(! is_equal(-dMin,  dMin, 0, 1));
   BOOST_CHECK(! is_equal( dMin,  0   , 0, 0));
   BOOST_CHECK(! is_equal( 0,     dMin, 0, 0));
   BOOST_CHECK(! is_equal(-dMin,  0   , 0, 0));
   BOOST_CHECK(! is_equal( 0,    -dMin, 0, 0));
}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
