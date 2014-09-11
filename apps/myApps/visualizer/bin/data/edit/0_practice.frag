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

float softnoise(vec2 pos, float scale) {
    vec2 smplpos = pos*scale;
    float c0= rand2((floor(smplpos)+vec2(0.0, 0.0))/scale).x;
    float c1= rand2((floor(smplpos)+vec2(1.0, 0.0))/scale).x;
    float c2= rand2((floor(smplpos)+vec2(0.0, 1.0))/scale).x;
    float c3= rand2((floor(smplpos)+vec2(1.0, 1.0))/scale).x;

    vec2 a = fract(smplpos);
    return
        mix(
                mix(c0, c1, smoothstep(0.0, 1.0, a.x)),
                mix(c2, c3, smoothstep(0.0, 1.0, a.x)),
                smoothstep(0.0, 1.0, a.y)
           );
}

float plasma(vec2 p) {
  p*=20.0;
  return (sin(p.x)*0.25+0.25)+(sin(p.y)*0.25+0.25);
}

float lengthN(vec2 v, float n) {
    vec2 temp = pow(abs(v), vec2(n));
    return pow(temp.x+temp.y+softnoise(v, 4.0), 1.0/n);
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
  //vec2 pos = (gl_FragCoord.xy*2.0 -resolution) / resolution.y;
  vec2 pos = gl_FragCoord.xy / resolution.y;

  //vec2 pos = (gl_FragCoord.xy*2.0 -resolution);
  //vec2 pos = gl_FragCoord.xy;
 
  gl_FragColor = vec4(rings(pos));
  //gl_FragColor = vec4(rand(pos).xyxy);
  // gl_FragColor = vec4(softnoise(pos, 4.0));
  
/*
  float color = 0.0;
  float s = 1.0;
  for (int i = 0; i < 6; ++i) {
      color += softnoise(pos+vec2(float(i)*0.02), s*4.0)/s/2.0;
      s*=2.0;
  }
  gl_FragColor = vec4(color);

*/

}
