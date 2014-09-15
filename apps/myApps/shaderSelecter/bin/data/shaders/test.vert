#version 120

//varying vec2 texcoord0;
attribute vec3 position;
attribute vec2 surfacePosAttrib;
varying vec2 surfacePosition;

void main() {
    surfacePosition = surfacePosAttrib;     
    gl_Position = ftransform();
     
    //texcoord0 = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
     
}
