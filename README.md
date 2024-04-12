# lovec library for C beginners

これはC言語入門者のためのライブラリ、ターミナルだけでゲームを作ったりできます。

## 機能

* スプライト

    画面上でキャラクタを動かせる

* ボタン

    ユーザーインタフェースの提供

* キーボード

    Enterを押さなくてもキー取得できる

## 使い方

### [ macos ]

まず、

```
#include "liblove.c"
```

します。

(スプライトの位置に小数点を使うには: liblove.f.c)

あとは sample.sprite.c などをご覧ください。

### [ other OS ]

liblove.c の調整が要るかもしれません。

## サンプルの動かし方

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

### AtoZはやうちゲーム(like BASIC)
```
% gcc a2z.BASIC.c
% ./a.out
```

### キーコードチェックツール
```
% gcc keycheck.c
% ./a.out
```

### スプライト サンプル
```
% gcc sample.sprite.c
% ./a.out
```

### スプライト サンプル(float version)
```
% gcc sample.sprite.f.c
% ./a.out
```
