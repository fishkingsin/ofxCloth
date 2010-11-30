#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	ofPoint gravity(0, 0);
	physics = new ofxPhysics2d(gravity);
	physics->checkBounds(false);
	physics->enableCollisions(true);
	physics->setNumIterations(10);
	
	bCreateParticles = false;
	mouseParticle = new ofxParticle();
	mouseParticle->setActive(false);
	mouseSpring = NULL;
	newParticle = NULL;
	
	newParticleIncrement = 0;
	bCreateParticleString = false;
	beginParticleString = NULL;
	endParticleString = NULL;
	
	bSetup = img.initGrabber(640,480);
	if(!bSetup)
		stillImg.loadImage("img.jpg");
	//img.loadImage("001_1024.jpg");
	grid = 20;
	_particles = new ofxParticle*[grid];
	for(int x = 0 ; x < grid  ;x++)
	{
		_particles[x] = new ofxParticle[grid];
	}
	makeSpring(ofGetWidth(),ofGetHeight());
	//makeSpring(img.getWidth(),img.getHeight());
	
	tuio.start(3333);
	max_tuio_constraints = 20;
	tuioParticle = new ofxParticle*[max_tuio_constraints];
	tuioSpring = new ofxSpring*[max_tuio_constraints];
	for(int i = 0 ; i<max_tuio_constraints ; i++ )
	{
		tuioParticle[i] = NULL;
		tuioSpring[i] = NULL;//new ofxSpring();
		
		
	}
}

//--------------------------------------------------------------
void testApp::update(){
	//tuio routin-----------------------------------------------
	if(bTuioMessage)tuio.getMessage();
	list<ofxTuioCursor*> cursorList = tuio.getTuioCursors();
	list<ofxTuioCursor*>::iterator tit;
	
	if(!bMousePressed)
	{
		for (tit=cursorList.begin(); tit != cursorList.end(); tit++) {
			ofxTuioCursor *blob = (*tit);
			int fingerId = blob->getFingerId();
			if(fingerId<max_tuio_constraints && blob->getX()>0 && blob->getY()>0)
			{
				if(tuioParticle[fingerId]==NULL)
				{
					tuioParticle[fingerId] = new ofxParticle();
					tuioParticle[fingerId]->setActive(false);
					tuioParticle[fingerId]->setRadius(50);
					physics->add(tuioParticle[fingerId]);
				}	
				else tuioParticle[fingerId]->set(blob->getX()*ofGetWidth(), blob->getY()*ofGetHeight());
				
				
				ofPoint tuioPoint = ofPoint(blob->getX()*ofGetWidth(), blob->getY()*ofGetHeight());
				ofxParticle* particleUnderTuio = physics->getParticleUnderPoint(tuioPoint);
				if(particleUnderTuio && tuioSpring[fingerId]!=NULL){
					float rest = tuioParticle[fingerId]->distanceTo(particleUnderTuio);
					
					tuioSpring[fingerId] = new ofxSpring(tuioParticle[fingerId], particleUnderTuio, rest);
					physics->add(tuioSpring[fingerId]);
					
				}
				
			}
			
		}
		
		if(cursorList.size()<1)
		{
			for(int i = 0 ; i<max_tuio_constraints; i++)
			{
				if(tuioParticle[i]!=NULL)
				{
					physics->deleteParticle(tuioParticle[i]);
					tuioParticle[i] = NULL;
				}
				if(tuioSpring[i]!=NULL)
				{
					physics->deleteConstraint(tuioSpring[i]);
					tuioSpring[i] = NULL;
				}
			}
		}
		
	}
	
	
	//tuio routin-----------------------------------------------
	
	
	img.update();
	
	
	mouseParticle->set(mouseX, mouseY);
	if(newParticle){
		newParticle->setRadius((sin(newParticleIncrement)*200) + 5);
		newParticleIncrement+= 0.01f;
		newParticle->moveTo(mouseX, mouseY);
	}
	if(bCreateParticles){
		if(ofGetFrameNum() % 5 == 0){
			float radius = ofRandom(5, 20);
			float mass = radius * 0.5f;
			ofxParticle* p = new ofxParticle(mouseX, mouseY, radius, mass);
			float randomStep = 5;
			ofPoint impulse(ofRandom(-randomStep, randomStep), ofRandom(-randomStep, randomStep));
			p->applyImpulse(impulse);
			particles.push_back(p);
			physics->add(p);
		}
	}
	for(int i=0; i<particles.size(); i++){
		if(particles[i]->y > ofGetHeight() + particles[i]->getRadius()){
			while(physics->getConstraintWithParticle(particles[i]) != NULL){
				physics->deleteConstraintsWithParticle(particles[i]);
			}
			physics->deleteParticle(particles[i]);
			particles.erase(particles.begin()+i);
		}
	}
	physics->update();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofTexture *ref;
	if(!bSetup)
		ref = &stillImg.getTextureReference();
	else {
		ref = &img.getTextureReference();
	}

	glEnable(GL_DEPTH_TEST);
	ref->bind();
	for (int i = 0; i < grid-1; i++)
	{
		for (int j = 0; j < grid-1; j++)
		{
			float texCoorX = img.getWidth()/grid*1.0f;
			float texCoorY = img.getHeight()/grid*1.0f;
			ofxParticle *p1 = &_particles[i][j];
			ofxParticle *p2 = &_particles[i+1][j];
			ofxParticle *p3 = &_particles[i+1][j+1];
			ofxParticle *p4 = &_particles[i][j+1];
			
			glBegin(GL_QUADS);
			
			glTexCoord2f((texCoorX*i),(texCoorY*j));
			glVertex2f(  p1->x, p1->y);
			
			glTexCoord2f((texCoorX*(i+1)),(texCoorY*j));
			glVertex2f(  p2->x,p2->y);
			
			glTexCoord2f((texCoorX*(i+1)),(texCoorY*(j+1)));
			glVertex2f(  p3->x, p3->y);
			
			glTexCoord2f((texCoorX*(i)),(texCoorY*(j+1)));
			glVertex2f(  p4->x, p4->y);
			
			glEnd();
		}
	}
	ref->unbind();
	glDisable(GL_DEPTH_TEST);
	ofNoFill();
	
	if (bRender)
	{
		ofSetColor(240, 240, 240);		physics->renderParticles();
		ofSetColor(100, 100, 100);		physics->renderConstraints();
	}
	
	//
	ofSetColor(255,255,255);
	string info = "num particles: " + ofToString(physics->getNumParticles()) + "\n" + 
	"num constraints: " + ofToString(physics->getNumConstraints()) + "\n" + 
	"fps: " + ofToString(ofGetFrameRate());
	ofDrawBitmapString(info, 20,20);
	//tuio.drawCursors();
	
	//	if(tuio.getTuioCursors().size()>0)
	//	{
	list<ofxTuioCursor*> cursorList = tuio.getTuioCursors();
	list<ofxTuioCursor*>::iterator tit;
	for (tit=cursorList.begin(); tit != cursorList.end(); tit++) {
		ofxTuioCursor *blob = (*tit);
		
		glColor3f(1.0,1.0,1.0);
		ofEllipse(blob->getX()*ofGetWidth(), blob->getY()*ofGetHeight(), 10.0, 10.0);
		string str = "FingerId: "+ofToString(blob->getFingerId())+"\nSessionId: "+ofToString((int)(blob->getSessionId()));
		
		ofDrawBitmapString(str, blob->getX()*ofGetWidth()-10.0, blob->getY()*ofGetHeight()+25.0);
	}
	//	}	
}

