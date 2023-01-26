#include "Hyperphysics.h"
#include "Settings.h"
#include <algorithm>
#include <armadillo>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <vector>

int main(int argn, char *argv[]) {
  if (argn < 2) {
    std::cout << "Usage: " << argv[0] << " <case.json>" << std::endl;
    return 1;
  }

  std::string caseFile = argv[1];

  // Setup
  Settings settings;
  settings.load(caseFile.c_str());
  std::string outPath = settings.getOutPath();
  double simDeltaT = settings.getDeltaT();

  int nElem = settings.getNElem();
  int step = settings.getSteps();
  int writeSteps = settings.getWriteSteps();

  RandomSettings rs = settings.getRandomSettings();
  if (rs.active)
    nElem += rs.nElements;

  Hyperphysics physics(nElem);
  physics.setDt(simDeltaT);

  if (settings.getVerbose())
    physics.setVerbose(true);

  if (rs.active) {
    physics.randomPosition(rs.minPosition, rs.maxPosition);
    physics.randomMasse(rs.minMass, rs.maxMass);
    physics.randomVelocity(rs.minVelocity, rs.maxVelocity);
  }
  int ii = 0;
  std::vector<Element> elementList = settings.getElements();
  std::for_each(elementList.begin(), elementList.end(), [&](Element &e) {
    physics.setElemPosition(ii, e.position[0], e.position[1], e.position[2]);
    physics.setElemMomentum(ii, e.velocity[0], e.velocity[1], e.velocity[2]);
    physics.setElemName(ii, e.name);
    physics.setElemMasse(ii++, e.mass);
  });

  // Print information about the simulation
  std::cout << "Simulation READY for " << omp_get_max_threads()
            << " processors " << std::endl;
  std::cout << nElem << " initialized" << std::endl;

  // Check if folder exists and create it
  // if it doesn't exist.
  std::filesystem::path outPathP(outPath);

  if (!std::filesystem::exists(outPathP)) {
    std::filesystem::create_directories(outPathP);
  }

  // Simulation Loop
  char filePath[255];
  char colFilePath[255];
  for (int i = 0; i < step; ++i) {
    snprintf(filePath, 255, "%s/data.%i.csv", outPath.c_str(), i);
    snprintf(colFilePath, 255, "%s/col.csv", outPath.c_str());
    physics.step();
    if (i % writeSteps == 0) {
      std::cout << "Time: " << i * simDeltaT << std::endl;
      std::ofstream outFile(filePath);
      if (!outFile.is_open()) {
        std::cout << "Cannot open file: " << filePath << std::endl;
        return 1;
      }
      outFile << "x, y, z, ux, uy, uz, masse, radius, time, name" << std::endl;
      outFile << physics;
      outFile.close();

      std::ofstream colFile(colFilePath);
      if (!colFile.is_open()) {
        std::cout << "Cannot open file: " << colFilePath << std::endl;
        return 1;
      }
        colFile << "Time, Element #1, Element #2" << std::endl;
        colFile << physics.getColisionResume();
        colFile.close();
    }
  }
  return 0;
}
