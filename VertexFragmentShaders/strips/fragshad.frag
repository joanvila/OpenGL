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

	int r = int(gl_FragCoord.y*0.1);
	if((r % 2) > 0){
		discard;
	}
		
}
