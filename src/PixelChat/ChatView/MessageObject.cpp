#include "MessageObject.h"

ofTrueTypeFont TextArea::font;

void TextArea::onStart() {
}

void TextArea::onUpdate() {
    if (!delayText) return;
    
    // delayTextがtrueのとき、つらつらと一文字ずつ描画するためにmessageに一つずつ追加する
    
    // 今の経過時間
    float elapsedTime = ofGetElapsedTimef() - startTime;
    
    // 1秒あたりに表示する文字数
    float cps = 20.0; // char / sec
    
    // 今あるべき文字数
    int expectedNum = MIN((int)u32message.length(), cps * elapsedTime);
    
    for (int i = delayTextIndex; i < expectedNum; ++i) {
        string s = ofxGoogleIME::UTF32toUTF8(u32message[i]);
        float w = font.stringWidth(message + s);
        if (w > getWidth()) {
            message += '\n' + s;
        } else {
            message += s;
        }
    }
    delayTextIndex = expectedNum;

    // 文字列の大きさに合わせてサイズを変更
    auto bbox = font.getStringBoundingBox(message, 0, font.getSize());
    setHeight(bbox.y + bbox.getHeight());

    if (delayTextIndex == u32message.length()) {
        delayText = false;
    }

}

void TextArea::onDraw() {
    // サムネイルが左に入るので、右に寄せて描画
    font.drawString(message, 0, font.getSize());
}

void TextArea::setString(string &str) {
    u32message = ofxGoogleIME::UTF8toUTF32(str);
    message = "";
    for (int i=0; i<u32message.size(); ++i) {
        string s = ofxGoogleIME::UTF32toUTF8(u32message[i]);
        float w = font.stringWidth(message + s);
        if (w > getWidth()) {
            message += '\n' + s;
        } else {
            message += s;
        }
    }
    
    // 文字列の大きさに合わせてサイズを変更
    auto bbox = font.getStringBoundingBox(message, 0, font.getSize());
    setHeight(bbox.y + bbox.getHeight());
}

void TextArea::setStringDelay(string &str) {
    u32message = ofxGoogleIME::UTF8toUTF32(str);
    startTime = ofGetElapsedTimef();
    delayText = true;
    delayTextIndex = 0;
}

MessageObject::MessageObject(ofJson json) {
    raw = json;
    valid = false;
    hasImage = false;
    
    if (raw.count("message") > 0 && raw["message"].is_object() &&
        raw["message"].count("role") > 0 && raw["message"]["role"].is_string() &&
        raw["message"].count("content") > 0 && raw["message"]["content"].is_string()) {
        
        role = raw["message"]["role"];
        
        // メッセージがJSONになっているかどうか確認
        string jsonString = raw["message"]["content"];
        bool parseSuccess = false;
        try {
            data = ofJson::parse(jsonString);
            parseSuccess = true;
        }
        catch (std::exception& e) {
            // エラーをキャッチして、メッセージを出力する
            std::cerr << "JSON parse error: " << e.what() << std::endl;
        }
        
        // JSONとして解釈できたら次に進む
        if (parseSuccess) {
            valid = true;
            
            // 画像データがあるかどうか確認
            if (data.count("image") > 0 && data["image"].is_object()) {
                auto imgJson = data["image"];
                if (imgJson.count("width") > 0 && imgJson["width"].is_number() &&
                    imgJson.count("height") > 0 && imgJson["height"].is_number() &&
                    imgJson.count("pixels") > 0 && imgJson["pixels"].is_array()) {
                    hasImage = true;
                    
                    int width = imgJson["width"];
                    int height = imgJson["height"];
                    
                    // make pixels
                    pixels.allocate(width, height, OF_IMAGE_COLOR);
                    for (int iy = 0; iy < imgJson["pixels"].size(); ++iy) {
                        if (iy >= height) break; // 高さより多い要素数の時
                        string pixelsString = imgJson["pixels"][iy];
                        for (int ix = 0; ix < pixelsString.length(); ++ix) {
                            if (ix >= width) break; // 幅より多い文字数の時
                            
                            ofColor c = pixelsString[ix] == '1' ? ofColor(255) : ofColor(0);
                            int i = ix + iy * width;
                            pixels[i*3+0] = c.r;
                            pixels[i*3+1] = c.g;
                            pixels[i*3+2] = c.b;
                        }
                    }
                }
            }
            
            if (data.count("message") > 0 && data["message"].is_string()) {
                message = data["message"];
            }
        } else {
            ofLogError("MessageObject") << "Data is not valid: " << data;
        }
    }else{
        ofLogError("MessageObject") << "JSON is not valid: " << data;
    }
}

void MessageObject::onStart() {
    if (hasImage) {
        // make thumbnail
        auto thumb = make_shared<Thumbnail>(pixels);
        thumb->setRect(ofRectangle(10, 10, 60, 60));
        addChild(thumb);
    }
    
    // jsonをもとに、テキストオブジェクトを作る
    textArea = make_shared<TextArea>();
    float margin = 15;
    textArea->setRect(ofRectangle(100, 70, getWidth() - 100 - margin, 0));
    
    // もしassistantのメッセージなら、ツラツラと出てくるように描画する
    if (raw["message"]["role"] == "assistant") {
        textArea->setStringDelay(message);
    }
    else {
        textArea->setString(message);
    }

    float h = textArea->getPos().y + textArea->getHeight() + margin;
    h = MAX(h, 50); // 最低50pixの大きさを持つ
    setHeight(h);
    addChild(textArea);
    
    // 親オブジェクトにリサイズ要求
    auto p = getParentBox();
    if (p) {
        p->updateList();
    }
}

void MessageObject::onUpdate() {
    if (textArea && textArea->delayText) {
        // 文字列がつらつらと出ている最中はリサイズ
        float margin = 15;
        float h = textArea->getPos().y + textArea->getHeight() + margin;
        h = MAX(h, 50); // 最低50pixの大きさを持つ
        setHeight(h);
        
        // 親オブジェクトにリストの調整要求
        auto p = getParentBox();
        if (p) {
            p->updateList();
        }
    }
}

void MessageObject::onDraw() {
    if (!valid) return;
    
    // ざぶとん
    ofFill();
    ofSetColor(0, 0, 100, 100);
    ofDrawRectangle(1, 1, getWidth() - 2, getHeight() - 2);
    
    // サムネイルが左に入るので、右に寄せて描画
    ofSetColor(180);
    TextArea::font.drawString(role, 100, 40);
}
