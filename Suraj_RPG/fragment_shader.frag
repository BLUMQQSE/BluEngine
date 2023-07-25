varying in vec4 vert_pos;

uniform sampler2D texture;
uniform bool hasTexture;
uniform vec2 lightPos;
uniform float intensity = 1;
uniform vec4 lightColor = vec4(1,1,1,1);

void main()
{
	//Ambient light
	vec4 ambient = vec4(0.2, 0.02, 0.2, 1.0);
	
	//Convert light to view coords
	lightPos = (gl_ModelViewProjectionMatrix * vec4(lightPos, 0, 1)).xy;
	
	//Calculate the vector from light to pixel (Make circular)
	vec2 lightToFrag = lightPos - vert_pos.xy;
	lightToFrag.y = lightToFrag.y / 1.7;

	//Length of the vector (distance)
	float vecLength = clamp(length(lightToFrag) * 2, 0, 1);

    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color and lighting

	lightColor.x = lightColor.x * intensity;
	lightColor.y = lightColor.y * intensity;
	lightColor.z = lightColor.z * intensity;

	gl_Color = lightColor;
	//gl_FragColor = gl_Color * pixel * (clamp(ambient + vec4(1-vecLength, 1-vecLength, 1-vecLength, 1), 0, 1));

	if(hasTexture == true)
	{
		gl_FragColor = gl_Color * pixel * (clamp(ambient + vec4(1-vecLength, 1-vecLength, 1-vecLength, 1), 0, 1));
	}
	else
	{
		gl_FragColor = gl_Color * (vec4(1-vecLength,1-vecLength,1-vecLength,1)+ambient);
	}
}