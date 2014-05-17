//
//  c1-1.c
//  patternExample
//
//  Created by 川口大志 on 2014/05/15.
//
//

#include <ofMain.h>
#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <gl.h>

#define KEY_ESC 27

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void myKbd(unsigned char key, int x, int y) {
    if (key == KEY_ESC) {
        exit(0);
    }
}

void myInit(char *progname) {
    int width = 500, height = 500;
    glutInitWindowPosition(0,0);
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGBA);
    
    
}
