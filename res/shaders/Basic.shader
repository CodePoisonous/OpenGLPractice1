#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float textSlotIndex;

out vec2 v_TexCoord;
out float v_TextSlotIndex;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_TextSlotIndex = textSlotIndex;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TextSlotIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[2];

void main()
{
	vec4 texColor = texture(u_Textures[int(v_TextSlotIndex)], v_TexCoord);
	color = texColor;
	//color = vec4(1.0);
};