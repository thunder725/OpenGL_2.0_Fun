#version 330 core
out vec4 FragColor;
in vec4 ourColor;
uniform vec4 inColor;

void main()
{
    // Add the base Color (white) to the color provided
    FragColor = ourColor + inColor;
}