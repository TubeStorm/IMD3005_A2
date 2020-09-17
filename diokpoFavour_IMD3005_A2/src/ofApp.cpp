#include "ofApp.h"
#include "affine.hpp"

//--------------------------------------------------------------
void ofApp::setup()
{
    //color 1 threshold
	m_threshold1 = 43;
	m_minRadius1 = 4.0f;
	m_maxRadius1 = 150.0f;
    
    //color 2 threshold
    m_threshold2 = 43;
    m_minRadius2 = 4.0f;
    m_maxRadius2 = 150.0f;

	m_gui.setup();
   
    
	if ( USE_WEB_CAM ) {
		m_webcam.setup( 320, 240 );
	}
	else {
		m_video.load("juggling.mp4");
        m_lightbulb.load("lightBulb_img.png");
        m_errorpage.load("systemFailure_img.png");

		m_video.setLoopState(OF_LOOP_NORMAL);
		m_video.play();
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
    //set threshold for color (range in which we will accept "similar" matches.
	m_contourFinder1.setThreshold( m_threshold1 );
	m_contourFinder1.setMinAreaRadius( m_minRadius1 );							//filter out sizes of blobs detected
	m_contourFinder1.setMaxAreaRadius( m_maxRadius1 );
	m_contourFinder1.setTargetColor( m_trackedBall1, ofxCv::TRACK_COLOR_RGB );
    
    m_contourFinder2.setThreshold( m_threshold2 );
    m_contourFinder2.setMinAreaRadius( m_minRadius2 );
    m_contourFinder2.setMaxAreaRadius( m_maxRadius2 );
    m_contourFinder2.setTargetColor( m_trackedBall2, ofxCv::TRACK_COLOR_RGB );
 
    
	if ( USE_WEB_CAM ) {
		m_webcam.update();

		if ( true == m_webcam.isFrameNew() ) {
			m_contourFinder1.findContours(m_webcam);
            m_contourFinder2.findContours(m_webcam);
		}
	}
	else {
		m_video.update();	//update video

		if (true == m_video.isFrameNew()) {
			m_contourFinder1.findContours(m_video);
            m_contourFinder2.findContours(m_video);
		}
	}
    
    
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground( ofColor::black );
	ofSetColor( ofFloatColor( 1.0f, 1.0f, 1.0f, 1.0f ) );		//reset color/tint

	if ( USE_WEB_CAM ) {
		m_webcam.draw(0, 0);
	}
	else {
		m_video.draw(0, 0);
        
        
	}
    //draw the video frame
	int numContours1 = m_contourFinder1.getContours().size();
    int numContours2 = m_contourFinder2.getContours().size();
    

    //now we are going to draw all contours / blobs
    cv::Point2f center;
    cv::Rect rect;
  
    cv::Point2f center2;
    cv::Rect rect2;
    
    
    
    for ( int i = 0; i < numContours1; ++i )
    {
        ofSetColor( ofColor::white );
        ofDrawRectangle(0,0,190,270);
        
        ofNoFill();
        
        ofSetColor( ofFloatColor( 1.0f, 0.0f, 0.0f ) );
        
        center = m_contourFinder1.getCenter( i );
        rect = m_contourFinder1.getBoundingRect( i );
       double area = m_contourFinder1.getContourArea( i );
        
        //drawing bounding box
        ofDrawRectangle( rect.x, rect.y, rect.width, rect.height  );
        ofSetColor( ofFloatColor( 0.0f, 1.0f, 0.0f ) );
        
        ofDrawBitmapString( "center" +ofToString( center.x ) + ", " + ofToString( center.y ), center.x, center.y );
        ofDrawBitmapString("1ST COLOR DETECTED", rect.x, rect.y);
        
        const std::vector<cv::Point> contour1 = m_contourFinder1.getContour( i );
        
        //drawing blob
        ofBeginShape();
        for ( int j = 0; j < contour1.size(); ++j )
        {
            ofVertex( contour1[ j ].x, contour1[ j ].y );
        }
        ofEndShape( true );
        
        //drawing our selected color
        ofFill();
        ofSetColor( m_trackedBall1 );
        ofDrawCircle(105, 60, 60); //(r,x,y)
        ofSetColor( ofColor::white );
    }
    
    for ( int i = 0; i < numContours2; ++i )
    {

        ofNoFill();
        
        ofSetColor( ofFloatColor( 1.0f, 0.0f, 0.0f ) );
        
        center2 = m_contourFinder2.getCenter( i );
        rect2 = m_contourFinder2.getBoundingRect( i );
        double area2 = m_contourFinder2.getContourArea( i );
        
        //drawing bounding box2
        ofDrawRectangle( rect2.x, rect2.y, rect2.width, rect2.height  );
        ofSetColor( ofFloatColor( 0.0f, 1.0f, 0.0f ) );
    
        ofDrawBitmapString( "center" +ofToString( center2.x ) + ", " + ofToString( center2.y ), center2.x, center2.y );
        ofDrawBitmapString("2ND COLOR DETECTED", rect2.x, rect2.y);
        
        const std::vector<cv::Point> contour2 = m_contourFinder2.getContour( i );
        
        //drawing blob2
        ofBeginShape();
        for ( int j = 0; j < contour2.size(); ++j )
        {
            ofVertex( contour2[ j ].x, contour2[ j ].y );
        }
        ofEndShape( true );
        
        //drawing our selected color
        ofFill();
        ofSetColor( m_trackedBall2 );
        ofDrawCircle(105, 199, 60); //(r,x,y)
        ofSetColor( ofColor::white );
        
        //when object is detected
        if(m_trackedBall2 != ofColor::white ){
           // cout<<"hyy";
        }
        
    }
    
    
    //if the two objects touch
    if(numContours1 > numContours2){
        cout<< "the two objects touched" <<endl;
    }
    
	//draw our GUI sliders for adjusting thresholds
	m_gui.begin();
        ImGui::Text( "s for pause" );
    
    
    ImGui::Text( "press 'q' for your first color to be RED" );
    ImGui::Text( "press 'W' for your first color to be BLUE" );
    ImGui::Separator();
    ImGui::SliderInt( "Threshold1", &m_threshold1, 0, 255 );
    ImGui::SliderFloat( "Min Radius1", &m_minRadius1, 0.0f, 1000.0f );
    ImGui::SliderFloat( "Max Radius1", &m_maxRadius1, 0.0f, 1000.0f );
    ImGui::Separator();
    ImGui::Text( "press 'e' for your second color to be RED" );
    ImGui::Text( "press 'r' for your second color to be BLUE" );
    ImGui::Separator();
    ImGui::SliderInt( "Threshold2", &m_threshold2, 0, 255 );
    ImGui::SliderFloat( "Min Radius2", &m_minRadius2, 0.0f, 1000.0f );
    ImGui::SliderFloat( "Max Radius2", &m_maxRadius2, 0.0f, 1000.0f );
    ImGui::Separator();
    ImGui::Spacing();
    
    
        ImGui::Text( "YOU CAN ALSO USE THE MOUSE BUTTONS TO SELECT YOUR COLORS" );
        ImGui::Text( "'middle click' to select your first color" );
        ImGui::Text( "'right click' to select your second color" );
    
    
    m_gui.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if ( key == 's' )
	{
		if ( !USE_WEB_CAM ) {
			m_video.setPaused(!m_video.isPaused());
		}
	}
    
    if ( key == 'q')
    {
        m_trackedBall1.r = 255;
        m_trackedBall1.g = 0;
        m_trackedBall1.b = 0;
    }
    if ( key == 'w')
    {
        m_trackedBall1.r = 0;
        m_trackedBall1.g = 7;
        m_trackedBall1.b = 255;
    }
    
    if ( key == 'e')
    {
        m_trackedBall2.r = 255;
        m_trackedBall2.g = 0;
        m_trackedBall2.b = 0;
    }
    if ( key == 'r')
    {
        m_trackedBall2.r = 0;
        m_trackedBall2.g = 7;
        m_trackedBall2.b = 255;
    }
    
    
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
  	//if right mouse button clicked then we select color under mousepointer (on video)
	//may want to check bounds ... try clicking outside video ;)

    if ( button == OF_MOUSE_BUTTON_MIDDLE )
	{
		if (USE_WEB_CAM)  {
			int index = (x + y * m_webcam.getWidth()) * m_webcam.getPixels().getBytesPerPixel();
			m_trackedBall1.r = m_webcam.getPixels()[index];
			m_trackedBall1.g = m_webcam.getPixels()[index + 1];
			m_trackedBall1.b = m_webcam.getPixels()[index + 2];
		}
		else {
            if((x < m_video.getWidth()) || (y < m_video.getHeight())){
			int index = (x + y * m_video.getWidth()) * m_video.getPixels().getBytesPerPixel();
			m_trackedBall1.r = m_video.getPixels()[index];
			m_trackedBall1.g = m_video.getPixels()[index + 1];
			m_trackedBall1.b = m_video.getPixels()[index + 2];
		}
        }
	}
  else if (button == OF_MOUSE_BUTTON_RIGHT)
  {
      if (USE_WEB_CAM)  {
        int index = (x + y * m_webcam.getWidth()) * m_webcam.getPixels().getBytesPerPixel();
        m_trackedBall2.r = m_webcam.getPixels()[index];
        m_trackedBall2.g = m_webcam.getPixels()[index + 1];
        m_trackedBall2.b = m_webcam.getPixels()[index + 2];
    }
    else {
        if((x < m_video.getWidth()) || (y < m_video.getHeight())){
        int index = (x + y * m_video.getWidth()) * m_video.getPixels().getBytesPerPixel();
        m_trackedBall2.r = m_video.getPixels()[index];
        m_trackedBall2.g = m_video.getPixels()[index + 1];
        m_trackedBall2.b = m_video.getPixels()[index + 2];
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

