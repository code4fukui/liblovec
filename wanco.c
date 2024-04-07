#include "liblove.c"



// Game resouces
Image resBtnWanco0 = {
    .w   = 7,
    .h   = 3,
    .buf = "-------"
           " wanco "
           "-------"
};
Image resBtnWanco1 = {
    .w   = 7,
    .h   = 3,
    .buf = "======="
           "|wanco|"
           "======="
};

Image resBtnNoppo0 = {
    .w   = 7,
    .h   = 3,
    .buf = "-------"
           " noppo "
           "-------"
};
Image resBtnNoppo1 = {
    .w   = 7,
    .h   = 3,
    .buf = "======="
           "|noppo|"
           "======="
};

Image resGhost = {
    .w   = 4,
    .h   = 3,
    .buf = " __ "
           "('')"
           " || "
};

Image resWanco = {
    .w   = 8,
    .h   = 4,
    .buf = "\0/----\\\0"
           "| O  O |"
           "|  ==  |"
           "\0\\----/\0"
};

Image resNoppo = {
    .w   = 6,
    .h   = 10,
    .buf = "\0/--\\\0"
           "|O  O|"
           "|    |"
           "| == |"
           "|    |"
           "|    |"
           "|    |"
           "|    |"
           "|    |"
           "\0\\--/\0"
};

Image resHouse = {
    .w   = 5,
    .h   = 8,
    .buf = "\0\0_\0\0"
           "\0/ \\\0"
           "/ _ \\"
           "| + |"
           "| = |"
           "| _ |"
           "|! !|"
           "||_||"
};

Image resHouseX = {
    .w   = 5,
    .h   = 8,
    .buf = "\0\0_\0\0"
           "\0/ \\\0"
           "/ _ \\"
           "| x |"
           "| = |"
           "| _ |"
           "|! !|"
           "||_||"
};

Image resMeter = {
    .w   = 12,
    .h   = 1,
    .buf = "[##########]"
};

Image resGround = {
    .w   = 96,
    .h   = 8,
    .buf = "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
           "                                                                                                "
           " [TAB]:select, [SPACE]:decide                                                                   "
           "                                                                                                "
};



// Game objects
typedef struct House House;
typedef struct Meter Meter;

Screen        g_scr;
Keyboard      g_kbd;
ButtonManager g_btm;
MoverManager  g_mvm;

Sprite g_gnd;
House g_house;
House g_housex;
Meter g_meterx;



// Game classes
//---
typedef struct MoverFall{
    Mover base;
    int sx_milli;
    int sy_milli;
} MoverFall;

void MoverFall_tick(Mover* p){
    ((MoverFall*)p)->sy_milli += 100;
    p->target->x_milli += ((MoverFall*)p)->sx_milli;
    p->target->y_milli += ((MoverFall*)p)->sy_milli;
}

MoverFall* MoverFall_(MoverFall* p, Sprite* target, int (*isFinished)(Mover*)){
    Mover_(&p->base, target, MoverFall_tick, isFinished);
    p->sx_milli = 0;
    p->sy_milli = 0;
    return p;
}

MoverFall* MoverFall__(MoverFall* p){
    Mover__(&p->base);
    return p;
}

//---
typedef struct MoverYura{
    Mover base;
    int sx_milli;
    int sy_milli;
} MoverYura;

void MoverYura_tick(Mover* p){
    p->target->x_milli += ((MoverYura*)p)->sx_milli;
    p->target->y_milli += ((MoverYura*)p)->sy_milli;
}

MoverYura* MoverYura_(MoverYura* p, Sprite* target, int (*isFinished)(Mover*)){
    Mover_(&p->base, target, MoverYura_tick, isFinished);
    p->sx_milli = 0;
    p->sy_milli = 0;
    return p;
}

MoverYura* MoverYura__(MoverYura* p){
    Mover__(&p->base);
    return p;
}

//---
typedef struct Wanco{
    Sprite base;
    int sx_milli;
    int op;
} Wanco;

