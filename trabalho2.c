#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Para compilar e executar (em linux):
// gcc -lglut -lGL -lGLU -lm nome_do_arquivo.c -o nome_do_arquivo.out && ./nome_do_arquivo.out

int mouse = 0;
GLfloat ctrlpoints[7][3];
GLint qtde_pontos = 0;
GLint qtde_contador = 0;

void DesenhaEixos(void) {
	// eixos secundarios
    int i = 0;
	glColor3f(0.7 ,0.7 ,0.7 );	// cor cinza
    glLineWidth(1.0);
    for (i = 0; i < 10; i++) {
        // verticais
        glBegin(GL_LINES);
            glVertex2f((GLfloat) i-5.0, -5.0);
            glVertex2f((GLfloat) i-5.0, 5.0);
        glEnd();

        // horizontais
        glBegin(GL_LINES);
            glVertex2f(-5.0, (GLfloat) i-5.0);
            glVertex2f(5.0, (GLfloat) i-5.0);
        glEnd();
    }

    glColor3f(0.9, 0.9, 0.9);	// cor branca
    // eixos principais
	glBegin(GL_LINES);
		glVertex2f(0, -5.0);
		glVertex2f(0, 5.0);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(-5.0, 0);
		glVertex2f(5.0, 0);
	glEnd();
}

void Desenha(void) {
    int i;

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    DesenhaEixos();

    /* The following code displays the control points as a line. */
    glPointSize(7.0);
    glColor3f(0.4, 0.4, 1.0);
    glBegin(GL_POINTS);
        for (i = 0; i < qtde_contador; i++) 
            glVertex3fv(&ctrlpoints[i][0]);
    glEnd();

    glLineWidth(3.0);
    glColor3f(0.7, 0.7, 1.0);
    glBegin(GL_LINE_STRIP);
        for (i = 0; i < qtde_contador; i++) 
            glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
    
    
    // Curva
    if(qtde_pontos == qtde_contador) {
        printf("\nDesenho pronto!");
        glLineWidth(3.0);
        glColor3f(1.0, 1.0, 0.0);

        glShadeModel(GL_FLAT);
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, qtde_pontos, &ctrlpoints[0][0]);
        glEnable(GL_MAP1_VERTEX_3);

        glBegin(GL_LINE_STRIP);
            for (i = 0; i <= 30; i++) 
                glEvalCoord1f((GLfloat) i/30.0);
        glEnd();
    }

    glClearColor(0.2, 0.2, 0.2, 0.0);
    glFlush();
}

void reshape(int w, int h) {
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

void GerenciaMouse(int button, int state, int x, int y) {
    float xt, yt;
    int temp_contador;

    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN && qtde_contador < qtde_pontos) {
            xt = (float)(x - 250)/50;
            yt = (float)(y - 250)/50*(-1);

            ctrlpoints[qtde_contador][0] = (GLfloat) xt;
            ctrlpoints[qtde_contador][1] = (GLfloat) yt; 
            ctrlpoints[qtde_contador][2] = 0.0;
            
            for(temp_contador = qtde_contador-1; temp_contador >= 0; temp_contador--) {
                if(ctrlpoints[temp_contador][0] == xt || ctrlpoints[temp_contador][1] == yt) {
                    printf("\n***** Nao repita pontos! *****\n");
                    qtde_contador--;
                }
            }

            printf("%d: x=%.2f; y=%.2f\n", qtde_contador+1, ctrlpoints[qtde_contador][0], ctrlpoints[qtde_contador][1]);
            qtde_contador++;
        } 
    glutPostRedisplay();
}

void menuPrincipal() {
    int opcao, valido = 0;

    printf("-------- Passo 1 --------\n");
	printf("Decida a quantidade de pontos da curva: \n");
    printf("3\n");
	printf("5\n");
    printf("7\n");
    scanf("%d", &qtde_pontos);

    if(!(qtde_pontos == 3 || qtde_pontos == 5 || qtde_pontos == 7)) {
        printf("\nEscolha uma quantidade de pontos valida!\n");
        exit(-1);
    }
    
    printf("-------- Passo 2 --------\n");
	printf("Escolha como deseja desenhar: \n");
	printf("1 - Usando cliques de mouse\n");
	printf("2 - Digitando as coordenadas\n");
    scanf("%d", &opcao);
    GLfloat temp_x, temp_y;
    int temp_contador;

    switch (opcao) {
    case 1:
        printf("\nPode clicar %d vezes\nCoordenadas:\n", qtde_pontos);
        mouse = 1;
        break;
    case 2:
        printf("\n======== ATENCAO ========\nO sistema de coordenadas vai de -5.0 a 5.0 em ambos os eixos");
        for (qtde_contador = 0; qtde_contador < qtde_pontos; qtde_contador++) {
            printf("\nDigite a coordenada x%d: ", qtde_contador+1);
            scanf("%f", &temp_x);
            printf("\nDigite a coordenada y%d: ", qtde_contador+1);
            scanf("%f", &temp_y);
            
            for(temp_contador = qtde_contador-1; temp_contador >= 0; temp_contador--) {
                if(ctrlpoints[temp_contador][0] == temp_x && ctrlpoints[temp_contador][1] == temp_y) {
                    printf("\n***** Nao repita pontos! *****\n");
                    qtde_contador--;
                    valido = 0;
                } else
                    valido = 1;
            }

            if(valido){
                if((temp_x <= 5.0 && temp_y <= 5.0) && (temp_x >= -5.0 && temp_y >= -5.0)) {
                    ctrlpoints[qtde_contador][0] = temp_x;
                    ctrlpoints[qtde_contador][1] = temp_y;
                    ctrlpoints[qtde_contador][2] = 0.0;
                } else {
                    printf("\n***** Defina pontos validos (entre -5.0 e 5.0)! *****\n");
                    qtde_contador--;
                }
            }
        }
        break;
    default:
        printf("\nOpcao nao existe. \nFinalizando...\n");
        break;
    }
}

int main(int argc, char** argv)
{
    // exibe menu com primeiras opcoes	
	menuPrincipal();

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow ("Trabalho 2");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(reshape);
    if(mouse)
        glutMouseFunc(GerenciaMouse);
    
    glutMainLoop();

    return 0;
}