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

#include "ecbuild/boost_test_framework.h"

#include "eckit/types/Types.h"
#include "eckit/utils/RendezvousHash.h"
#include "eckit/utils/Translator.h"

#include "eckit/testing/Setup.h"

using namespace std;
using namespace eckit;
using namespace eckit::testing;

namespace eckit {
namespace test {

//----------------------------------------------------------------------------------------------------------------------

BOOST_GLOBAL_FIXTURE( Setup );

BOOST_AUTO_TEST_SUITE( test_eckit_rendezvous_hash )

BOOST_AUTO_TEST_CASE( test_eckit_utils_rendezvous_hash_constructor )
{
    BOOST_TEST_MESSAGE( Here() );

    std::set<std::string> nodes;

    nodes.insert("node01");
    nodes.insert("node02");
    nodes.insert("node03");
    nodes.insert("node04");

    eckit::RendezvousHash rendezvous(nodes, &RendezvousHash::md5);

    std::map<std::string, std::string> dict;

    dict["class"]  = "od";
    dict["stream"] = "oper";
    dict["type"]   = "fc";
    dict["level"]  = "1";

    std::string n = rendezvous.selectNode(dict);

    BOOST_TEST_MESSAGE( n );

    BOOST_CHECK_EQUAL( nodes.count(n), 1 );
}

BOOST_AUTO_TEST_CASE( test_eckit_utils_rendezvous_hash_distribution )
{
    BOOST_TEST_MESSAGE( Here() );

    eckit::Translator<size_t,std::string> toStr;

    std::vector<std::string> params;

    params.push_back( "2t" );
    params.push_back( "2d" );
    params.push_back( "z" );
    params.push_back( "u" );
    params.push_back( "v" );
    params.push_back( "130.128" );
    params.push_back( "138.128" );


    std::set<std::string> nodes;

    nodes.insert("node01");
    nodes.insert("node02");
    nodes.insert("node03");
    nodes.insert("node04");
    nodes.insert("node05");
    nodes.insert("node06");
    nodes.insert("node07");

    eckit::RendezvousHash rendezvous(nodes);

    std::map<std::string, std::string> dict;
    dict["class"]  = "od";
    dict["stream"] = "oper";
    dict["type"]   = "fc";

    std::map<std::string, size_t> counts;


    for(std::vector<std::string>::iterator param = params.begin(); param != params.end(); ++param ) {

        dict["param"] = *param;

        for(size_t step = 0; step < 240; ++step) {

            dict["step"]  = toStr(step);

            for( size_t level = 0; level < 10; ++level ) {

                dict["level"]  = toStr(level);

                counts[ rendezvous.selectNode(dict) ]++;

            }
        }

    }

    BOOST_TEST_MESSAGE( counts );
}

BOOST_AUTO_TEST_CASE( test_eckit_utils_rendezvous_hash_empty_dict )
{
    BOOST_TEST_MESSAGE( Here() );

    std::map<std::string, std::string> dict;

    eckit::RendezvousHash rendezvous;
    rendezvous.addNode("node01");
    rendezvous.addNode("node02");

    BOOST_CHECK_NO_THROW(rendezvous.selectNode(dict)); /* don't throw on empty dictionary */
}

BOOST_AUTO_TEST_CASE( test_eckit_utils_rendezvous_hash_throws_empty_node_list )
{
    BOOST_TEST_MESSAGE( Here() );

    std::map<std::string, std::string> dict;

    eckit::RendezvousHash rendezvous(&RendezvousHash::md5);
    dict["class"]  = "od";
    dict["stream"] = "oper";
    dict["type"]   = "fc";

    BOOST_CHECK_THROW(rendezvous.selectNode(dict), eckit::BadParameter); /* throw on empty node list */

    rendezvous.addNode("node01");
    rendezvous.addNode("node02");

    BOOST_CHECK_NO_THROW(rendezvous.selectNode(dict));
}

BOOST_AUTO_TEST_CASE( test_eckit_utils_rendezvous_hash_add_node )
{
    BOOST_TEST_MESSAGE( Here() );

    eckit::Translator<size_t,std::string> toStr;

    std::vector<std::string> params;
    params.push_back( "2t" );
    params.push_back( "2d" );
    params.push_back( "z" );
    params.push_back( "w" );

    std::map<std::string, std::string> dict;
    dict["class"]  = "od";
    dict["stream"] = "oper";
    dict["type"]   = "fc";

    eckit::RendezvousHash rendezvous;

    rendezvous.addNode("node01");
    rendezvous.addNode("node02");


    const size_t nsteps  = 25;
    const size_t nlevels = 100;


    std::map<std::string, size_t> counts;

    for(std::vector<std::string>::iterator param = params.begin(); param != params.end(); ++param ) {

        dict["param"] = *param;

        for(size_t step = 0; step < nsteps; ++step) {

            dict["step"]  = toStr(step);

            for( size_t level = 0; level < nlevels; ++level ) {

                dict["level"]  = toStr(level);

                counts[ rendezvous.selectNode(dict) ]++;

            }
        }
    }

    BOOST_TEST_MESSAGE( counts );

    std::map<std::string, size_t> counts2;

    rendezvous.addNode("node03");
    rendezvous.addNode("node04");

    for(std::vector<std::string>::iterator param = params.begin(); param != params.end(); ++param ) {

        dict["param"] = *param;

        for(size_t step = 0; step < nsteps; ++step) {

            dict["step"]  = toStr(step);

            for( size_t level = 0; level < nlevels; ++level ) {

                dict["level"]  = toStr(level);

                counts2[ rendezvous.selectNode(dict) ]++;

            }
        }
    }

    BOOST_TEST_MESSAGE( counts2 );
}

BOOST_AUTO_TEST_SUITE_END()

//----------------------------------------------------------------------------------------------------------------------

} // namespace test
} // namespace eckit

