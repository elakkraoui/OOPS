#include "fckit/Main.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/thread/Mutex.h"
#include "eckit/thread/Once.h"

// Temporary until ECKIT-166 is fixed, only included for MacOSX
#ifdef BUG_ECKIT_166
#include <mpi.h>
#endif

static eckit::Once<eckit::Mutex> local_mutex;

namespace fckit{

Main::Main(
    int argc, char **argv,
    const char* homeenv)
    : eckit::Main(argc,argv,homeenv)
{
}

void Main::initialise(
    int argc, char** argv,
    const char* homeenv)
{
    eckit::AutoLock<eckit::Mutex> lock(local_mutex);
    if( not ready() ) {
        new Main(argc,argv,homeenv);
    }
}

void Main::finalise()
{
// Temporary until ECKIT-166 is fixed, only included for MacOSX
#ifdef BUG_ECKIT_166
    bool using_mpi = (::getenv("OMPI_COMM_WORLD_SIZE") || ::getenv("ALPS_APP_PE"));
    if( using_mpi ) {
      int mpi_initialized = 1;
      MPI_Initialized(&mpi_initialized);
      int mpi_finalized = 1;
      MPI_Finalized(&mpi_finalized);
      if( not mpi_finalized and mpi_initialized ) {
        MPI_Finalize();
      }
    }
#endif
}

} // namespace fckit

