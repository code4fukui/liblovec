#include "liblove.c"

int main() {
    Keyboard kbd;
    Keyboard_(&kbd);

    int gameflg = 1;
    while (gameflg) {
        char c = Keyboard_inkey(&kbd);
        if (c) {
            printf("[%d]\n", (int)c);
        }
    }
    Keyboard__(&kbd);
    return 0;
}
