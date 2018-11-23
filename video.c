


#include <video.h>
#include <types.h>
#include <asm.h>


#define VIDEO_ADDR 0xb8000

#define VIDEO_COLS 80
#define VIDEO_ROWS 25

#define VIDEO_ATTR 0x0f

unsigned int col, row;

inline static void setcursor();
static void putchar(char);
static void putstr(char*);
static void puthex(uint32);


void
video_setup(){

    char *addr = (char*) VIDEO_ADDR;
    int i,j;
    for(i=0;i<VIDEO_ROWS;i++){
        for(j=0;j<VIDEO_COLS;j++){
            *addr++ = 0x00;
            *addr++ = VIDEO_ATTR;
        }
    }

    col=0;
    row=0;

    setcursor();

}


inline static void setcursor(){

    int pos = row * VIDEO_COLS + col;
    outb(0x0f, 0x3d4);
    outb(pos & 0xff, 0x3d5);
    outb(0x0e, 0x3d4);
    outb((pos >> 8) & 0xff, 0x3d5);

}


static void putchar(char c){

    if(0x20 <= c && c <= 0x7e){
        char* addr = (char*) (VIDEO_ADDR + row * VIDEO_COLS * 2 + col * 2);
        *addr++ = c;
        *addr++ = VIDEO_ATTR;
        if(col == VIDEO_COLS-1){
            if(row == VIDEO_ROWS-1){
                addr = (char*)VIDEO_ADDR;
                char *addr2 = (char*) (VIDEO_ADDR + VIDEO_COLS*2);
                int i;
                for(i=0;i<(VIDEO_ROWS-1) * VIDEO_COLS;i++){
                    *addr++= *addr2++;
                    *addr++= *addr2++;
                }
                for(i=0;i<VIDEO_COLS;i++){
                    *addr++ = 0x00;
                    *addr++ = VIDEO_ATTR;
                }
                col=0;
                row=VIDEO_ROWS-1;
            }else{
                col=0;
                row++;
            }
        }else{
            col++;
        }
    }else{
        if(c=='\r' || c=='\n'){
            if(row == VIDEO_ROWS-1){
                char *addr = (char*)VIDEO_ADDR;
                char *addr2 = (char*) (VIDEO_ADDR + VIDEO_COLS*2);
                int i;
                for(i=0;i<(VIDEO_ROWS-1) * VIDEO_COLS;i++){
                    *addr++= *addr2++;
                    *addr++= *addr2++;
                }
                for(i=0;i<VIDEO_COLS;i++){
                    *addr++ = 0x00;
                    *addr++ = VIDEO_ATTR;
                }
                col=0;
                row=VIDEO_ROWS-1;
            }else{
                row++;
                col=0;
            }
        }else if(c=='\b'){
            /* TODO */
            if(col==0){
                if(row!=0){
                    col=VIDEO_COLS-1;
                    row--;
                }
            }else{
                col--;
            }
            char* addr = (char*) (VIDEO_ADDR + row * VIDEO_COLS * 2 + col * 2);
            *addr++ = 0x00;
            *addr++ = VIDEO_ATTR;
        }
    }

}


void putc(char c){
    putchar(c);
    setcursor();
}

void puts(char *str){
    while(*str != 0)
        putchar(*str++);
    putchar('\n');
    setcursor();
}

static void putstr(char *str){
    while(*str != 0)
        putchar(*str++);
    setcursor();
}

static void puthex(uint32 v){
    char *s = "00000000";
    s += 8;
    int i;
    char c;
    for(i=0;i<8;i++){
        s--;
        c = v&0xf;
        if(c<0xa)
            *s = '0' + c;
        else
            *s = 'a' + c - 0xa;
        v>>=4;
    }
    putstr(s);
}

void
printf(char *fmt, ...){

    void **arg = (void**)&fmt + 1;

    while(*fmt){
        switch(*fmt){
            case '%':
                switch(*++fmt){
                    case '%':
                        putchar('%');
                        break;
                    case 'c':
                        putchar(*(char*)arg);
                        arg++;
                        break;
                    case 's':
                        putstr(*(char**)arg);
                        arg++;
                        break;
                    case 'x':
                        puthex(*(uint32*)arg);
                        arg++;
                        break;
                }
                fmt++;
                break;
            default:
                putchar(*fmt++);
                break;
        }

    }

    setcursor();

}

