#shader vertex
#version 450 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 Color;
layout(location = 3) in float texIndex;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_texIndex;
uniform mat4 u_ProjMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Transform;

void main()
{
   gl_Position = u_ProjMatrix * u_ViewMatrix * u_Transform * position;
   v_TexCoord = texCoord;
   v_texIndex = texIndex;
   v_Color = Color;
};

#shader fragment
#version 450 core
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_texIndex;


uniform sampler2D u_Texture[32];

void main()
{
	vec4 texColor = texture(u_Texture[int(v_texIndex)], v_TexCoord);
	color = v_Color * texColor;
};