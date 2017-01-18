/*
 *  test_Timer.cpp
 *
 *  Created by Pete Willemsen on 10/6/09.
 *  Copyright 2009 Department of Computer Science, University of Minnesota-Duluth. All rights reserved.
 *
 * This file is part of SIVE Lab library (libsive).
 *
 */

#include <iostream>
#include <cmath>
#include <vector>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/error_of.hpp>
#include <boost/accumulators/statistics/error_of_mean.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/min.hpp>

/* On Windows, min and max are preprocessor macros. This will cause a
   conflict with the boost accumulators, and by defining the NOMINMAX
   directive we will not invoke the macros. */
#define NOMINMAX

#include <boost/timer/timer.hpp>
#include <boost/chrono/chrono.hpp>

#include "Random.h"
#include "Timer.h"

using namespace boost::accumulators;

using boost::timer::nanosecond_type;
using boost::timer::cpu_times;
using boost::timer::cpu_timer;
using boost::timer::auto_cpu_timer;

void localSleepFunction(long stime_in_ms)
{
#ifdef WIN32
  Sleep(stime_in_ms);
#else
  usleep( stime_in_ms * 1000 );
#endif
}

using namespace sivelab;

int main(int argc, char *argv[])
{
  Random prng;
  std::vector<long> sleepTimes;

  // 
  // First timer example uses the classes I provided -- Timer
  // 

  // Sleep times in milliseconds!
  for (int i=1; i<=10; i++)
    sleepTimes.push_back( i*10 );
  for (int i=0; i<10; i++)
    sleepTimes.push_back( (int)floor(prng.uniform() * 1000) );

  // Define an accumulator set for calculating the mean, the min, and max 
  accumulator_set<double, stats<tag::mean, tag::error_of<tag::mean>, tag::max, tag::min > > acc;

  Timer t0;
  for (unsigned int i=0; i<sleepTimes.size(); i++) {
    Timer_t startTime = t0.tic();
    localSleepFunction( sleepTimes[i] );
    Timer_t endTime = t0.tic();
    Timer_t diffTime = t0.deltam(startTime, endTime);
    std::cout << "Low-res timer result after sleeping for " << sleepTimes[i] << " milliseconds --> result = " << diffTime << " ms [diff = " << diffTime - sleepTimes[i] << "]" << std::endl;
    
    acc( diffTime - sleepTimes[i] );
  }

  std::cout << "Mean difference between specified microsecond sleep and measured time: " << mean(acc) 
	    << " ms, error of mean: " << error_of<tag::mean>(acc) 
	    << ", range of differences: [" << min(acc) << ", " << max(acc) << "]" << std::endl << std::endl;


  // Define an accumulator set for calculating the mean, the min, and max 
  accumulator_set<double, stats<tag::mean, tag::error_of<tag::mean>, tag::max, tag::min > > acc1;

  Timer t1(true);
  for (unsigned int i=0; i<sleepTimes.size(); i++)
    {
      Timer_t startTime = t1.tic();
      localSleepFunction( sleepTimes[i] );
      Timer_t endTime = t1.tic();
      Timer_t diffTime = t1.deltam(startTime, endTime);
      std::cout << "High-res timer result after sleeping for " << sleepTimes[i] << " milliseconds --> result = " << diffTime << " ms [diff = " << diffTime - sleepTimes[i] << "]" << std::endl;

      acc1( diffTime - sleepTimes[i] );
    }

  std::cout << "Mean difference between specified microsecond sleep and measured time: " << mean(acc1) 
	    << " ms, error of mean: " << error_of<tag::mean>(acc1) 
	    << ", range of differences: [" << min(acc1) << ", " << max(acc1) << "]" << std::endl << std::endl;



  // //////////////// //////////////// //////////////// ////////////////
  // 
  // Second timer example uses boost's cpu_timer classes - currently
  // only uses the wall time for comparison
  // 
  // //////////////// //////////////// //////////////// ////////////////
  // Define an accumulator set for calculating the mean, the min, and max 
  accumulator_set<double, stats<tag::mean, tag::error_of<tag::mean>, tag::max, tag::min > > acc2;

  cpu_times timeVal;
  cpu_timer bTimer;
  for (unsigned int i=0; i<sleepTimes.size(); i++) {

    bTimer.start();
    localSleepFunction( sleepTimes[i] );
    timeVal.wall = bTimer.elapsed().wall;  // in nanoseconds

    double diffTime = (timeVal.wall/1000000.0) - sleepTimes[i];   // in ms
    std::cout << "Boost cpu_timer result after sleeping for " << sleepTimes[i] << " milliseconds --> result = " << timeVal.wall / 1000000.0 << " ms [diff = " << diffTime << " ms]" << std::endl;

    acc2( diffTime );
  }
  
  std::cout << "Mean difference between specified millisecond sleep and measured time: " << mean(acc2) 
	    << " ms, error of mean: " << error_of<tag::mean>(acc2) 
	    << ", range of differences: [" << min(acc2) << ", " << max(acc2) << "]" << std::endl << std::endl;


  // Define an accumulator set for calculating the mean, the min, and max 
  accumulator_set<double, stats<tag::mean > > finalAcc;
  finalAcc( mean(acc) );
  finalAcc( mean(acc1) );
  finalAcc( mean(acc2) );
  std::cout << "******************\nThus, on average, on this machine architecture and OS, the millsecond sleep timer sleeps " << mean(finalAcc) << " ms more than specified!\n******************" << std::endl;
}
