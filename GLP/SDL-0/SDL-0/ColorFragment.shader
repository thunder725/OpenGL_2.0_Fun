#version 330 core

// Color to be output by the shader and recieved by the GPU
out vec4 FragColor;

// Color to be recieved from the GPU (by itself or by other shaders)
in vec4 ourColor;



void main()
{
    FragColor = ourColor;
}