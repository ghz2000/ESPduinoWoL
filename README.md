# ESPduinoWoL
Arduino WoL for Universal ESPduino

![Universal ESPduino](https://i0.wp.com/ghz2000.com/wordpress/wp-content/uploads/2017/03/IMG_6899.jpg?resize=650%2C488 "WoL for Universal ESPduino")

詳しい使い方は以下を御覧ください｡
http://ghz2000.com/wordpress/?p=848


ESP8266 ( WROOM-02 ) を使って WoL パケットを送信するサンプルです｡

AdvancedOTA.h の中の ap_ssid, ap_pass, sta_ssid, sta_pass, を適切に変更してください｡
apはAPモードのSSID､ sta は 親機のSSIDを入れます｡

WoL.ino の中の ipBroadcast(192.168.230.255) を適切なネットワークアドレスに変更してください｡
192.168.11.x/24 なネットワークの場合､ 192.168.11.255 となります｡

#define MACADDR "000d5ec70e04" を適切なMACアドレスに変更してください｡
WindowsPCのMACアドレスは cmd.exe から ipconfig /all で見ることが出来ます｡

その他､お使いのPCをWoLで起動出来るように設定する必要があります｡
以下の記事を参考に設定してください｡
http://www.atmarkit.co.jp/ait/articles/0602/25/news014.html

お使いのESP8266 にカッコイイボタンを接続します｡
接続ポートは16番でプルアップ10kΩ程度でプルアップしておきます｡
カッコイイボタンは押下時にGNDに落ちるように接続します｡
