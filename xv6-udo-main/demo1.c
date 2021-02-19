// stage1demo1
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    int x,y;
    printf(1, "\n\n\n\n\nDemo1 stage 4: functions: setvideomode(13h),  setpixel(), drawline()\npress any key to continue");

    //demo setvideomode() and setpixel()
    getch();
    printf(1, "pixel demo:\n");
    setvideomode(0x13);
    for(x=0;x<320;x++)
        for(y=0;y<200;y++)
            setpixel(x, y, (x*y/2)&255);
    
    //demo setvideomode() and drawline()
    getch();
    printf(1, "drawline demo:\n");
    setvideomode(0x13);


    for(x=0;x<128;x++){
        drawline( 160,  10, 32+x,  90, (x/4)&31);
        drawline(32+x,  90, 32+x, 110, (x/4)&31);
        drawline( 160, 190, 32+x, 110, (x/4)&31);

        drawline(  160,  10, 287-x,  90, (x/4)&31);
        drawline(287-x,  90, 287-x, 110, (x/4)&31);
        drawline(  160, 190, 287-x, 110, (x/4)&31);
        
    }

    //back to text mode
    getch();
    setvideomode(0x03);
    printf(1, "back to mode 0x03\n");
    exit();
}
