#include "ChatView.h"

ChatView::ChatView() {
    
}

void ChatView::onSetup() {
}

void ChatView::onUpdate() {
    
}

void ChatView::onDraw() {
    ofSetColor(40, 120);
    ofDrawRectangle(0, 0, getWidth(), getHeight());
    
}

void ChatView::addMessage(ofJson message) {
    auto newMsgObj = make_shared<MessageObject>(message);
    
    if (newMsgObj->isValid()) {
        messages.push_back(newMsgObj);
        addElement(newMsgObj);
        //updateMessagesPosition();
    }
    else {
        newMsgObj->destroy();
    }
}

void ChatView::updateMessagesPosition() {
    int y = 0;
    for (auto &m : messages) {
        m->setPos(0, y);
        y+= m->getHeight();
    }
}

shared_ptr<MessageObject> ChatView::getLastMessageObject() {
    if (messages.empty()) return nullptr;
    else return messages.back();
}
