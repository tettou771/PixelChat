#pragma once
#include "ofxComponent.h"
#include "DotObject.h"
#include "../ChatView/Thumbnail.h"

class DotView : public ofxComponentBase {
public:
    DotView();
    
    void onStart() override;
    void onUpdate() override;
    void onDraw() override;
    void onLocalMatrixChanged() override;
    
    void setPixelsNum(int col, int row);
    int getNumCol(){return nCol;}
    int getNumRow(){return nRow;}
    int getNumDots();
    void setColors(vector<ofColor> _colors);
    
    // 画像が選択された時のイベント
    void onThumbnailSelected(ofPixels& pixels);
    
    // Tweak this colors;
    vector<ofColor> colors;
    
private:
    int nCol, nRow;
    void updateDotsPosition();
    vector<shared_ptr<DotObject> >dots;
};
