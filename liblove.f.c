#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>

#define SCR_MAX_W 256
#define SCR_MAX_H 256
#define SCR_INITBG  ' '
#define SCR_MAX_SPR 1024

#define IMG_MAX_W SCR_MAX_W
#define IMG_MAX_H SCR_MAX_H

#define BTM_MAX_BTN 256
#define MVM_MAX_MVR 256

#define KEY_LEFT  (-68)
#define KEY_RIGHT (-67)
#define KEY_UP    (-65)
#define KEY_DOWN  (-66)



// Image
typedef struct Image{
    int w;
    int h;
    char buf[IMG_MAX_W*IMG_MAX_H];
} Image;



// Sprite
typedef struct Sprite{
    Image* img;
    float x;
    float y;
    void (*tick)(struct Sprite*);
} Sprite;

void Sprite_tick_dummy(Sprite* p){}

Sprite* Sprite_(Sprite* p, Image* img, void (*tick)(Sprite*)){
    p->img = img;
    p->x = 0.0F;
    p->y = 0.0F;
    p->tick = tick;
    return p;
}

Sprite* Sprite__(Sprite* p){
    return p;
}

char Sprite_getChar(Sprite* p, int x, int y){
    char r = 0;

    int myx = x - (int)p->x;
    int myy = y - (int)p->y;
    if(
        (0 <= myx && myx < p->img->w) &&
        (0 <= myy && myy < p->img->h) &&
    1){
        r = p->img->buf[p->img->w*myy+myx];
    }

    return r;
}



// Button
typedef struct Button{
    Sprite base;
    Image* img0;
    Image* img1;
    void (*press)(struct Button*);
} Button;

void Button_dummy(){}

Button* Button_(Button* p, Image* img0, Image* img1, void (*press)(Button*)){
    Sprite_(&p->base, img0, Sprite_tick_dummy);
    p->img0 = img0;
    p->img1 = img1;
    p->press = press;
    return p;
}

Button* Button__(Button* p){
    Sprite__(&p->base);
    return p;
}

Button* Button_setActive(Button* p, int act){
    p->base.img = act ? p->img1 : p->img0;
    return p;
}

Button* Button_press(Button* p){
    p->press(p);
    return p;
}



// Mover
typedef struct Mover{
    Sprite* target;
    void (*tick)(struct Mover*);
    int (*isFinished)(struct Mover*);
} Mover;

void Mover_tick_dummy(Mover* p){}
int Mover_isFinished_dummy(Mover* p){ return 0; }

Mover* Mover_(Mover* p, Sprite* target, void (*tick)(Mover*), int (*isFinished)(Mover*)){
    p->target = target;
    p->tick = tick;
    p->isFinished = isFinished;
    return p;
}

Mover* Mover__(Mover* p){
    return p;
}

int Mover_tick(Mover* p){
    p->tick(p);
    return p->isFinished(p);
}



// Screen
typedef struct Screen{
    char    buf[SCR_MAX_W*SCR_MAX_H];
    Sprite* sprs[SCR_MAX_SPR];
    int     sprs_n;
    int     w;
    int     h;
} Screen;

Screen* Screen_(Screen* p, int w, int h){
    setbuf(stdout, NULL);
    write(STDOUT_FILENO, "\e[?1049h", 8);

    int i=0;
    while(i<w*h){
        p->buf[i] = SCR_INITBG;
        ++i;
    }

    p->sprs_n = 0;
    p->w = w;
    p->h = h;

    return p;
}

Screen* Screen__(Screen* p){
    write(STDOUT_FILENO, "\e[?1049l", 8);
    return p;
}

char Screen_set(Screen* p, int x, int y, char c){
    char r = -1;

    unsigned pos = p->w*y+x;
    if(pos < sizeof(p->buf)){
        r = p->buf[pos];
        p->buf[pos] = c;
    }

    return r;
}

Sprite* Screen_addSprite(Screen* p, Sprite* s){
    p->sprs[p->sprs_n++] = s;
    return s;
}

Sprite* Screen_remSprite(Screen* p, Sprite* s){
    int i = 0;
    while(i<p->sprs_n && p->sprs[i]!=s) ++i;

    while((i+1) < p->sprs_n){
        p->sprs[i] = p->sprs[i+1];
        ++i;
    }
    --p->sprs_n;

    return s;
}

Screen* Screen_draw(Screen* p){
    write(STDOUT_FILENO, "\e[2J\e[H", 7);
    int y=0;
    while(y<p->h){
        int x=0;
        while(x<p->w){
            int i = p->sprs_n;
            char c=0;
            while( 0<=--i && !(c=Sprite_getChar(p->sprs[i], x, y)) );

            write(STDOUT_FILENO, c ? &c : &p->buf[p->w*y+x], 1);
            ++x;
        }
        write(STDOUT_FILENO, "\n", 1);
        ++y;
    }
    return p;
}

Screen* Screen_tick(Screen* p){
    int i = p->sprs_n;
    while( 0<=--i ) p->sprs[i]->tick(p->sprs[i]);

    return p;
}



// Button manager
typedef struct ButtonManager{
    Button* btns[BTM_MAX_BTN];
    int     btns_n;
    int     cur;
    Button  sentry;
} ButtonManager;

ButtonManager* ButtonManager_(ButtonManager* p){
    p->cur = 0;
    p->btns[0] = Button_(&p->sentry, 0, 0, Button_dummy);
    p->btns_n = 1;
    return p;
}

ButtonManager* ButtonManager__(ButtonManager* p){
    return p;
}

Button* ButtonManager_addButton(ButtonManager* p, Button* b){
    p->btns[p->btns_n++] = b;
    return b;
}

void ButtonManager_activeNext(ButtonManager* p){
    Button_setActive(p->btns[p->cur], 0);
    p->cur = (++p->cur) % p->btns_n;
    Button_setActive(p->btns[p->cur], 1);
}

void ButtonManager_runActive(ButtonManager* p){
    Button_press(p->btns[p->cur]);
}



// Mover manager
typedef struct MoverManager{
    Mover* mvrs[MVM_MAX_MVR];
    int    mvrs_n;
} MoverManager;

Mover* MoverManager_addMover(MoverManager* p, Mover* m){
    p->mvrs[p->mvrs_n++] = m;
    return m;
}

Mover* MoverManager_remMover(MoverManager* p, Mover* m){
    int i = 0;
    while(i<p->mvrs_n && p->mvrs[i]!=m) ++i;

    while((i+1) < p->mvrs_n){
        p->mvrs[i] = p->mvrs[i+1];
        ++i;
    }
    --p->mvrs_n;

    return m;
}

MoverManager* MoverManager_tick(MoverManager* p){
    int i = p->mvrs_n;
    while( 0<=--i ){
        if( Mover_tick(p->mvrs[i]) ){
            MoverManager_remMover(p, p->mvrs[i]);
        }
    }

    return p;
}



// Keyboard
typedef struct Keyboard{
    // none
} Keyboard;

Keyboard* Keyboard_(Keyboard* p){
    struct termios ti;
    tcgetattr( STDIN_FILENO, &ti);
    ti.c_lflag &= ~(ICANON|ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &ti);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    return p;
}

Keyboard* Keyboard__(Keyboard* p){
    return p;
}

char Keyboard_inkey(Keyboard* p){
    char c;
    return (read(STDIN_FILENO, &c, 1) == 1)
        ? (c == '\e')
            ? (read(STDIN_FILENO, &c, 1) == 1)
                ? (c == '[')
                    ? (read(STDIN_FILENO, &c, 1) == 1)
                        ? -c
                        : 0
                    : 0
                : '\e'
            : c
        : 0;
}
