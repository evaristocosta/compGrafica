#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Para compilar:
// gcc -lglut -lGL -lGLU -lm nome.c -o nome.out

#define TRIANGULO 1
#define QUADRADO 2
#define HEXAGONO 3

float pontoCentral_x = 0.0, pontoCentral_y = 0.0, aresta = 0.0;
int criarComClique = 0;
GLint primitiva;
GLfloat vertice[12],
		x_trans = 0.0, y_trans = 0.0,
		escala = 1.0,
		rotacao = 0.0,
		reflexao = 1.0,
		cisalhamento = 0.0;


void DesenhaEixos(void) {
    int i = 0;
	// eixos secundarios
	glColor3f(0.5f,0.5f,0.5f);	// cor cinza
    for (i = -5; i < 5; i++) {
        // verticais
        glBegin(GL_LINES);
            glVertex2f((GLfloat) i*50,-250);
            glVertex2f((GLfloat) i*50,250);
        glEnd();

        // horizontais
        glBegin(GL_LINES);
            glVertex2f(-250,(GLfloat) i*50);
            glVertex2f(250,(GLfloat) i*50);
        glEnd();
    }

    // eixos principais
    glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_LINES);
		glVertex2f(0,-250);
		glVertex2f(0,250);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(-250,0);
		glVertex2f(250,0);
	glEnd();
}

void DesenhaTriangulo(void) {
	glBegin(GL_TRIANGLES);
		glVertex2f(vertice[0], vertice[1]*reflexao);
		glVertex2f(vertice[2], vertice[3]*reflexao);
		glVertex2f(vertice[4], vertice[5]*reflexao);
	glEnd();
}
void DesenhaQuadrado(void) {
	glBegin(GL_QUADS);
		glVertex2f(vertice[0], vertice[1]);
		glVertex2f(vertice[2], vertice[3]);
		glVertex2f(vertice[4], vertice[5]);
		glVertex2f(vertice[6], vertice[7]);
	glEnd();
}
void DesenhaHexagono(void) {
     glBegin(GL_POLYGON);
		glVertex2f(vertice[0], vertice[1]);
		glVertex2f(vertice[2], vertice[3]);
		glVertex2f(vertice[4], vertice[5]);
		glVertex2f(vertice[6], vertice[7]);
		glVertex2f(vertice[8], vertice[9]);
		glVertex2f(vertice[10], vertice[11]);
	glEnd();
}

