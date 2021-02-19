struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int getch(void);
// TODO: Declare your user APIs for your system calls.
int setvideomode(int mode);
int setpixel0(int x, int y, int colour);
int drawline0(int x0, int y0, int x1, int y1, int colour);
struct graph_op_chunk_sk;
int sendgraphcommands(struct graph_op_chunk_sk *ops);
// user APIs ulib.c
#include "uservideo.h"
void begingraphics();
void endgraphics();
int setpixel(int x, int y, int colour);
int drawline(int x0, int y0, int x1, int y1, int colour);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
