#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main()
{
   gl_Position = uProjection * uView * uModel * vec4(aPos.x, aPos.y, 0.0f, 1.0f);
}