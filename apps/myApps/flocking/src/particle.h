#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

// C style 80s :)

typedef struct{
	int			count;
	ofVec3f	sum;
	float		distance;
	float		strength;
} flockingForce;
 

// C++ style 90s :)
/*
class flockingForce {
	public:
	
	int			count;
	ofVec2f	sum;
	float		distance;
	float		strength;
	
};
*/



class particle: public ofBaseApp{
public:
    ofVec3f pos;
    ofVec3f vel;
    ofVec3f frc;   // frc is also know as acceleration (newton says "f=ma")
    
    
    particle();
    virtual ~particle(){};
    
    void resetForce();
    void addForce(float x, float y, float z);
    
    void addRepulsionForce(float x, float y, float z, float radius, float scale);
    
    void addAttractionForce(float x, float y, float z, float radius, float scale);
    
    void addRepulsionForce(particle &p, float radius, float scale);
    void addAttractionForce(particle &p, float radius, float scale);
    void addClockwiseForce(particle &p, float radius, float scale);
    void addCounterClockwiseForce(particle &p, float radius, float scale);
	
    void addDampingForce();
    
    void addForFlocking(particle &p);
    void addFlockingForce();
	
    flockingForce cohesion;
    flockingForce seperation;
    flockingForce alignment;
	
	
    void setInitialCondition(float px, float py, float pz, float vx, float vy, float vz);
    void update();
    void draw();
    void draw(string MODE);
	
    void bounceOffWalls();
	
    float damping;
    
    
protected:
private:

};

#endif // PARTICLE_H
