//
// Created by isatis on 7/18/17.
//

#include "Hyperphysics.h"
arma::Mat<double> Hyperphysics::getElemPosition(int e){
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

void Hyperphysics::printAllPositions() {
	std::cout << position << std::endl;
}

int Hyperphysics::getNElements() {
	return nElements;
}

void Hyperphysics::setElemPosition(int e,arma::mat pos) {
	_isElement(e);
	position.row(e) = pos;
}

void Hyperphysics::setElemMomentum(int e, arma::mat mom) {
	_isElement(e);
	momentum.row(e) = mom;
}

void Hyperphysics::setElemMasse(int e, double x) {
	_isElement(e);
	masse(e)=x;
}
void Hyperphysics::deleteElem(int e) {
	_isElement(e);
	position.shed_row(e);
	momentum.shed_row(e);
	masse.shed_row(e);
	--nElements;
}
void Hyperphysics::randomPosition() {
	position =  position.randu() * arma::Mat<double>({{width,0,0},{0,height,0},{0,0,depth}});
}

void Hyperphysics::randomMasse() {
	masse = 1e6 + masse.randu() * 1e6;
}

void Hyperphysics::step(){
	double G = -6.67408e-11;

	//Should use armadillo a bit more
	arma::Mat<int> collide;
	std::cout << "    Force Step" << std::endl;
	std::cout << "    Number of elements: " << nElements << std::endl;
	#pragma omp parallel for
	for(int ii = 0; ii < nElements;++ii){
		for(int jj=ii+1;jj<nElements;++jj){
			if (arma::norm(position.row(ii) - position.row(jj)) > 1) {
				double norm = arma::norm(position.row(jj) - position.row(ii));
				double force = -G * masse(ii) * masse(jj) / pow(norm, 3);
				momentum.row(ii) +=10*(position.row(jj) - position.row(ii)) / norm *force;
				momentum.row(jj) -=10*(position.row(jj) - position.row(ii)) / norm *force;
			}else{
				if(collide.is_empty()){
					collide = {{ii,jj}};
				}else{
					arma::Mat<int> collided ={{ii,jj}};
					bool isNew = true;
					for(int ii=0;ii<collide.n_rows;++ii){
						if (collided(0) == collide(ii,1)){
							isNew = false;
						}
					}
				if(isNew){
					collide = arma::join_cols(collide,collided);
				}
				}
			}
		}
	}
	

	//Update positions 
	position+=momentum/arma::repmat(masse,1,3);

	//Collision step
	std::cout << "    Collisions Step"<<std::endl;
	collide = arma::sort(collide,"descend");
	arma::Col<int> toDelete;
	for(int ii = 0; ii < collide.n_rows;++ii){
		int f = collide(ii,0);
		int s = collide(ii,1);
		std::cout << "        Collision: " << f << " - " << s << std::endl;
		masse.row(s) += masse.row(f);
		momentum.row(s) += momentum.row(f);
		position.row(s) =  masse(f) > masse(s) ? position.row(f) : position.row(s);
		if(toDelete.is_empty()){
			toDelete = {f};
		}else{
			toDelete = arma::join_cols(toDelete, arma::Col<int>({f}));
		}
	}

	std::cout << "    Cleaning Step"<<std::endl;
	for(int ii = 0; ii<toDelete.n_rows;++ii){
		deleteElem(toDelete(ii));
	}
}

arma::Mat<double> Hyperphysics::getAllPositions() {
	return position;
}

std::ostream& operator<<(std::ostream &out,const Hyperphysics &phy){
	int n=phy.nElements;
	for(int ii=0;ii<n;++ii){
		out<<phy.position(ii,0) <<", ";
		out<<phy.position(ii,1) <<", ";
		out<<phy.position(ii,2) <<", ";
		out<<phy.momentum(ii,0) <<", ";
		out<<phy.momentum(ii,1) <<", ";
		out<<phy.momentum(ii,2) <<", ";
		out<<phy.masse(ii) << ", ";
		out<< pow(phy.masse(ii)/250/3.1416,0.3333) << std::endl;
	}
	return out;
}

void Hyperphysics::setAllMomentums(arma::mat toSet){
	if( momentum.n_rows == toSet.n_rows && momentum.n_cols == toSet.n_cols){
		momentum = toSet;
	}else{
		std::cout << "Cannot set Momentum, array not the same size" << std::endl;
	}
}

void Hyperphysics::_isElement(int e) {
	if(e<0 || e>nElements){
		std::cout << "ERROR: Position asked is not valid!" << std::endl;
		std::cout << "       Size: "<< nElements <<" Asked: " << e << std::endl;
		exit(1);
	}
}
