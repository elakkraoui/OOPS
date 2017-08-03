/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/filesystem/LocalPathName.h"
#include "eckit/log/Log.h"
#include "eckit/os/BackTrace.h"
#include "eckit/runtime/Main.h"

using namespace std;
using namespace eckit;

namespace eckit_test {

struct CTxt { std::string name_; };

//----------------------------------------------------------------------------------------------------------------------


static void output_callback_noctxt( void* ctxt, const char* msg )
{
//    std::cout << "[DEBUG] -- " << Here() << "\n" << BackTrace::dump() << "\n" << std::endl;
    std::cout << "[FORWARD OUT] -- " << msg ;
}


static void output_callback_withctxt( void* ctxt, const char* msg )
{
//    std::cout << "[DEBUG] -- " << Here() << "\n" << BackTrace::dump() << "\n" << std::endl;
    std::cout << "[FORWARD OUT] -- CTXT [" << static_cast<CTxt*>(ctxt)->name_ << "] -- " << msg ;
}



/// tests without callback
void test_callback_none()
{
    Log::info().clear();

    Log::info()          << "info message 1" << std::endl;

    Log::warning()       << "warning message 1" << std::endl;

    Log::error()         << "error message 1" << std::endl;
}



/// tests with null context
void test_callback_noctxt()
{
    Log::info().setCallback(&output_callback_noctxt);

    Log::info()          << "info message 1" << std::endl;

    Log::warning()       << "warning message 1" << std::endl;

    Log::error()         << "error message 1" << std::endl;

    Log::info().clear();
}


/// tests with context
void test_callback_withctxt()
{
    CTxt ctxt;
    ctxt.name_ = "MyTest";

    Log::info().setCallback(&output_callback_withctxt, &ctxt);

    Log::info()          << "info message 1" << std::endl;

    Log::warning()       << "warning message 1" << std::endl;

    Log::error()         << "error message 1" << std::endl;

    Log::info().clear();
}

} // namespace eckit_test

//----------------------------------------------------------------------------------------------------------------------

using namespace eckit_test;

int main(int argc, char **argv)
{


    test_callback_none();
    test_callback_noctxt();
    test_callback_withctxt();

    return 0;
}
