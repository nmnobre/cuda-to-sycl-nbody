// Copyright (C) 2016 - 2018 Sarah Le Luron
// Copyright (C) 2022 Codeplay Software Limited
 
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <algorithm>

#include "sim_param.hpp"
#include "simulator.dp.hpp"


using namespace std;
using namespace simulation;

int main(int argc, char **argv) {

   SimParam params;
   params.parseArgs(argc, argv);

   DiskGalaxySimulator nbodySim(params);

   std::vector<float> stepTimes;
   int step{0};

   // Main loop
   float stepTime = 0.0;
   while(step < params.numFrames){

      nbodySim.stepSim();
      if(!(step % 20)) stepTime = nbodySim.getLastStepTime();

      step++;
      int warmSteps{2};
      if (step > warmSteps) {
         stepTimes.push_back(nbodySim.getLastStepTime());
         float cumStepTime =
             std::accumulate(stepTimes.begin(), stepTimes.end(), 0.0);
         float meanTime = cumStepTime / stepTimes.size();
         float accum{0.0};
         std::for_each(stepTimes.begin(), stepTimes.end(),
                       [&](const float time) {
                          accum += std::pow((time - meanTime), 2);
                       });
         float stdDev = std::pow(accum / stepTimes.size(), 0.5);
         std::cout << "At step " << step << " kernel time is "
                   << stepTimes.back() << " and mean is " << meanTime
                   << " and stddev is: " << stdDev << "\n";
      }
   }

   return 0;
}
