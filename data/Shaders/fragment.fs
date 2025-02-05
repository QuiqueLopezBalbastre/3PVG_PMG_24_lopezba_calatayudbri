#version 330 core
	

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse;
uniform vec3 ambientLight; // Color de la luz ambiental
uniform float ambientIntensity; // Intensidad de la luz ambiental

void main()
{
	vec4 textureColor = texture(texture_diffuse, TexCoords);
	if (textureColor.a < 0.1) { // Si no hay textura, usar un color base
		textureColor = vec4(0.8, 0.8, 0.8, 1.0); // Gris claro
	}
	vec3 finalColor = textureColor.rgb * ambientLight * ambientIntensity;
	FragColor = vec4(finalColor, textureColor.a);
	// FragColor = vec4(TexCoords, 0.0, 1.0); // ver las normales
}