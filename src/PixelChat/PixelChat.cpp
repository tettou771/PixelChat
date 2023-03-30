#include "PixelChat.h"

PixelChat::PixelChat() {
    
}

void PixelChat::onSetup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // スクロールビューで表示
    chatView = make_shared<ChatView>();
    chatView->setAlign(ListBox::Align::FitWidth); // 要素を幅で合わせる
    auto scrollView = make_shared<ScrollView>(ScrollView::FitWidth);
    scrollView->setRect(ofRectangle(100, 100, 1000, 1000));
    scrollView->setContents(chatView);
    addChild(scrollView);
    
    dotView = make_shared<DotView>();
    dotView->setPixelsNum(16, 16);
    dotView->setRect(ofRectangle(1200, 100, 700, 700));
    addChild(dotView);
    
    for (auto &c : dotView->colors) {
        c = ofColor(0);
    }
        
    // setup chatGPT
    string apiKey;
    ofJson configJson = ofLoadJson("config.json");
    apiKey = configJson["apiKey"];
    chatGPT.setup(apiKey);
    
    // send system message
    string systemMessage = R"(You are having a conversation with a user through an API. To ensure the API understands, always reply with JSON formatted like the sample below, but omit any unnecessary elements:
    
    {
    "message": "This is a white triangle on black.",
    "image": {
    "width": 16,
    "height": 16,
    "pixels": [
    "0000000110000000",
    "0000000110000000",
    "0000001111000000",
    "0000001111000000",
    "0000011111100000",
    "0000011111100000",
    "0000111111110000",
    "0000111111110000",
    "0001111111111000",
    "0001111111111000",
    "0011111111111100",
    "0011111111111100",
    "0111111111111110",
    "0111111111111110",
    "1111111111111111",
    "1111111111111111"
    ]
    },
    "error": "Write any error messages here if there is an issue"
    }

    message: A message for the user, which they can read as text.
    image: Image data exchanged with the user. Always use 16x16 pixel images. If there are no changes to the image, you don't need to send the image element.
    width: The width of the image. Always set this to 16.
    height: The height of the image. Always set this to 16.
    pixels: The image data, with pixels arranged from the top left. Each horizontal row of the image is an element in the array. The number of characters corresponds to the image width, and the number of array elements corresponds to the image height. 0 represents a black pixel, and 1 represents a white pixel. For example, a triangle would be represented as in the sample JSON with 16x16 pixels. Always ensure there is no contradiction between the width and height.
    error: Write any error messages here if there is an issue.

    Unlike regular conversations, the image holds significant meaning. Additionally, the user's messages will be sent in the same format, so consider the images in the image elements when replying. Users may refer to the images, so be aware of this even when subjects are missing in the messages.

    Note that users will not be able to read your response if it's not in JSON format. Always put text messages in the message element, and always follow the JSON format above.
    )";
    
    chatGPT.setSystem(systemMessage);
    chatGPT.setModel("gpt-3.5-turbo");
    //chatGPT.setModel("gpt-4");
    
    /*
    vector<string> models;
    ofxChatGPT::ErrorCode errorCode;
    tie(models, errorCode) = chatGPT.getModelList();
    for (auto m : models) {
        ofLog() << "Model " << m;
    }
     */
    
    // フォントをロード
    string fontName;
#ifdef TARGET_OS_MAC
    fontName = "HiraKakuStdN-W4";
#elif defined WIN32
    fontName = "Meiryo.ttf";
#else
    fontName = OF_TTF_SANS;
#endif
    ofTrueTypeFontSettings settings(fontName, 30);
    settings.addRanges(ofAlphabet::Latin);
    settings.addRanges(ofAlphabet::Japanese);
    settings.addRange(ofUnicode::KatakanaHalfAndFullwidthForms);
    settings.addRange(ofUnicode::range{0x301, 0x303F}); // 日本語の句読点などの記号
    TextArea::font.load(settings);
    
    ime.setFont(fontName, 40);
    ime.enable();
    setWidth(ofGetWidth());
    setHeight(ofGetHeight());
    ime.setPos(20, getHeight() - 200);
}

void PixelChat::onUpdate(){
    
}

void PixelChat::onDraw(){

    /*
    // Display the conversation on the screen.
    stringstream conversationText;
    // Iterate through the conversation messages and build the display text.
    for (const ofJson &message : chatGPT.getConversation()) {
        conversationText << message["role"] << ": " << message["content"] << "\n";
    }
    // Draw the conversation text on the screen.
    ofDrawBitmapString("conversation:\n" + conversationText.str(), 20, 70);
     */

}

void PixelChat::onLocalMatrixChanged() {
    setWidth(ofGetWidth());
    setHeight(ofGetHeight());
    ime.setPos(20, getHeight() - 200);
}

void PixelChat::onKeyPressed(ofKeyEventArgs &key) {
    if (key.key == OF_KEY_RETURN && ofGetKeyPressed(OF_KEY_CONTROL)) {
        // Ignore if text is empty.
        if (ime.getString() == "") {
            return;
        }
        
        // send to chatgpt
        ofxChatGPT::ErrorCode errorCode;
        string message = ime.getString();

        // pixel chat data
        ofJson msgData;
        int width = dotView->getNumCol();
        int height = dotView->getNumRow();
        msgData["message"] = message;
        msgData["image"]["width"] = width;
        msgData["image"]["height"] = height;
        msgData["image"]["pixels"] = ofJson::array();
        for (int iy = 0; iy < height; ++iy) {
            string pixelsString = "";
            for (int ix = 0; ix < width; ++ix) {
                int i = ix + iy * width;
                char c = (dotView->colors[i].r > 100) ? '1' : '0';
                pixelsString += c;
            }
            msgData["image"]["pixels"].push_back(pixelsString);
        }

        ofJson newUserMsg;
        newUserMsg["message"]["role"] = "user";
        newUserMsg["message"]["content"] = msgData.dump();
        chatView->addMessage(newUserMsg);
        
        ofLogVerbose("PixelChat") << "User: " << newUserMsg;

        string gptResponse;
        tie(gptResponse, errorCode) = chatGPT.chatWithHistory(newUserMsg.dump());
        
        ofJson newGPTMsg;
        newGPTMsg["message"]["role"] = "assistant";
        newGPTMsg["message"]["content"] = gptResponse;
        
        ofLogVerbose("PixelChat") << "GPT: " << newGPTMsg;

        chatView->addMessage(newGPTMsg);
        
        // If the message has image, apply to DotView
        auto lastMsg = chatView->getLastMessageObject();
        if (lastMsg && lastMsg->hasImage) {
            dotView->onThumbnailSelected(lastMsg->pixels);
        }

        ime.clear();
    }
}
