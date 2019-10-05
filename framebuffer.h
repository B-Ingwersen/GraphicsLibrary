#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

char * FRAME_BUFFER_MASTER;
int FBFD_MASTER;
int FB_SCREENSIZE;

screenData FRAME_BUFFER_SCREEN;

Uint32 * getFrameBuffer() {
    FBFD_MASTER = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    FB_SCREENSIZE = 0;
    FRAME_BUFFER_MASTER = 0;
    int x = 0, y = 0;
    long int location = 0;

    // Open the file for reading and writing
    FBFD_MASTER = open("/dev/fb0", O_RDWR);
    if (FBFD_MASTER == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(FBFD_MASTER, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(FBFD_MASTER, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    FB_SCREENSIZE = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    WINDOW_WIDTH_PARENT = vinfo.xres;
    WINDOW_HEIGHT_PARENT = vinfo.yres;

    // Map the device to memory
    FRAME_BUFFER_MASTER = (char *)mmap(0, FB_SCREENSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, FBFD_MASTER, 0);
    if ((long)FRAME_BUFFER_MASTER == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    return (Uint32 *)(FRAME_BUFFER_MASTER);
}

void disconnectFrameBuffer() {
    munmap(FRAME_BUFFER_MASTER, FB_SCREENSIZE);
    close(FBFD_MASTER);
}