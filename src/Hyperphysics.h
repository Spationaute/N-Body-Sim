//
// Created by isatis on 7/18/17.
//

#ifndef NBODY_HYPERPHYSICS_H
#define NBODY_HYPERPHYSICS_H

#include <armadillo>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <sstream>
#include <unistd.h>
#include <utility>
#include <vector>

typedef std::pair<unsigned int, unsigned int> Colision;
typedef std::vector<std::pair<double, Colision>> ColisionLog;

class Hyperphysics {
public:
  Hyperphysics(int number) {
    nElements = number;
    position = arma::zeros(nElements, 3);
    momentum = arma::zeros(nElements, 3);
    masse = arma::zeros(nElements, 1);
    verbose = false;
    currentT = 0;

    this->dt = 1;
    for (int ii = 0; ii < nElements; ++ii) {
      std::stringstream baseName;
      baseName << "Element #" << ii;
      names.push_back(baseName.str());
    }
  }
  ~Hyperphysics() {}
  int getNElements();
  arma::Mat<double> getAllPositions();
  arma::Mat<double> getElemPosition(int e);
  arma::Mat<double> getElemMomentum(int e);
  double getElemMasse(int e);
  void printElemDesc(int e);

  void setElemName(int e, std::string &nameToSet);
  void setElemPosition(int e, arma::mat pos);
  void setElemPosition(int e, double x, double y, double z);
  void setElemPosition(int e, double pos[3]);
  void setElemMomentum(int e, arma::mat mass);
  void setElemMomentum(int e, double x, double y, double z);
  void setElemMomentum(int e, double mom[3]);
  void setElemVelocity(int e, double x, double y, double z);
  void setElemVelocity(int e, double vel[3]);

  void setElemMasse(int e, double x);
  void setDt(double t);
  void setAllMomentums(arma::mat toSet);
  void setVerbose(bool set);
  void deleteElem(int e);

  void randomPosition(double minPos[3], double maxPos[3]);
  void randomMasse(double min, double max);
  void randomVelocity(double minVel[3], double maxVel[3]);
  void step();
  void collisions(arma::Mat<int> collindingIndexes);
  void printAllPositions();
  std::string getColisionResume();
  friend std::ostream &operator<<(std::ostream &out, const Hyperphysics &phy);

private:
  int nElements;

  double width;
  double height;
  double depth;
  double dt;
  double currentT;
  std::vector<std::string> names;
  arma::Mat<double> position;
  arma::Mat<double> momentum;
  arma::Mat<double> masse;
  ColisionLog colisions;

  void _isElement(int e);
  bool verbose;
};

#endif // NBODY_HYPERPHYSICS_H
