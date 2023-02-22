#include "Render.h"

Render::Render()
{
}

Render::~Render()
{
}

void Render::Draw(Mesh* inmesh)
{
	glBindVertexArray(inmesh->mVAO);
	glDrawElements(GL_TRIANGLES, inmesh->mSizeofIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

