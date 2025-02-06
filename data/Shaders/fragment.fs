#version 330 core
	

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform vec3 ambientLight; // Color de la luz ambiental
uniform float ambientIntensity; // Intensidad de la luz ambiental
uniform int LightType;

void main()
{
	vec4 textureColor = texture(texture_diffuse, TexCoords);
	if (textureColor.a < 0.1) { // Si no hay textura, usar un color base
		textureColor = vec4(0.8, 0.8, 0.8, 1.0); // Gris claro
	}
	vec3 finalColor = textureColor.rgb;

    // Calcular el color final según el tipo de luz
    if (LightType == 0) { // Directional Light
        finalColor = textureColor.rgb * ambientLight * ambientIntensity * 0.8; // Ejemplo para luz direccional
    } else if (LightType == 1) { // Point Light
        finalColor = textureColor.rgb * ambientLight * ambientIntensity * 1.2; // Ejemplo para luz puntual
		FragColor = vec4(TexCoords, 0.0, 1.0);
    } else if (LightType == 2) { // Spot Light
        finalColor = textureColor.rgb * ambientLight * ambientIntensity * 1.5; // Ejemplo para luz focal
    } else if (LightType == 3) { // Ambient Light
        finalColor = textureColor.rgb * ambientLight * ambientIntensity; // Luz ambiental (sin cambios)
    }
	FragColor = vec4(finalColor, textureColor.a);
	// FragColor = vec4(TexCoords, 0.0, 1.0); // ver las normales
}