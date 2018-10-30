#include "ofApp.h"
#define PIN_WATER 0

void ofApp::setup(){
    ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);
   
    arduino.connect("/dev/cu.usbmodem14101");
    arduino.sendFirmwareVersionRequest();
    
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
//    primitive = ofIcoSpherePrimitive(100 , 1);

}

void ofApp::update(){
    arduino.update();
    
    auto triangles = primitive.getMesh().getUniqueFaces();
    float displacement = sin(ofGetElapsedTimef() * 1) * 10;
    
    for (int i = 0; i < triangles.size(); ++i){
        ofVec3f faceNormal = triangles [i].getFaceNormal();
        for (int j = 0; j< 0; j++){
            triangles[i].setVertex(j,
                                   triangles[i].getVertex(j)+ faceNormal * displacement);
        }
    }

    primitive.getMesh().setFromTriangles(triangles);
}

void ofApp::draw(){
    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    
    primitive.rotateDeg(spinX, 0, 0, 0);
    primitive.rotateDeg(spinY, 0, 0, 0);
    
    if (drawWireFrame){
        primitive.drawWireframe();
    } else {
        primitive.draw();
    }
}

void ofApp::setupArduino(const int& version) {
    ofRemoveListener(arduino.EInitialized, this, &ofApp::setupArduino);
    
    ofLog() << "Arduino firmware found: " << arduino.getFirmwareName()
    << " v" << arduino.getMajorFirmwareVersion() << "." << arduino.getMinorFirmwareVersion();
    
    arduino.sendAnalogPinReporting(PIN_WATER, ARD_ANALOG);
    
  
    ofAddListener(arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}

void ofApp::analogPinChanged(const int& pinNum) {
    ofLogNotice() << "Analog Pin " << pinNum << " value: " << arduino.getAnalog(pinNum) << endl;
    
    if (pinNum == PIN_WATER){
        int value =  arduino.getAnalog(pinNum);
        
        if (value > 525 ){
            currentSize += 1;
            primitive = ofIcoSpherePrimitive(currentSize , 1);

        }
    }
}

void ofApp::keyPressed(int key){
    if (key == ' '){
        drawWireFrame = !drawWireFrame;
    }
    if (key == 'r'){
        currentSize = 0;
    }
}
