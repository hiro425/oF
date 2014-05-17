//IQ Version - http://pouet.net/topic.php?which=7931
// las style mod - might be a good alternative in some situations
// rotwang: @mod* some smoothsteps @mod* aspect
#ifdef GL_ES
precision highp float;
#endif

uniform vec2 resolution;
uniform float time;

vec4 distance2Color(float d) {
    vec4 c = mix(vec4(1,0,0,1), vec4(0,1,0,1), sign(d) * 0.5 + 0.5);
    c *= abs(sin(d*50.));
    c *= smoothstep(0., abs(d), 0.25);
    return c;
}   

void main(void)
{
    float theta2 = radians(10.) * time;
    vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy;
    p.x *= resolution.x / resolution.y;
    p = mat2(cos(theta2), -sin(theta2), sin(theta2), cos(theta2)) * p;
    p.x += .3 * sin(p.y * 3. + time * 5.);
    float theta = time * radians(20. + p.x * .2);
    p *= 4. + 3. * smoothstep(0., 4.5, mod(time * 5., 5.));
    p += vec2(sin(time * 7.), cos(time * 13.));
    float f_wand = p.y;
    float c = 3.;
    p.xy = mod(p.xy, c) - .5 * c;
    p = mat2(cos(theta), -sin(theta), sin(theta), cos(theta)) * p;
    float f_kugel = length(p) - 1.;
    float d8 = min(f_wand, f_kugel);
    d8 = f_kugel;
    d8 -= .1 * sin(p.x * p.y * 10. + time * 3.);
    
    gl_FragColor = distance2Color(d8);
}
