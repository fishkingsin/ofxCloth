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

/*
based on the sorting algorithm by Zach Lieberman which can be found here
http://www.makingthingsmove.org/blog/?p=251
*/

#pragma once

#include "ofxCollisionSolverBase.h"
static bool compare(ofxParticle* a, ofxParticle* b);
bool compare(ofxParticle* a, ofxParticle* b){
	return a->x < b->x;
}

class ofxSortingCollisionSolver: public ofxCollisionSolverBase{
public:
	void solve(vector<ofxParticle*>& particles){
		int numParticles = particles.size();
		if(numParticles < 2)return;
		int maxDist = particles[0]->getRadius();
		int partRadius;
		for(int i=1; i<numParticles; i++){
			partRadius = particles[i]->getRadius();
			if(partRadius > maxDist){
				maxDist = partRadius;
			}
		}
		maxDist *= 2;
		float rest;
		ofPoint delta;
		float dist;
		sort(particles.begin(), particles.end(), compare);
		for(int i=0; i<numParticles; i++){
			ofxParticle& a = *particles[i];
			for(int j = i-1; j >= 0; j--){
				ofxParticle& b = *particles[j];
				delta.x = a.x - b.x;
				if(fabs(delta.x) > maxDist) break;
				if(!a.isActive() and !b.isActive())continue;
				rest = a.getRadius() + b.getRadius();
				delta.y = a.y - b.y;
				dist = delta.x*delta.x + delta.y*delta.y;
				if(dist > rest*rest)continue;
				dist = dist < 1 ? 1 : sqrt(dist);
				float moveAmount;
				if(a.isActive() and b.isActive()){
					moveAmount = (dist - rest) / (dist * (a.getInvMass() + b.getInvMass()));
					float moveAmountWeighted = moveAmount * a.getInvMass();
					a.x -= delta.x * moveAmountWeighted;
					a.y -= delta.y * moveAmountWeighted;
					moveAmountWeighted = moveAmount * b.getInvMass();
					b.x += delta.x * moveAmountWeighted;
					b.y += delta.y * moveAmountWeighted;
				} else {
					moveAmount = (dist - rest) / dist;
					if(a.isActive()){
						a.x -= delta.x * moveAmount;
						a.y -= delta.y * moveAmount;
					} else {
						b.x += delta.x * moveAmount;
						b.y += delta.y * moveAmount;
					}
				}
			}
		}
	}
};