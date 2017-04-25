// Agent.cpp: implementation of the SIMAgent class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Behavior.h"
#include "Agent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//Construct static variables
//Their real values are set in static function SIMAgent::InitValues()
vector<SIMAgent*> SIMAgent::agents;
bool SIMAgent::debug = false;
float SIMAgent::radius = 20.0; //Radius of the bounding sphere of every agent
float SIMAgent::Mass = 1.0; //Mass of every agent
float SIMAgent::Inertia = 1.0; //Maximum inertia
float SIMAgent::MaxVelocity = 20.0; //Maximum velocity
float SIMAgent::MaxForce = 10.0; //Maximum force
float SIMAgent::MaxTorque = 40.0; //Maximum torque
float SIMAgent::MaxAngVel = 10.0; //Maximum angular velocity
float SIMAgent::Kv0 = 1.0; //Velocity control: f = m * Kv0 * (vd - v)
float SIMAgent::Kp1 = 1.0; //Heading control: tau = I * ( -Kv1 * thetaDot - Kp1 * theta + Kp1 * thetad)
float SIMAgent::Kv1 = 1.0; //Heading control: tau = I * ( -Kv1 * thetaDot - Kp1 * theta + Kp1 * thetad)
float SIMAgent::KArrival = 1.0; // Behavior settings. See comments in cpp file for details
float SIMAgent::KDeparture = 1.0;
float SIMAgent::KNoise = 1.0;
float SIMAgent::KWander = 1.0;
float SIMAgent::KAvoid = 1.0;
float SIMAgent::TAvoid = 1.0;
float SIMAgent::RNeighborhood = 1.0;
float SIMAgent::KSeparate = 1.0;
float SIMAgent::KAlign = 1.0;
float SIMAgent::KCohesion = 1.0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
////////////////////////////////////////////////////////////////////// 
vec2 SIMAgent::WorldToLocal(vec2 &w)
{
	vec2 l;
	float s = sin(state[1]);
	float c = cos(state[1]);
	l[0] = c * w[0] + s * w[1];
	l[1] = -s * w[0] + c * w[1];
	return l;
}

vec2 SIMAgent::LocalToWorld(vec2 &l)
{
	vec2 w;
	float s = sin(state[1]);
	float c = cos(state[1]);
	w[0] = c * l[0] - s * l[1];
	w[1] = s * l[0] + c * l[1];
	return w;
}

void SIMAgent::SetVTheta(vec2 &V)
{
	vd = V.Length();
	if (vd < 0.0001) {
		thetad = 0.0;
	}
	else {
		if (abs(V[0]) < 0.0001) {
			if (V[1] > 0)
				thetad = M_PI / 2.0;
			else
				thetad = M_PI / -2.0;
		}
		else
			thetad = atan2(V[1], V[0]);
	}

	ClampAngle(thetad);
}

SIMAgent::SIMAgent()
{
}

SIMAgent::~SIMAgent()
{
}

void SIMAgent::Display()
{
	glPushMatrix();
	glTranslatef(GPos[0], rootHeight[frameNum], GPos[1]);
	if (showLeader && this == SIMAgent::agents[0]) {
		float red[] = { 1.0, 0.0, 0.0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
		glPushMatrix();
		glTranslatef(0.0, 100.0, 0.0);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutSolidCone(10.0, 20.0, 24, 24);
		glPopMatrix();
	}

	float specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);

	if (SIMAgent::debug) {
		glutSolidSphere(10.0, 24, 24);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glRotatef(-state[1] / M_PI * 180.0, 0.0, 1.0, 0.0);
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f(12 + state[2], 0.0, 0.0);
		glEnd();
		glPopMatrix();
		glPushMatrix();
		glColor3f(0.0, 1.0, 0.0);
		glRotatef(-thetad / M_PI * 180.0, 0.0, 1.0, 0.0);
		glLineWidth(3.0);
		glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f(12 + vd, 0.0, 0.0);
		glEnd();
		glPopMatrix();
		glLineWidth(1.0);
		glEnable(GL_LIGHTING);
	}
	else {
		glRotatef(90.0f - state[1] * Rad2Deg, 0.0, 1.0, 0.0);
		//glutSolidSphere(6.0f, 12, 12);
		glCallList(AgentDisplay::listNames + frameNum);
	}
	glPopMatrix();
}

