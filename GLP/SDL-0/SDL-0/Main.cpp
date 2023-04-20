#include <iostream>
#include <SDL.h>
#include <glew.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

#define GLEW_STATIC

float shiftingSpeed = 90.00f;

struct HSV_Struct
{
	float hue;
	float saturation;
	float value;
};

struct RGB_Struct
{
	float red;
	float green;
	float blue;
};

string LoadShader(string fileName) {
	ifstream myFile;
	myFile.open(fileName);
	if (myFile.fail()) {
		cerr << "Error - failed to open " << fileName << endl;
	}
	string fileText = "";
	string line = "";
	while (getline(myFile, line)) {
		fileText += line + '\n';
	}

	myFile.close();
	return fileText;
}

// Code from https://www.rapidtables.com/convert/color/hsv-to-rgb.html
RGB_Struct HSVToRGB(HSV_Struct HSVColor)
{
	RGB_Struct _temp = RGB_Struct{0.0f, 0.0f, 0.0f};

	float C = HSVColor.value * HSVColor.saturation;

	/*
	int _stepOne = (int)(HSVColor.hue / 60 * 10000);
	float _stepTwo = (_stepOne % 2) / 10000;
	float _stepThree = _stepTwo - 1;
	float _stepFour = std::abs(_stepThree);
	float _stepFive = 1 - _stepFour;
	float _stepSix = C * _stepFive;
	float X = _stepSix;
	*/

	float X = C * (1 - abs((fmod((HSVColor.hue / 60), 2) - 1)));
	float m = HSVColor.value - C;

	if (HSVColor.hue >= 0 && HSVColor.hue < 60)
	{
		_temp.red = C;
		_temp.green = X;
		_temp.blue = 0;
	}
	else if (HSVColor.hue >= 60 && HSVColor.hue < 120)
	{
		_temp.red = X;
		_temp.green = C;
		_temp.blue = 0;
	}
	else if (HSVColor.hue >= 120 && HSVColor.hue < 180)
	{
		_temp.red = 0;
		_temp.green = C;
		_temp.blue = X;
	}
	else if (HSVColor.hue >=180 && HSVColor.hue < 240)
	{
		_temp.red = 0;
		_temp.green = X;
		_temp.blue = C;
	}
	else if (HSVColor.hue >= 240 && HSVColor.hue < 300)
	{
		_temp.red = X;
		_temp.green = 0;
		_temp.blue = C;
	}
	else
	{
		_temp.red = C;
		_temp.green = 0;
		_temp.blue = X;
	}


	_temp.red += m;
	_temp.green += m;
	_temp.blue += m;

	return _temp;
}

// Code from https://www.rapidtables.com/convert/color/rgb-to-hsv.html
HSV_Struct RGBToHSV(RGB_Struct RGBColor)
{
	HSV_Struct _temp = HSV_Struct{ 0.0f, 0.0f, 0.0f};

	float Cmax = max(RGBColor.red, RGBColor.green);
	Cmax = max(RGBColor.blue, Cmax);
	float Cmin = min(RGBColor.red, RGBColor.green);
	Cmin = min(RGBColor.blue, Cmin);
	float Delta = Cmax - Cmin;

	_temp.value = Cmax;

	if (Cmax == 0)
	{
		_temp.saturation = 0;
	}
	else
	{
		_temp.saturation = Delta / Cmax;
	}

	if (Delta == 0)
	{
		_temp.hue = 0;
	}
	else if (Cmax = RGBColor.red)
	{
		// it gives an error when using % so i'll use std::fmod instead
		_temp.hue = (float)(60 * (fmod(((RGBColor.green - RGBColor.blue) / Delta * 10000), 6)));
	}
	else if (Cmax = RGBColor.green)
	{
		_temp.hue = 60 * (((RGBColor.blue - RGBColor.red) / Delta) + 2);
	}
	else
	{
		_temp.hue = 60 * (((RGBColor.red - RGBColor.green) / Delta) + 4);
	}

	return _temp;
}



#define PI 3.14159265f


int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!";
	}


	///////////SETTING UP SDL/////////////
