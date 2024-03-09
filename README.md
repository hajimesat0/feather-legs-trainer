# 羽根足(feather-legs-trainer)

羽根のように軽いバドミントンフットワークを育成するためのトレーニングシステムソフトウェア

Software for electric training system to help you develop feather-light footwork for badminton.

## システムの目指すところ

* バドミントンフットワークの強化
* 選手(プレイヤー)の視覚的な認識能力からフットワークへの反射神経を育成する
* 電子制御により、柔軟な練習メニューの組み立てと(人力に対して)高精度な記録をもとに、データに基づく効率的な育成を促進する

<img src="images\目的レベルユースケース図.drawio.svg" alt="目的レベルユースケース図" title="目的レベルユースケース図">

## 操作ユースケース

コーチ・プレイヤーの行う操作

<img src="images\操作レベルユースケース図.drawio.svg" alt="操作レベルユースケース図" title="操作レベルユースケース図">

## 操作者指示時の反応時間計測

1. コーチはスマホからLED点灯を指示する
2. ESPは指示されたLEDを点灯する
3. ESPは計測開始する
4. プレイヤーは指示されたボタンを押す
5. ESPは計測停止する
6. ESPは反応時間(=停止時刻-開始時刻)をスマホへ送信する
7. スマホは反応時間を記録する
   1. 複数回の記録を保持できること
   2. 指示したボタンを合わせて記録すること
8. コーチは反応時間を確認する

<img src="images\反応時間計測シーケンス図.drawio.svg" alt="反応時間計測シーケンス図" title="反応時間計測シーケンス図">

## パラメータを設定して連続点灯と反応時間の記録計測

コーチから逐次の点灯指示を出すのではなく、一連の点灯指示を設定して、それに対する反応時間を記録する。
練習強度をパラメータとして設定しておくことで、同じ強度の練習を繰り返せることと、期間を空けて同強度の練習を実施するときのスキルアップを計測できる。

1. コーチは練習強度を設定する
2. スマホ上で再生されているwebページは、設定された練習強度に従って、一連の点灯指示をESPに送信する
3. ESPは指示されたLEDを点灯する
4. ESPは計測開始する
5. プレイヤーは指示されたボタンを押す
6. ESPは計測停止する
7. ESPは反応時間(=停止時刻-開始時刻)をスマホへ送信する
8. スマホは反応時間を記録する
9. （パラメータに従って繰り返し点灯する場合は3へ戻る）
10. 結果を表示する
    1.  結果には各計測記録とその平均値が含まれる

<img src="images\連続点灯反応時間計測シーケンス図.drawio.svg" alt="連続点灯反応時間計測シーケンス図" title="連続点灯反応時間計測シーケンス図">

### パラメータ

* 点灯間隔[ms]
* 点灯回数


## ESP側状態遷移図

ボタンのLED点灯をトリガとして反応時間を計測する。
ボタンは複数個に対応できることを想定するが、同時に2つ以上のボタンに対して時間測定の指示は行わないこととする。つまり、時間計測はいずれかのボタン1つに対してのみ。

<img src="images\ボタン制御状態遷移図.drawio.svg" alt="(ESP側)ボタン制御状態遷移図" title="(ESP側)ボタン制御状態遷移図">

> [!Note]
> 将来的にプレイヤーが中央位置へ戻るまでの反応時間計測などの仕様が増える可能性があるが、現時点では考慮していない。

## ESP側クラス図

<img src="images\ボタン制御クラス図.drawio.svg" alt="ボタン制御クラス図" title="ボタン制御クラス図">

## 操作デバイスとボタン制御デバイス(ESP)間のやり取り

### LED点灯要求(操作デバイス→ボタン制御デバイス)

``` json
{
  "command": "ReqLightOn",
  "buttonId": {
    "type": "integer",
    "minimum": 0,
    "maximum": 15
  },
  "onoff": {
    "type": "boolean"
  }
}
```

### リセット要求(操作デバイス→ボタン制御デバイス)

``` json
{
  "command": "ReqReset"
}
```

### 測定結果応答(ボタン制御デバイス→操作デバイス)

``` json
{
  "command": "ResResult",
  "buttonId": {
    "type": "integer",
    "minimum": 0,
    "maximum": 15
  },
  "reactionTime": {
    "type": "integer",
    "minimum": 0
  }
}
```

### LED点灯要求応答(ボタン制御デバイス→操作デバイス)

``` json
{
  "command": "AckLightOn",
  "buttonId": {
    "type": "integer",
    "minimum": 0,
    "maximum": 15
  },
  "onoff": {
    "type": "boolean"
  }
}
```

## memo

* How to upload files to FileSystem(SPIFFS) using PlatformIO
  * https://labo.mycabin.net/electronics-programming/arduino-esp32/1268/
  * https://qiita.com/kobatan/items/dc26c28c0e283ba6f4c3
* How to build WebSocket server
  * https://registry.platformio.org/libraries/esphome/ESPAsyncWebServer-esphome#methods-for-controlling-websocket-connections
  * https://qiita.com/norippy_i/items/6b2da67602cd52a0412f
* How to interrupt by GPIO
  * https://blog.goo.ne.jp/jh7ubc/e/8c97dc4bfad4f93d44301f8341083a69
* About data schema
  * https://json-schema.org/
  * https://eng-blog.iij.ad.jp/archives/6813
