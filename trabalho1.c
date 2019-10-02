#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Para compilar:
// gcc -lglut -lGL -lGLU -lm nome.c -o nome.out

#define TRIANGULO 1
#define QUADRADO 2
#define HEXAGONO 3

GLint primitiva;
GLfloat vertice[12],
		x_trans = 0.0, y_trans = 0.0,
		escala = 1.0,
		rotacao = 0.0;

void DesenhaEixos(void) {
	// eixos principais
	glBegin(GL_LINES);
		glVertex2i(250,0);
		glVertex2i(250,500);
	glEnd();

	glBegin(GL_LINES);
		glVertex2i(0,250);
		glVertex2i(500,250);
	glEnd();

	// eixos secundarios
	glColor3f(0.5f,0.5f,0.5f);	// cor cinza
	// horizontais
	glBegin(GL_LINES);
		glVertex2i(0,50);
		glVertex2i(500,50);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(0,150);
		glVertex2i(500,150);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(0,350);
		glVertex2i(500,350);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(0,450);
		glVertex2i(500,450);
	glEnd();
	// verticais
	glBegin(GL_LINES);
		glVertex2i(50,0);
		glVertex2i(50,500);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(150,0);
		glVertex2i(150,500);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(350,0);
		glVertex2i(350,500);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(450,0);
		glVertex2i(450,500);
	glEnd();	
}

void DesenhaTriangulo(void) {
	glBegin(GL_TRIANGLES);
		glVertex2f(vertice[0], vertice[1]);
		glVertex2f(vertice[2], vertice[3]);
		glVertex2f(vertice[4], vertice[5]);
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

	// Define a cor dos eixos
	glColor3f(1.0f,1.0f,1.0f);
	DesenhaEixos();

	// Define a cor corrente
	glColor3f(1.0f,1.0f,0.0f);

	// translacao
	glTranslatef(x_trans, y_trans, 0.0f);
	
	// escala
	glTranslatef(250.0, 250.0, 0.0);
	glScalef(escala, escala, 1.0);

	// rotacao
	glRotatef(rotacao, 0.0, 0.0, 1.0);
	glTranslatef(-250.0, -250.0, 0.0);

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

void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
	// Evita a divisao por zero
	if(h == 0) h = 1;
	       
	// Especifica as dimensões da Viewport
	glViewport(0, 0, w, h);

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Estabelece a janela de seleção (left, right, bottom, top)
	if (w <= h) 
		gluOrtho2D (0.0f, 500.0f, 0.0f, 500.0f*h/w);
	else 
		gluOrtho2D (0.0f, 500.0f*w/h, 0.0f, 500.0f);
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
	float pontoCentral, aresta;

	printf("Selecione o comportamento: \n");
	printf("1 - Regular\n");
	printf("2 - Irregular\n");
	scanf("%d", &opcao);

	printf("-------- Passo 3 --------\n");
	switch (opcao) {
	case 1:
		printf("Diga o ponto central: \n");
		scanf("%f", &pontoCentral);

		printf("Diga o tamanho da aresta: \n");
		scanf("%f", &aresta);

		if (primitiva == QUADRADO) {
			// centro + desvio + aresta
			vertice[0] = 250.0 + pontoCentral - aresta;	// x1 
			vertice[1] = 250.0 + pontoCentral + aresta;	// y1 
			vertice[2] = 250.0 + pontoCentral - aresta;	// x2 
			vertice[3] = 250.0 + pontoCentral - aresta;	// y2 
			vertice[4] = 250.0 + pontoCentral + aresta;	// x3 
			vertice[5] = 250.0 + pontoCentral - aresta;	// y3 
			vertice[6] = 250.0 + pontoCentral + aresta;	// x4 
			vertice[7] = 250.0 + pontoCentral + aresta;	// y4 
		} else if (primitiva == TRIANGULO) {
			vertice[0] = 250.0 + pontoCentral 		  ;	// x1 
			vertice[1] = 250.0 + pontoCentral + aresta;	// y1 
			vertice[2] = 250.0 + pontoCentral - aresta;	// x2 
			vertice[3] = 250.0 + pontoCentral - aresta;	// y2 
			vertice[4] = 250.0 + pontoCentral + aresta;	// x3 
			vertice[5] = 250.0 + pontoCentral - aresta;	// y3 
		} else if (primitiva == HEXAGONO) { //http://calculo.cc/temas/temas_geometria_analitica/recta/imagenes/problemas/geometrico/p_20_graf.gif
			vertice[0] = 250.0 + pontoCentral - aresta;	// x1 
			vertice[1] = 250.0 + pontoCentral + (aresta*sqrt(3));	// y1 
			vertice[2] = 250.0 + pontoCentral - (aresta*2);	// x2 
			vertice[3] = 250.0 + pontoCentral   	  ;	// y2 
			vertice[4] = 250.0 + pontoCentral - aresta;	// x3 
			vertice[5] = 250.0 + pontoCentral - (aresta*sqrt(3));	// y3 
			vertice[6] = 250.0 + pontoCentral + aresta;	// x4 
			vertice[7] = 250.0 + pontoCentral - (aresta*sqrt(3));	// y4 
			vertice[8] = 250.0 + pontoCentral + (aresta*2);	// x5 
			vertice[9] = 250.0 + pontoCentral 		  ;	// y5 
			vertice[10] = 250.0 + pontoCentral + aresta;	// x6 
			vertice[11] = 250.0 + pontoCentral + (aresta*sqrt(3));	// y6 
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

		for (size_t i = 0; i < 11; i++) {
			vertice[i] += 250.0;
		}
		
		break;
	}
}

// Gerenciamento do menu com as opções de cores           
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
void MenuPrincipal(int op){}

// Criacao do Menu
void CriaMenu() 
{
    int menu, submenu1, submenu2, submenu3;

	submenu1 = glutCreateMenu(MenuTranslacao);
	glutAddMenuEntry("Acima",0);
	glutAddMenuEntry("Abaixo",1);
	glutAddMenuEntry("Direita",2);
	glutAddMenuEntry("Esquerda",3);

    submenu2 = glutCreateMenu(MenuEscala);
	glutAddMenuEntry("x0.5",0);
	//glutAddMenuEntry("x1.5",1);
	glutAddMenuEntry("x2.0",1);

    submenu3 = glutCreateMenu(MenuRotacao);
	glutAddMenuEntry("30º",0);
	glutAddMenuEntry("45º",1);
	glutAddMenuEntry("60º",2);
	
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
	glutReshapeFunc(AlteraTamanhoJanela);
	
	glutMouseFunc(GerenciaMouse);
	
	Inicializa();
	glutMainLoop();

	return 0;
}