#pragma once
#include "ofxComponentUI.h"
using namespace ofxComponent;

class Thumbnail : public ofxComponentBase {
public:
    Thumbnail(ofPixels &pixels);
    
    void onDraw() override;
    void onMousePressedOverComponent(ofMouseEventArgs &mouse);
    ofFbo& getFbo() {return fbo;}
    
    // サムネイルがクリックされた時に発生するイベント
    static ofEvent<ofPixels> selectedEvents;
    
private:
    ofFbo fbo;
    ofPixels *pixels;
};
