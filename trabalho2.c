/*	=========================================================================
*				    TRABALHO 2: Curvas em 2D usando OpenGL
*
*   Desenvolver um programa que utilize rotinas OpenGL e que atenda
*   aos seguintes requisitos:
*   R1) O programa deverá desenhar uma Curva Bézier em formato
*   2D conforme pontos de controle inseridos pelo usuário.
*   R2) O usuário poderá informar por digitação 3, 5 ou 7 pontos de
*   controle para a curva.
*   R3) o usuário poderá informar alternativamente por cliques de
*   mouse 3, 5 ou 7 pontos de controle para a curva.
*   R4) Deverá ser desenhado um sistema de referência na tela de
*   visualização, com escala indicativa das distâncias (dividir em 10
*   segmentos cada eixo do sistema de coordenadas).
*   R5) As entradas de coordenadas devem ser validadas conforme
*   segue:
*       - As coordenadas devem estar dentro dos limites do
*       sistema de coordenadas do universo (da tela);
*       - Não permitir a entrada de pontos repetidos (iguais);
*       - Permitir a entrada de pontos alinhados.
*   
*   Todos os requisitos foram atendidos e implementados.
*       
*	Este software foi desenvolvido por:
*	- Lucas Costa 
*	- RA: 1683993
*	=========================================================================   
*/

// Para compilar e executar (em linux):
// gcc -lglut -lGL -lGLU -lm nome_do_arquivo.c -o nome_do_arquivo.out && ./nome_do_arquivo.out

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// >>>>>> VARIAVEIS GLOBAIS >>>>>>
// define se deve haver uso do mouse
int mouse = 0;
// pontos da curva a ser plotada
GLfloat pontos_controle_curva[7][3];
GLint qtde_pontos = 0, qtde_contador = 0;

// Funcao com unico proposito de desenhar os eixos no plano do desenho
void DesenhaEixos(void) {
    int i = 0;
	// eixos secundarios
	glColor3f(0.7 ,0.7 ,0.7 );
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

    // eixos principais
    glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_LINES);
		glVertex2f(0, -5.0);
		glVertex2f(0, 5.0);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(-5.0, 0);
		glVertex2f(5.0, 0);
	glEnd();
}

//	>>>>>> Funcao principal de desenho na tela >>>>>>
//  Responsavel por aplicar os detalhes da curva definida 
void Desenha(void) {
    int i;

    // definicoes padrao
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    // desenha os eixos da tela
    DesenhaEixos();

    // desenha os pontos definidos por clique ou manual
    glPointSize(7.0);
    glColor3f(0.4, 0.4, 1.0);
    glBegin(GL_POINTS);
        for (i = 0; i < qtde_contador; i++) 
            glVertex3fv(&pontos_controle_curva[i][0]);
    glEnd();

    // desenha a linha que liga os pontos definidos
    glLineWidth(3.0);
    glColor3f(0.7, 0.7, 1.0);
    glBegin(GL_LINE_STRIP);
        for (i = 0; i < qtde_contador; i++) 
            glVertex3fv(&pontos_controle_curva[i][0]);
    glEnd();
    
    // >>>>>> Funcao de desenho da curva >>>>>>
    // apenas realiza o desenho apos finalizar a definicao de pontos
    // necessario para quando os pontos sao definidos por clique
    if(qtde_pontos == qtde_contador) {
        glLineWidth(3.0);
        glColor3f(1.0, 1.0, 0.0);

        glShadeModel(GL_FLAT);
        
        // define um avaliador unidimensional que usa a equacao polinomial de Bernstein para 
        // uma quantidade n de pontos, caracterizando uma curva de Bezier.
        // mais detalhes em https://www.glprogramming.com/red/chapter12.html
        glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, qtde_pontos, &pontos_controle_curva[0][0]);
        // habilita o avaliador
        glEnable(GL_MAP1_VERTEX_3);
        // desenha a curva com resolucao de 30 pontos
        glBegin(GL_LINE_STRIP);
            for (i = 0; i <= 30; i++) 
                glEvalCoord1f((GLfloat) i/30.0);
        glEnd();
    }

    // cor do fundo
    glClearColor(0.2, 0.2, 0.2, 0.0);
    glFlush();
}

