# Homework 3 : Agent based Simulation Assignment 

## IDS6938-Simulation Techniques - [University of Central Florida](http://www.ist.ucf.edu/grad/)

###### ~~The assignment is due: **Monday, April  24 at 11:59PM (EST)**~~ Taking extra day - due date: **Tuesday, April 25th at 11:59 PM (EST)**
---
### Part 1: Behaviors

The goal of this assignment is to enable the steering behavioral animation of agents. We represent an agent as a two-dimensional disk with mass (**m**) and moment-of-inertia (**I**) that is capable of generating a force **F** along body x-axis with a torque about the z-axis. For this assignment assume that the agent only translates in the world x-y plane and rotates about the body z-axis.

#### (a) Compute Derivative Vector and Implement Initial Values - 10 points** 

###### Compute derivative vector given input and state vectors
* Calling the function and setting derivative vector to the appropriate values

```C++
void SIMAgent::FindDeriv()
{
	deriv[0] = state[2]; //velocity of the agent in local body coordinates
	deriv[1] = state[3]; //angular velocity of agent in world coordinates
	deriv[2] = input[0] / Mass; //force in local body coordinates divided by mass
	deriv[3] = input[1]/Inertia; //torque in local body coordinates divided by inertia
    }
```

###### Implement *SIMAGENT::InitValues()*
* These were adjusted concurrently with the construction of functions for the implementation of behaviors in steps **(b)** and **(c)**
	* Once was added for a behavior function, the corresponding initial values were adjusted

```C++
void SIMAgent::InitValues()
{	
	Kv0 = 10.0; //Velocity control
	Kp1 = -10.0; //Heading control
	Kv1 = 10; //Heading control
	KArrival = 600; //Behavior settings
	KDeparture = 7000;
	KNoise = 200;
	KWander = 200;
	KAvoid = #;
	TAvoid = #;
	RNeighborhood = #;
	KSeparate = #;
	KAlign = #;
	KCohesion = #;
    }
```

#### (b) Implement Individual Behaviors for Agents - 20 points**
###### Create the following behaviors through appropriate computation of V<sub> d</sub> and θ<sub>d</sub> commands
* **v<sub>d</sub>** - agent's max velocity
* **θ<sub>d</sub>** - target angle theta d

###### Seek
* Based on April 4th class and 'Seek and Flee' webcourses page
* Agents were moving in the opposite of desired directions
* Checked my code for all the possible errors noted in responses to Piazza post by Rebecca Leis
	* Pi is the right place (in flee block not seek); goal and position subtraction is not backwards
	* Moved onto adjusting initial values per Joey Netterville's comment on original post
		* Initial values affecting Seek and Flee behaviors: *Kv0, Kp1, Kv1*
	* The inital values were all set to 10 when the opposite direction behavior is observed
	* Increasing/decreasing these values did not correct agent behavior (range min 2, range max 1000)
	* Alternated +/- value of *Kv0*
	 	* Agents glided backwards and disappeared from GUI ** *but* ** at least now the agents moved towards the target!
	 	* Regardless of this value's size, with the negative value added the agent always faced the wrong direction while moving towards the target and then disappeared
	* Alternated +/- value of *Kp1*
	 	* Started at -10, agents face and move in the right direction of the target, and move around the target until they eventually form a circle the target - i.e., *seek* behavior
	* For good measure, alternated +/- value of *Kv1*
		* Agents spun around in a circle and slowly moved towards target
	* Across these initial values, negative sign correct agents' direction or movement towards the target but can introduce other errors
	* Only a negative *Kp1* value achieves desired behavior outcome
* As time passes, the agents begin to form clusters

```C++
vec2 SIMAgent::Seek()
{	
	vec2 tmp; //call the variable

	tmp = goal - GPos; // desired velocity - shortest path from current position to the target
	thetad = atan2(tmp[1], tmp[0]); // derive new angle agent should target
	vd = SIMAgent::MaxVelocity; // agent's max velocity
	tmp = vec2(cos(thetad)* vd, sin(thetad)* vd); // convert to Cartesian coordinates

	return tmp; //return coordinates

}
```

