#include "DotObject.h"

DotObject::DotObject(ofColor *_color) {
    color = _color;
}

void DotObject::onStart() {
    
}

void DotObject::onUpdate() {
    
}

void DotObject::onDraw() {
    if (color) {
        ofFill();
        ofSetColor(*color);
        ofDrawRectangle(0, 0, getWidth(), getHeight());
    }
    
    ofNoFill();
    ofSetColor(100, 100);
    ofDrawRectangle(0.5, 0.5, getWidth()-1, getHeight()-1);
}

void DotObject::onMousePressed(ofMouseEventArgs &mouse) {
    onMouse(mouse);
}

void DotObject::onMouseDragged(ofMouseEventArgs &mouse) {
    onMouse(mouse);
}

void DotObject::onMouse(ofMouseEventArgs &mouse) {
    if (!isMouseOver()) return;
    if (color == nullptr) return;

    // draw the pixel
    if (mouse.button == OF_MOUSE_BUTTON_LEFT) {
        *color = ofColor(255);
    }else{
        *color = ofColor(0);
    }
}
