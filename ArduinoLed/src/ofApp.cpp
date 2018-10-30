#include "ofApp.h"

#define PIN_LED 12
#define PIN_BUTTON 11
#define PIN_LDR 0


void ofApp::setup() { 
    // set listener - call ofApp::setupArduino when arduino is ready for use
    ofAddListener(arduino.EInitialized, this, &ofApp::setupArduino);
    
    // connect to the Arduino
    // use the same device name used in the Arduino IDE
    arduino.connect("/dev/cu.usbmodem14101");
    arduino.sendFirmwareVersionRequest(); // workaround for ofArduino/firmata bug
    
    
    x = ofRandom(0, ofGetWidth());
    y = ofRandom(0, ofGetHeight());
    
    speedX = ofRandom(-2, 2);
    speedY = ofRandom(-2, 2);
    
    radius = 40;
    color = ofColor::blueSteel;
}

void ofApp::update() {
    arduino.update();
    
    if (x < 0) {
        x = 0;
        speedX = -speedX;
    }
    
    if (x > ofGetWidth()) {
        x = ofGetWidth();
        speedX = -speedX;
    }
    if (y < 0) {
        y = 0;
        speedY = -speedY;
    }
    
    if (y > ofGetHeight()) {
        y = ofGetHeight();
        speedY = -speedY;
    }
    
    x += speedX;
    y += speedY;
}

void ofApp::draw() {
    ofSetColor(color);
    ofDrawCircle(x, y, radius);
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
            speedX = 0;
            speedY = 0;
        }
        
        if (value == 0){
            speedX = 1;
            speedY = 1;
        }
    }
}

void ofApp::analogPinChanged(const int& pinNum) {
    // get value with arduino.getAnalog(pinNum));
    ofLogNotice() << "Analog Pin " << pinNum << " value: " << arduino.getAnalog(pinNum) << endl;
   
    if (pinNum == PIN_LDR){
        int value =  arduino.getAnalog(pinNum);
    
        if (value > 200) {
            color = ofColor::red;
        }
        
        if (value < 200) {
            color = ofColor::blueSteel;
        }
    }
}

void ofApp::keyPressed(int key) {
}

void ofApp::mousePressed(int x, int y, int button) {
    // switch the LED on
    arduino.sendDigital(PIN_LED, ARD_HIGH);
}

void ofApp::mouseReleased(int x, int y, int button) {
    // switch the LED off
    arduino.sendDigital(PIN_LED, ARD_LOW);
}