* Youtube: [Seek](https://youtu.be/iib-sMjH-mg)

![](images/seek.png?raw=true)


###### Flee
* Based on April 4th class and 'Seek and Flee' webcourses page

```C++
vec2 SIMAgent::Flee()
{
	vec2 tmp; //call the variable
	
	tmp = goal - GPos; //shortest path from current position to the target
	thetad = atan2(tmp[1], tmp[0]); //derive new angle agent should target
	thetad = thetad + M_PI; //add 180 degree to Seek desired velocity angle thetad
	vd = SIMAgent::MaxVelocity; //agent velocity
	tmp = vec2(cos(thetad)* vd, sin(thetad)* vd); //Cartesian

	return tmp; //return coordinates
}

```

* Youtube: [Flee](https://youtu.be/iQa5eLWF6VU)
	* Includes Seek behavior to show differences between both behaviors
	
![](images/flee.png?raw=true)

###### Arrival
* Based on April 4th class and 'Arrival and Departure' webcourses page
* If statement based on lecture and "Steering Behaviors in C# and C++" by Simon Coenen (simoncoenen.com/downloads/ai_paper.pdf)
	* Should steer agent towards the target and slow down as it comes closer

```C++
vec2 SIMAgent::Arrival()
{
	vec2 tmp;
	double dist;
	double m;
	double arrived;
	
	tmp = goal - GPos; //shortest path from the current position to the target
	dist = tmp.Length(); // distance to target
	vd = dist - SIMAgent::KArrival; //desired velocity 
	//vd = dist * SIMAgent::KArrival;
	thetad = atan2(tmp[1], tmp[0]); // desired orientation

	if (dist > 2) { 
		tmp = vec2(cos(thetad)* vd, sin(thetad)* vd); // Cartesian coordinates
	}
	else {
		tmp = vec2(cos(thetad), sin(thetad));
	}
}
```

* Youtube: [Arrival - Basic](https://youtu.be/h-8T3fzy9D4)
	* As you can see in the video, I did not execute this behavior perfectly - the first time the agents approach the target they do not slow down and come to a stop only once they've reached the target
	* As the agents repeat this behavior, they begin to slow down and stop before reaching the target
* Youtube: [Arrival - Moving the Target](https://youtu.be/opaS4oHdhT8)
	* This video shows how the agents arrival behavior adjusts with the changing location of the target
	* Again, the agents reach the target, stop, and then back up - they repeat this behavior infinitely

![](images/arrival.png?raw=true)


###### Departure
```C++
vec2 SIMAgent::Departure()
{
	vec2 tmp;
	double dist;
	
	tmp = goal - GPos; // shortest path from the current position to the target
	dist = tmp.Length(); // distance to target
	thetad = atan2(tmp[1], tmp[0]); // derive target angle
	thetad = thetad + M_PI; //opposite direction
	vd = (1.0/dist) * SIMAgent::KDeparture ; // max velocity with damping, stop agents from departing infinitely
	tmp = vec2(cos(thetad)* vd, sin(thetad)* vd); //convert to Cartesian coordinates

	return tmp;
}
```
* Youtube: [Departure](https://youtu.be/8oYlwopYqmc)
	* Video shows differences between Seek, Flee, and Departure behaviors
	
![](images/depart.png?raw=true)

###### Wander
```C++
vec2 SIMAgent::Wander()
{
	vec2 tmp; 

	angle = float(rand() % 360) / 180.0 * M_PI; // pick a random angle
	thetad = angle;
	vd = SIMAgent::MaxVelocity;
	tmp = vec2(cos(thetad) * vd * SIMAgent::KNoise, sin(thetad) * vd * SIMAgent::KNoise) * SIMAgent::KWander;
    
    return tmp;
}
```
* I was not successful in my attempt to simulate Wander behavior
	* Observed behavior was similiar to that of flee

###### Obstacle Avoidance
```C++
...
```

#### (c) Implement Group Behaviors for Agents - 20 points**

###### Seperation

```C++
...
```

###### Cohesion

```C++
...
```


###### Alignment
```C++
...
```

###### Flocking

```C++
...
```

###### Leader Following

```C++
...
```


# Part 2 - Simulating a simple pedestrian flow

#### (a) Subway Model
* YouTube: [Subway Hall](https://youtu.be/dKimwcThuOg) 

![](images/anylogic_subwayhall.PNG?raw=true)


#### (b) Maze Model - 20 points**: 

* Youtube: [Maze](https://youtu.be/rq88RDzNj14)

![](images/maze2d.PNG?raw=true)
![](images/logicandcharts.PNG?raw=true)

#### (c) UCF Building - Bookstore within the John T Washington Center - 30 points**

* I chose to model and analyze the Barnes & Noble Bookstore in the John T Washington Center on UCF's Main Campus.

| John T Washington Center  |
| ------------- |
| ![](images/jtwashington.jpg?raw=true) |

| View of Main Entrance from the Breezeway |
| ------------- |
| ![](images/bookstore.png?raw=true) |

| Crowds Gather Near the Breezeway|
| ------------- |
| ![](images/bookstore1.png?raw=true) |

| View from the bookstore's Starbuck's Coffee Shop in the Bookstore |
| ------------- |
| ![](images/coffee.png?raw=true) |

| View from Starbuck's Coffee Shop in the Bookstore |
| ------------- |
| ![](images/bookstore2.png?raw=true) |


* At the end of each semester, textbook rental returns are due back the last day of the final exam period. As the deadline draws near and the semester comes to a close, more and more students make their way to this central bookstore to return or sell back textbooks. During this time, the bookstore staff constructs a temporary queue circling around the store's many merchandise displays and extending across a large portion of the store's floorspace. This queue somewhat blocks access to other areas of the store - the coffee shop and the side entrance/exit. As the main entrance becomes a starting point for the textbook services queue, the side door serves as an entry point for zombies, I mean students, seeking a caffiene boost. Of course, professors and other faculty may drop in to pick up some coffee as well. And without a doubt there will be students enter through the side door only to realize how long the textbook services queue is...some will leave, but others might see a friend in line and jump in - the rule breakers.

* On a moderately busy day during the final exam period, one can expect to find a relatively long queue
* The bookstore relies on two rows of registers to provide the desired services
	* Youtube: [Busy Day](https://youtu.be/6iRDJAnWLpk)

![](images/sbusy6.png?raw=true)
![](images/somewhatbusy6.png?raw=true)

* To a certain extent, the building does meet the needs of its users. However, much of the merchandise sold in the store takes up a lot of floorspace.
	* It's not certain this would increase efficieny (and decrease wait time)
* UCF's bookstore could be updated to better address the shifting needs of students.
	* E-books and textbook rentals are very popular - they're convenient and cheaper than traditional textbook purchase
	* Online shopping is also very popular - who goes to the bookstore to buy the overpriced school merch?!
		* The bookstore could consider downsizing it's apparel and accessories merchandise - afterall, it is a *book* store
		* Without knowing their profits from said merchandise, it's impossible to say this would truly benefit the bookstore - it may however benefit the users (i.e., students)
* The model used was not entirely realistic.
	* Some individuals coming in through the main entrance might just want some coffee
	* BUT the individuals who want coffee and see the queue at the main entrance would likely take the detour to the side door
* Alternatively, merchandise displays could be updated with lightweight, collapsable materials to meet temporary need for floorspace during final exam period. 