//Create a simple window
	int width = 720;
	int height = 720;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);
	//SDL_WINDOW_OPENGL is a u32 flag !


	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}



	//Set the viewing frame through which we will see the objects
	glViewport(0, 0, width, height);

	//Put the color you want here for the background
	glClearColor(0.153f, 0.153f, 0.16f, 1.0f);




	//Describe the shape by its vertices


	// Number of Exterior Points
	int numberOfExteriorPoints = 3;
	float sizeOfCircle = 0.7f;

	// Can't create an array with a variable size so just replace the first number by the value above
	float proceduralVertices[(3+2) * 3];

	// Do a loop for each exterior vertice,
	// + the interior one which is 0, 0, 0,
	// + the end one which is the same as index 1 (not the first point) to close the circle
	for (int i = 0; i < (numberOfExteriorPoints + 2); i++)
	{
		if (i == 0)
		{
			proceduralVertices[0] = 0.0f;
			proceduralVertices[1] = 0.0f;
			proceduralVertices[2] = 0.0f;
		}
		else
		{
			proceduralVertices[(3 * i)] = cos(  (i - 1) * PI * 2 / numberOfExteriorPoints  )  * sizeOfCircle;


			proceduralVertices[(3 * i) + 1] = sin(  (i - 1) * PI * 2 / numberOfExteriorPoints  )  * sizeOfCircle;


			proceduralVertices[(3 * i) + 2] = 0.0f;
		}

		// cout << proceduralVertices[(3*i)] << "  " << proceduralVertices[(3 * i) +1] << endl;
	}

	float vertices[] = {

		/*  Broken stuff
		//// positions             // colors
		0.0f, -0.8f, 0.0f,      1.0f, 0.0f, 0.0f, //H
		-0.8f, 0.6f, 0.0f,       0.0f, 1.0f, 0.0f,//I
		-0.4f, 0.4f, 0.0f,     1.0f, 1.0f, 1.0f,//F
		0.4f,  0.4f, 0.0f,      1.0f, 1.0f, 1.0f,//G
		0.8f,  0.6f, 0.0f,       0.0f, 1.0f, 0.0f,//J

		-0.4f, 0.4f, 0.0f,     0.0f, 1.0f, 1.0f,//F
		0.4f,  0.4f, 0.0f,      1.0f, 1.0f, 1.0f,//G
		0.0f,  0.8f, 0.0f,       1.0f, 1.0f, 1.0f,//E
		*/


		/*
		0.0f, -0.8f, 0.0f,      1.0f, 0.0f, 0.0f,
		-0.8f, 0.6f, 0.0f,       1.0f, 1.0f, 0.0f,
		-0.4f, 0.4f, 0.0f,     0.0f, 1.0f, 0.0f,
		0.0f, 0.8f, 0.0f,     0.0f, 1.0f, 1.0f,
		0.4f,  0.4f, 0.0f,      0.0f, 0.0f, 1.0f,
		0.8f,  0.6f, 0.0f,       1.0f, 0.0f, 1.0f,
		*/


		/*
		//  Circle with the colors in the vertices
		0.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,	
		-0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	
		0.0f, 0.7f, 0.0f,	   0.75f, 0.5f, 0.0f,	
		0.5f, 0.5f, 0.0f,     0.5f, 1.0f, 0.0f,
		0.7f, 0.0f, 0.0f,	   0.25f, 1.0f, 0.5f,	
		0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f,
		0.0f, -0.7f, 0.0f,      0.25f, 0.5f, 1.0f,
		-0.5f, -0.5f, 0.0f,     0.5f, 0.0f, 1.0f,
		-0.7f, 0.0f, 0.0f,     0.75f, 0.0f, 0.5f,
		-0.5f, 0.5f, 0.0f,     1.0f, 0.0f, 0.0f, */
		

		
		//  Circle with white vertices
		0.0f, 0.0f, 0.0f,      1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,
		-0.25f, 0.6f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.0f, 0.7f, 0.0f,	   1.0f, 1.0f, 0.0f,
		0.25f, 0.6f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,
		0.6f, 0.25f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.7f, 0.0f, 0.0f,	   1.0f, 0.0f, 0.0f,
		0.6f, -0.25f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,
		0.25f, -0.6f, 0.0f,		1.0f, 1.0f, 1.0f,
		0.0f, -0.7f, 0.0f,      1.0f, 1.0f, 1.0f,
		-0.25f, -0.6f, 0.0f,		1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,
		-0.6f, -0.25f, 0.0f,		1.0f, 1.0f, 1.0f,
		-0.7f, 0.0f, 0.0f,     1.0f, 1.0f, 1.0f,
		-0.6f, 0.25f, 0.0f,		1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,     1.0f, 1.0f, 1.0f,
		



		/*  宮  Symbol
		// ==========================  HAT  ==========================
		// Hat Bar
		-0.85f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.85f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.85f, 0.67f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.85f, 0.67f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Hat Left 
		-0.85f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.85f, 0.46f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.78f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.78f, 0.46f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Hat Right
		0.85f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.85f, 0.46f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.78f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.78f, 0.46f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Hat Top
		-0.11f, 0.87f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.04f, 0.91f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.03f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.07f, 0.75f, 0.0f,		0.0f, 0.0f, 0.0f,



		// ==========================  TOP SQUARE & LINK  ==========================

		// Top Square Top Line
		-0.68f, 0.54f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.68f, 0.54f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.68f, 0.47f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.68f, 0.47f, 0.0f,		0.0f, 0.0f, 0.0f,


		// Top Square Left Line
		-0.68f, 0.54f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.61f, 0.54f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.68f, 0.02f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.61f, 0.02f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Top Square Right Line
		0.68f, 0.54f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.61f, 0.54f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.68f, 0.02f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.61f, 0.02f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Top Square Bottom Line
		-0.68f, 0.16f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.68f, 0.16f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.68f, 0.09f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.68f, 0.09f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Squares Link
		-0.17f, 0.10f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.09f, 0.10f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.29f, -0.12f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.21f, -0.12f, 0.0f,		0.0f, 0.0f, 0.0f,


		// ==========================  BOTTOM SQUARE  ==========================

		// Bottom Square Top Line
		-0.74f, -0.12f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.74f, -0.12f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.74f, -0.19f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.74f, -0.19f, 0.0f,		0.0f, 0.0f, 0.0f,


		// Bottom Square Left Line
		-0.74f, -0.12f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.80f, -0.12f, 0.0f,	0.0f, 0.0f, 0.0f,
		-0.74f, -0.76f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.80f, -0.76f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Bottom Square Right Line
		0.74f, -0.12f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.80f, -0.12f, 0.0f,	0.0f, 0.0f, 0.0f,
		0.74f, -0.76f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.80f, -0.76f, 0.0f,		0.0f, 0.0f, 0.0f,

		// Bottom Square Bottom Line
		-0.74f, -0.63f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.74f, -0.63f, 0.0f,		0.0f, 0.0f, 0.0f,
		-0.74f, -0.70f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.74f, -0.70f, 0.0f,		0.0f, 0.0f, 0.0f,
		*/
	};


	//Create an ID to be given at object generation
	unsigned int vbo;

	//Pass how many buffers should be created and the reference of the ID to get the value set
	glGenBuffers(1, &vbo);

	string vs = LoadShader("ColorVertex.shader");
	const char* vertexShaderSource = vs.c_str();
	string fs = LoadShader("ColorFragment.shader");
	const char* fragmentShaderSource = fs.c_str();


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);


	//now that we have a vertex shader, let’s put the code text inside
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);


	//aaaaand… Compile !
	glCompileShader(vertexShader);


	//Do the same with the fragment shader !
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();



	//now attach shaders to use to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//and link it 
	glLinkProgram(shaderProgram);

	//now that the program is complete, we can use it 
	glUseProgram(shaderProgram);



	//Create one ID to be given at object generation
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//Finally send the vertices array in the array buffer 
	glBufferData(GL_ARRAY_BUFFER, sizeof(proceduralVertices), proceduralVertices, GL_STATIC_DRAW);



	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	//Use depth management
	glEnable(GL_DEPTH_TEST);

	//0 is our origin, the higher the z, the farther the object
	glDepthFunc(GL_LESS);



	//Create a space to save all of our 6 points' hues.
	float currentGlobalHueOffset = 0.00f;

	//	360 / total of exterior points to divide the color spectrum by
	float colorOffsetPerPoint = (360.0f / numberOfExteriorPoints);



	//GameLoop
	bool isRunning = true;
	while (isRunning) {
		// Inputs
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen



		//Shader to use next
		glUseProgram(shaderProgram);

		//VAO to use next
		glBindVertexArray(vao);

		//OMG WE FINALLY DRAW ! We use the GL_TRIANGLES primitive type
		//We draw from vertex 0 and we will be drawing 4 vertices



		/*  宮 DRAWING
		for (int i = 0; i < 13; i++)
		{
			glDrawArrays(GL_TRIANGLE_STRIP, 4 * i, 4);
		}
		*/


		glDrawArrays(GL_TRIANGLE_FAN, 0, 128  +2);


		//Get the time in seconds 
		float timeValue = (float)SDL_GetTicks() / 1000;



		float redColor = 0.00f;
		float greenColor = 0.00f;
		float blueColor = 0.00f;

		/* Stuff to interp between 2 colors
		// I want the color to be a gradient between #1C6CF0 and #7E45B6
		// 0.11f, 0.42f, 0.94f    &    0.71f, 0.27f, 0.49f
		// Difference is 0.6f, -0.15f, -0.45f

		// So I'll start at color 1 and use a sin to interpolate
		// I'll modify the sin to be between 0 and 1 instead of -1 and 1
		// when sin = 0 <=> color = 1     when sin = 1 <=> color = 2


		// SIKE! It'll be between #1C6CF0 and pure white because #7E45B6 is ugly


		// Sin between 0 and 1
		float usableTime = (sin(timeValue) / 2) + 0.5f;

		float redColor = 0.11f + (usableTime * 0.89f);
		float greenColor = 0.42f + (usableTime * 0.56f);
		float blueColor = 0.94f + (usableTime * 0.06f);
		*/


		// Now we're gonna do a little thing called HSV HueShifting
		// Update the point's hue modifier
		currentGlobalHueOffset = fmod(timeValue * shiftingSpeed, 360);		

		// Use program
		glUseProgram(shaderProgram);


		/*
		// "Uniform" is a data state value stuff like "public" or "static"
		// the INT is the location/ID/stuff of the variable
		// With this line we say "register the ID of the variable "inColor" in the program "shaderProgram" so I can access it later"
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "inColor");		
		// And then we Write the Uniform value whose ID we got beforehand
		glUniform4f(vertexColorLocation, (redColor), (greenColor), (blueColor), 1.0f);
		*/


		// Position to float vertically
		float xPosition = 1.0f;
		int XvertexPosLocation = glGetUniformLocation(shaderProgram, "XanimPos");
		glUniform1f(XvertexPosLocation, xPosition);
		float yPosition = (cos(timeValue * 1) / 7.0f);
		int YvertexPosLocation = glGetUniformLocation(shaderProgram, "YanimPos");
		glUniform1f(YvertexPosLocation, yPosition);

		glUniform1f(glGetUniformLocation(shaderProgram, "hueOffsetPerVertex"), colorOffsetPerPoint);
		glUniform1f(glGetUniformLocation(shaderProgram, "globalHueOffset"), currentGlobalHueOffset);

		SDL_GL_SwapWindow(Window); // Swapbuffer		
	}
	// Quit
	SDL_DestroyWindow(Window);
	SDL_GL_DeleteContext(Context);

	//cin.get();
	return 0;
}

/*
#include <iostream>
#include <SDL.h>
#include <glew.h>
using namespace std;
//#define GLEW_STATIC

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!\n";
	}
	///////////SETTING UP SDL/////////////
	//Create a simple window
	int width = 400;
	int height = 300;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);
	//SDL_WINDOW_OPENGL is a u32 flag !


	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}

	cin.get();
	return 0;
}
*/