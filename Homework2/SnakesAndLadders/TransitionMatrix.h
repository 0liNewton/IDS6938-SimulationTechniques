int size = 10;  //TODO
Eigen::MatrixXf TransitionMatrix(size, size);
Eigen::VectorXf v(size);

unsigned int ROLLS = 10; //TODO

double prob = 3;  //TODO



void SetTransitionMatrix()
{
	TransitionMatrix.setZero();

	/*TransitionMatrix(0, 0) = prob;
	TransitionMatrix(1, 0) = prob;
	TransitionMatrix(0, 1) = prob; 
	TransitionMatrix(2, 0) = prob;*/

	for (int i = 0; i < TransitionMatrix.rows() - 6; i++) //code from 03/21 class
	{
		TransitionMatrix(i, i + 1) = prob; 
		TransitionMatrix(i, i + 2) = prob;
		TransitionMatrix(i, i + 3) = prob;
		TransitionMatrix(i, i + 4) = prob;
		TransitionMatrix(i, i + 5) = prob;
		TransitionMatrix(i, i + 6) = prob;
		}

	//TO DO: ADD CODE FOR LAST 6 ROWS 

	/*
	TransitionMatrix() = prob;
	TransitionMatrix() = prob;
	TransitionMatrix() = prob;
	TransitionMatrix() = prob;
	TransitionMatrix() = prob;
	TransitionMatrix() = prob;
	*/

	std::cout << TransitionMatrix << std::endl;
	exit(1);
}