void testApp::createParticleString(ofxParticle*begin, ofxParticle*end, int numParticles){
	
	for(int y = 0 ; y < 2 ; y++)
	{
		ofPoint step = (*end - *begin) / (numParticles-1);
		ofPoint particlePos = *begin;
		ofxParticle* prevParticle = begin;
		prevParticle->setActive(false);
		end->setActive(false);
		
		
		particlePos += step;
		if(!physics->has(begin)) physics->add(begin);
		if(!physics->has(end)) physics->add(end);
		for(int i=1; i<numParticles; i++){
			ofxParticle* p = new ofxParticle(particlePos, 5);
			float rest = prevParticle->distanceTo(p);
			ofxSpring* s = new ofxSpring(prevParticle, p, rest, 1.1);
			physics->add(p);
			physics->add(s);
			if(i==numParticles-1)p->setActive(false);
			prevParticle = p;
			particlePos += step;
			
			
		}
		step.y+=y*10;
		float rest = prevParticle->distanceTo(end);
		ofxSpring* s = new ofxSpring(prevParticle, end, rest);
		physics->add(s);
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key == ' '){
		bCreateParticleString = true;
	}
	if(key==OF_KEY_RETURN)
	{
		bRender = !bRender;
	}
	if(key=='t')bTuioMessage=!bTuioMessage;
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == 'c' or key == 'C'){
		physics->deleteAll();
		particles.clear();
		for(int x = 0 ; x < grid  ;x++)
		{
			for(int y = 0 ; y < grid  ;y++)
			{
				physics->add(&_particles[x][y]);
			}
		}
		for (int i = 0; i < grid; i++)
		{
			for (int j = 0; j < grid; j++)
			{
				
				if (j > 0)
				{
					ofxParticle *p1 = &_particles[i][j - 1];
					ofxParticle *p2 = &_particles[i][j];
					float rest = gridSizeY;//p1->distanceTo(p2);
					ofxSpring* s = new ofxSpring(p1, p2, rest, strength);
					physics->add(s);
				}
				if (i > 0)
				{
					ofxParticle *p1 = &_particles[i - 1][j];
					ofxParticle *p2 = &_particles[i][j];
					
					float rest = gridSizeX;// p1->distanceTo(p2);
					ofxSpring* s = new ofxSpring(p1, p2, rest, strength);
					physics->add(s);
				}
			}
		}
	}
	if(key == ' '){
		
		bCreateParticleString = false;
	}
	
	if(key == 'b' or key == 'B'){
		physics->checkBounds(!physics->boundsCheck());
	}
	
	if(key == ' '){
		if(beginParticleString){
			physics->deleteParticle(beginParticleString);
			beginParticleString = NULL;
		}
	}
	if(key=='f')
	{
		ofToggleFullscreen();
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	bMousePressed = true;
	if(button == 0){
		if(bCreateParticleString){
			beginParticleString = new ofxParticle(x, y, 5);
			beginParticleString->setActive(false);
			physics->add(beginParticleString);
		} else {
			ofPoint mousePoint = ofPoint(x, y);
			ofxParticle* particleUnderMouse = physics->getParticleUnderPoint(mousePoint);
			if(particleUnderMouse){
				float rest = mouseParticle->distanceTo(particleUnderMouse);
				mouseSpring = new ofxSpring(mouseParticle, particleUnderMouse, rest);
				physics->add(mouseSpring);
			} else {
				if(newParticle == NULL){
					newParticle = new ofxParticle(x, y, 1);
					newParticle->setActive(false);
					physics->add(newParticle);
					particles.push_back(newParticle);
				} 
			}
			
		}
	}
	if(button == 2){
		bCreateParticles = true;
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	bMousePressed = false;
	if(bCreateParticleString){
		if(beginParticleString){
			endParticleString = new ofxParticle(x, y, 5);
			endParticleString->setActive(false);
			int numParticles = (int)(beginParticleString->distanceTo(endParticleString)/10);
			createParticleString(beginParticleString, endParticleString, numParticles);
			beginParticleString = NULL;
			endParticleString = NULL;
		}
		bCreateParticleString = false;
	} 
	
	if(mouseSpring){
		physics->deleteConstraint(mouseSpring);
		mouseSpring = NULL;
	}
	if(newParticle){
		newParticle->setActive(true);
		newParticle->setMass(newParticle->getRadius() * 0.5);
		newParticle = NULL;
		newParticleIncrement = 0;
	}
	bCreateParticles = false;
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

void testApp::makeSpring(int _width,int _height)
{
	grid = 20;
	gridSizeX = _width/grid*1.0;
	gridSizeY = _height/grid*1.0;
	
	//	gridSizeX = img.getWidth()/grid;
	//	gridSizeY = img.getHeight()/grid;
	gridPosX = ofGetWidth()/2-_width/2;
	gridPosY =	ofGetHeight()/2-_height/2;
	strength = 0.1;
	
	pSize = 10;
	
	for(int x = 0 ; x < grid  ;x++)
	{
		for(int y = 0 ; y < grid  ;y++)
		{
			ofPoint particlePos = ofPoint(gridPosX+x*gridSizeX,gridPosY+y*gridSizeY);
			ofxParticle* p = new ofxParticle(particlePos, pSize);
			_particles[x][y] = *p;
			physics->add(&_particles[x][y]);
			
			
		}
	}
	for (int i = 0; i < grid; i++)
    {
        for (int j = 0; j < grid; j++)
        {
			
            if (j > 0)
            {
                ofxParticle *p1 = &_particles[i][j - 1];
                ofxParticle *p2 = &_particles[i][j];
				float rest = p1->distanceTo(p2);
				ofxSpring* s = new ofxSpring(p1, p2, gridSizeY, strength);
				physics->add(s);
            }
            if (i > 0)
            {
                ofxParticle *p1 = &_particles[i - 1][j];
                ofxParticle *p2 = &_particles[i][j];
				
				float rest = p1->distanceTo(p2);
				ofxSpring* s = new ofxSpring(p1, p2, gridSizeX, strength);
				physics->add(s);
            }
        }
    }
    for(int i = 0 ; i < grid; i++)
    {
		_particles[i][0].setActive(false);
		_particles[0][i].setActive(false);
		_particles[grid - 1][i].setActive(false);
		_particles[i][grid - 1].setActive(false);
    }
	//	_particles[0][0].setActive(false);
	//	_particles[grid-1][0].setActive(false);
	//_particles[grid - 1][0].setActive(false);
	//_particles[grid-1][grid - 1].setActive(false);
	
}

