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
#include "ofMain.h"

class ofxParticle: public ofPoint{
protected:
	ofPoint oldPos, accel;
	float drag, mass, invMass, radius;
	bool bActive;
	bool bCollide;
	void init(float _x, float _y, float _radius, float _mass, float _drag);
public:
	ofxParticle();
	ofxParticle(float x, float y, float _radius = 10, float _mass = 1, float _drag = 0.8f);
	ofxParticle(ofPoint _iniPos, float _radius = 10, float _mass = 1, float _drag = 0.8f);
	
	virtual ~ofxParticle(){}
	
	virtual void updateParticle(float timeStep = 1.0f);
	virtual void render(){debugRender();}
	void debugRender();
	
	void applyForce(ofPoint& _force);
	void applyImpulse(ofPoint& _impulse);
	
	void setDrag(float _drag);
	float getDrag();
	void setMass(float _mass);
	float getMass();
	float getInvMass();
	void setRadius(float _radius);
	float getRadius();
	
	void setActive(bool _bActive);
	bool isActive();
	
	float distanceTo(ofxParticle*p);
	float distanceToSquared(ofxParticle*p);
	
	bool isPointInside(const ofPoint& p);
	void moveTo(ofPoint& _target);
	void moveTo(float _x, float _y);
	void moveBy(ofPoint& _amount);
	void moveBy(float _x, float _y);
	void lerp(ofPoint& _target, float _interp);
	void moveTowards(ofPoint& _target, float strength);
	void applyAttractionForce(const ofPoint& target, float amount);
	void applyRepulsionForce(const ofPoint& target, float amount);
	void stopMotion();
	
	void setVel(ofPoint _vel);
	ofPoint getVel();
	void setSpeed(float _speed);
	
	void enableCollisions(bool _enable);
	bool collisionsEnabled();
};





