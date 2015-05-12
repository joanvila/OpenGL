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
uniform int pintarCow;

vec3 matambFinal;
vec3 matdiffFinal;
vec3 matspecFinal;
float matshinFinal;

// Valors per als components que necessitem dels focus de llum
vec3 colFocus = vec3(0.8, 0.8, 0.8);
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
vec3 posFocus = vec3(1, 1, 1);  // en SCA

out vec3 fcolor;

vec3 Lambert (vec3 NormSCO, vec3 L)
{
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats

    // Inicialitzem color a component ambient
    vec3 colRes = llumAmbient * matambFinal;

    // Afegim component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colRes + colFocus * matdiffFinal * dot (L, NormSCO);
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

    if ((dot(R, V) < 0) || (matshinFinal == 0))
      return colRes;  // no hi ha component especular

    // Afegim la component especular
    float shine = pow(max(0.0, dot(R, V)), matshinFinal);
    return (colRes + matspecFinal * colFocus * shine);
}

void main()
{
    if (pintarCow == 1){
      matambFinal = vec3(0.2,0.2,0.2);
      matdiffFinal = vec3(0.7,0.7,0.7);
      matspecFinal = vec3(1.0,1.0,1.0);
      matshinFinal = 2.0;
    } else {
      matambFinal = matamb;
      matdiffFinal = matdiff;
      matspecFinal = matspec;
      matshinFinal = matshin;
    }
    vec3 NormSCO;
		mat3 NormalMatrix = inverse(transpose(mat3(view*TG)));
		NormSCO = normalize(NormalMatrix*normal);

		vec4 vertSCO = view * TG * vec4 (vertex, 1.0);
		vec4 posFocusSCO = view * vec4 (posFocus, 1.0);

		vec3 L = normalize(posFocusSCO.xyz - vertSCO.xyz);

		fcolor = Phong(NormSCO, L, vertSCO);

    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
