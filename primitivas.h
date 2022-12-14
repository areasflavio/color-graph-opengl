#include <vector>
#include <cmath>

#include "cg.h"

void desenhaEsfera(unsigned int& id, color cor, GLfloat raio, GLuint nStacks, GLuint nSectors) {

	std::vector< std::vector<GLuint> > indices;

	std::vector<vec3> pontos;
	const GLfloat PI = 3.14159265359;

	GLfloat deltaPhi = PI / nStacks;
	GLfloat deltaTheta = 2 * PI / nSectors;

	for (GLuint i = 0; i <= nStacks; i++) {
		GLfloat phi = -PI / 2.0 + i * deltaPhi;
		GLfloat temp = raio * cos(phi);
		GLfloat y = raio * sin(phi);

		std::vector<GLuint> pt;

		for (GLuint j = 0; j < nSectors; j++) {
			GLfloat theta = j * deltaTheta;
			GLfloat x = temp * sin(theta);
			GLfloat z = temp * cos(theta);

			pontos.push_back(vec3(x, y, z));
			GLuint index = pontos.size() - 1;
			pt.push_back(index);
		} // next j

		indices.push_back(pt);
	} // next i

	glNewList(id, GL_COMPILE);

	glColor3fv(cor);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	for (GLuint i = 0; i < nStacks; i++) {
		glBegin(GL_TRIANGLE_STRIP);

		for (GLuint j = 0; j < nSectors; j++) {
			GLuint index = indices[i][j];
			glVertex3fv(&pontos[index].x);
			index = indices[i + 1][j];
			glVertex3fv(&pontos[index].x);

			if (j == nSectors - 1) {
				index = indices[i][0];
				glVertex3fv(&pontos[index].x);
				index = indices[i + 1][0];
				glVertex3fv(&pontos[index].x);
			}
		}
		glEnd();
	}
	glDisable(GL_CULL_FACE);
	glEndList();
}


void desenhaArestas(unsigned int id, color cor) {
	glNewList(id, GL_COMPILE);
	glColor3fv(cor);
	glBegin(GL_LINES);

	// A - B 
	glVertex3f(-25/3.0,50/3.0,-125/2.0);
	glVertex3f(-50/3.0,0,-150/2.0);

	// A - F
	glVertex3f(-50/3.0,0,-150/2.0);
	glVertex3f(-25/3.0,-50/3.0,-175/2.0);

	// B - C 
	glVertex3f( 25/3.0,50/3.0,-175/2.0);
	glVertex3f(-25/3.0,50/3.0,-125/2.0);

	// B - E 
	glVertex3f(-25/3.0,50/3.0,-125/2.0);
	glVertex3f( 25/3.0,-50/3.0,-125/2.0);

	// B - F 
	glVertex3f( -25/3.0,50/3.0,-125/2.0);
	glVertex3f( -25/3.0,-50/3.0,-175/2.0);

	// C - D 
	glVertex3f(25/3.0,50/3.0,-175/2.0);
	glVertex3f(50/3.0,0,-150/2.0);

	// C - E 
	glVertex3f( 25/3.0,50/3.0,-175/2.0);
	glVertex3f( 25/3.0,-50/3.0,-125/2.0);

	// C - F 
	glVertex3f(25/3.0,50/3.0,-175/2.0);
	glVertex3f(-25/3.0,-50/3.0,-175/2.0);

	// D - E
	glVertex3f(50/3.0,0,-150/2.0);
	glVertex3f(25/3.0,-50/3.0,-125/2.0);

	// F - E 
	glVertex3f(25/3.0,-50/3.0,-125/2.0);
	glVertex3f(-25/3.0,-50/3.0,-175/2.0);

	glEnd();
	glEndList();
}