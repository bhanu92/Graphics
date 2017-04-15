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




        eyeDirection = normalize(-vec3(Position));	// since we are in eye coordinates
													// eye position is 0,0,0
        // for local lights, compute per-fragment direction,
        // halfVector, and attenuation
        if (Lights[0].isLocal)
		{
		    lightDirection = Lights[0].position - vec3(Position);
            float lightDistance = length(lightDirection);
            lightDirection = normalize(lightDirection);

			attenuation = 1.0 /
				(Lights[0].constantAttenuation
				+ Lights[0].linearAttenuation    * lightDistance
				+ Lights[0].quadraticAttenuation * lightDistance
						                                  * lightDistance);
				if (Lights[0].isSpot)
				{
					vec3 myConeDirection = normalize(Lights[0].coneDirection);
					float spotCos = dot(lightDirection,
										-myConeDirection);
					if (spotCos < Lights[0].spotCosCutoff)
						attenuation = 0.0;
					else
						attenuation *= pow(spotCos, Lights[0].spotExponent);
				}
            halfVector = normalize(lightDirection + eyeDirection);
        }
		else
		// directional light
		{
			lightDirection = normalize(Lights[0].position);
			halfVector = normalize(lightDirection + eyeDirection);
        }

	myNormal = normalize(Normal);

        float cosineVals[8];

        int i;

        for(i = 0; i < 8; i++){
                float temp = float(i) / 7.0f;
                cosineVals[i] = temp;
        }


        float dotNL = dot(myNormal, lightDirection);

        for(i = 1; i < 8; i++){
                if (dotNL < cosineVals[i] && dotNL >= cosineVals[i-1]){
                        dotNL = cosineVals[i-1];
                        break;
                }
        }

        diffuseCoeff  = max(0.0, dotNL);
        specularCoeff = max(0.0, dot(myNormal, halfVector));


	scatteredLight += Lights[0].color * ambient +
                  Lights[0].color * (diffuseCoeff * diffuse);

	vec3 rgb = min(scatteredLight, vec3(1.0));

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
