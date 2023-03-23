#pragma once
#include "ofxComponentUI.h"
#include "Thumbnail.h"
using namespace ofxComponent;

class MessageObject : public ListElement {
public:
    MessageObject(ofJson json);
    
    void onStart() override;
    void onDraw() override;;
    
    ofJson raw, data;
    ofPixels pixels;
    bool hasImage;
    string message, role;
    bool isValid() {return valid;}
    
private:
    bool valid = false;
};
