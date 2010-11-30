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

#include "ofxPhysics2d.h"


ofxPhysics2d::ofxPhysics2d(ofPoint _grav, bool _bCollisions, int _numIterations, ofPoint _worldMin, ofPoint _worldMax, bool _bCheckBounds, bool _bGravity, ofxCollisionSolverBase* _collisionSolver){
	gravity.set(_grav.x, _grav.y);
	bCollisions = _bCollisions;
	numIterations = _numIterations;
	bCheckBounds = _bCheckBounds;
	bGravity = _bGravity;
	worldMin.set(_worldMin.x, _worldMin.y);
	worldMax.set(_worldMax.x, _worldMax.y);
	collisionSolver = NULL;
	if(_collisionSolver == NULL){
		//setCollisionSolver(new ofxSimpleCollisionSolver());
		setCollisionSolver(new ofxSortingCollisionSolver);
	} else {
		setCollisionSolver(_collisionSolver);
	}
	numParticles = numConstraints = 0;
}

ofxPhysics2d::~ofxPhysics2d(){
	for(int i=0; i<particles.size(); i++){
		delete particles[i];	
	}
	particles.clear();
	for(int i=0; i<constraints.size(); i++){
		delete constraints[i];
	}
	constraints.clear();
	delete collisionSolver;
}

void ofxPhysics2d::setCollisionSolver(ofxCollisionSolverBase* cs){
	if(collisionSolver){
		delete collisionSolver;
	}
	collisionSolver = cs;
}

void ofxPhysics2d::update(float timeStep){
	numParticles = particles.size();
	numConstraints = constraints.size();
	if(bGravity){
		for(int i=0; i<numParticles; i++){
			particles[i]->applyImpulse(gravity);	
		}
	}
	for(int i=0; i<numParticles; i++){
		particles[i]->updateParticle(timeStep);
	}
	
	ofxCollisionSolverBase& collisionSolverRef = *collisionSolver;
	for(int n=0; n<numIterations; n++){
		for(int i=0; i<numConstraints; i++){
			constraints[i]->update();
		}
		if(bCollisions){
			collisionSolverRef.solve(particles);
		}
		if(bCheckBounds){
			boundsConstrain();
		}
	}
}

void ofxPhysics2d::renderParticles(){
	for(int i=0; i<numParticles; i++){
		particles[i]->render();
	}
}

void ofxPhysics2d::renderConstraints(){
	for(int i=0; i<numConstraints; i++){
		constraints[i]->render();	
	}
}

void ofxPhysics2d::debugRender(){
	renderParticles();
	renderConstraints();
}

void ofxPhysics2d::addParticle(ofxParticle* _p, bool _enableCollisions){
	particles.push_back(_p);
	_p->enableCollisions(_enableCollisions);
}

void ofxPhysics2d::deleteParticle(ofxParticle* _p){
	for(int i=0; i<particles.size(); i++){
		if(_p == particles[i]){
			particles.erase(particles.begin() + i);
			delete _p;
			return;
		}
	}
}

void ofxPhysics2d::removeParticle(ofxParticle*p){
	for(int i=0; i<particles.size(); i++){
		if(particles[i] == p){
			particles.erase(particles.begin() + i);
			return;
		}
	}
}

void ofxPhysics2d::addConstraint(ofxConstraint* _c){
	constraints.push_back(_c);
}

void ofxPhysics2d::deleteConstraint(ofxConstraint*_c){
	for(int i=0; i<constraints.size(); i++){
		if(_c == constraints[i]){
			delete constraints[i];
			constraints.erase(constraints.begin() + i);
			return;
		}
	}
}

void ofxPhysics2d::add(ofxParticle*p){
	particles.push_back(p);
	p->enableCollisions(bCollisions);
}

void ofxPhysics2d::add(ofxParticle*p, bool _enableCollisions){
	particles.push_back(p);
	p->enableCollisions(_enableCollisions);
}

void ofxPhysics2d::add(ofxConstraint*c){
	constraints.push_back(c);
}

bool ofxPhysics2d::hasParticle(ofxParticle*p){
	for(int i=0; i<numParticles; i++){
		if(particles[i] == p) return true;	
	}
	return false;
}

bool ofxPhysics2d::hasConstraint(ofxConstraint*c){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i] == c) return true;	
	}
	return false;
}

bool ofxPhysics2d::has(ofxParticle*p){
	return hasParticle(p);
}

bool ofxPhysics2d::has(ofxConstraint*c){
	return hasConstraint(c);	
}

