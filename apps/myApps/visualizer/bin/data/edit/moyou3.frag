uniform vec2 resolution;
uniform float time;
uniform float vol;

vec2 rand(vec2 pos) {
    return 
        fract(
                pow(pos+2.0, pos.yx+2.0)
                *22222.0
             )
        ;
}

vec2 rand2(vec2 pos) {
    return rand(rand(pos));
}

float plasma(vec2 p) {
  p*=20.0;
  return (sin(p.x)*0.25+0.25)+(sin(p.y)*0.25+0.25);
}

float lengthN(vec2 v, float n) {
    vec2 temp = pow(abs(v), vec2(n));
    return pow(temp.x+temp.y+rand(v).x, 1.0/n);
}

float rings(vec2 p) {
    //return sin(((length(p))+time)*16.0);
    vec2 p2 = mod(p*8.0, 4.0) - 2.0;
    //vec2 p2 = mod(p*8.0, 4.0) - 2.0;
    //vec2 p2 = p*8.0;
    //return sin((lengthN(p2, 4.0))*16.0);
    return sin(lengthN(p2,4.0)*16.0 -time*50.);
}


void main() {
  vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
  //vec2 pos = (gl_FragCoord.xy*2.0 -resolution);
  //vec2 pos = gl_FragCoord.xy;
 
  gl_FragColor = vec4(rings(pos));
  //gl_FragColor = vec4(rand2(pos).xyxy);
}
