#pragma once
#include "ofxComponent.h"

using namespace ofxComponent;

class DotObject : public ofxComponentBase {
public:
    DotObject(ofColor *_color);
    void onStart() override;
    void onUpdate() override;
    void onDraw() override;
    void onMousePressed(ofMouseEventArgs& mouse) override;
    void onMouseDragged(ofMouseEventArgs& mouse) override;

    // draw
    void onMouse(ofMouseEventArgs& mouse);

    ofColor *color = nullptr;
};
