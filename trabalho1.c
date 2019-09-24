#include <GL/glut.h>
#include <string.h>
#include <stdio.h>

// Para compilar:
// gcc -lglut -lGL -lGLU -lm nome.c -o nome.out

#define TRIANGULO 1
#define QUADRADO 2
#define HEXAGONO 3

GLint primitiva;
GLfloat vertice[12];

void DesenhaEixos(void) {
	glBegin(GL_LINES);
		glVertex2i(200,0);
		glVertex2i(200,400);
	glEnd();

	glBegin(GL_LINES);
		glVertex2i(0,200);
		glVertex2i(400,200);
	glEnd();

	glColor3f(0.5f,0.5f,0.5f);

	glBegin(GL_LINES);
		glVertex2i(0,100);
		glVertex2i(400,100);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(0,300);
		glVertex2i(400,300);
	glEnd();
	
	glBegin(GL_LINES);
		glVertex2i(100,0);
		glVertex2i(100,400);
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(300,0);
		glVertex2i(300,400);
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
		glVertex2f(-25.0f, -25.0f);
		glVertex2f(-25.0f, 25.0f);
		glVertex2f(25.0f, 25.0f);
		glVertex2f(25.0f, -25.0f);
	glEnd();
}
void DesenhaHexagono(void) {
     glBegin(GL_POLYGON);
		glVertex2i(100,100);
		glVertex2i(75,125);
		glVertex2i(100,150);
		glVertex2i(150,150);
		glVertex2i(175,125);
		glVertex2i(150,100);
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
    // Define a cor de fundo da janela de visualização como preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
		gluOrtho2D (0.0f, 400.0f, 0.0f, 400.0f*h/w);
	else 
		gluOrtho2D (0.0f, 400.0f*w/h, 0.0f, 400.0f);
}


int main(int argc, char** argv) {
	int opcao;
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

	vertice[0] = 200.0f;
	vertice[1] = 300.0f;
	vertice[2] = 100.0f;
	vertice[3] = 100.0f;
	vertice[4] = 300.0f;
	vertice[5] = 100.0f;


	//primitiva = HEXAGONO;
	glutInit (&argc, argv);
	// double buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// tamanho e posicao padrao
	glutInitWindowSize(400,400);
	//glutInitWindowPosition(20,20);

	// janela e desenha
	glutCreateWindow("Trabalho 1");
	glutDisplayFunc(Desenha);

	//funcao necessaria
	glutReshapeFunc(AlteraTamanhoJanela);
	Inicializa();
	glutMainLoop();

	return 0;
}