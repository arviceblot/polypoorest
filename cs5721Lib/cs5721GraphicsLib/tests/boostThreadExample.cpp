#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

// Needed for the boost system
#include <boost/thread.hpp>

#ifdef WIN32
#include <sys/types.h>
#include <fcntl.h>
#include <windows.h>
#include <winbase.h>
#endif

#include "Random.h"

void localSleepFunction(long stime_in_ms)
{
#ifdef WIN32
  Sleep(stime_in_ms);
#else
  usleep( stime_in_ms * 1000 );
#endif
}

const int gbl_numThreads = 10;
const int gbl_N = 1000;

class threadData
{
public:
  threadData() 
    : counter(0) 
  {
  }

  int counter;
  boost::mutex local_mutex;
};

class WorkerThread
{
public:
  
  WorkerThread(threadData *td)
    : m_threadData(td)
  {
  }

  void operator()()  
  {  
    sivelab::Random prng;
    for (int li=0; li<gbl_N; li++) {

      // wait a random amount in milliseconds so the timers on
      // different systems work ok
      //
      // PRNG is in [0, 1] so scale that by 10 to wait anywhere
      // from a small number of ms to nearly 10 ms.
      localSleepFunction( (int)(prng.uniform() * 10) );
      
      m_threadData->local_mutex.lock();
      m_threadData->counter++;
      m_threadData->local_mutex.unlock();
    }
    
    // exit the thread
    std::cout << "Exiting thread" << std::endl;
  
    // The return of this function via "return 0" is identical to
    // calling a pthread_exit call and the return value is used as the
    // exit status, so return 0 is identical to:
    // pthread_exit(NULL);  
  }
  
private:
  threadData *m_threadData;
};


int main( int argc, char *argv[] ) 
{
  // Allocate one copy of the data that will be shared between all the
  // threads.
  threadData *data = new threadData;

  // Create threads and store their IDs in the pthread_t array below.
  int numThreads = gbl_numThreads;
  std::vector<boost::thread*> threads(numThreads);

  for (int threadNum=0; threadNum < numThreads; threadNum++) {
    WorkerThread wt(data);
    threads[threadNum] = new boost::thread(wt);
    std::cout << "Starting thread, number=" << threads[threadNum]->get_id() << std::endl;
  }
  
  // The main trunk of the application should wait for each thread to
  // complete before exiting, else all threads be lost.  The
  // join function allows this to occur.
  for (int threadNum=0; threadNum < numThreads; threadNum++) {
    threads[threadNum]->join();
  }

  int totalCount = gbl_N * gbl_numThreads;
  if (data->counter == totalCount)
    std::cout << "SUCCESS!  Final data counter = " << data->counter << std::endl;
  else 
    std::cout << "FAIL! Data counter = " << data->counter << ", but should have been " << totalCount << std::endl;

  return 0;
}
