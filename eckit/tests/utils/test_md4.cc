/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#define BOOST_TEST_MODULE test_eckit_utils

#include <iostream>

#include "ecbuild/boost_test_framework.h"

#include "eckit/utils/MD4.h"
#include "eckit/log/Timer.h"
#include "eckit/io/Buffer.h"
#include "eckit/log/Bytes.h"
#include "eckit/log/Seconds.h"

using namespace std;
using namespace eckit;


namespace eckit {
namespace test {

//----------------------------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( test_eckit_utils_md4 )

BOOST_AUTO_TEST_CASE( test_eckit_utils_md4_constructor )
{
    MD4 hash;

        const char* msg = "The quick brown fox jumps over the lazy dog";

        hash.add(msg,strlen(msg));

        std::string res ("1bee69a46ba811185c194762abaeae90");

        BOOST_CHECK_EQUAL( res , hash.digest() );
}

BOOST_AUTO_TEST_CASE( test_eckit_utils_md4_constructor_string )
{
    MD4 hash( "The quick brown fox jumps over the lazy cog" );

    std::string res ("b86e130ce7028da59e672d56ad0113df");

    BOOST_CHECK_EQUAL( res , hash.digest() );
}

// original test suite from RFC-1320

BOOST_AUTO_TEST_CASE( test_eckit_utils_md4_test_suite_from_rfc1320 )
{
    BOOST_CHECK_EQUAL( MD4("").digest(), std::string("31d6cfe0d16ae931b73c59d7e0c089c0"));
    BOOST_CHECK_EQUAL( MD4("a").digest(), std::string("bde52cb31de33e46245e05fbdbd6fb24"));
    BOOST_CHECK_EQUAL( MD4("abc").digest(), std::string("a448017aaf21d8525fc10ae87aa6729d"));
    BOOST_CHECK_EQUAL( MD4("message digest").digest(), std::string("d9130a8164549fe818874806e1c7014b"));
    BOOST_CHECK_EQUAL( MD4("abcdefghijklmnopqrstuvwxyz").digest(), std::string("d79e1c308aa5bbcdeea8ed63df412da9"));
    BOOST_CHECK_EQUAL( MD4("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").digest(), std::string("043f8582f241db351ce627e153e7f0e4"));
    BOOST_CHECK_EQUAL( MD4("12345678901234567890123456789012345678901234567890123456789012345678901234567890").digest(), std::string("e33b4ddc9c38f2199c3e7b164fcc0536"));
}

BOOST_AUTO_TEST_CASE( test_eckit_utils_md4_compute )
{
    std::string msg ( "The quick brown fox jumps over the lazy cog" );

    std::string res ("b86e130ce7028da59e672d56ad0113df");

    MD4 hash;
    BOOST_CHECK_EQUAL( res , hash.compute(msg.c_str(), msg.size()));
}

BOOST_AUTO_TEST_CASE( test_eckit_utils_md4_reset )
{
    MD4 hash( "FOOBAR" );

    hash.reset(); // reset initial state

    std::string msg ( "The quick brown fox jumps over the lazy dog" );

    hash.add(msg.c_str(), msg.size());

    std::string res ("1bee69a46ba811185c194762abaeae90");

    BOOST_CHECK_EQUAL( res , hash.digest());
}

BOOST_AUTO_TEST_SUITE_END()


//----------------------------------------------------------------------------------------------------------------------


} // end namespace test
} // end namespace eckit
