#include "MessageObject.h"

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
            
            if (hasImage) {
                setHeight(60);
            } else {
                setHeight(50);
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
        thumb->setRect(ofRectangle(10, 10, 40, 40));
        addChild(thumb);
    }
}

void MessageObject::onDraw() {
    if (!valid) return;
    
    // サムネイルが左に入るので、右に寄せて描画
    ofDrawBitmapString(role, 60, 20);
    ofDrawBitmapString(message, 60, 40);
}
