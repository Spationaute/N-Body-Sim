#include <iostream>
#include <fstream>
#include <armadillo>
#include <omp.h>
#include "Hyperphysics.h"

int main() {
    //Setup
    double simWidth =  300;
    double simHeight = 300;
    double simDepth  = 50;
    int nElem    = 3000;
    int step     = 20000;

    Hyperphysics physics(nElem, simWidth, simHeight,simDepth);
    physics.randomPosition();
    physics.randomMasse();
    physics.setElemPosition(0,arma::mat {{simWidth/2,simHeight/2,simDepth/2}});
    physics.setElemMasse(0,1e9);
    //Print information about the simulation
    std::cout << "Simulation READY for " << omp_get_max_threads() << " processors "<<std::endl;
    std::cout << nElem << " initialized" << std::endl;
    char filePath[255];

    //Simulation Loop
    for (int i = 1; i < step ; ++i) {
        snprintf(filePath,255,"data.csv.%i",i);
        physics.step();
	if(i%10==1){
        	std::ofstream outFile(filePath);
		outFile<< "x, y, z, ux, uy, uz, masse, radius" <<std::endl;
        	outFile<< physics ;
        	outFile.close();
	}
        std::cout << "Time: " << i << std::endl;
    }
    return 0;
}
