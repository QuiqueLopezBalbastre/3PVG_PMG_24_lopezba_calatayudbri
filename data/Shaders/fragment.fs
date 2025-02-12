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
uniform float pointLightRadius; // Radio de la point light

// Luz focal (Spotlight)
uniform vec3 spotlightColor;
uniform vec3 spotlightPosition;
uniform vec3 spotlightDirection;
uniform float spotlightIntensity;
uniform float spotlightCutoff; // Ángulo de corte interior (en radianes)
uniform float spotlightOuterCutoff; // Ángulo de corte exterior (en radianes)


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
        vec3 diffuse = diff * textureColor.rgb *pointLightColor * pointLightIntensity;

        // Calcular la atenuación de la luz puntual
        float distance = length(pointLightPosition - FragPos);
        
        float attenuation = 1.0 - clamp(distance / pointLightRadius, 0.0, pointLightIntensity); 
        attenuation = attenuation * attenuation; // Atenuación cuadrática para un decaimiento más suave

        // Aplicar la luz difusa y la atenuación
        finalColor = textureColor.rgb * (diffuse * attenuation);
        
    } else if (LightType == 2) { // Spot Light
      
    vec3 lightDir = normalize(spotlightPosition - FragPos);
        float theta = dot(lightDir, normalize(-spotlightDirection));
        float epsilon = spotlightCutoff - spotlightOuterCutoff;
        float intensity = clamp((theta - spotlightOuterCutoff) / epsilon, 0.0, 1.0);

        if (theta < spotlightOuterCutoff) {
            float diff = max(dot(Normal, lightDir), 0.0);
            vec3 diffuse = diff * spotlightColor * spotlightIntensity * intensity;
            float distance = length(spotlightPosition - FragPos);
            float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * (distance * distance));
            finalColor = textureColor.rgb * diffuse * attenuation;
        } else {
            finalColor = textureColor.rgb * spotlightColor * spotlightIntensity; // Fuera del cono de luz
        }

    } else if (LightType == 3) { // Ambient Light
        finalColor = textureColor.rgb * ambientLight * ambientIntensity; // Luz ambiental (sin cambios)
    }else
    {
	    FragColor = vec4(TexCoords, 0.0, 1.0); // ver las normales
    }
        FragColor = vec4(finalColor, textureColor.a);
    
}