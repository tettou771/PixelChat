#include "DotView.h"

DotView::DotView() {
    nCol = nRow = 0;
}

void DotView::onStart() {
    ofAddListener(Thumbnail::selectedEvents, this, &DotView::onThumbnailSelected);
}

void DotView::onUpdate() {
    
}

void DotView::onDraw() {
    
}


void DotView::onLocalMatrixChanged() {
    updateDotsPosition();
}

void DotView::setPixelsNum(int col, int row) {
    nCol = MAX(0, col);
    nRow = MAX(0, row);
    
    // Set colors num to fit pixels;
    if (colors.size() != getNumDots()) {
        while (colors.size() > getNumDots()) {
            colors.erase(colors.end() - 1);
        }
        while (colors.size() < getNumDots()) {
            colors.push_back(ofColor());
        }
    }
    
    updateDotsPosition();
}

int DotView::getNumDots() {
    return nCol * nRow;
}

void DotView::setColors(vector<ofColor> _colors) {
    colors = _colors;
}

void DotView::onThumbnailSelected(ofPixels &pixels) {
    auto &p = pixels;
    for (int i=0; i<getNumDots(); ++i) {
        if (i >= p.getWidth() * p.getHeight()) break;
        
        colors[i].r = p[i*3+0];
        colors[i].g = p[i*3+1];
        colors[i].b = p[i*3+2];
    }
}

void DotView::updateDotsPosition() {
    int w = getWidth();
    int h = getHeight();
    float sizeW = float(w) / nCol;
    float sizeH = float(h) / nRow;
    
    int nDots = getNumDots();
    if (dots.size() != nDots) {
        for (auto p : dots) {
            p->destroy();
        }
        dots.clear();
        for (int i=0; i<nDots; ++i) {
            auto p = make_shared<DotObject>(&colors[i]);
            dots.push_back(p);
            addChild(p);
        }
    }
    
    // Update position/size
    for (int ix = 0; ix < nCol; ++ix) {
        for (int iy = 0; iy < nRow; ++iy) {
            int ip = ix + iy * nCol;
            float x = ix * sizeW;
            float y = iy * sizeH;
            dots[ip]->setRect(ofRectangle(x, y, sizeW, sizeH));
        }
    }
}