void SIMAgent::ClearAgents() {
	for (unsigned int i = 0; i < agents.size(); i++)
		delete SIMAgent::agents[i];
	SIMAgent::agents.clear();
}

SIMAgent::SIMAgent(float* color, CEnvironment* env) {
	memcpy(this->color, color, sizeof(float) * 3);
	dimState = 4;
	dimInput = 2;
	frameNum = rand() % totalFrames;
	this->env = env;
	float angle = float(rand() % 360) / 180.0 * M_PI;
	vWander[0] = cos(angle) * SIMAgent::KWander;
	vWander[1] = sin(angle) * SIMAgent::KWander;
	angle = float(rand() % 360) / 180.0 * M_PI;
	v0[0] = cos(angle) * SIMAgent::MaxVelocity / 2.0;
	v0[1] = sin(angle) * SIMAgent::MaxVelocity / 2.0;
	SIMAgent::agents.push_back(this);
}

void SIMAgent::SetInitState(float pos[], float angle)
{
	int i;
	GPos[0] = pos[0];
	GPos[1] = pos[1];
	for (i = 0; i < dimState; i++) {
		state[i] = 0.0;
	}
	for (i = 0; i < dimInput; i++)
		input[i] = 0.0;
	state[1] = angle;
}

void SIMAgent::Sense(int type)
{
	showLeader = (type == LEADER);
	goal = env->goal;
	switch (type) {
	case SEEK: Seek(); break;
	case FLEE: Flee(); break;
	case ARRIVAL: Arrival(); break;
	case DEPARTURE: Departure(); break;
	case WANDER: Wander(); break;
	case AVOID: Avoid(); break;
	case SEPARATION: Separation(); break;
	case ALIGNMENT: Alignment(); break;
	case COHESION: Cohesion(); break;
	case FLOCKING: Flocking(); break;
	case LEADER: Leader(); break;
	}
}

void SIMAgent::Act(float deltaT)
{
	int dframe;
	this->deltaT = deltaT;
	FindDeriv();
	UpdateState();
	if (abs(state[2]) < 0.001)
		dframe = 0;
	else {
		dframe = int(state[2] / SIMAgent::MaxVelocity * 2 + 1);
	}
	frameNum += dframe;
	if (frameNum >= totalFrames) {
		frameNum = frameNum - totalFrames;
	}
}

void SIMAgent::SetV0()
{
	v0 = env->goal - GPos;
	v0 = v0.Normalize() * SIMAgent::MaxVelocity / 2;
}

/*
Sets the intial Values
*/
void SIMAgent::InitValues()
{
	/*********************************************
	Set initial value for control and behavior settings. Find out appropriate values for:
	*********************************************/
	Kv0 = 10.0; // from definition Velocity control: f = m * Kv0 * (vd - v)

	// From "Translating Steering Behavior Pursuit Sample" (StackExchange: Game Development)
		// Heading refers to a vector pointing in the direction the entity is heading

	Kp1 = -10.0; // Heading control - changed to negative to get agents moving in desired direction (opposite of what was being observed when this value was positive)
	Kv1 = 10.0; // Heading control
	KArrival = 1.0; // Behavior settings
	KDeparture = 7000.0;
	KNoise = 0.5;
	KWander = 500.0;
	KAvoid = 0.0;
	TAvoid = 0.0;
	RNeighborhood = 0.0;
	KSeparate = 0.0;
	KAlign = 0.0;
	KCohesion = 0.0;
}

/*
*	Apply the control rules given desired velocity vd and desired orientation thetad.
*/
void SIMAgent::Control()
{
	//*********************************************
	// TODO: Add code here - Based on Readme and several Piazza posts 
	// This function sets input[0] and input[1] after being called
	//State vector dimension: 4
	/*
	*	State Vector: 4 dimensions
	*  0 : position in local coordinates. Useless.
	*  1 : orientation angle in global coordinates.
	*  2 : velocity in local coordinates.
	*  3 : angular velocity in global coordinates.
	*/
	Truncate(vd, -SIMAgent::MaxVelocity, SIMAgent::MaxVelocity);
	
	//set vector input[0] 
	//f = m * Kv0 * (vd - v) -> input[0] = mass of agent * velocity force * (desired velocity - velocity of agent)

	input[0] = SIMAgent::Mass * SIMAgent::Kv0 * (vd - state[2]); // force in local body coordinates
	Truncate(input[0], -SIMAgent::MaxForce, SIMAgent::MaxForce); 
 
	double dangle = AngleDiff(state[1], thetad);
	
	//set input[1] -> tau = I * ( -Kv1 * thetaDot - (Kp1 * theta + Kp1 * thetad)) -> 
	//input[1] = inertia of agent *   - heading control equation * angular velocity in global coordinates
	//input[1] = SIMAgent::Inertia * [..whatever thetaDot] is?
	input[1] = SIMAgent::Inertia * (SIMAgent::Kp1 * dangle - SIMAgent::Kv1 * state[3]); // torque in local body coordinates -- should 
	Truncate(input[1], -SIMAgent::MaxTorque, SIMAgent::MaxTorque);
	
	//*********************************************
}

