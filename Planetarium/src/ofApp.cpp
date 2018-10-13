#include "ofApp.h"

#define PIN_LED 12
#define PIN_BUTTON 11
#define PIN_LDR 0


void ofApp::setup() {
    ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);
    
    arduino.connect("/dev/cu.usbmodem14101");
    arduino.sendFirmwareVersionRequest(); // workaround for ofArduino/firmata bug
    
    ofBackground(ofColor::black);
    
    gui.setup();
    gui.add(drawGrid.set("Grid", false));
    gui.add(useLight.set("Light", true));
    
    light.setDirectional();
    light.setDiffuseColor(ofColor::white);
    light.enable();
}

void ofApp::update() {
    arduino.update();
    
    rotateY += 0.1;
}

void ofApp::draw() {
    
    ofEnableDepthTest();
    
    cam.setVFlip(true);
    cam.begin();
    
    if (doRotate) {
        ofRotateYDeg(rotateY);
    }
    
    if (useLight) {
        ofEnableLighting();
    } else {
        ofDisableLighting();
    }
    
    ofSetColor(ofColor::yellow);
    ofDrawSphere(0, 0, 0, 100);
    
    ofSetColor(ofColor::blue);
    ofDrawSphere(200, 0, 0, 50);
    
    if (drawGrid) ofDrawGrid(200);
    
    cam.end();
    ofDisableDepthTest();
    
    if (drawGui) {
        ofDisableLighting();
        gui.draw();
    }
}

void ofApp::setupArduino(const int& version) {
    // remove listener because we don't need it anymore
    ofRemoveListener(arduino.EInitialized, this, &ofApp::setupArduino);
    
    // print firmware name and version to the console
    ofLog() << "Arduino firmware found: " << arduino.getFirmwareName()
    << " v" << arduino.getMajorFirmwareVersion() << "." << arduino.getMinorFirmwareVersion();
    
    arduino.sendDigitalPinMode(PIN_LED, ARD_OUTPUT);
    arduino.sendDigitalPinMode(PIN_BUTTON, ARD_INPUT);
    arduino.sendAnalogPinReporting(PIN_LDR, ARD_ANALOG);
    
    // set listeners for pin events
    ofAddListener(arduino.EDigitalPinChanged, this, &ofApp::digitalPinChanged);
    ofAddListener(arduino.EAnalogPinChanged, this, &ofApp::analogPinChanged);
}


void ofApp::digitalPinChanged(const int& pinNum) {
    // get value with arduino.getDigital(pinNum)
    ofLogNotice() << "Digital Pin " << pinNum << " value: " << arduino.getDigital(pinNum) << endl;
    
    if (pinNum == PIN_BUTTON){
        int value =  arduino.getDigital(pinNum);
        
        if (value == 1){
            rotateY += 0.1;
        }
        
        if (value == 0){
            rotateY += 0;
        }
    }
}


void ofApp::keyPressed(int key) {
    if (key == 'l') useLight = !useLight;
    if (key == 'g') drawGrid = !drawGrid;
    if (key == ' ') drawGui = !drawGui;
    if (key == 'r') doRotate = !doRotate;
}


