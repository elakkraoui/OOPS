/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <unistd.h>

#include "eckit/filesystem/LocalPathName.h"

#include "eckit/log/Log.h"
#include "eckit/log/Channel.h"
#include "eckit/log/CallbackTarget.h"

#include "eckit/os/BackTrace.h"

#include "eckit/runtime/Tool.h"

#include "eckit/thread/AutoLock.h"
#include "eckit/thread/StaticMutex.h"
#include "eckit/thread/Thread.h"
#include "eckit/thread/ThreadControler.h"

using namespace std;
using namespace eckit;


namespace eckit_test {

//----------------------------------------------------------------------------------------------------------------------

static StaticMutex static_mutex;

static void callback_logger( void* ctxt, const char* msg )
{
    AutoLock<StaticMutex> lock(static_mutex); ///< usually global resources like this need to be protected by mutex

    std::cout << "[TEST] -- " << msg ;
}

static void callback_special( void* ctxt, const char* msg )
{
    std::cout << ">>> " << msg ;
}

//----------------------------------------------------------------------------------------------------------------------

#define LOOPS 3
#define WAIT  10000

template< int N >
class TLog : public Thread
{
    void run()
    {
        // this shows how you can change the callback per thread
        if( N == 2 ) {
            Log::info().setCallback(&callback_special);
        }

        for( int i = 0; i < LOOPS*N; ++i )
        {
           ::usleep(N*WAIT);

           Log::info() << "thread [" << N << "] -- " << i << std::endl;
        }

        Log::info() << "thread [" << N << "] -- done !" << std::endl;
    }
};


class TestApp : public eckit::Tool {
public:

    TestApp(int argc,char **argv) : Tool(argc,argv) {}

    virtual ~TestApp() {}

    virtual void run()
    {
        Log::info().setCallback(&callback_logger);

        Log::info() << ">>> starting ... " << std::endl;

        ThreadControler t1( new TLog<1>(), false );
        ThreadControler t2( new TLog<2>(), false );
        ThreadControler t3( new TLog<3>(), false );

        t1.start();
        t2.start();
        t3.start();

        t1.wait();
        t2.wait();
        t3.wait();

        Log::info() << ">>> finished!" << std::endl;

    }
};

} // namespace eckit_test

//----------------------------------------------------------------------------------------------------------------------

using namespace eckit_test;

int main(int argc,char **argv)
{
    eckit_test::TestApp app(argc,argv);
    return app.start();
}
