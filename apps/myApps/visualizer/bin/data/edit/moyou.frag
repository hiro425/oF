uniform vec2 resolution;
uniform float time;

float plasma(vec2 p) {
  p*=20.0;
  return (sin(p.x)*0.25+0.25)+(sin(p.y)*0.25+0.25);
}

float lengthN(vec2 v, float n) {
    vec2 temp = pow(abs(v), vec2(n));
    return pow(temp.x+temp.y, 1.0/n);
}

float rings(vec2 p) {
    //return sin(((length(p))+time)*16.0);
    return sin((lengthN(p, 2.0))*16.0+time)+1.;
}


void main() {
  //vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
  vec2 pos = (gl_FragCoord.xy*2.0 -resolution);
  //vec2 pos = gl_FragCoord.xy;
 
  gl_FragColor = vec4(rings(pos));
}
