#include "Thumbnail.h"

ofEvent<ofPixels> Thumbnail::selectedEvents;

Thumbnail::Thumbnail(ofPixels &pixels) {
    this->pixels = &pixels;
    
    // make thumbnail
    ofFboSettings s;
    s.width = pixels.getWidth();
    s.height = pixels.getHeight();
    s.maxFilter = GL_NEAREST;
    ofImage img;
    img.setFromPixels(pixels);
    fbo.allocate(s);
    fbo.begin();
    img.draw(0, 0);
    fbo.end();
}

void Thumbnail::onDraw() {
    if (!fbo.isAllocated()) return;
    fbo.draw(0, 0, getWidth(), getHeight());
}

void Thumbnail::onMousePressedOverComponent(ofMouseEventArgs &mouse) {
    ofNotifyEvent(selectedEvents, *pixels);
}
