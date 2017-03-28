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
	myfile.open("markov_results.txt");

	
   // TODO add Markov vector - Matrix multiplication
	//for (int i = 0; i<v.size(); i++) {
		
	//return v;
//}
	v = v.transpose() * TransitionMatrix;
	std::vector<double> v;
	//Eigen::VectorXf v(size);
	//vec myvector = 
	//myvector.transpose() * Matrix


	//std::cout <<  v << std::endl;
	//myfile << v << std::endl;  //this is just a sample, becareful how you print to file so you can mine useful stats
	
	myfile.close();
	
	myfile.open("markov-output.txt");
	for (auto p : v) {
		myfile << std::fixed << std::setprecision(5) << std::setw(2)
			<< p << std::endl;
	myfile.close();
	 }

  return 1;
}