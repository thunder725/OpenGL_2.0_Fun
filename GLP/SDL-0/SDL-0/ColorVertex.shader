#version 330 core

// Position & Color to be given to the shader by the GPU or other shaders
in vec3 pos;
in vec3 color;
in int gl_VertexID;

// Output to the GPU & other shaders
out vec4 ourColor;

// Accessible from Main
uniform float YanimPos;
uniform float XanimPos;

uniform float hueOffsetPerVertex;
uniform float globalHueOffset;

vec4 colorFromHue(float hue)
{
	vec4 finalColor = vec4(1.0, 1.0, 1.0, 1.0);

	float X = 1.0 * (1.0 - abs((mod((hue / 60.0), 2) - 1.0)));
	
	if (hue >= 0.0 && hue < 60.0)
	{
		finalColor.x = 1.0;
		finalColor.y = X;
		finalColor.z = 0.0;
	}
	else if (hue >= 60.0 && hue < 120.0)
	{
		finalColor.x = X;
		finalColor.y = 1.0;
		finalColor.z = 0.0;
	}
	else if (hue >= 120.0 && hue < 180.0)
	{
		finalColor.x = 0.0;
		finalColor.y = 1.0;
		finalColor.z = X;
	}
	else if (hue >= 180.0 && hue < 240.0)
	{
		finalColor.x = 0.0;
		finalColor.y = X;
		finalColor.z = 1.0;
	}
	else if (hue >= 240.0 && hue < 300.0)
	{
		finalColor.x = X;
		finalColor.y = 0.0;
		finalColor.z = 1.0;
	}
	else
	{
		finalColor.x = 1.0;
		finalColor.y = 0.0;
		finalColor.z = X;
	}
	
	return (finalColor);
}


void main()
{
    // Move up & down with the (sin) offset we give
    gl_Position = vec4(pos.x * XanimPos, pos.y + YanimPos, pos.z, 1.0);

	// ourColor = vec4(VertexOffset/255.0);

	//ourColor = vec4(gl_VertexID / 130.0);

	//return;

	// Debug to check if gl_VertexID works (it works)
	// ourColor = vec4(gl_VertexID / 10.0);
	if (gl_VertexID == 0)
	{
		ourColor = vec4(1.0);
	}
	else
	{
		float VertexOffset = mod(globalHueOffset + (gl_VertexID * hueOffsetPerVertex), 360);

		ourColor = colorFromHue(VertexOffset);
	}
    
}