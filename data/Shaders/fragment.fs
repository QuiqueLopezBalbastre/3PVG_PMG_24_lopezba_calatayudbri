#version 330 core
	

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;

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

uniform vec3 directionalLightColor;
uniform vec3 directionalLightDirection;
uniform float directionalLightIntensity;

uniform int LightType;


vec3 CalculatePointLight(vec3 normal, vec3 textureColor)
{
    // Calcular la dirección de la luz
        vec3 lightDir = normalize(pointLightPosition - FragPos);

        // Calcular la intensidad de la luz basada en el ángulo entre la normal y la dirección de la luz
        float diff = max(dot(normal, lightDir), 0.0); // Producto punto entre la normal y la dirección de la luz
        vec3 diffuse = diff * textureColor.rgb; //pointLightIntensity;

        // Calcular la atenuación de la luz puntual
        float distance = length(pointLightPosition - FragPos);
        
       
        float attenuation = clamp(1.0f - (distance * distance)/(pointLightRadius * pointLightRadius), 0.0f, 1.0f);


        // Aplicar la luz difusa y la atenuación
        diffuse *= attenuation;
        vec3 finalColor = pointLightColor * diffuse;
        
        return finalColor; 
}

vec3 CalculateSpotLight(vec3 normal, vec3 textureColor)
{
    
    vec3 lightDir = normalize(spotlightPosition - FragPos);
        float theta = dot(lightDir, normalize(-spotlightDirection));
        float epsilon = spotlightCutoff - spotlightOuterCutoff;
        float intensity = clamp((theta - spotlightOuterCutoff) / epsilon, 0.0, 1.0);
        vec3 finalColor = textureColor.rgb;

        if (theta < spotlightOuterCutoff) {
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = diff * spotlightColor * spotlightIntensity * intensity;
            float distance = length(spotlightPosition - FragPos);
            float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * (distance * distance));

           finalColor = textureColor.rgb * diffuse * attenuation;
        } else {
            finalColor = textureColor.rgb * spotlightColor * spotlightIntensity; // Fuera del cono de luz
        }

        return finalColor;
}

vec3 CalculateDirectionalLight(vec3 normal, vec3 textureColor)
{
     // Calcular la dirección de la luz (ya está normalizada)
        vec3 lightDir = normalize(-directionalLightDirection);

        // Calcular la intensidad de la luz basada en el ángulo entre la normal y la dirección de la luz
        float diff = max(dot(normal, lightDir), 0.0); // Producto punto entre la normal y la dirección de la luz
        vec3 diffuse = diff * textureColor.rgb * directionalLightColor * directionalLightIntensity;

        // Combinar la luz ambiental y la luz difusa
      vec3 finalColor = textureColor.rgb * diffuse;

      return finalColor; 
}

void main()
{
	vec4 textureColor = texture(texture_diffuse1, TexCoords);
	if (textureColor.a < 0.1) { // Si no hay textura, usar un color base
		textureColor = vec4(0.8, 0.8, 0.8, 1.0); // Gris claro
	}
	vec3 finalColor = textureColor.rgb;
    vec3 normalized_normal = normalize(Normal);

    // Calcular el color final según el tipo de luz
    if (LightType == 0) { // Directional Light
        finalColor = CalculateDirectionalLight(normalized_normal,textureColor.rgb);
    } else if (LightType == 1) { // Point Light
    finalColor = CalculatePointLight(normalized_normal,textureColor.rgb);
    } else if (LightType == 2) { // Spot Light
      finalColor = CalculateSpotLight(normalized_normal,textureColor.rgb);
    }else
    {
	    FragColor = vec4(TexCoords, 0.0, 1.0); // ver las normales
    }
        FragColor = vec4(finalColor, 1.0);
}