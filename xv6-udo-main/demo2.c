// stage1demo1
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int x,y;
    printf(1, "\n\n\n\n\nDemo2 stage 4: functions: setvideomode(12h),  setpixel(), drawline()\npress any key to continue");

    //demo setvideomode() and setpixel()
    getch();
    printf(1, "pixel demo:\n");
    setvideomode(0x12);
    for(x=0;x<640;x++)
        for(y=0;y<480;y++)
            setpixel(x, y, (x*y/16)&15);
    
    //demo setvideomode() and drawline()
    getch();
    printf(1, "drawline demo:\n");
    setvideomode(0x12);


    for(x=0;x<600;x++){
        drawline(  320,   10, 40+x,  210, ((x+16)/16)&15);
        drawline( 40+x,  220, 40+x,  260, ((x+16)/16)&15);
        drawline( 40+x,  270,  320,  470, ((x+16)/16)&15);
    }

    //back to text mode
    getch();
    setvideomode(0x03);
    printf(1, "back to mode 0x03\n");
    exit();
}
