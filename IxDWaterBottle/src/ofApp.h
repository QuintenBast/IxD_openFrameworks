#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    
    ofIcoSpherePrimitive primitive;
    
    float spinX = 1;
    float spinY = 2;
    
    float amount = 0;
    
    bool drawWireFrame = true;
    
    int currentSize = 0;
    
private:
    ofArduino arduino;
    
    void setupArduino(const int& version);
    
    void digitalPinChanged(const int& pinNum);
    
    void analogPinChanged(const int& pinNum);
    
    ofColor color;    
};
