# liblovec
The liblovec library is for C language beginners, allowing them to create terminal-based games.

## Features
- Sprites
    - Enables moving characters on the screen
- Buttons
    - Provides user interface
- Keyboard
    - Allows key input without pressing Enter

## Usage
### [macOS]
First, include the library:
```
#include "liblove.c"
```
(For using floating-point coordinates for sprites, include "liblove.f.c" instead)

Then, see the sample.sprite.c file for examples.

### [Other OS]
The liblove.c file may need adjustment.

## Running Samples
### Dog Fight
```
% gcc wanco.c
% ./a.out
```

### Nyan Cat Descent
```
% gcc kawakudari.c
% ./a.out
```

### A to Z Typing Game
```
% gcc a2z.c
% ./a.out
```

### A to Z Typing Game (like BASIC)
```
% gcc a2z.BASIC.c
% ./a.out
```

### Keycode Check Tool
```
% gcc keycheck.c
% ./a.out
```

### Sprite Sample
```
% gcc sample.sprite.c
% ./a.out
```

### Sprite Sample (float version)
```
% gcc sample.sprite.f.c
% ./a.out
```