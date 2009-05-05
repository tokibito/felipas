felipas.dll

使い方はサンプルを見てお察しください。
_LoadFelicaLibraryの後にPollingOnceを呼べばとりあえずIDを取得できる。
_StartPollingをする前には、毎回 _SetCallbackを実行する必要があります。
_FreeFelicaLibraryすることを忘れずに。

作者: Shinya Okano(tokibito/nullpobug)
HP: http://tokibito.orz.hm/
    http://d.hatena.ne.jp/nullpobug/

ライセンス: 修正BSD

※注意点とか
FeliCaのポートウェアがインストールされている必要があります。
Windows専用です。
felica.dllを呼び出しています。

PaSoRi、FeliCaは株式会社ソニーの登録商標です。
http://www.sony.co.jp/Products/felica/

本ソフトウェアはSONYや、FeliCaの技術を用いる他の企業とは一切関係ありません。