void ofxPhysics2d::setNumIterations(unsigned int _n){
	numIterations = _n;	
}

int ofxPhysics2d::getNumIterations(){
	return numIterations;	
}

void ofxPhysics2d::enableCollisions(bool _bEnable){
	bCollisions = _bEnable;
}

bool ofxPhysics2d::collisionsEnabled(){
	return bCollisions;	
}

void ofxPhysics2d::checkBounds(bool _bCheck){
	bCheckBounds = _bCheck;
}

bool ofxPhysics2d::boundsCheck(){
	return bCheckBounds;
}

void ofxPhysics2d::enableGravity(bool _bGravity){
	bGravity = _bGravity;
}

bool ofxPhysics2d::gravityEnabled(){
	return bGravity;
}

void ofxPhysics2d::removeConstraintsWithParticle(ofxParticle*p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p){
			constraints.erase(constraints.begin() + i);
		}
	}
}

bool ofxPhysics2d::hasConstraintsWithParticle(ofxParticle*p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p)return true;
	}
	return false;
}

void ofxPhysics2d::deleteConstraintsWithParticle(ofxParticle *p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p){
			delete constraints[i];
			constraints.erase(constraints.begin() + i);
		}
	}
}

ofxConstraint* ofxPhysics2d::getConstraintWithParticle(ofxParticle *p){
	for(int i=0; i<numConstraints; i++){
		if(constraints[i]->getA() == p or constraints[i]->getB() == p){
			return constraints[i];	
		}
	}
	return NULL;
}

ofxParticle* ofxPhysics2d::getNearestParticle(ofPoint point){
	if(numParticles == 0) return NULL;
	if(numParticles == 1) return particles[0];
	int _index = 0;
	float minDistSQ, dx, dy, distSQ;
	dx = particles[0]->x - point.x;
	dy = particles[0]->y - point.y;
	minDistSQ = dx*dx+dy*dy;
	for(int i=1; i<numParticles; i++){
		dx = particles[i]->x - point.x;
		dy = particles[i]->y - point.y;
		distSQ = dx*dx+dy*dy;
		if(distSQ < minDistSQ){
			minDistSQ = distSQ;
			_index = i;
		}
	}
	return particles[_index];
}


ofxParticle* ofxPhysics2d::getParticleUnderPoint(ofPoint& point){
	for(int i=0; i<numParticles; i++){
		float dx = particles[i]->x - point.x;
		float dy = particles[i]->y - point.y;
		float radius = particles[i]->getRadius();
		if(dx*dx+dy*dy < radius*radius) return particles[i];
	}
	return NULL;
}

void ofxPhysics2d::boundsConstrain(){
	float pRadius;
	for(int i=0; i<numParticles; i++){
		ofxParticle& pRef = *particles[i];
		pRadius = pRef.getRadius();
		pRef.x = MAX(worldMin.x + pRadius, MIN(worldMax.x - pRadius, pRef.x));
		pRef.y = MAX(worldMin.y + pRadius, MIN(worldMax.y - pRadius, pRef.y));
	}
}

int ofxPhysics2d::getNumParticles(){
	return numParticles;	
}

int ofxPhysics2d::getNumConstraints(){
	return numConstraints;	
}

vector<ofxParticle*>& ofxPhysics2d::getParticles(){
	return particles;
}

vector<ofxConstraint*>& ofxPhysics2d::getConstraints(){
	return constraints;
}

ofPoint& ofxPhysics2d::getGravity(){
	return gravity;
}

void ofxPhysics2d::setGravity(ofPoint _gravity){
	gravity.set(_gravity.x, _gravity.y);
}

void ofxPhysics2d::removeAllParticles(){
	particles.clear();
}

void ofxPhysics2d::deleteAllParticles(){
	for(int i=0; i<numParticles; i++){
		delete particles[i];
	}
	particles.clear();
}

void ofxPhysics2d::removeAllConstraints(){
	constraints.clear();
}

void ofxPhysics2d::deleteAllConstraints(){
	for(int i=0; i<numConstraints; i++){
		delete constraints[i];
	}
	constraints.clear();
}

void ofxPhysics2d::removeAll(){
	removeAllParticles();
	removeAllConstraints();
}

void ofxPhysics2d::deleteAll(){
	deleteAllParticles();
	deleteAllConstraints();
}

void ofxPhysics2d::reserveForParticles(int numberOfParticles){
	particles.reserve(numberOfParticles);
}

void ofxPhysics2d::reserveForConstraints(int numberOfConstraints){
	constraints.reserve(numberOfConstraints);
}











