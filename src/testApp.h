#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxPhysics2d.h"
#define MAX_PARTICLES 200
#include "ofxTuio.h"
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	void makeSpring(int _width,int _height);
	
	ofxPhysics2d*physics;
	ofxParticle* mouseParticle;
	ofxSpring*mouseSpring;
	ofxParticle* newParticle;
	bool bCreateParticles;
	float newParticleIncrement;
	
	bool bCreateParticleString;
	ofxParticle* beginParticleString;
	ofxParticle* endParticleString;
	vector<ofxParticle*>particles;
	void createParticleString(ofxParticle*begin, ofxParticle*end, int numParticles);
	
	ofxParticle **_particles;
	int grid;
	float gridSizeX,gridSizeY,gridPosX,gridPosY,pSize;
	float strength;
	
	ofVideoGrabber img;
	bool bSetup;
	ofImage stillImg;
	bool bRender;
	
	myTuioClient tuio;
	ofxParticle** tuioParticle;
	ofxSpring** tuioSpring;
	int max_tuio_constraints;
	bool bMousePressed;
	bool bTuioMessage;
	
};


#endif
