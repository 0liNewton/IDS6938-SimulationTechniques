int size = 10;  //TODO - change to 101
Eigen::MatrixXf TransitionMatrix(size, size);
Eigen::VectorXf v(size);


unsigned int ROLLS = 1; //TODO

double prob = 3;  //TODO - change to 1.0/6.0



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
	
	// A Markov Chain defines the probability of a move from state *i* to state *j* by a **Transition Matrix**, *T*.
	/* for (int j = ; j < TransitionMatrix.rows(); j++)
	{
	TransitionMatrix(j, ) = prob;
	TransitionMatrix(j, ) = prob;
	TransitionMatrix(j, ) = prob;
	TransitionMatrix(j, ) = prob;
	TransitionMatrix(j, ) = prob;
	TransitionMatrix(j, ) = prob;
	}
	*/

	std::cout << TransitionMatrix << std::endl;
	exit(1);
}