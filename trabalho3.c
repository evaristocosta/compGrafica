// Para compilar e executar (em linux):
// gcc -lglut -lGL -lGLU -lm trabalho3.c -o trabalho3.out && ./trabalho3.out

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

GLfloat angle, fAspect;

// Função callback chamada para fazer o desenho
void DesenhaEixos () {
    int i = 0;

    glColor3f(0.6, 0.6, 0.6);
    glLineWidth(1.0);
    for (i = -5; i <= 5; i++) {
        // verticais
        glBegin(GL_LINES);
        glVertex3f((GLfloat)i * 10, 0.0, -50);
        glVertex3f((GLfloat)i * 10, 0.0, 50);
        glEnd();

        // horizontais
        glBegin(GL_LINES);
        glVertex3f(-50, 0.0, (GLfloat)i * 10);
        glVertex3f(50, 0.0, (GLfloat)i * 10);
        glEnd();
    }

    glLineWidth(2.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 75, 0);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(75, 0, 0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 75);
    glEnd();

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);
    // glLineWidth(1.5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, -75, 0);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(-75, 0, 0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, -75);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glLineWidth(1.0);
}


void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    DesenhaEixos();
    
    glColor3f(1.0, 0.25, 0.0);

    // Desenha o teapot com a cor corrente (wire-frame)
    glutWireTeapot(20.0);

    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa(void) {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    angle = 45;
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void) {
    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Especifica a projeção perspectiva
    gluPerspective(angle, fAspect, .5, 500);

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    // Especifica posição do observador e do alvo
    gluLookAt(80, 80, 200, 0, 0, 0, 0, 1, 0);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    // Para previnir uma divisão por zero
    if (h == 0) h = 1;

    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);

    // Calcula a correção de aspecto
    fAspect = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-in
            if (angle >= 10) angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-out
            if (angle <= 130) angle += 5;
        }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

// Programa Principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(650, 600);
    glutCreateWindow("Visualizacao 3D");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    Inicializa();
    glutMainLoop();

    return 0;
}
