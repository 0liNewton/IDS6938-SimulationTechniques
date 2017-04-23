# Homework 3 : Agent based Simulation Assignment 

## IDS6938-Simulation Techniques - [University of Central Florida](http://www.ist.ucf.edu/grad/)

######The assignment is due: **Monday, April  24 at 11:59PM (EST)** Taking extra day - new due date: **Tuesday, April 25th at 11:59 PM**
---
### Part 1: Behaviors

The goal of this assignment is to enable the steering behavioral animation of agents. We represent an agent as a two-dimensional disk with mass (**m**) and moment-of-inertia (**I**) that is capable of generating a force **F** along body x-axis with a torque about the z-axis. For this assignment assume that the agent only translates in the world x-y plane and rotates about the body z-axis.

#### (a) Compute derivative vector and implment initial values - 10 points** 

###### Compute derivative vector given input and state vectors
* Calling the function and setting derivative vector to the appropriate values

```C++
void SIMAgent::FindDeriv()
{	
	deriv[0] = input[0]/Mass; //force in local body coordinates divided by mass
	deriv[1] = input[1]/Inertia; //torque in local body coordinates divided by inertia
	deriv[2] = state[2]; //velocity of the agent in local body coordinates
	deriv[3] = state[3]; //angular velocity of agent in world coordinates
    }
```

###### Implement *SIMAGENT::InitValues()*
* ...played around with some values...

```C++
void SIMAgent::InitValues()
{	
	Kv0 = #; //Velocity control
	Kp1 = #; //Heading control
	Kv1 = #; //Heading control
	KArrival = #; //Behavior settings
	KDeparture = #;
	KNoise = #;
	KWander = #;
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

```C++
vec2 SIMAgent::Seek()
{	
	vec2 tmp; //call the variable

	tmp = goal - GPos; //shortest path from current position to the target
	tmp.Normalize();
	thetad = atan2(tmp[1], tmp[0]); //derive new angle agent should target
	vd = SIMAgent::MaxVelocity;
	tmp = vec2(cos(thetad)* vd, sin(thetad)* vd);

	return tmp;

}
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Flee
* Based on April 4th class and 'Seek and Flee' webcourses page

```C++
vec2 SIMAgent::Flee()
{
	vec2 tmp; //call the variable
	
	tmp = goal - GPos; //shortest path from current position to the target
	tmp.Normalize();
	thetad = atan2(tmp[1], tmp[0]); //derive new angle agent should target
	thetad = thetad + M_PI; //add 180 degree to Seek desired velocity angle thetad
	vd = SIMAgent::MaxVelocity;
	tmp = vec2(cos(thetad)* vd, sin(thetad)* vd);

	return tmp;
}

```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Arrival
```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

######Departure
```C++
...
```
* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Wander
```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Obstacle Avoidance
```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description


#### (c) Implement Group Behaviors for Agents - 20 points**

###### Seperation

```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Cohesion

```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Alignment
```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Flocking

```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description

###### Leader Following

```C++
...
```

* Add images of GUI
* Add youtube videos of GUI, and 
* Add description


# Part 2 - Simulating a simple pedestrian flow

The goal of this part of the assigment is to simulate the pedestrain flow for various configurations. Pedestrian traffic simulation is an important, sometimes required, stage for certain construction projects such as shopping centers, airports, railway stations, or stadiums. hese analyses can be used by architects in the design stage, by facilities owners contemplating changes, by city planners to evaluate rush hours, or even by civil authorities to simulate evacuations or other emergencies. 

![](images/flow.png?raw=true)

**(a) Subway Entrance Model

![](images/anylogic_subwayhall.PNG?raw=true)

* Done - add description of what was done and what was observed

**(b) Maze Model - 20 points**: Create a maze. Generate the pdm, stats, and animations like before. Vary [parameters in your agents](https://help.anylogic.com/index.jsp?topic=/com.xj.anylogic.help/html/_PL/reference/Attributes.html) to give them different behaviors and show the pedistrians change how they navigate the maze.

* Done - add screen shot image and description

![](images/anylogic_maze.PNG?raw=true)


* https://youtu.be/mYlZyxZ6FYE

**(c) UCF Building - Bookstore within the John T Washington Center - 30 points**: Model and analyze a building (or floor/outdoor space/stadium) on campus. (There is help on piazza how to find rough building plans - we expect something plausible). Create a senario: evacuation, daily office routine, special event, normal egress.... etc., and model and design your own experiment to determine if the building design suits the needs of its users. Start with photographs of your site, describe your site, describe your senario and hypothesis. Then use an agent-based pedistrian simulation to visualize your experiment's results.

* John T Washington Center (aka the breezeway) - UCF Bookstore
* Setting: either beginning or end of the semester), when students are buying/renting/selling/returning textbooks
* Number of students coming to the bookstore as rental deadlines approach



