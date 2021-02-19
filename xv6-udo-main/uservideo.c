#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"


struct graph_op_chunk_sk *graph_op_buffer=0;
struct graph_op_chunk_sk *graph_op_buffer_end=0;
uchar graph_op_buffer_mode=0;
void graph_op_buffer_write(const void *data, int ldata){
    struct graph_op_chunk_sk *tmpchunk;
    if(graph_op_buffer_end==0)
        return;
    if(ldata>GRAPH_OP_BUFFERSZ-graph_op_buffer_end->len){//not enough memory for new data, assign a new datachunk
        tmpchunk=(struct graph_op_chunk_sk *)malloc(sizeof(struct graph_op_chunk_sk));
        if(tmpchunk==0)
            return;
        memset(tmpchunk, 0, sizeof(struct graph_op_chunk_sk));
        graph_op_buffer_end->next=tmpchunk;
        graph_op_buffer_end=tmpchunk;
    }
    memmove(graph_op_buffer_end->datachunk+graph_op_buffer_end->len, data, ldata);
    graph_op_buffer_end->len+=ldata;
}

void begingraphics(){
    graph_op_buffer=(struct graph_op_chunk_sk *)malloc(sizeof(struct graph_op_chunk_sk));
    if(graph_op_buffer!=0){
        memset(graph_op_buffer, 0, sizeof(struct graph_op_chunk_sk));
        graph_op_buffer_end=graph_op_buffer;
        graph_op_buffer_mode=1;
    }
    else
        graph_op_buffer_mode=0;
}
void endgraphics(){
    struct graph_op_chunk_sk *tmpchunk;
    if(graph_op_buffer_end!=0)
        sendgraphcommands(graph_op_buffer);
    graph_op_buffer_mode=0;
    while(graph_op_buffer!=0){
        tmpchunk=graph_op_buffer->next;
        free(graph_op_buffer);
        graph_op_buffer=tmpchunk;
    }
    graph_op_buffer_end=0;
}

int setpixel(int x, int y, int colour){
    uchar opcode;
    int op[3];
    if(graph_op_buffer_mode==0)
        return setpixel0(x, y, colour);
    opcode=1;
    op[0]=x;
    op[1]=y;
    op[2]=colour;
    graph_op_buffer_write(&opcode, sizeof(opcode));
    graph_op_buffer_write(&op, 3*sizeof(*op));
    return 0;
}
int drawline(int x0, int y0, int x1, int y1, int colour){
    uchar opcode;
    int op[5];
    if(graph_op_buffer_mode==0)
        return drawline0(x0, y0, x1, y1, colour);
    opcode=2;
    op[0]=x0;
    op[1]=y0;
    op[2]=x1;
    op[3]=y1;
    op[4]=colour;
    graph_op_buffer_write(&opcode, sizeof(opcode));
    graph_op_buffer_write(&op, 5*sizeof(*op));
    return 0;
}