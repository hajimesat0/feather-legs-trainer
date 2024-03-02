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

1. 操作者はスマホからLED点灯を指示する
2. ESPは指示されたLEDを点灯する
3. ESPは計測開始する
4. 訓練者は指示されたボタンを押す
5. ESPは計測停止する
6. ESPは反応時間(=停止時刻-開始時刻)をスマホへ送信する
7. スマホは反応時間を記録する
   1. 複数回の記録を保持できること
   2. 指示したボタンを合わせて記録すること
8. 操作者は反応時間を確認する

<img src="images\反応時間計測シーケンス図.drawio.svg" alt="反応時間計測シーケンス図" title="反応時間計測シーケンス図">


## memo

* How to upload files to FileSystem(SPIFFS) using PlatformIO
  * https://labo.mycabin.net/electronics-programming/arduino-esp32/1268/
  * https://qiita.com/kobatan/items/dc26c28c0e283ba6f4c3
* How to build WebSocket server
  * https://registry.platformio.org/libraries/esphome/ESPAsyncWebServer-esphome#methods-for-controlling-websocket-connections
  * https://qiita.com/norippy_i/items/6b2da67602cd52a0412f
* How to interrupt by GPIO
  * https://blog.goo.ne.jp/jh7ubc/e/8c97dc4bfad4f93d44301f8341083a69

