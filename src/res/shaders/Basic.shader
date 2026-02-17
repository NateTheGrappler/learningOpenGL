#shader vertex
#version 330 core

//this specifies the location of the data in the array buffer that is sent
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

void main()
{
	//set both the position for the vertex given what is in the array, and also set up vertex color to be pased in
   gl_Position = vec4(aPos.xyz, 1.0);
   vertexColor = vec4(aColor.xyz, 1.0); 
   TexCoord = aTexCoord;
};



#shader fragement
#version 330 core
in vec4 vertexColor;	  //the variable that was passed from vertex shader earlier down the line
in vec2 TexCoord;

out vec4 FragColor;		  //the out variable that this passes through to the pipline to color the pixe

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
   //FragColor = texture(ourTexture, TexCoord);
   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
};
