# PixelChat

ChatGPTでビットマップイメージを含んだチャットをするためのツールです。

テキストのメッセージと一緒にビットマップ（文字列の配列で表現）をJSONでまとめて送受信しています。

ただ会話するだけではなく、絵について話せます。

ただし、GPT-

[サンプル画像](screenshot.png)

## Build

openFrameworks 0.11.2 + Mac

ビルドする時は、openFlameworks-info.plist の以下の項目を変更してください。

High Resolution Capable NO -> YES

使用するには bin/data/config.json を作成し、あなたのOpenAIのアカウントで作成したAPI-KEYを書いてください。

```JSON
{
	"apiKey":"your-api-key"
}
```

## RequiredAddons

[ofxChatGPT](http://github.com/tettou771/ofxChatGPT)

[ofxGoogleIME](http://github.com/tettou771/ofxGoogleIME)

[ofxComponent](http://github.com/tettou771/)

[ofxComponentUI](http://github.com/tettou771/ofxComponentUI)

# Author

tettou771 + ChatGPT
