// stage1demo1
#include "types.h"
#include "stat.h"
#include "user.h"

void demo(){
    int x,y;
    for(y=0;y<240;y++)
        for(x=0;x<640;x++){
            setpixel(x,     y, (x*y/16)&15);
            setpixel(x, 479-y, (x*y/16)&15);
        }
    for(x=0;x<320;x++)
        for(y=0;y<480;y++){
            setpixel(    x, y, 0);
            setpixel(639-x, y, 0);
        }
    for(x=0;x<310;x++){
        drawline(320-x/16,  30, 320-x, 220, x/16);
        drawline(320+x/16,  30, 320+x, 220, x/16);

        drawline(320-x/16, 450, 320-x, 260, x/16);
        drawline(320+x/16, 450, 320+x, 260, x/16);

        drawline(320+x   ,  230, 320+x, 250, x/16);
        drawline(320-x   ,  230, 320-x, 250, x/16);

        drawline(320+x/16,   0, 320+x/16,  20, x/16);
        drawline(320-x/16,   0, 320-x/16,  20, x/16);

        drawline(320+x/16, 460, 320+x/16, 479, x/16);
        drawline(320-x/16, 460, 320-x/16, 479, x/16);
    }
}

int main(int argc, char *argv[]) {
    int t0, t1;
    printf(1, "\n\nDemo3 stage 4: functions: setvideomode(12h),  setpixel(), drawline(), begingraphics(), endgraphics()\n");

    setvideomode(0x12);

    t0=uptime();
    demo();
    t1=uptime();
    printf(1, "demo duration:%d system ticks\n", t1-t0);

    t0=uptime();
    begingraphics();
    demo();
    endgraphics();
    t1=uptime();
    printf(1, "demo duration:%d system ticks when using begingraphics() and endgraphics()\n\n", t1-t0);

    setvideomode(0x03);
    exit();
}
