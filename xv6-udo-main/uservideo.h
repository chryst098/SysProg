#ifndef _USERVIDEO_H_
#define _USERVIDEO_H_

/*
struct graph_op_setpixel{
    int x;
    int y;
    int colour;
};
struct graph_op_drawline{
    int x0;
    int y0;
    int x1;
    int y1;
    int colour;
};*/

#define GRAPH_OP_BUFFERSZ 65536

struct graph_op_chunk_sk{
    int len;
    unsigned char datachunk[GRAPH_OP_BUFFERSZ];
    struct graph_op_chunk_sk *next;
};


#endif