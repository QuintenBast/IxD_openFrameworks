#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    ofxPanel gui;
    
    ofParameter<bool> drawGrid;
    ofParameter<bool> useLight;
    bool drawGui = true;
    
    ofLight light;
    ofEasyCam cam;
    
private:
    ofArduino arduino;
    
    void setupArduino(const int& version);

    void digitalPinChanged(const int& pinNum);
    
    void analogPinChanged(const int& pinNum);
    
    float rotateY;
    
    bool doRotate;
};
