#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.);
	if (gl_FragCoord.x < 400.){
		if (gl_FragCoord.y < 400) FragColor = vec4(1.0,0.5,0.0,1.0);
		else FragColor = vec4(0.3,0.5,1.0,1.0);
	}
	else {
		if (gl_FragCoord.y < 400) FragColor = vec4(0.4,1.0,0.4,1.0);
		else FragColor = vec4(0.6,0.0,0.0,1.0);
	} 

	if (gl_FragCoord.y > 50 && gl_FragCoord.y < 100) discard;
	if (gl_FragCoord.y > 150 && gl_FragCoord.y < 200) discard;
	if (gl_FragCoord.y > 250 && gl_FragCoord.y < 300) discard;
	if (gl_FragCoord.y > 350 && gl_FragCoord.y < 400) discard;
	if (gl_FragCoord.y > 450 && gl_FragCoord.y < 500) discard;
	if (gl_FragCoord.y > 550 && gl_FragCoord.y < 600) discard;
	if (gl_FragCoord.y > 650 && gl_FragCoord.y < 700) discard;
	if (gl_FragCoord.y > 750 && gl_FragCoord.y < 800) discard;
		
}
