/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#define BOOST_TEST_MODULE test_eckit_types

#include "ecbuild/boost_test_framework.h"

#include "eckit/types/Types.h"

#include "eckit/log/Log.h"


using namespace std;
using namespace eckit;

// ------------------------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE( test_eckit_print_vector )

BOOST_AUTO_TEST_CASE( test_eckit_print_vector_string )
{
    vector<string> vstr;
    vstr.push_back("Hello");
    vstr.push_back("World");
    vstr.push_back("test");
    vstr.push_back("case");

    stringstream s;
    s << vstr;

    BOOST_CHECK_EQUAL("[Hello,World,test,case]", s.str());
}

BOOST_AUTO_TEST_CASE( test_eckit_print_vector_pair )
{
    vector<pair<string, int> > vpair;
    vpair.push_back(make_pair("k1", 123));
    vpair.push_back(make_pair("k1", 124));
    vpair.push_back(make_pair("k1", 125));
    vpair.push_back(make_pair("k2", 125));

    stringstream s;
    s << vpair;

    BOOST_CHECK_EQUAL("[<k1,123>,<k1,124>,<k1,125>,<k2,125>]", s.str());
}

BOOST_AUTO_TEST_CASE( test_eckit_print_vector_ints )
{
    vector<int> vint;
    vint.push_back(123);
    vint.push_back(124);
    vint.push_back(125);
    vint.push_back(126);
    vint.push_back(127);
    vint.push_back(129);
    vint.push_back(131);
    vint.push_back(133);
    vint.push_back(135);
    vint.push_back(135);
    vint.push_back(135);
    vint.push_back(135);
    vint.push_back(1);

    stringstream s;
    s << vint;

    BOOST_CHECK_EQUAL("[123-127,129-135-2,3*135,1]", s.str());
}

BOOST_AUTO_TEST_CASE( test_eckit_print_vector_doubs )
{
    // These should not contract into ranges.
    vector<double> vdoub;
    vdoub.push_back(123.0);
    vdoub.push_back(124.0);
    vdoub.push_back(125.0);
    vdoub.push_back(126.0);
    vdoub.push_back(127.0);
    vdoub.push_back(129.0);
    vdoub.push_back(131.0);
    vdoub.push_back(133.0);
    vdoub.push_back(135.0);
    vdoub.push_back(135.0);
    vdoub.push_back(135.0);
    vdoub.push_back(135.0);
    vdoub.push_back(1.0);

    stringstream s;
    s << vdoub;

    BOOST_CHECK_EQUAL("[123,124,125,126,127,129,131,133,135,135,135,135,1]", s.str());
}

BOOST_AUTO_TEST_SUITE_END()

// ------------------------------------------------------------------------------------------------
