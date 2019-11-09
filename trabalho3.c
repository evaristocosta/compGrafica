/*	=========================================================================
 *	    			TRABALHO 3: Representação de Sólidos
 *
 *   Desenvolver um programa que utilize rotinas OpenGL (ou biblioteca
 *   gráfica equivalente) e que atenda aos seguintes requisitos:
 *   R1) Permitir ao usuário escolher no mínimo os seguintes sólidos a
 *   serem representados graficamente: Cubo, Esfera, Cone, Tetraedro.
 *   R2) Permitir ao usuário digitar (ou entrar via mouse se aplicável) os
 *   parâmetros que sejam necessários para o desenho dos sólidos.
 *   R3) Para qualquer um dos sólidos a que se refere o requisito R1,
 *   permitir ao usuário escolher entre a representação sólida (solid) ou
 *   aramada (wireframe).
 *   R4) Pode ser utilizado opcionalmente cliques de mouse para
 *   determinar o centro de cada figura, caso o desenvolvedor prefira
 *   fazer desta forma.
 *   R5) Desenhar a figura na tela conforme parâmetros de entrada
 *   fornecidos pelo usuário (requisitos R1 a R4).
 *   R6) O usuário poderá escolher qualquer uma das seguintes
 *   Transformações a serem realizadas sobre uma figura já desenhada:
 *       - Translação, Rotação.
 *   R7) Conforme a transformação escolhida, o programa deverá
 *   solicitar ao usuário os parâmetros correspondentes e necessários
 *   para realizar a transformação.
 *   R8) Após confirmação do usuário o programa aplica a
 *   transformação escolhida conforme os parâmetros informados (R5,
 *   R6 e R7), calcula e mostra o resultado na tela.
 *   R9) Deverá ser desenhado um sistema de referência 3D
 *   centralizado na tela de visualização, com escala indicativa das
 *   distâncias (sugestão: dividir em 10 segmentos cada eixo do sistema
 *   de coordenadas).
 *   R10) As entradas de coordenadas devem ser validadas conforme
 *   segue:
 *       - As coordenadas devem estar dentro dos limites do sistema de
 *   coordenadas do universo (da tela);
 *       - Não permitir a entrada de pontos repetidos (iguais).
 *
 *   Todos os requisitos foram atendidos e implementados.
 *
 *	Este software foi desenvolvido por:
 *	- Lucas Costa
 *	- RA: 1683993
 *	=========================================================================
 */

// Para compilar e executar (em linux):
// gcc -lglut -lGL -lGLU -lm trabalho3.c GLP/glutp.c -o trabalho3.out &&
// ./trabalho3.out

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLP/glutp.h"

// inteiros de definicao para os objetos que podem ser construidos
#define CUBO 1
#define ESFERA 2
#define CONE 3
#define TETRAEDRO 4

// >>>>>> VARIAVEIS GLOBAIS >>>>>>
GLint primitiva;
// definem objeto e perspectiva da camera
GLfloat angulo, aspecto, tamanho, tamanho_f, resolucao;
// usadas para translacao
GLfloat transl_x = 0.0, transl_y = 0.0, transl_z = 0.0;
// para rotacao
GLfloat rot_x = 0.0, rot_y = 0.0, rot_z = 0.0;
// para manipulacao de camera
GLint posicao_h = 80, posicao_v = 80, aproximacao = 100;
// para troca entre visao aramada ou nao
bool aramado = true;

