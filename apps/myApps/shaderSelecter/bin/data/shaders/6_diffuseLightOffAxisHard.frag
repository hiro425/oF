#ifdef GL_ES
precision mediump float;
#endif

uniform vec2  resolution;
uniform float time;
uniform float vol;

const int num_x = 10;
const int num_y = 10;
float w = resolution.x;
float h = resolution.y;
float PI = 3.141592653589793238;

void main() {
    float color = 0.0;
    for (int i = 0; i <= num_x; ++i) {
        for (int j = 0; j < num_y; ++j) {
            //float x = w/2.0 + (float(i-num_x/2)) * w/float(num_x); //default
            //float y = h/2.0 + (float(j-num_y/2)) * h/float(num_y); //defalut
            //float x = w/2.0 + (float(i-num_x/2)) * h/float(num_x) * vol; //rect
            //float y = h/2.0 + (float(j-num_y/2)) * h/float(num_y) * vol; //rect
            float x 
                = w/2.0
                + h/2.0/float(num_y-j) 
                //+ h/2.0/float(num_y)*float(j+1) 
                * cos(2.0*PI + 360.0/float(num_x)*float(i) * vol)
                //↑これを外してもおもしろいよ
                * vol * 5.0; 
            float y 
                = h/2.0
                + h/2.0/float(num_y-j) 
                //+ h/2.0/float(num_y)*float(j+1) 
                * sin(2.0*PI + 360.0/float(num_x)*float(i)
                        + 360.0/float(num_x*num_y)*float(j)*vol)
                * vol * 5.0; //cir
                //+ sin(2.0*PI+360.0/float(j)) * vol; //cir
            vec2 pos = vec2(x, y);
            float dist = length(gl_FragCoord.xy - pos);

            //float size = 1.0 - 0.5 * sin(time * float((j-i+10)));
            float size = 8.5 * sin(float((num_y-j)) * vol);
            color += pow(size/dist, 2.0);
        }
    }
    gl_FragColor = vec4(vec3(color), 1.0);
}

//rasen toka yaritaine
