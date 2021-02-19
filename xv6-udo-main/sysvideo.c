#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"
#include "uservideo.h"

int sys_getch(void) {
    return consoleget();
}

// TODO: Implement your system call for switching video modes (and any other video-related syscalls)
// in here.
extern uint currentvgamode;

int sys_setvideomode(){
    int mode, i;
    uchar* vmem;
    if(argint(0, &mode)<0)
        return -1;
    switch(mode){
        case 0x03:
            return consolevgamode(mode);
        case 0x12:
            consolevgamode(mode);
            for(i=0;i<4;i++){
                consolevgaplane(i);
                vmem=consolevgabuffer();
                memset(vmem, 0, 640*480/8);
            }
            return 0;
        case 0x13:
            consolevgamode(mode);
            vmem=consolevgabuffer();
            memset(vmem, 0, 320*200);
            return 0;
    }
    return -1;
}

void loc_setpixel(int x, int y, int colour){
    uchar* vmem;
    int i;
    if(currentvgamode==0x12){
        if( x<0 || x>=640 || y<0 || y>=480) // if out of limits dont set pixel
            return;
        for(i=0;i<4;i++){
            consolevgaplane(i);
            vmem=consolevgabuffer();
            vmem[(x+640*y)>>3]=(((colour>>(i))&1)<<(7-((x+640*y)&7)))|(vmem[(x+640*y)>>3]&~(128>>((x+640*y)&7)));
        }
    }
    else if(currentvgamode==0x13){
        if( x<0 || x>=320 || y<0 || y>=200) // if out of limits dont set pixel
            return;
        vmem=consolevgabuffer();
        vmem[x+320*y]=colour&255;
    }
}
int loc_drawline(int x0, int y0, int x1, int y1, int colour){
    //https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#All_cases
    //plotLine(int x0, int y0, int x1, int y1)
    //    dx =  abs(x1-x0);
    //    sx = x0<x1 ? 1 : -1;
    //    dy = -abs(y1-y0);
    //    sy = y0<y1 ? 1 : -1;
    //    err = dx+dy;  /* error value e_xy */
    //    while (true)   /* loop */
    //        plot(x0, y0);
    //        if (x0 == x1 && y0 == y1) break;
    //        e2 = 2*err;
    //        if (e2 >= dy) /* e_xy+e_x > 0 */
    //            err += dy;
    //            x0 += sx;
    //        end if
    //        if (e2 <= dx) /* e_xy+e_y < 0 */
    //            err += dx;
    //            y0 += sy;
    //        end if
    //    end while
    //
    int dx, sx, dy, sy, err, e2;
    #define abs(a) ((a)<0?-(a):(a))
    dx =  abs(x1-x0);
    sx = x0<x1 ? 1 : -1;
    dy = -abs(y1-y0);
    sy = y0<y1 ? 1 : -1;
    err = dx+dy;  /* error value e_xy */
    while (true){   /* loop */
        loc_setpixel(x0,y0,colour);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2*err;
        if (e2 >= dy){/* e_xy+e_x > 0 */
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx){/* e_xy+e_y < 0 */
            err += dx;
            y0 += sy;
        }
    }
    return 0;
}

int sys_setpixel0(){
    int x;
    int y;
    int colour;
    if( (argint(0, &x)<0) || (argint(1, &y)<0) || (argint(2, &colour)<0) )
        return -1;
    loc_setpixel(x,y,colour);
    return 0;
}

int sys_drawline0(){
    int x0;
    int y0;
    int x1;
    int y1;
    int colour;
    if(currentvgamode!=0x12 && currentvgamode!=0x13){
        return -1;
    }
    if( (argint(0, &x0)<0) || (argint(1, &y0)<0) || (argint(2, &x1)<0) || (argint(3, &y1)<0) || (argint(4, &colour)<0) )
        return -1;
    return loc_drawline(x0, y0, x1, y1, colour);
}
extern void cprintf(char *fmt, ...);

int sys_sendgraphcommands(void){
    struct graph_op_chunk_sk *ops;
    struct graph_op_chunk_sk *tmpchunk;
    struct proc *curproc;
    int pos, opsz;
    uchar opcode;
    const int *op;
    int nsp=0, ndl=0;

    if(argint(0, (int*)&ops)<0)
        return -1;
    curproc = myproc();
    for(tmpchunk=ops;tmpchunk!=0;tmpchunk=tmpchunk->next){
        if(((uint)tmpchunk)>=curproc->sz || ((uint)tmpchunk) + sizeof(struct graph_op_chunk_sk) >curproc->sz){// limit check, based on int argptr(int n, char **pp, int size) from syscall.c
            return -1;
        }
        for(pos=0;pos<tmpchunk->len;){
            opcode=tmpchunk->datachunk[pos++];
            if(opcode==1)
                opsz=3*sizeof(int);
            else if(opcode==2)
                opsz=5*sizeof(int);
            else
                return -1;
            if(pos+opsz>tmpchunk->len){
                tmpchunk=tmpchunk->next;
                if(tmpchunk==0)
                    break;
                if(((uint)tmpchunk)>=curproc->sz || ((uint)tmpchunk) + sizeof(struct graph_op_chunk_sk) >curproc->sz){// limit check, based on int argptr(int n, char **pp, int size) from syscall.c
                    return -1;
                }
                pos=0;
            }
            op=(const int*)&tmpchunk->datachunk[pos];
            pos+=opsz;
            if(opcode==1){
                nsp++;
                loc_setpixel(op[0], op[1], op[2]);
            }
            else if(opcode==2){
                ndl++;
                loc_drawline(op[0], op[1], op[2], op[3], op[4]);
            }
        }
        if(tmpchunk==0)
            break;
    }
    return 0;
}