// Funcao com unico proposito de desenhar os eixos no plano do desenho
void DesenhaEixos() {
    int i = 0;

    glColor3f(0.6, 0.6, 0.6);
    glLineWidth(1.0);
    // malha de horizonte
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

    // eixos sentido positivo
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

    // eixos sentido negativo (pontilhados)
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

//	>>>>>> Funcao principal de desenho na tela >>>>>>
// Responsavel pelo desenho de objetos, bem como verificacao de
// viabilidade
void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    DesenhaEixos();
    glColor3f(1.0, 1.0, 0.0);

    // variavel temporaria p/ verificar posicao
    float transl_temp = (100.0 - tamanho_f) / 2;

    // limite de translacao é o plano (100 x 100 x 100)
    // verifica para cada eixo
    if(tamanho_f + (abs(transl_x) * 2) > 100.0) {
        printf("\n***************\n");
        printf("Translacao maxima no eixo x atingida!");
        printf("\n***************\n");

        transl_x = (transl_x > 0) ? transl_temp : -transl_temp;
    } else if (tamanho_f + (abs(transl_y) * 2) > 100.0) {
        printf("\n***************\n");
        printf("Translacao maxima no eixo y atingida!");
        printf("\n***************\n");

        transl_y = (transl_y > 0) ? transl_temp : -transl_temp;
    } else if (tamanho_f + (abs(transl_z) * 2) > 100.0) {
        printf("\n***************\n");
        printf("Translacao maxima no eixo z atingida!");
        printf("\n***************\n");

        transl_z = (transl_z > 0) ? transl_temp : -transl_temp;
    }

        // efetua translacao
        glTranslatef(transl_x, transl_y, transl_z);
        // rotacao para cada eixo
        glRotatef(rot_x, 1.0, 0.0, 0.0);
        glRotatef(rot_y, 0.0, 1.0, 0.0);
        glRotatef(rot_z, 0.0, 0.0, 1.0);

        // Desenha o objeto desejado/selecionado
        switch (primitiva) {
            case CUBO:
                aramado ? glutWireCube(tamanho) : glutSolidCube(tamanho);
                break;
            case ESFERA:
                aramado ? glutWireSphere(tamanho, resolucao, resolucao)
                        : glutSolidSphere(tamanho, resolucao, resolucao);
                break;
            case CONE:
                aramado
                    ? glutWireCone(tamanho / 2, tamanho, resolucao, resolucao)
                    : glutSolidCone(tamanho / 2, tamanho, resolucao, resolucao);
                break;
            case TETRAEDRO:
                // FUNCAO PROPRIA
                aramado ? glutWireTetrahedronP(tamanho)
                        : glutSolidTetrahedronP(tamanho);

                break;
        }

        // cancela efeito de translacao e rotacao
        // para a camera
        glTranslatef(-transl_x, -transl_y, -transl_z);
        glRotatef(-rot_x, 1.0, 0.0, 0.0);
        glRotatef(-rot_y, 0.0, 1.0, 0.0);
        glRotatef(-rot_z, 0.0, 0.0, 1.0);

        // Executa os comandos OpenGL
        glutSwapBuffers();
}

// inicializa parâmetros de rendering
void Inicializa(void) {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    angulo = 45;
}

// especifica o volume de visualização
void EspecificaParametrosVisualizacao(void) {
    // sistema de coordenadas de projeção
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // projeção perspectiva
    gluPerspective(angulo, aspecto, .5, 500);

    // sistema de coordenadas do modelo
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // posição do observador e do alvo
    gluLookAt(posicao_h, posicao_v, aproximacao, 0, 0, 0, 0, 1, 0);
}

// callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;

    // tamanho da viewport
    glViewport(0, 0, w, h);
    // correção de aspecto
    aspecto = (GLfloat)w / (GLfloat)h;

    EspecificaParametrosVisualizacao();
}

//	>>>>>> Funcao de controle via teclado >>>>>>
/*  Quando nao é escolhida a entrada manual, é possivel performar
 *   transformacoes usando o teclado.
 *   Controles:
 *   - Translação:
 *       eixo x: r, t
 *       eixo y: f, g
 *       eixo z: c, v
 *   - Rotacao:
 *       eixo x: y, u
 *       eixo y: h, j
 *       eixo z: b, n
 *  - Camera:
 *       horizontal: a, d
 *       vertical: w, s
 *       zoom, q, e
 */
void GerenciaTeclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'z':
        case 'Z':
            aramado = !aramado;
            break;

        case 'd':
        case 'D':
            if (posicao_h <= 200) posicao_h += 10;
            if (aproximacao > 0) aproximacao -= 10;
            break;
        case 'a':
        case 'A':
            if (posicao_h > 0) posicao_h -= 10;
            if (aproximacao <= 200) aproximacao += 10;
            break;
        case 'w':
        case 'W':
            if (posicao_v <= 200) posicao_v += 10;
            break;
        case 's':
        case 'S':
            if (posicao_v > -200) posicao_v -= 10;
            break;
        case 'q':
        case 'Q':
            if (angulo >= 10) angulo -= 5;
            break;
        case 'e':
        case 'E':
            if (angulo <= 130) angulo += 5;
            break;

        // transladar
        case 't':
            transl_x += 5;
            break;
        case 'r':
            transl_x -= 5;
            break;
        case 'f':
            transl_y += 5;
            break;
        case 'g':
            transl_y -= 5;
            break;
        case 'c':
            transl_z += 5;
            break;
        case 'v':
            transl_z -= 5;
            break;

        // rotacionar
        case 'y':
            rot_x += 5.0;
            break;
        case 'u':
            rot_x -= 5.0;
            break;
        case 'h':
            rot_y += 5.0;
            break;
        case 'j':
            rot_y -= 5.0;
            break;
        case 'b':
            rot_z += 5.0;
            break;
        case 'n':
            rot_z -= 5.0;
            break;
    }
    EspecificaParametrosVisualizacao();
    glutPostRedisplay();
}

// funcao de direcionamento do usuario
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
            scanf("%f", &tamanho_f);
            // tamanho_f é um "tamanho falso", usado na
            // verificacao de translacao
            tamanho = tamanho_f;
            primitiva = CUBO;
            break;
        case 2:
            printf("Defina a resolucao:\n");
            scanf("%f", &resolucao);
            while (resolucao > 500) {
                printf(
                    "Tamanho maximo excedido! Escolha um valor menor "
                    "(<=500):\n");
                scanf("%f", &resolucao);
            }
            printf("Defina o tamanho (diametro):\n");
            scanf("%f", &tamanho_f);
            tamanho = tamanho_f / 2;
            primitiva = ESFERA;
            break;
        case 3:
            printf("Defina a resolucao:\n");
            scanf("%f", &resolucao);
            while (resolucao > 500) {
                printf(
                    "Tamanho maximo excedido! Escolha um valor menor "
                    "(<=500):\n");
                scanf("%f", &resolucao);
            }
            printf("Defina a base (diametro):\n");
            scanf("%f", &tamanho_f);
            tamanho = tamanho_f;
            primitiva = CONE;
            break;
        case 4:
            printf("Defina o tamanho:\n");
            scanf("%f", &tamanho_f);
            tamanho = tamanho_f / ( 2.0 * sqrt(6.0) / 3.0);
            primitiva = TETRAEDRO;
            break;
    }
    
    while (tamanho_f > 100.0) {
        printf(
            "Tamanho maximo excedido! Escolha um valor menor (<=100):\n");
        scanf("%f", &tamanho_f);
        if(primitiva == ESFERA) tamanho = tamanho_f / 2;
        else tamanho = tamanho_f;
    }
    
}

// caso seja feita a operacao manual
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

    int opcao;
    printf("Usar entrada manual?\n");
    printf("1- Sim\n");
    printf("0- Nao\n");
    scanf("%d", &opcao);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(650, 600);
    glutCreateWindow("Solidos 3D");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(GerenciaTeclado);
    Inicializa();
    // para o caso de desejar operacao manual
    if (opcao) glutIdleFunc(menuSecundario);

    glutMainLoop();

    return 0;
}
