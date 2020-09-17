#pragma once

#include "ofMain.h"

#include "ofxImGui.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include<cmath>

class ofApp : public ofBaseApp{

	//static const bool USE_WEB_CAM = false;		//a const to switch between web cam and video
	#define USE_WEB_CAM false

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int x, int y, int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

    ofxImGui::Gui				m_gui;
    ofxPanel                    gui;

	ofVideoPlayer			m_video;
	ofVideoGrabber			m_webcam;			//a class for managing a webcam
    ofImage                 m_lightbulb;
    ofImage                 m_errorpage;
    
	ofxCv::ContourFinder	m_contourFinder1;
    ofxCv::ContourFinder    m_contourFinder2;

	ofColor                 m_trackedBall1;
    ofColor                 m_trackedBall2;
    
	int  					m_threshold1;
	float					m_minRadius1;
	float					m_maxRadius1;
    int                     m_threshold2;
    float                   m_minRadius2;
    float                   m_maxRadius2;

};