/*
*	Compute derivative vector given input and state vectors
*  This function sets derive vector to appropriate values after being called
*/
void SIMAgent::FindDeriv()
{
	/*********************************************
	"Compute derivative vector given input and state vectors" - readme
	- call function
	- then function sets deriv vector to appropriate value
	*********************************************/
	
	deriv[0] = state[2]; //velocity of the agent in local body coordinates
	deriv[1] = state[3]; //angular velocity of agent in world coordinates
	deriv[2] = input[0] / Mass; //force in local body coordinates divided by mass
	deriv[3] = input[1] / Inertia; //torque in local body coordinates divided by inertia
}

/*
*  Update the state vector given derivative vector
*  Compute global position and store it in GPos
*  Perform validation check to make sure all values are within MAX values
*/

void SIMAgent::UpdateState()
{
	// Copied from Dr. Kider's Piazza post
	
	for (int i = 0; i < dimState; i++){
	state[i] += deriv[i] * deltaT;
	}
	state[0] =0.0;

	//Clamp(state[1], -M_PI, M_PI);
	ClampAngle(state[1]);
	Truncate(state[2], -SIMAgent::MaxVelocity, SIMAgent::MaxVelocity);

	vec2 GVelocity;
	GVelocity[0] = state[2] * cos(state[1]);
	GVelocity[1] = state[2] * sin(state[1]);
	GPos += GVelocity;

	Truncate(GPos[0], -1.0 * env->groundSize, env->groundSize);
	Truncate(GPos[1], -1.0 * env->groundSize, env->groundSize);

	Truncate(state[3], -SIMAgent::MaxAngVel, SIMAgent::MaxAngVel);

}

/*
*  Seek behavior
*  Global goal position is in goal
*  Agent's global position is in GPos
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/

vec2 SIMAgent::Seek()
{
	/*********************************************
	Based on April 4th class and 'Seek and Flee' webcourses page
	*********************************************/
	
	vec2 tmp; // call the variable
	
	tmp = goal - GPos; // shortest path from current position to the target
	thetad = atan2(tmp[1], tmp[0]); // derive new angle agent should target
	//thetad = thetad + M_PI; //add 180 degree to Seek desired velocity angle thetad
	vd = SIMAgent::MaxVelocity; // define agent's velocity
	tmp = vec2(cos(thetad)* vd, sin(thetad)* vd); // convert to Cartesian coordinaets
	
	return tmp; // return coordinates

}

/*
*  Flee behavior
*  Global goal position is in goal
*  Agent's global position is in GPos
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/

vec2 SIMAgent::Flee()
{
	/*********************************************
	Based on April 4th class and 'Seek and Flee' webcourses page
	*********************************************/

	vec2 tmp; //call the variable
	
	tmp = goal - GPos; //shortest path from current position to the target
	thetad = atan2(tmp[1], tmp[0]); //derive new angle agent should target
	thetad = thetad + M_PI; //add 180 degree to Seek desired velocity angle thetad
	vd = SIMAgent::MaxVelocity;
	tmp = vec2(cos(thetad)* vd, sin(thetad)* vd); //convert to Cartesian

	return tmp; //return coordinates
}

/*
*  Arrival behavior
*  Global goal position is in goal
*  Agent's global position is in GPos
*  Arrival setting is in SIMAgent::KArrival
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/

vec2 SIMAgent::Arrival()
{
	/*********************************************
	Based on April 4th class and 'Arrival and Departure' webcourses page
	*********************************************/
	vec2 tmp;
	double dist;
	
	tmp = goal - GPos; //shortest path from the current position to the target
	dist = tmp.Length(); // distance to target
	thetad = atan2(tmp[1], tmp[0]); // desired orientation
	vd = dist * SIMAgent::KArrival; // desired velocity
