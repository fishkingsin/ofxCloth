/* 
 * Copyright (c) 2009, Rui Madeira
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * http://creativecommons.org/licenses/LGPL/2.1/
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */



#pragma once

#include "particles/ofxParticle.h"
#include "constraints/ofxConstraint.h"
#include "constraints/ofxSpring.h"
#include "constraints/ofxMaxDistSpring.h"
#include "constraints/ofxMinDistSpring.h"
#include "constraints/ofxInequalityConstraint.h"
#include "constraints/ofxSupportConstraint.h"
#include "constraints/ofxFollowerConstraint.h"
#include "constraints/ofxCollisionConstraint.h"
#include "collision/ofxSimpleCollisionsolver.h"
#include "collision/ofxSortingCollisionSolver.h"

class ofxPhysics2d{
protected:
	vector<ofxParticle*>particles;
	vector<ofxConstraint*>constraints;
	ofxCollisionSolverBase* collisionSolver;
	ofPoint gravity;
	void boundsConstrain();
	bool bCollisions;
	bool bCheckBounds;
	bool bGravity;
	ofPoint worldMin, worldMax;
	int numIterations;
	int numParticles;
	int numConstraints;
public:
	
	ofxPhysics2d(ofPoint _grav = ofPoint(), 
				 bool _bCollisions = false, 
				 int _numIterations = 10,
				 ofPoint _worldMin = ofPoint(), 
				 ofPoint _worldMax = ofPoint(ofGetWidth(), ofGetHeight()),
				 bool _bCheckBounds = true,
				 bool _bEnableGravity = true,
				 ofxCollisionSolverBase* _collisionSolver = NULL);
	~ofxPhysics2d();
	
	void setCollisionSolver(ofxCollisionSolverBase* cs);
	virtual void update(float timeStep = 1.0f);
	virtual void renderParticles();
	virtual void renderConstraints();
	virtual void render(){debugRender();}
	void debugRender();
	void addParticle(ofxParticle *_p, bool _addToCollisions = false);
	void deleteParticle(ofxParticle*_p);
	void removeParticle(ofxParticle* p);
	void addConstraint(ofxConstraint*_c);
	void add(ofxParticle*p);
	void add(ofxParticle*p, bool _enableCollisions);
	void add(ofxConstraint*c);
	void deleteConstraint(ofxConstraint*_c);
	bool hasParticle(ofxParticle*p);
	bool hasConstraint(ofxConstraint*c);
	bool has(ofxParticle* p);
	bool has(ofxConstraint* c);
	void setNumIterations(unsigned int _n);
	int getNumIterations();
	void enableCollisions(bool _bEnable);
	bool collisionsEnabled();
	void checkBounds(bool _bCheck);
	bool boundsCheck();
	void enableGravity(bool _bGravity);
	bool gravityEnabled();
	void removeConstraintsWithParticle(ofxParticle *p);
	void deleteConstraintsWithParticle(ofxParticle *p);
	bool hasConstraintsWithParticle(ofxParticle*p);
	ofxConstraint* getConstraintWithParticle(ofxParticle*p);
	ofxParticle* getNearestParticle(ofPoint point);
	ofxParticle* getParticleUnderPoint(ofPoint& point);
	int getNumParticles();
	int getNumConstraints();
	vector<ofxParticle*>& getParticles();
	vector<ofxConstraint*>& getConstraints();
	ofPoint& getGravity();
	void setGravity(ofPoint _gravity);
	
	void removeAllParticles();
	void deleteAllParticles();
	void removeAllConstraints();
	void deleteAllConstraints();
	void removeAll();
	void deleteAll();
	void reserveForParticles(int numberOfParticles);
	void reserveForConstraints(int numberOfConstraints);
};

