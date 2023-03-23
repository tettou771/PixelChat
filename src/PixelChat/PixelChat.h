#pragma once
#include "ofxComponentUI.h"
#include "ofxChatGPT.h"
#include "ofxTextInputField.h"
#include "PixelView/DotView.h"
#include "ChatView/ChatView.h"


using namespace ofxComponent;

class PixelChat: public ofxComponentBase {
public:
    PixelChat();
    void onSetup() override;
	void onUpdate() override;
	void onDraw() override;
    void onKeyPressed(ofKeyEventArgs &key) override;
    void onLocalMatrixChanged() override;

    shared_ptr<DotView> dotView;
    shared_ptr<ChatView> chatView;

    ofxChatGPT chatGPT;
    ofxTextInputField inputField;
private:
};