/*	>>>>>> Funcao de alteracao de tamanho de janela >>>>>>
	Permite que o usuario altere o tamanho da janela sem afetar
	as dimensoes do objeto desenhado. Alem disso, permite redefinir
	o ponto central da janela, como sendo o centro real e nao o 
	canto inferior esquerdo
*/
void AlteraTamanhoJanela(int w, int h) {
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

// >>>>>> Funcao de gerenciamento de botoes do mouse >>>>>>
// Permite desenhar a curva a partir de cliques do mouse
void GerenciaMouse(int button, int state, int x, int y) {
    int temp_contador;
    // pontos da curva
    float xt, yt;

    // identifica cliques do botao esquerdo
    if (button == GLUT_LEFT_BUTTON)
        // apenas para a quantidade de pontos escolhida
        if (state == GLUT_DOWN && qtde_contador < qtde_pontos) {
            // transforma para a escala usada
            xt = (float)(x - 250)/50;
            yt = (float)(y - 250)/50*(-1);

            // adiciona aos pontos de controle
            pontos_controle_curva[qtde_contador][0] = (GLfloat) xt;
            pontos_controle_curva[qtde_contador][1] = (GLfloat) yt; 
            pontos_controle_curva[qtde_contador][2] = 0.0;
            
            // verifica se houve repeticao
            for(temp_contador = qtde_contador-1; temp_contador >= 0; temp_contador--) {
                if(pontos_controle_curva[temp_contador][0] == xt || pontos_controle_curva[temp_contador][1] == yt) {
                    printf("\n***** Nao repita pontos! *****\n");
                    qtde_contador--;
                }
            }

            printf("%d: x=%.2f; y=%.2f\n", qtde_contador+1, pontos_controle_curva[qtde_contador][0], pontos_controle_curva[qtde_contador][1]);
            qtde_contador++;
        } 
    glutPostRedisplay();
}

// funcao de direcionamento do usuario
void menuPrincipal() {
    int opcao, valido = 0;

    printf("-------- Passo 1 --------\n");
	printf("Decida a quantidade de pontos da curva: \n");
    printf("3\n");
	printf("5\n");
    printf("7\n");
    scanf("%d", &qtde_pontos);

    // verifica se esta dentro do definido nos requisitos
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
        // habilita desenho por cliques do mouse
        mouse = 1;
        break;
    case 2:
        printf("\n======== ATENCAO ========\nO sistema de coordenadas vai de -5.0 a 5.0 em ambos os eixos");
        for (qtde_contador = 0; qtde_contador < qtde_pontos; qtde_contador++) {
            printf("\nDigite a coordenada x%d: ", qtde_contador+1);
            scanf("%f", &temp_x);
            printf("\nDigite a coordenada y%d: ", qtde_contador+1);
            scanf("%f", &temp_y);
            
            // verifica se pontos sao repetidos
            for(temp_contador = qtde_contador-1; temp_contador >= 0; temp_contador--) {
                if(pontos_controle_curva[temp_contador][0] == temp_x && pontos_controle_curva[temp_contador][1] == temp_y) {
                    printf("\n***** Nao repita pontos! *****\n");
                    qtde_contador--;
                    valido = 0;
                } else
                    valido = 1;
            }

            // se nao forem, verifica se estao dentro do universo
            if(valido){
                if((temp_x <= 5.0 && temp_y <= 5.0) && (temp_x >= -5.0 && temp_y >= -5.0)) {
                    pontos_controle_curva[qtde_contador][0] = temp_x;
                    pontos_controle_curva[qtde_contador][1] = temp_y;
                    pontos_controle_curva[qtde_contador][2] = 0.0;
                } else {
                    printf("\n***** Defina pontos validos (entre -5.0 e 5.0)! *****\n");
                    qtde_contador--;
                }
            }
        }
        break;
    default:
        printf("\nOpcao nao existe. \nFinalizando...\n");
        exit(-1);
        break;
    }
}

int main(int argc, char** argv)
{
    // exibe menu com primeiras opcoes	
	menuPrincipal();

    // define propriedades da janela
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("Trabalho 2");
    
    // inicia funcoes de desenho
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    if(mouse)
        glutMouseFunc(GerenciaMouse);
    
    glutMainLoop();

    return 0;
}