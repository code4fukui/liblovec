# liblovec

これはC言語入門者向けのライブラリで、ターミナルだけでゲームを作ることができます。

## 機能

- スプライト
    - 画面上でキャラクタを動かせます。
- ボタン
    - ユーザーインターフェースを提供します。
- キーボード
    - Enterを押さなくてもキーを取得できます。

## 使い方

### macOS

まず、`#include "liblove.c"`を行います。

(スプライトの位置に小数点を使う場合は、`liblove.f.c`を使用します。)

サンプルコードは、`sample.sprite.c`などをご覧ください。

### その他のOS

`liblove.c`の調整が必要な場合があります。

## サンプルの実行方法

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

### AtoZはやうちゲーム
```
% gcc a2z.c
% ./a.out
```

### AtoZはやうちゲーム(BASICライク)
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

### スプライトサンプル(float版)
```
% gcc sample.sprite.f.c
% ./a.out
```