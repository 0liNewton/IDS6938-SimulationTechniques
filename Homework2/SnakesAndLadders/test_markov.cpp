#include <iostream>
#include <functional>   
#include <numeric> 
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <Eigen\dense>

#include "markov.h"
#include "TransitionMatrix.h"

int main(){

	SetTransitionMatrix();

	//Output Vector
	v.setZero();
	v(0) = 1.0;

	// Print Results to File
	std::ofstream myfile;
	std::vector<double> markovresult;
	myfile.open("markov_results.txt");

   // TODO add Markov vector - Matrix multiplication

	for (int j = 0; j < ; j++) {
		for (int i = 0; i < ; i++) {
			//std::vector<double> v;
			//std::cout <<  v << std::endl;
		v = v.transpose() * TransitionMatrix;
		
	}

	//std::vector<double> v;

	//myfile << v << std::endl;  //this is just a sample, becareful how you print to file so you can mine useful stats

	myfile.close();
  return 1;
}