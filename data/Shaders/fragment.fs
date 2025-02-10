#version 330 core
	

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D texture_diffuse;
uniform vec3 ambientLight; // Color de la luz ambiental
uniform float ambientIntensity; // Intensidad de la luz ambiental

uniform vec3 pointLightColor; // Color de la point light
uniform vec3 pointLightPosition; // Posición de la point light
uniform float pointLightIntensity; // Intensidad de la point light
uniform float pointLightRadius; // Radio de la luz puntual
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
   // Calcular la dirección de la luz
        vec3 lightDir = normalize(pointLightPosition - FragPos);

        // Calcular la intensidad de la luz basada en el ángulo entre la normal y la dirección de la luz
        float diff = max(dot(Normal, lightDir), 0.0); // Producto punto entre la normal y la dirección de la luz
        vec3 diffuse = diff * pointLightColor * pointLightIntensity;

        // Calcular la atenuación de la luz puntual
        float distance = length(pointLightPosition - FragPos);
        
        float attenuation = 1.0 - smoothstep(0.0, pointLightRadius, distance);

        // Aplicar la luz difusa y la atenuación
        finalColor = textureColor.rgb * (ambientLight * ambientIntensity + diffuse * attenuation);
        
    } else if (LightType == 2) { // Spot Light
        finalColor = textureColor.rgb * ambientLight * ambientIntensity * 1.5; // Ejemplo para luz focal
    } else if (LightType == 3) { // Ambient Light
        finalColor = textureColor.rgb * ambientLight * ambientIntensity; // Luz ambiental (sin cambios)
    }else
    {
	    FragColor = vec4(TexCoords, 0.0, 1.0); // ver las normales
    }

    float distanceToLight = length(pointLightPosition - FragPos);
    if (distanceToLight < 1.0) { // Si el fragmento está cerca de la luz
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Muestra la posición de la luz en rojo
    } else {
        FragColor = vec4(finalColor, textureColor.a);
    }
}