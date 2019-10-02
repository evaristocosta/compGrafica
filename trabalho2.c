#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>

GLfloat ctrlpoints[7][3] = {
        { -4.0, -4.0, 0.0}, 
        { -2.0, 4.0, 0.0}, 
        {2.0, -4.0, 0.0}, 
        {4.0, 5.0, 0.0}, 
        {4.0, 4.0, 0.0}}; 
GLint qtde_pontos = 0;

void DesenhaEixos(void) {
	// eixos secundarios
	glColor3f(0.5f,0.5f,0.5f);	// cor cinza
    int i = 0;
    for (i = 0; i < 10; i++) {
        // verticais
        glBegin(GL_LINES);
            glVertex2f((GLfloat) i-5.0,-5.0);
            glVertex2f((GLfloat) i-5.0,5.0);
        glEnd();

        // horizontais
        glBegin(GL_LINES);
            glVertex2f(-5.0,(GLfloat) i-5.0);
            glVertex2f(5.0,(GLfloat) i-5.0);
        glEnd();
    }

    glColor3f(1.0f,1.0f,1.0f);	// cor branca
    // eixos principais
	glBegin(GL_LINES);
		glVertex2f(0,-5.0);
		glVertex2f(0,5.0);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(-5.0,0);
		glVertex2f(5.0,0);
	glEnd();
}

void Desenha(void) {
    int i;

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    // Cor da curva
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glShadeModel(GL_FLAT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);

    DesenhaEixos();
	
    glBegin(GL_LINE_STRIP);
        for (i = 0; i <= 30; i++) 
            glEvalCoord1f((GLfloat) i/30.0);
    glEnd();
    /* The following code displays the control points as a line. */
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_STRIP);
        for (i = 0; i < qtde_pontos; i++) 
            glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
                5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
    else
        glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 
                5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    qtde_pontos = 3;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);

    glutCreateWindow ("Trabalho 2");
    //init ();
    glutDisplayFunc(Desenha);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}