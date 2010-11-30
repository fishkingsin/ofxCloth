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

// spring constraint
class ofxSpring:public ofxConstraint{
public:
	ofxSpring(ofxParticle *_a, ofxParticle *_b, float _rest, float _strength = 1.0f):
	ofxConstraint(_a, _b, _rest, _strength, OFX_SPRING){}
	
	virtual void update(){
		if(isOff()) return;
		ofxParticle& aRef = *a;
		ofxParticle& bRef = *b;
		if(!aRef.isActive() and !bRef.isActive()) return;
		delta.x = bRef.x - aRef.x;
		delta.y = bRef.y - aRef.y;
		float dist = delta.x*delta.x + delta.y*delta.y;
		dist = dist < 1 ? 1 : sqrt(dist);
		float moveAmount;
		if(aRef.isActive() && bRef.isActive()){
			moveAmount = strength*(dist - rest)/(dist*(aRef.getInvMass() + bRef.getInvMass()));
			float moveAmountWeighted = moveAmount * aRef.getInvMass();
			aRef.x += delta.x * moveAmountWeighted;
			aRef.y += delta.y * moveAmountWeighted;
			moveAmountWeighted = moveAmount * bRef.getInvMass();
			bRef.x -= delta.x * moveAmountWeighted;
			bRef.y -= delta.y * moveAmountWeighted;
		} else {
			moveAmount = strength*(dist - rest) / dist;
			if(aRef.isActive()){
				aRef.x += delta.x * moveAmount;
				aRef.y += delta.y * moveAmount;
			} else {
				bRef.x -= delta.x * moveAmount;
				bRef.y -= delta.y * moveAmount;
			}
		}
	}
};

