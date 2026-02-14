#shader vertex
#version 330 core
layout (location = 0) in vec4 aPos;
void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
};



#shader fragement
#version 330 core
out vec4 FragColor;
void main()
{
   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
};



#shader fragmentYellow
#version 330 core
out vec4 FragColor;
void main()
{
   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
};