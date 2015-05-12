#version 330 core

in vec3 matambF;
in vec3 matdiffF;
in vec3 matspecF;
in float matshinF;

in vec4 vertSCO;
in vec3 NormSCO;
in vec4 posFocusSCO;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform vec3 posFocus;
uniform vec3 colFocus;
uniform vec3 llumAmbient;

out vec4 FragColor;

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambF;

    // Afegim component difusa, si n'hi hax
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiffF * dot (L, NormSCO);  //colFocus = color llum focus matdiff = coeficient de reflexió difusa del material
    return (colRes);
}

vec3 Phong (vec3 NormSCO, vec3 L, vec4 vertSCO)
{
    // Els vectors estan normalitzats

    // Inicialitzem color a Lambert
    vec3 colRes = Lambert (NormSCO, L);

    // Calculem R i V
    if (dot(NormSCO,L) < 0)
      return colRes;  // no hi ha component especular

    vec3 R = reflect(-L, NormSCO); // equival a: normalize (2.0*dot(NormSCO,L)*NormSCO - L);
    vec3 V = normalize(-vertSCO.xyz);

    if ((dot(R, V) < 0) || (matshinF == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinF);
    return (colRes + matspecF * colFocus * shine);
}

void main()
{
	vec3 NormSCOF = normalize(NormSCO);

	vec3 L = normalize(posFocusSCO.xyz - vertSCO.xyz);

	vec3 fcolor = Phong(NormSCOF, L, vertSCO);

	FragColor = vec4(fcolor,1);
}
