#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 posFocus;
uniform vec3 colFocus;
uniform vec3 llumAmbient;

out vec3 matambF;
out vec3 matdiffF;
out vec3 matspecF;
out float matshinF;

out vec4 vertSCO;
out vec3 NormSCO;
out vec4 posFocusSCO;

void main()
{
		matambF = matamb;
		matdiffF = matdiff;
		matspecF = matspec;
		matshinF = matshin;

		mat3 NormalMatrix = inverse(transpose(mat3(view*TG)));
		NormSCO = normalize(NormalMatrix*normal);

		vertSCO = view * TG * vec4 (vertex, 1.0);
		posFocusSCO = vec4 (posFocus, 1.0); //Si no ens passen el punt en SCO := view * vec4 (posFocus, 1.0);

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
