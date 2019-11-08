// Para compilar e executar (em linux):
// gcc -lglut -lGL -lGLU -lm trabalho3.c GLP/glutp.c -o trabalho3.out &&
// ./trabalho3.out

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLP/glutp.h"

// inteiros de definicao para os objetos que podem ser construidos
#define CUBO 1
#define ESFERA 2
#define CONE 3
#define TETRAEDRO 4

GLint primitiva;
GLfloat angulo, aspecto, tamanho, resolucao;
GLfloat transl_x = 0.0, transl_y = 0.0, transl_z = 0.0;
GLfloat rot_x = 0.0, rot_y = 0.0, rot_z = 0.0, rot_angl = 0.0;

bool aramado = true;

// Função callback chamada para fazer o desenho
void DesenhaEixos() {
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
    glVertex3f(0, 25, 0);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(25, 0, 0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 25);
    glEnd();

    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0F0F);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, -25, 0);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(-25, 0, 0);
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, -25);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glLineWidth(1.0);
}

void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    DesenhaEixos();

    glColor3f(1.0, 1.0, 0.0);

    // efetua translacao
    glTranslatef(transl_x, transl_y, transl_z);
    glRotatef(rot_x, 1.0, 0.0, 0.0);
    glRotatef(rot_y, 0.0, 1.0, 0.0);
    glRotatef(rot_z, 0.0, 0.0, 1.0);

    // Desenha o teapot com a cor corrente (wire-frame)
    switch (primitiva) {
        case CUBO:
            aramado ? glutWireCube(tamanho) : glutSolidCube(tamanho);
            break;
        case ESFERA:
            aramado ? glutWireSphere(tamanho, resolucao, resolucao)
                    : glutSolidSphere(tamanho, resolucao, resolucao);
            break;
        case CONE:
            aramado ? glutWireCone(tamanho / 2, tamanho, resolucao, resolucao)
                    : glutSolidCone(tamanho / 2, tamanho, resolucao, resolucao);
            break;
        case TETRAEDRO:
            aramado ? glutWireTetrahedronP(tamanho)
                    : glutWireTetrahedronP(tamanho);

            break;
    }

    glTranslatef(-transl_x, -transl_y, -transl_z);
    glRotatef(-rot_x, 1.0, 0.0, 0.0);
    glRotatef(-rot_y, 0.0, 1.0, 0.0);
    glRotatef(-rot_z, 0.0, 0.0, 1.0);

    // Executa os comandos OpenGL
    glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa(void) {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    angulo = 45;
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void) {
    // Especifica sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    // Inicializa sistema de coordenadas de projeção
    glLoadIdentity();

    // Especifica a projeção perspectiva
    gluPerspective(angulo, aspecto, .5, 500);

    // Especifica sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    // Inicializa sistema de coordenadas do modelo
    glLoadIdentity();

    // Especifica posição do observador e do alvo
    gluLookAt(80, 80, 100, 0, 0, 0, 0, 1, 0);
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    // Para previnir uma divisão por zero
    if (h == 0) h = 1;

    // Especifica o tamanho da viewport
    glViewport(0, 0, w, h);

    // Calcula a correção de aspecto
    aspecto = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

void GerenciaTeclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'z':
        case 'Z':
            aramado = !aramado;
            break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-in
            if (angulo >= 10) angulo -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-out
            if (angulo <= 130) angulo += 5;
        }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}
void menu() {
    int opcao;

    printf("-------- Passo 1 --------\n");
    printf("Selecione o objeto: \n");
    printf("1 - Cubo\n");
    printf("2 - Esfera\n");
    printf("3 - Cone\n");
    printf("4 - Tetraedro\n");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Defina o tamanho:\n");
            scanf("%f", &tamanho);
            primitiva = CUBO;
            break;
        case 2:
            printf("Defina o tamanho (raio):\n");
            scanf("%f", &tamanho);
            printf("Defina a resolucao:\n");
            scanf("%f", &resolucao);
            primitiva = ESFERA;
            break;
        case 3:
            printf("Defina a base (raio):\n");
            scanf("%f", &tamanho);
            printf("Defina a resolucao:\n");
            scanf("%f", &resolucao);
            primitiva = CONE;
            break;
        case 4:
            printf("Defina o tamanho:\n");
            scanf("%f", &tamanho);
            primitiva = TETRAEDRO;
            break;
    }
}

void menuSecundario() {
    int opcao;
    char eixo;

    printf("---- Entrada manual ----\n");
    printf("Escolha uma operacao:\n");
    printf("1- Alterar representacao (solido/aramado)\n");
    printf("2- Transladar\n");
    printf("3- Rotacionar\n");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            aramado = !aramado;
            break;
        case 2:
            printf("Defina o eixo (x/y/z):\n");
            scanf(" %c", &eixo);
            printf("Defina a posicao:\n");
            switch (eixo) {
                case 'x':
                    scanf("%f", &transl_x);
                    break;
                case 'y':
                    scanf("%f", &transl_y);
                    break;
                case 'z':
                    scanf("%f", &transl_z);
                    break;
                default:
                    break;
            }

            break;
        case 3:
            printf("Defina o eixo (x/y/z):\n");
            scanf(" %c", &eixo);
            printf("Defina o angulo neste eixo:\n");
            switch (eixo) {
                case 'x':
                    scanf("%f", &rot_x);
                    break;
                case 'y':
                    scanf("%f", &rot_y);
                    break;
                case 'z':
                    scanf("%f", &rot_z);
                    break;
                default:
                    break;
            }

            break;
        default:
            printf("Opcao invalida!\n");
            break;
    }
    
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

// Programa Principal
int main(int argc, char** argv) {
    menu();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(650, 600);
    glutCreateWindow("Visualizacao 3D");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutMouseFunc(GerenciaMouse);
    glutKeyboardFunc(GerenciaTeclado);
    Inicializa();

    glutIdleFunc(menuSecundario);
    glutMainLoop();

    return 0;
}