int Wanco_isGhostMoveFinished(Mover* yura){
    if( yura->target->y_milli + yura->target->img->h*1000 < 0 ){
        free( Sprite__( Screen_remSprite(&g_scr, yura->target)) );
        free( MoverYura__((MoverYura*)yura) );
        return 1;
    }

    return 0;
}

void Wanco_tick(Sprite* p){
    p->x_milli += ((Wanco*)p)->sx_milli;
    if(p->x_milli < ((Sprite*)&g_housex)->x_milli+((Sprite*)&g_housex)->img->w*1000){
        House* House_attacked(House* p, int op);
        House_attacked(&g_housex, ((Wanco*)p)->op);

        Sprite* ghost = Sprite_( malloc(sizeof(Sprite)), &resGhost, Sprite_tick_dummy);{
            ghost->x_milli = p->x_milli + p->img->w*1000/2 - ghost->img->w*1000/2;
            ghost->y_milli = p->y_milli + p->img->h*1000/2 - ghost->img->h*1000/2;
        }
        Screen_addSprite(&g_scr, ghost);

        MoverYura* yura = MoverYura_( malloc(sizeof(MoverYura)), (Sprite*)ghost, Wanco_isGhostMoveFinished);{
            yura->sx_milli = 0;
            yura->sy_milli = -900;
        }
        MoverManager_addMover(&g_mvm, (Mover*)yura);

        free( Sprite__(Screen_remSprite(&g_scr, p)) );
    }
}

Wanco* Wanco_(Wanco* p, Image* pWanco){
    Sprite_(&p->base, pWanco, Wanco_tick);
    p->sx_milli = 0;
    p->op = 0;
    return p;
}

Wanco* Wanco__(Wanco* p){
    Sprite__(&p->base);
    return p;
}

//---
typedef struct Meter{
    Sprite base;
    Image img;
} Meter;

Meter* Meter_(Meter* p){
    p->img = resMeter;
    Sprite_(&p->base, &p->img, Sprite_tick_dummy);
    return p;
}

Meter* Meter__(Meter* p){
    Sprite__(&p->base);
    return p;
}

void Meter_set(Meter* p, int percent){
    int i = 9;
    while(percent/10 <= i){
        p->img.buf[1+i] = '_';
        --i;
    }
    while(0 <= i){
        p->img.buf[1+i] = '#';
        --i;
    }
}

//---
typedef struct House{
    Sprite base;
    int hp_max;
    int hp;
} House;

House* House_(House* p, Image* pHouse){
    Sprite_(&p->base, pHouse, Sprite_tick_dummy);
    p->hp = INT32_MAX;
    return p;
}

House* House__(House* p){
    Sprite__(&p->base);
    return p;
}

int House_isMoveFinished(Mover* fall){
    if( g_gnd.y_milli-(4*1000) < fall->target->y_milli ){
        free( MoverFall__((MoverFall*)fall) );
        return 1;
    }

    return 0;
}

House* House_attacked(House* p, int op){
    if( 0 < p->hp){
        if( 0 < (p->hp -= op) ){
            // none
        }else{
            p->hp = 0;

            MoverFall* fall = MoverFall_( malloc(sizeof(MoverFall)), (Sprite*)p, House_isMoveFinished);{
                fall->sx_milli = -120;
                fall->sy_milli = -600;
            }
            MoverManager_addMover(&g_mvm, (Mover*)fall);
        }
        Meter_set(&g_meterx, p->hp*100/p->hp_max);
    }else{
        // none
    }
    return p;
}



// Game events
void onWanco(){
    Wanco* wanco = Wanco_( malloc(sizeof(Wanco)), &resWanco );{
        ((Sprite*)wanco)->x_milli = ((Sprite*)&g_house)->x_milli - ((Sprite*)wanco)->img->w/2*1000;
        ((Sprite*)wanco)->y_milli = g_gnd.y_milli - ((Sprite*)wanco)->img->h*1000;
        wanco->sx_milli = -1000;
        wanco->op = 2;
    }
    Screen_addSprite(&g_scr, (Sprite*)wanco);
    write(STDOUT_FILENO, "\a", 1);
}

