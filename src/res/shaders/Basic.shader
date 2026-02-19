#shader vertex
#version 330 core

//this specifies the location of the data in the array buffer that is sent
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	//set both the position for the vertex given what is in the array, and also set up vertex color to be pased in
   gl_Position = projection * view * model * vec4(aPos.xyz, 1.0);
   TexCoord = aTexCoord;
};



#shader fragement
#version 330 core
in vec2 TexCoord;

out vec4 FragColor;		  //the out variable that this passes through to the pipline to color the pixe

uniform sampler2D texture1;

void main()
{
   FragColor = texture(texture1, TexCoord);
};