void Desenha(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	           
	glClear(GL_COLOR_BUFFER_BIT);

	DesenhaEixos();

	// Define a cor corrente
	glColor3f(1.0f,1.0f,0.0f);

	// translacao
	glTranslatef(x_trans, y_trans, 0.0f);
	
	// escala
	glScalef(escala, escala, 1.0);

	// rotacao
	glRotatef(rotacao, 0.0, 0.0, 1.0);

	// cisalhamento
	GLfloat cisalhar[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		cisalhamento, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	glMultMatrixf(cisalhar);

	// Desenha uma primitiva     
	switch (primitiva) {
		case QUADRADO:  
			DesenhaQuadrado();
			break;
		case TRIANGULO: 
			DesenhaTriangulo();
	        break;
		case HEXAGONO:   
			DesenhaHexagono();
	        break;
	}

	glutSwapBuffers();
}

// Inicializa parâmetros de rendering
void Inicializa (void) {   
    // Define a cor de fundo da janela de visualização como cinza escuro
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-250.0, 250.0, -250.0*(GLfloat)h/(GLfloat)w, 
                250.0*(GLfloat)h/(GLfloat)w, -250.0, 250.0);
    else
        glOrtho(-250.0*(GLfloat)w/(GLfloat)h, 
                250.0*(GLfloat)w/(GLfloat)h, -250.0, 250.0, -250.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void menuPrincipal() {
	int opcao;

	printf("-------- Passo 1 --------\n");
	printf("Selecione o objeto: \n");
	printf("1 - Quadrado\n");
	printf("2 - Triangulo\n");
	printf("3 - Hexagono\n");
	scanf("%d", &opcao);

	switch(opcao) {
		case 1:
			primitiva = QUADRADO;
			break;
		case 2:
			primitiva = TRIANGULO;
			break;
		case 3:
			primitiva = HEXAGONO;
			break;
	}

	printf("-------- Passo 2 --------\n");

	printf("Selecione o comportamento: \n");
	printf("1 - Regular\n");
	printf("2 - Irregular\n");
	scanf("%d", &opcao);

	printf("-------- Passo 3 --------\n");
	switch (opcao) {
	case 1:
		printf("Diga o ponto central em x: \n");
		scanf("%f", &pontoCentral_x);

		printf("Diga o ponto central em y: \n");
		scanf("%f", &pontoCentral_y);

		printf("Diga o tamanho da aresta: \n");
		scanf("%f", &aresta);

		if (primitiva == QUADRADO) {
			// centro + desvio + aresta
			vertice[0] = pontoCentral_x - aresta;	// x1 
			vertice[1] = pontoCentral_y + aresta;	// y1 
			vertice[2] = pontoCentral_x - aresta;	// x2 
			vertice[3] = pontoCentral_y - aresta;	// y2 
			vertice[4] = pontoCentral_x + aresta;	// x3 
			vertice[5] = pontoCentral_y - aresta;	// y3 
			vertice[6] = pontoCentral_x + aresta;	// x4 
			vertice[7] = pontoCentral_y + aresta;	// y4 
		} else if (primitiva == TRIANGULO) {
			vertice[0] = pontoCentral_x 		  ;	// x1 
			vertice[1] = pontoCentral_y + aresta;	// y1 
			vertice[2] = pontoCentral_x - aresta;	// x2 
			vertice[3] = pontoCentral_y - aresta;	// y2 
			vertice[4] = pontoCentral_x + aresta;	// x3 
			vertice[5] = pontoCentral_y - aresta;	// y3 
		} else if (primitiva == HEXAGONO) { //http://calculo.cc/temas/temas_geometria_analitica/recta/imagenes/problemas/geometrico/p_20_graf.gif
			vertice[0] = pontoCentral_x - aresta;	// x1 
			vertice[1] = pontoCentral_y + (aresta*sqrt(3));	// y1 
			vertice[2] = pontoCentral_x - (aresta*2);	// x2 
			vertice[3] = pontoCentral_y   	  ;	// y2 
			vertice[4] = pontoCentral_x - aresta;	// x3 
			vertice[5] = pontoCentral_y - (aresta*sqrt(3));	// y3 
			vertice[6] = pontoCentral_x + aresta;	// x4 
			vertice[7] = pontoCentral_y - (aresta*sqrt(3));	// y4 
			vertice[8] = pontoCentral_x + (aresta*2);	// x5 
			vertice[9] = pontoCentral_y 		  ;	// y5 
			vertice[10] = pontoCentral_x + aresta;	// x6 
			vertice[11] = pontoCentral_y + (aresta*sqrt(3));	// y6 
		}
		break;
	case 2:
		printf("Digite a seguir as coordenadas do objeto\n");
		printf("x1 = "); scanf("%f", &vertice[0]);
		printf("y1 = "); scanf("%f", &vertice[1]);

		printf("x2 = "); scanf("%f", &vertice[2]);
		printf("y2 = "); scanf("%f", &vertice[3]);

		printf("x3 = "); scanf("%f", &vertice[4]);
		printf("y3 = "); scanf("%f", &vertice[5]);

		if (primitiva == QUADRADO) {
			printf("x4 = "); scanf("%f", &vertice[6]);
			printf("y4 = "); scanf("%f", &vertice[7]);
		}
		if (primitiva == HEXAGONO) {
			printf("x5 = "); scanf("%f", &vertice[8]);
			printf("y5 = "); scanf("%f", &vertice[9]);

			printf("x6 = "); scanf("%f", &vertice[10]);
			printf("y6 = "); scanf("%f", &vertice[11]);
		}
		break;
	}
}

// Gerenciamento do menu      
void MenuTranslacao(int op) {
	switch(op) {
		case 0:
			y_trans += 10.0;
			break;
		case 1:
			y_trans -= 10.0;
			break;
		case 2:
			x_trans += 10.0;
			break;
		case 3:
			x_trans -= 10.0;
			break;
	}
	glutPostRedisplay();
}   

void MenuEscala(int op) {
	switch (op)	{
	case 0:
		escala /= 2;
		break;
	case 1:
		escala *= 2;
	}
}

void MenuRotacao(int op){
	switch (op) {
	case 0:
		rotacao += 30.0;
		break;
	case 1:
		rotacao += 45.0;
		break;
	case 2:
		rotacao += 60.0;
		break;
	}
}

// Gerenciamento do menu principal           
void MenuPrincipal(int op) {
	switch (op)	{
	case 0:
		reflexao *= -1.0;
		break;
	case 1:
		cisalhamento += 0.1;
		break;
	}
}

// Criacao do Menu
void CriaMenu() {
    int menu, submenu1, submenu2, submenu3;

	submenu1 = glutCreateMenu(MenuTranslacao);
	glutAddMenuEntry("Acima",0);
	glutAddMenuEntry("Abaixo",1);
	glutAddMenuEntry("Direita",2);
	glutAddMenuEntry("Esquerda",3);

    submenu2 = glutCreateMenu(MenuEscala);
	glutAddMenuEntry("x0.5",0);
	glutAddMenuEntry("x2.0",1);

    submenu3 = glutCreateMenu(MenuRotacao);
	glutAddMenuEntry("30 graus",0);
	glutAddMenuEntry("45 graus",1);
	glutAddMenuEntry("60 graus",2);

	menu = glutCreateMenu(MenuPrincipal);
	glutAddMenuEntry("Reflexao",0);
	glutAddMenuEntry("Cisalhamento",1);
	glutAddSubMenu("Translacao",submenu1);
    glutAddSubMenu("Escala",submenu2);
	glutAddSubMenu("Rotacao",submenu3);
    
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GerenciaMouse(int button, int state, int x, int y) {        
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) 
			CriaMenu();
			
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	// exibe menu com primeiras opcoes	
	menuPrincipal();

	glutInit (&argc, argv);
	// double buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// tamanho e posicao padrao
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);

	// janela e desenha
	glutCreateWindow("Trabalho 1");
	glutDisplayFunc(Desenha);

	// funcao necessaria
	glutReshapeFunc(reshape);
	
	glutMouseFunc(GerenciaMouse);
	
	Inicializa();
	glutMainLoop();

	return 0;
}