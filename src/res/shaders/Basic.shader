#shader vertex
#version 330 core

//this specifies the location of the data in the array buffer that is sent
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aColor;

out vec4 vertexColor;

void main()
{
	//set both the position for the vertex given what is in the array, and also set up vertex color to be pased in
   gl_Position = vec4(-aPos.xyz, 1.0);
   vertexColor = vec4(aColor.xyz, 1.0); 
};



#shader fragement
#version 330 core
out vec4 FragColor;		  //the out variable that this passes through to the pipline to color the pixe
in vec4 vertexColor;	  //the variable that was passed from vertex shader earlier down the line

//uniform vec4 globalColor; //a uniform is a global variable that can be accessed by any shader in the program

void main()
{
	//have the output color be the already set up vertex color
   FragColor = vertexColor;
};
