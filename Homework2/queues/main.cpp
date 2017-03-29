/*
  A simple program that simulates 5 MM1 queues at an airport. The first queue
  feeds 3 other queues that work in parallel. These feed a final queue.
  Simulation runs until error below threshold and, at the end,
  a performance evaluation report is shown, and logs are generated
  for plotting and analysis.
*/

using namespace std;

#include <random>
#include "customer.h"
#include "mm1_queue.h"


std::random_device rd;

void pause()
{
 std::string sInputString;
 // Wait for input from stdin (the keyboard)
 cout << "Continue (y)?";
 std::cin >> sInputString;
}



int main(int argc, char* argv[])
{
   int next = 0;
//   test_random_number_generator();

   for (int cur_lambda = 3; cur_lambda < 34; cur_lambda++)
   {
      
	   //TODO Create MM1_Queue objects to capture the airport senario.
	   MM1_Queue  checkin;
	   checkin.set_file_names("checkinlog.txt", "checkinwait.txt", "checkinserv.txt");
	   checkin.set_lambda(6);
	   checkin.set_mu(53);
	   checkin.initialize();
	   checkin.set_seed(1, rd());

	   MM1_Queue  security;
	   security.set_file_names("securitylog.txt", "securitywait.txt", "securityserv.txt");
	   security.set_lambda(6);
	   security.set_mu(20);
	   security.autogenerate_new_arrivals(false);
	   security.initialize();
	   security.set_seed(rd(), rd());

	   MM1_Queue  security1;
	   security1.set_file_names("security1log.txt", "security1wait.txt", "security1serv.txt");
	   security1.set_lambda(6);
	   security1.set_mu(20);
	   security1.autogenerate_new_arrivals(false);
	   security1.initialize();
	   security1.set_seed(rd(), rd());

	   MM1_Queue  security2;
	   security2.set_file_names("security2log.txt", "security2wait.txt", "security2serv.txt");
	   security2.set_lambda(6);
	   security2.set_mu(20);
	   security2.autogenerate_new_arrivals(false);
	   security2.initialize();
	   security2.set_seed(rd(), rd());

	   MM1_Queue  boarding;
	   boarding.set_file_names("boardinglog.txt", "boardingwait.txt", "boardingserv.txt");
	   boarding.set_lambda(6);
	   boarding.set_mu(80);
	   boarding.initialize();
	   boarding.set_seed(1, rd());
	   //************************************************************


   for (; 
		//TODO: add is_within_error_range check
	   !checkin.is_within_error_range(0.002) ||
	   !security.is_within_error_range(0.002) ||
	   !security1.is_within_error_range(0.002) ||
	   !security2.is_within_error_range(0.002)
	   ;)
   {
	   Customer cust = checkin.process_next_event();    // =  TODO: process next event;
	   Customer cust2 = security.process_next_event();   // =  TODO: process next event;
	   Customer cust3 = security1.process_next_event();   // =  TODO: process next event;
	   Customer cust4 = security2.process_next_event();   // =  TODO: process next event;
	   //TODO: one more process_next_event for the last object.

       if (cust.get_type() == Customer::COMPLETED())
       {
          switch(next)
          {
            case 0:
				//TODO add_external_arrival() for your security gates;
				security.add_external_arrival();
                 break;
            case 1:
				//TODO add_external_arrival() for your security gates;
				security1.add_external_arrival();
                 break;
            case 2:
                //TODO add_external_arrival() for your security gates;
				security2.add_external_arrival();
                 break;
          }
          next++;
          if (next%3==0) next = 0;
       }
       if (cust2.get_type() == Customer::COMPLETED() || cust3.get_type() == Customer::COMPLETED() || cust4.get_type() == Customer::COMPLETED())
       {
		   //TODO add_external_arrival(); on your final boarding MM1_Queue object
		   boarding.add_external_arrival();
       }
   }

   //TODO Output statistics airport senario.

   checkin.get_current_time();// cout << "checkin current time" << endl;
   security.get_current_time();
   security1.get_current_time();
   security2.get_current_time();

	//response time comparison, waiting time, expected queue length, mean # of cust, utilization, idle
   checkin.plot_results_output();// cout << "checkin comparisons" << endl;
   security.plot_results_output(); //cout << "Security comparisons" << endl;
   security1.plot_results_output(); //cout << "Security  comparisons" << endl;
   security2.plot_results_output(); //cout << "Security comparisons" << endl;

   checkin.output(); cout << "Checkin Output" << endl;
   security.output(); cout << "Security Output" << endl;
   security1.output(); cout << "Security 1 Output" << endl;
   security2.output(); cout << "Security 2 Output" << endl;
   boarding.output(); cout << "Boarding output" << endl;
   exit(1);
   //**************************************************************************

   }

   return(0);
}

