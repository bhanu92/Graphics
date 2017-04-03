#version 330 core

// Fragment shader for multiple lights.

struct LightProperties {
    bool isEnabled;
    bool isLocal;
    bool isSpot;
    vec3 ambient;
    vec3 color;
    vec3 position;
    vec3 halfVector;
    vec3 coneDirection;
    float spotCosCutoff;
    float spotExponent;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

// the set of lights to apply, per invocation of this shader
const int MAXLIGHTS = 4;
uniform LightProperties Lights[MAXLIGHTS];

// material description
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

uniform bool isTextureMapping ;
uniform sampler2D texID;

in vec3 Normal;		// normal in eye coordinates
in vec4 Position;	// vertex position in eye coordinates
in vec2 tIndex;         // texture map indices

out vec4 FragColor;

void main()
{
	vec3 scatteredLight = vec3(0.f);
	vec3 reflectedLight = vec3(0.f);
	vec3 eyeDirection;
	vec3 lightDirection;
	vec3 halfVector;
	vec3 myNormal;
	float attenuation = 1.0f;
	float diffuseCoeff;
	float specularCoeff;

    // loop over all the lights
    for (int light = 0; light < MAXLIGHTS; ++light)
	{
        if (!Lights[light].isEnabled)
            continue;

        attenuation = 1.0;

        eyeDirection = normalize(-vec3(Position));	// since we are in eye coordinates
													// eye position is 0,0,0
        // for local lights, compute per-fragment direction,
        // halfVector, and attenuation
        if (Lights[light].isLocal)
		{
		    lightDirection = Lights[light].position - vec3(Position);
            float lightDistance = length(lightDirection);
            lightDirection = normalize(lightDirection);

			attenuation = 1.0 /
				(Lights[light].constantAttenuation
				+ Lights[light].linearAttenuation    * lightDistance
				+ Lights[light].quadraticAttenuation * lightDistance
						                                  * lightDistance);
				if (Lights[light].isSpot)
				{
					vec3 myConeDirection = normalize(Lights[light].coneDirection);
					float spotCos = dot(lightDirection,
										-myConeDirection);
					if (spotCos < Lights[light].spotCosCutoff)
						attenuation = 0.0;
					else
						attenuation *= pow(spotCos, Lights[light].spotExponent);
				}
            halfVector = normalize(lightDirection + eyeDirection);
        }
		else
		// directional light
		{
			lightDirection = normalize(Lights[light].position);
			halfVector = normalize(lightDirection + eyeDirection);
        }

		myNormal = normalize(Normal);

        diffuseCoeff  = max(0.0, dot(myNormal, lightDirection));
        specularCoeff = max(0.0, dot(myNormal, halfVector));

        if (diffuseCoeff == 0.0)
            specularCoeff = 0.0;
        else
            specularCoeff = pow(specularCoeff, shininess); // * Strength;

		// Accumulate all the lights� effects as it interacts with material properties

		scatteredLight += Lights[light].ambient * ambient * attenuation +
                          Lights[light].color * (diffuseCoeff * diffuse) * attenuation;
        reflectedLight += Lights[light].color * (specularCoeff * specular) * attenuation;

    }

	vec3 rgb = min(scatteredLight + reflectedLight, vec3(1.0));

        if(isTextureMapping == true){
                vec4 color= texture(texID,tIndex)*vec4(rgb,1.0f);
                if(color.a == 0){
                        discard;
                }
                FragColor = color;
        }
        else{
                FragColor = vec4(rgb, 0.0f);
        }
}
