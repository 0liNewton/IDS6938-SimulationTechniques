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
	//std::Eigen::VectorXf

	for (int i = 0; i < v.size(); i++) {
		v = v.transpose() * TransitionMatrix;
		std::vector<double> rawX;
		//std::cout << VectorXf.pushback(v)
	}
	//v = v.transpose() * TransitionMatrix;
	//std::vector<double> v;
	//std::cout << 'v '<< std::endl;
	
	//myfile << v << std::endl;
	//Eigen::VectorXf v(size);
	//vec myvector = 
	//std::cout <<  v << std::endl;

	//myfile << v << std::endl;  //this is just a sample, becareful how you print to file so you can mine useful stats

	myfile.close();
  return 1;
}