/*
	Added if statement based on lecture and "Steering Behaviors in C# and C++" by Simon Coenen (simoncoenen.com/downloads/ai_paper.pdf)
	*Should steer agent towards the target and slow down as it comes closer
*/
	if (dist < 30) { //agent's bounding radius is set at 20?
		
		tmp = vec2(cos(thetad)* vd, sin(thetad)* vd); // Cartesian coordinates
	}

	else
		tmp = vec2(cos(thetad)* vd, sin(thetad)* vd);

	return tmp; // return coordinates
}

/*
*	Departure behavior
*  Global goal position is in goal
*  Agent's global position is in GPos
*  Departure setting is in KDeparture
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/

vec2 SIMAgent::Departure()
{
	/*********************************************
	'Arrival and Departure' webcourses page
	*********************************************/
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

/*
*	Wander behavior
*  VWander is in vWander
*  V0(nominal velocity) is in v0
*  Wander setting is in KWander
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/
vec2 SIMAgent::Wander()
{
	/*********************************************
	'Wander and Avoid' webcourses
	*********************************************/
	vec2 tmp; // call the variable

	tmp = v0 - vWander; // ?? nominal minus wander (peeked definition)
	float randomangle;
	randomangle = float(rand() % 360) / 180.0 * M_PI; // pick a random angle
	thetad = atan2(tmp[1], tmp[0]) * SIMAgent::KNoise;
	vd = SIMAgent::MaxVelocity; // define agent's velocity
	thetad = randomangle; // set thetad to random angle
	tmp = vec2(cos(thetad) * vd, sin(thetad) * vd) * SIMAgent::KWander; // convert to Cartesian coordinaets
	
	return tmp; // return coordinates
}

/*
*	Avoid behavior
*  Obstacles are in env->obstacles
*  Number of obstacles is env->obstaclesNum
*  env->obstacles[i][0] and env->obstacles[i][1] are the global position of the ith obstacle
*  env->obstacles[i][2] is the radius of the ith obstacle
*  Agent bounding sphere radius is in SIMAgent::radius
*  Avoidance settings are in SIMAgent::TAvoid and SIMAgent::KAvoid
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/
vec2 SIMAgent::Avoid()
{
	/*********************************************
	// TODO: Add code here
	*********************************************/
	vec2 tmp;
	
	return tmp;
}

/*
*	Separation behavior
*  SIMAgent::agents[i] gives the pointer to the ith agent in the environment
*  Separation settings are in SIMAgent::RNeighborhood and SIMAgent::KSeperate
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/
vec2 SIMAgent::Separation()
{
	/*********************************************
	// TODO: Add code here
	*********************************************/
	vec2 tmp;

	return tmp;
}

/*
*	Alignment behavior
*  SIMAgent::agents[i] gives the pointer to the ith agent in the environment
*  Alignment settings are in SIMAgent::RNeighborhood and SIMAgent::KAlign
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/
vec2 SIMAgent::Alignment()
{
	/*********************************************
	// TODO: Add code here
	*********************************************/
	vec2 tmp;

	return tmp;
}

/*
*	Cohesion behavior
*  SIMAgent::agents[i] gives the pointer to the ith agent in the environment
*  Cohesion settings are in SIMAgent::RNeighborhood and SIMAgent::KCohesion
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/
vec2 SIMAgent::Cohesion()
{
	/*********************************************
	// TODO: Add code here
	*********************************************/
	vec2 tmp;


	return tmp;
}

/*
*	Flocking behavior
*  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/
vec2 SIMAgent::Flocking()
{
	/*********************************************
	// TODO: Add code here
	*********************************************/
	vec2 tmp;

	return tmp;
}

/*
*	Flocking behavior
*  Utilize the Separation, Arrival behaviors to determine the desired velocity vector
*  You need to find the leader, who is always the first agent in SIMAgent::agents
*  You need to compute the desired velocity and desired orientation
*  Store them into vd and thetad respectively
*  return a vec2 that represents the goal velocity with its direction being thetad and its norm being vd
*/
vec2 SIMAgent::Leader()
{
	/*********************************************
	// TODO: Add code here
	*********************************************/
	vec2 tmp;

	return tmp;
}