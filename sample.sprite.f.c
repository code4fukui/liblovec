#include "liblove.f.c"

Image resHint = {
    .w   = 55,
    .h   = 1,
    .buf = "Move by Arrow-keys, beep by Space-key, exit by ESC-key."
};

Image resFace = {
    .w   = 8,
    .h   = 4,
    .buf = "\0/----\\\0"
           "| O  O |"
           "|  ==  |"
           "\0\\----/\0"
};

int main(){
    Screen scr;
    Screen_(&scr, 80, 20);
    Keyboard kbd;
    Keyboard_(&kbd);

    Sprite hint;
    Sprite_(&hint, &resHint, Sprite_tick_dummy);
    hint.x =  1.0F;
    hint.y = 18.0F;
    Screen_addSprite(&scr, &hint);

    Sprite face;
    Sprite_(&face, &resFace, Sprite_tick_dummy);
    face.x = 40.0F;
    face.y = 10.0F;
    Screen_addSprite(&scr, &face);

    char c=0;
    while(!( c=='\e' )){
        Screen_draw(&scr);
        usleep(10000);

        c = Keyboard_inkey(&kbd);
        if(c == KEY_LEFT){
            face.x -= 1.5F;
        }else
        if(c == KEY_RIGHT){
            face.x += 1.5F;
        }else
        if(c == KEY_UP){
            face.y -= 1.0F;
        }else
        if(c == KEY_DOWN){
            face.y += 1.0F;
        }else
        if(c == ' '){
            putchar('\a');
        }
    }

    Keyboard__(&kbd);
    Screen__(&scr);
    return 0;
}
