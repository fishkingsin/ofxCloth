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

#include "ofxConstraint.h"

// a constraint that is only active when the distance between the particles is smaller or bigger than the defined values
class ofxInequalityConstraint:public ofxConstraint{
protected:
	float minRest, minSQ, maxRest, maxSQ;
public:
	ofxInequalityConstraint(ofxParticle*_a, ofxParticle*_b, float _minRest, float _maxRest, float _strength):
	ofxConstraint(_a, _b, _minRest, _strength, OFX_INEQUALITY_CONSTRAINT){
		minRest = _minRest;
		maxRest = _maxRest;
		minSQ = minRest*minRest;
		maxSQ = maxRest*maxRest;
	}
	
	virtual void update(){
		if(isOff()) return;
		ofxParticle& aRef = *a;
		ofxParticle& bRef = *b;
		if(!aRef.isActive() and !bRef.isActive()) return;
		delta.x = bRef.x - aRef.x;
		delta.y = bRef.y - aRef.y;
		float dist = delta.x*delta.x+delta.y*delta.y;
		if(dist > minSQ && dist < maxSQ) return;
		if(dist < minSQ) rest = minRest;
		else rest = maxRest;
		if(dist < 1) dist = 1;
		else dist = sqrt(dist);
		float moveAmount;
		if(aRef.isActive() && bRef.isActive()){
			moveAmount = strength*(dist - rest) / dist * (a->getInvMass() + b->getInvMass());
			float moveAmountWeighted = moveAmount * aRef.getInvMass();
			aRef.x += moveAmount * moveAmountWeighted;
			aRef.y += moveAmount * moveAmountWeighted;
			moveAmountWeighted = moveAmount * bRef.getInvMass();
			bRef.x -= moveAmount * moveAmountWeighted;
			bRef.y -= moveAmount * moveAmountWeighted;
		} else {
			moveAmount = strength*(dist - rest) / dist;
			if(aRef.isActive()){
				aRef.x += moveAmount*delta.x;
				aRef.y += moveAmount*delta.y;
			} else {
				bRef.x -= moveAmount*delta.x;
				bRef.y -= moveAmount*delta.y;
			}
		}
	}
};