void onNoppo(){
    Wanco* wanco = Wanco_( malloc(sizeof(Wanco)), &resNoppo );{
        ((Sprite*)wanco)->x_milli = ((Sprite*)&g_house)->x_milli - ((Sprite*)wanco)->img->w/2*1000;
        ((Sprite*)wanco)->y_milli = g_gnd.y_milli - ((Sprite*)wanco)->img->h*1000;
        wanco->sx_milli = -300;
        wanco->op = 5;
    }
    Screen_addSprite(&g_scr, (Sprite*)wanco);
    write(STDOUT_FILENO, "\a", 1);
}



// Game process
int main(){
    const int W = 96;
    const int H = 24;

    Screen_(&g_scr, W, H);
    Keyboard_(&g_kbd);
    ButtonManager_(&g_btm);

    // init something
    Sprite_(&g_gnd, &resGround, Sprite_tick_dummy);
    g_gnd.x_milli = (0)*1000;
    g_gnd.y_milli = (H-g_gnd.img->h)*1000;

    House_(&g_house, &resHouse);
    ((Sprite*)&g_house)->x_milli = (W-3-((Sprite*)&g_house)->img->w)*1000;
    ((Sprite*)&g_house)->y_milli = g_gnd.y_milli - (((Sprite*)&g_house)->img->h)*1000;

    House_(&g_housex, &resHouseX);
    ((Sprite*)&g_housex)->x_milli = (3)*1000;
    ((Sprite*)&g_housex)->y_milli = g_gnd.y_milli - (((Sprite*)&g_housex)->img->h)*1000;
    g_housex.hp_max = g_housex.hp = 10;

    Meter_(&g_meterx);
    ((Sprite*)&g_meterx)->x_milli = (1)*1000;
    ((Sprite*)&g_meterx)->y_milli = (1)*1000;

    Button btnNoppo;
    Button_(&btnNoppo, &resBtnNoppo0, &resBtnNoppo1, onNoppo);
    ((Sprite*)&btnNoppo)->x_milli = (W-2-btnNoppo.img0->w)*1000;
    ((Sprite*)&btnNoppo)->y_milli = (H-2-btnNoppo.img0->h)*1000;

    Button btnWanco;
    Button_(&btnWanco, &resBtnWanco0, &resBtnWanco1, onWanco);
    ((Sprite*)&btnWanco)->x_milli = ((Sprite*)&btnNoppo)->x_milli - (2+btnWanco.img0->w)*1000;
    ((Sprite*)&btnWanco)->y_milli = (H-2-btnWanco.img0->h)*1000;

    Screen_addSprite(&g_scr, (Sprite*)&g_house);
    Screen_addSprite(&g_scr, (Sprite*)&g_housex);
    Screen_addSprite(&g_scr, (Sprite*)&g_meterx);
    Screen_addSprite(&g_scr, &g_gnd);
    Screen_addSprite(&g_scr, (Sprite*)&btnWanco);
    Screen_addSprite(&g_scr, (Sprite*)&btnNoppo);

    ButtonManager_addButton(&g_btm, &btnWanco);
    ButtonManager_addButton(&g_btm, &btnNoppo);
    //---

    char c=0;
    while(!( c=='\e' )){
        Screen_draw(&g_scr);
        usleep(50000);

        MoverManager_tick(&g_mvm);
        Screen_tick(&g_scr);

        c = Keyboard_inkey(&g_kbd);
        if(c == ' '){
            ButtonManager_runActive(&g_btm);
        }else
        if(c == '\t'){
            ButtonManager_activeNext(&g_btm);
        }
        //---
    }

    ButtonManager__(&g_btm);
    Keyboard__(&g_kbd);
    Screen__(&g_scr);

    return 0;
}
