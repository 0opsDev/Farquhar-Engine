#include"VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &ID); //ep2

	glBindBuffer(GL_ARRAY_BUFFER, ID); //ep2
	//store verts in vbo
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); //ep2 
}

void VBO::Bind() 
{
	glBindBuffer(GL_ARRAY_BUFFER, ID); //ep2
}
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); //ep2
}
void VBO::Delete()
{
	glDeleteBuffers(1, &ID); //ep2
}