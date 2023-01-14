//
// Created by isatis on 7/18/17.
//

#ifndef NBODY_HYPERPHYSICS_H
#define NBODY_HYPERPHYSICS_H

#include <vector>
#include <iostream>
#include <armadillo>
#include <unistd.h>
#include <omp.h>
#include <math.h>

class Hyperphysics {
public:
    Hyperphysics(int number,double width,double height,double depth){
        nElements=number;
        position = arma::zeros(nElements,3);
        momentum = arma::zeros(nElements,3);
        masse    = arma::zeros(nElements,1);
        this->width= width;
        this->height=height;
        this->depth = depth;
    }
    ~Hyperphysics(){

    }
    int               getNElements();
    arma::Mat<double> getAllPositions();
    arma::Mat<double> getElemPosition(int e);
    arma::Mat<double> getElemMomentum(int e);
    double            getElemMasse(int e);

    void setElemPosition(int e, arma::mat pos);
    void setElemMomentum(int e, arma::mat mass);
    void setElemMasse(int e, double x);
    void setAllMomentums(arma::mat toSet);

    void deleteElem(int e);

    void randomPosition();
    void randomMasse();

    void step();
    void printAllPositions();
    friend std::ostream& operator<<(std::ostream &out,const Hyperphysics &phy);


private:
    int nElements;
    double width;
    double height;
    double depth;
    arma::Mat<double> position;
    arma::Mat<double> momentum;
    arma::Mat<double> masse;

    void _isElement(int e);
};


#endif //NBODY_HYPERPHYSICS_H
