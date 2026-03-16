# liblovec

liblovec は、C言語初心者向けのライブラリで、ターミナル上で簡単なゲームを作成できます。

## 機能
- スプライト
    - 画面上でキャラクターを移動できます
- ボタン
    - ユーザーインターフェースを提供します
- キーボード
    - Enterを押さずにキー入力を取得できます

## 使い方

### macOS
まず、`#include "liblove.c"` を行います。

(スプライトの位置に小数点を使う場合は、`liblove.f.c` を使用します)

サンプルコードは `sample.sprite.c` などを参照してください。

### その他のOS
`liblove.c` の調整が必要な場合があります。

## サンプル

### わんこ大喧嘩
```
% gcc wanco.c
% ./a.out
```

### にゃんこかわくだり
```
% gcc kawakudari.c
% ./a.out
```

### A to Zはやうちゲーム
```
% gcc a2z.c
% ./a.out
```

### A to Zはやうちゲーム (BASICライク)
```
% gcc a2z.BASIC.c
% ./a.out
```

### キーコードチェックツール
```
% gcc keycheck.c
% ./a.out
```

### スプライトサンプル
```
% gcc sample.sprite.c
% ./a.out
```

### スプライトサンプル (float版)
```
% gcc sample.sprite.f.c
% ./a.out
```