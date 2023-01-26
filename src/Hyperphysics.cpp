//
// Created by isatis on 7/18/17.
//

#include "Hyperphysics.h"
arma::Mat<double> Hyperphysics::getElemPosition(int e) {
  _isElement(e);
  return position.row(e);
}

double Hyperphysics::getElemMasse(int e) {
  _isElement(e);
  return masse(e);
}

arma::Mat<double> Hyperphysics::getElemMomentum(int e) {
  _isElement(e);
  return momentum.row(e);
}

void Hyperphysics::printAllPositions() { std::cout << position << std::endl; }

int Hyperphysics::getNElements() { return nElements; }

void Hyperphysics::setElemPosition(int e, arma::mat pos) {
  _isElement(e);
  position.row(e) = pos;
}

void Hyperphysics::setElemPosition(int e, double x, double y, double z) {
  _isElement(e);
  position.row(e) = {x, y, z};
}

void Hyperphysics::setVerbose(bool set) { verbose = set; }

void Hyperphysics::setElemMomentum(int e, arma::mat mom) {
  _isElement(e);
  momentum.row(e) = mom;
}

void Hyperphysics::setElemMomentum(int e, double x, double y, double z) {
  _isElement(e);
  momentum.row(e) = {x, y, z};
}

void Hyperphysics::setElemName(int e, std::string &nameToSet) {
  _isElement(e);
  names[e] = nameToSet;
}
void Hyperphysics::setElemMasse(int e, double x) {
  _isElement(e);
  masse(e) = x;
}

void Hyperphysics::setDt(double t) { this->dt = t; }

void Hyperphysics::deleteElem(int e) {
  _isElement(e);
  position.shed_row(e);
  momentum.shed_row(e);
  masse.shed_row(e);
  --nElements;
}
void Hyperphysics::randomPosition(double minPos[3], double maxPos[3]) {
  arma::mat delta = {{abs(maxPos[0] - minPos[0]), 0, 0},
                     {0, abs(maxPos[1] - minPos[1]), 0},
                     {0, 0, abs(maxPos[2] - minPos[2])}};
  arma::mat base = arma::repmat(arma::rowvec({minPos[0], minPos[1], minPos[2]}),this->nElements, 1);
  this->position = position.randu() * delta + base;
}
void Hyperphysics::randomVelocity(double minVel[3], double maxVel[3]) {
    arma::mat delta = {{abs(maxVel[0] - minVel[0]), 0, 0},
                       {0, abs(maxVel[1] - minVel[1]), 0},
                       {0, 0, abs(maxVel[2] - minVel[2])}};
    arma::mat base = arma::repmat(arma::rowvec({minVel[0], minVel[1], minVel[2]}),this->nElements, 1);
    this->momentum = momentum.randu() * delta + base;
}

void Hyperphysics::randomMasse(double min, double max) {
  this->masse = masse.randu() * abs(max - min) + min;
}

void Hyperphysics::step() {
  double G = -6.67408e-11;
  currentT += dt;
  // Should use armadillo a bit more
  arma::Mat<int> collide;
  if (verbose) {
    std::cout << "    Force Step" << std::endl;
    std::cout << "    Number of elements: " << nElements << std::endl;
  }
#pragma omp parallel for
  for (int ii = 0; ii < nElements; ++ii) {
    for (int jj = ii + 1; jj < nElements; ++jj) {
        double radOne = pow(masse(ii) / 250 / 3.1416, 0.3333);
        double radTwo = pow(masse(jj) / 250 / 3.1416, 0.3333);
        if (arma::norm(position.row(ii) - position.row(jj)) > radOne+radTwo) {
        double norm = arma::norm(position.row(jj) - position.row(ii));
        double force = dt * (-G * masse(ii) * masse(jj) / pow(norm, 2));
        momentum.row(ii) +=
            force * (position.row(jj) - position.row(ii)) / norm;
        momentum.row(jj) -=
            force * (position.row(jj) - position.row(ii)) / norm;
      } else {
        if (collide.is_empty()) {
          collide = {{ii, jj}};
        } else {
          arma::Mat<int> collided = {{ii, jj}};
          bool isNew = true;
          for (int ii = 0; ii < collide.n_rows; ++ii) {
            if (collided(0) == collide(ii, 1)) {
              isNew = false;
            }
          }
          if (isNew) {
            collide = arma::join_cols(collide, collided);
          }
        }
      }
    }
  }

  // Update positions
  position += dt * momentum / arma::repmat(masse, 1, 3);

  // Collisions Step
  collisions(collide);
}

void Hyperphysics::collisions(arma::Mat<int> collindingIndexes) {
  if (verbose) {
    std::cout << "    Collisions Step" << std::endl;
  }

  collindingIndexes = arma::sort(collindingIndexes, "descend");
  arma::Col<int> toDelete;

  for (int ii = 0; ii < collindingIndexes.n_rows; ++ii) {
    int f = collindingIndexes(ii, 0);
    int s = collindingIndexes(ii, 1);

    if (verbose) {
      std::cout << "        Collision: " << f << " - " << s << std::endl;
    }

    colisions.push_back({currentT, {f, s}});
    masse.row(s) += masse.row(f);
    momentum.row(s) += momentum.row(f);
    position.row(s) = masse(f) > masse(s) ? position.row(f) : position.row(s);

    if (toDelete.is_empty()) {
      toDelete = {f};
    } else {
      toDelete = arma::join_cols(toDelete, arma::Col<int>({f}));
    }
  }

  if (verbose) {
    std::cout << "    Cleaning Step" << std::endl;
  }

  for (int ii = 0; ii < toDelete.n_rows; ++ii) {
    deleteElem(toDelete(ii));
  }
}

arma::Mat<double> Hyperphysics::getAllPositions() { return position; }

std::ostream &operator<<(std::ostream &out, const Hyperphysics &phy) {
  int n = phy.nElements;
  for (int ii = 0; ii < n; ++ii) {
    out << phy.position(ii, 0) << ", ";
    out << phy.position(ii, 1) << ", ";
    out << phy.position(ii, 2) << ", ";
    out << phy.momentum(ii, 0) / phy.masse(ii) << ", ";
    out << phy.momentum(ii, 1) / phy.masse(ii) << ", ";
    out << phy.momentum(ii, 2) / phy.masse(ii) << ", ";
    out << phy.masse(ii) << ", ";
    out << pow(phy.masse(ii) / 250 / 3.1416, 0.3333) << ", ";
    out << phy.currentT << ", " << phy.names[ii] << std::endl;
  }
  return out;
}

void Hyperphysics::setAllMomentums(arma::mat toSet) {
  if (momentum.n_rows == toSet.n_rows && momentum.n_cols == toSet.n_cols) {
    momentum = toSet;
  } else {
    std::cout << "Cannot set Momentum, array not the same size" << std::endl;
  }
}

void Hyperphysics::_isElement(int e) {
  if (e < 0 || e > nElements) {
    std::cout << "ERROR: Position asked is not valid!" << std::endl;
    std::cout << "       Size: " << nElements << " Asked: " << e << std::endl;
    exit(1);
  }
}

std::string Hyperphysics::getColisionResume() {
  std::stringstream ss;
  std::for_each(colisions.begin(), colisions.end(),
                [&ss](const std::pair<double, Colision> &p) {
                  ss << p.first << ", " << p.second.first << ", "
                     << p.second.second << std::endl;
                });
  return ss.str();
}