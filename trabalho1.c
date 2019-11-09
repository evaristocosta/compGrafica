/*	=========================================================================
 *				TRABALHO 1: Transformações Geométricas em 2D
 *
 *	Este software representa a solução do proposto na Pratica 1
 *	da disciplina de Computacao Grafica, a qual foi dividica em
 *	8 requisitos:
 *	R1) Permitir ao usuário escolher no mínimo as seguintes figuras
 *	geométricas planas a serem desenhadas: Triângulo, Quadrado,
 *	Hexágono.
 *	R2) Permitir ao usuário digitar as coordenadas dos vértices de cada
 *	figura. Neste caso, os polígonos a serem desenhados poderão ser
 *	irregulares.
 *	R3) Permitir ao usuário digitar o ponto central e o tamanho da
 *	aresta (borda) de cada polígono. Neste caso os polígonos
 *	desenhados serão regulares.
 *	R4) Pode ser utilizado adicional e opcionalmente cliques de mouse
 *	para determinar os vértices e/ou o centro de cada figura, caso o
 *	desenvolvedor se sinta apto a fazer desta forma.
 *	R5) Desenhar a figura na tela conforme parâmetros de entrada
 *	fornecidos pelo usuário (requisitos R1 a R4).
 *	R6) O usuário poderá escolher qualquer uma das seguintes TGs a
 *	serem realizadas sobre uma figura já desenhada:
 *	- Translação, Escala, Rotação, Reflexão, Cisalhamento
 *	R7) Conforme a TG escolhida, o programa deverá solicitar ao
 *	usuário os parâmetros correspondentes e necessários para realizar
 *	a transformação.
 *	R8) Após confirmação do usuário o programa aplica a TG escolhida
 *	conforme os parâmetros informados (R5, R6 e R7), calcula e mostra
 *	o resultado na tela.
 *	R9) Deverá ser desenhado um sistema de referência na tela de
 *	visualização, com escala indicativa das distâncias (dividir em 10
 *segmentos cada eixo do sistema de coordenadas). R10) As entradas de
 *coordenadas devem ser validadas conforme segue:
 *	- As coordenadas devem estar dentro dos limites do sistema de
 *coordenadas do universo (da tela);
 *  - Não permitir a entrada de pontos repetidos (iguais);
 *  - Permitir a entrada de pontos alinhados;
 *  - O desenho de polígonos convexos ou côncavos é permitido.
 *	R11) Desenvolver o sistema de forma que haja flexibilidade
 *	para a inclusão de transformações de poliedros em 3D (prática futura).
 *
 *	Com excecao do requisito R4 (que era opcional), todos requisitos
 *	foram implementados.
 *
 *	Este software foi desenvolvido por:
 *	- Lucas Costa
 *	- RA: 1683993
 *
 *	=========================================================================
 */

// Para compilar e executar (em linux):
// gcc -lglut -lGL -lGLU -lm trabalho1.c -o trabalho1.out && ./trabalho1.out

#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// inteiros de definicao para os objetos que podem ser construidos
#define TRIANGULO 1
#define QUADRADO 2
#define HEXAGONO 3

// >>>>>> VARIAVEIS GLOBAIS >>>>>>
// para definir objetos regulares
float pontoCentral_x = 0.0, pontoCentral_y = 0.0, aresta = 0.0;
// define qual objeto deve ser construido
GLint primitiva;
GLfloat vertice[12],               // pontos do objeto
    x_trans = 0.0, y_trans = 0.0,  // variaveis de translacao
    escala = 1.0,                  // variavel de escala (porcentagem)
    rotacao = 0.0,                 // variavel para rotacao (angulo)
    reflexao = 1.0,      // variavel de reflexao (positivo ou negativo)
    cisalhamento = 0.0;  // variavel para o cisalhamento

// Funcao com unico proposito de desenhar os eixos no plano do desenho
void DesenhaEixos(void) {
    int i = 0;
    // eixos secundarios
    glColor3f(0.7, 0.7, 0.7);  // cor cinza
    for (i = -5; i < 5; i++) {
        // verticais
        glBegin(GL_LINES);
        glVertex2f((GLfloat)i * 50, -250);
        glVertex2f((GLfloat)i * 50, 250);
        glEnd();

        // horizontais
        glBegin(GL_LINES);
        glVertex2f(-250, (GLfloat)i * 50);
        glVertex2f(250, (GLfloat)i * 50);
        glEnd();
    }

    // eixos principais
    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_LINES);
    glVertex2f(0, -250);
    glVertex2f(0, 250);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-250, 0);
    glVertex2f(250, 0);
    glEnd();
}

// Determina o desenho de um quadrado
void DesenhaQuadrado(void) {
    glBegin(GL_QUADS);
    glVertex2f(vertice[0], vertice[1]);
    glVertex2f(vertice[2], vertice[3]);
    glVertex2f(vertice[4], vertice[5]);
    glVertex2f(vertice[6], vertice[7]);
    glEnd();
}

// Determina o desenho de um triangulo
void DesenhaTriangulo(void) {
    glBegin(GL_TRIANGLES);
    glVertex2f(vertice[0], vertice[1]);
    glVertex2f(vertice[2], vertice[3]);
    glVertex2f(vertice[4], vertice[5]);
    glEnd();
}
// Determina o desenho de um hexagono
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

/*	>>>>>> Funcao principal de desenho na tela >>>>>>
        Responsavel por aplicar todas as transformacoes requeridas
        e funcoes anteriormente definidas; recarrega a cada
        interacao com o usuario e aplica as	diferencas.
*/
void Desenha(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    // define os eixos da figura
    DesenhaEixos();

    // Define a cor corrente
    glColor3f(1.0, 1.0, 0.0);  // cor amarela

    // efetua translacao
    glTranslatef(x_trans, y_trans, 0.0);

    // efetua escala
    glScalef(escala, escala, 1.0);

    // efetua rotacao
    glRotatef(rotacao, 0.0, 0.0, 1.0);

    // efetua cisalhamento
    GLfloat cisalhar[16] = {1.0, 0.0, 0.0, 0.0, cisalhamento, 1.0, 0.0, 0.0,
                            0.0, 0.0, 1.0, 0.0, 0.0,          0.0, 0.0, 1.0};
    glMultMatrixf(cisalhar);

    // efetua reflexao
    GLfloat reflexao_m[16] = {1.0, 0.0, 0.0, 0.0, 0.0, reflexao, 0.0, 0.0,
                              0.0, 0.0, 1.0, 0.0, 0.0, 0.0,      0.0, 1.0};
    glMultMatrixf(reflexao_m);

    // Desenha o objeto escolhido
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

    // cor do fundo
    glClearColor(0.2, 0.2, 0.2, 0.0);
    glutSwapBuffers();
}

/*	>>>>>> Funcao de alteracao de tamanho de janela >>>>>>
        Permite que o usuario altere o tamanho da janela sem afetar
        as dimensoes do objeto desenhado. Alem disso, permite redefinir
        o ponto central da janela, como sendo o centro real e nao o
        canto inferior esquerdo
*/
void AlteraTamanhoJanela(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-250.0, 250.0, -250.0 * (GLfloat)h / (GLfloat)w,
                250.0 * (GLfloat)h / (GLfloat)w, -250.0, 250.0);
    else
        glOrtho(-250.0 * (GLfloat)w / (GLfloat)h,
                250.0 * (GLfloat)w / (GLfloat)h, -250.0, 250.0, -250.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// funcao de direcionamento do usuario
void menu() {
    int opcao;

    printf("-------- Passo 1 --------\n");
    printf("Selecione o objeto: \n");
    printf("1 - Quadrado\n");
    printf("2 - Triangulo\n");
    printf("3 - Hexagono\n");
    scanf("%d", &opcao);

    switch (opcao) {
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
    while (1) {
        bool valido = true;
        switch (opcao) {
            /* 	Caso o usuario tenha escolhido objeto regular, ele deve
                    definir o ponto central em x e y e dizer o tamanho que
                    o objeto deve ter, com base na aresta, sendo esta a
                    distancia do ponto central
            */
            case 1:
                printf("Diga o ponto central em x: \n");
                scanf("%f", &pontoCentral_x);

                printf("Diga o ponto central em y: \n");
                scanf("%f", &pontoCentral_y);

                printf("Diga o tamanho da aresta: \n");
                scanf("%f", &aresta);

                if (primitiva == QUADRADO) {
                    // centro + desvio + aresta
                    vertice[0] = pontoCentral_x - aresta;  // x1
                    vertice[1] = pontoCentral_y + aresta;  // y1
                    vertice[2] = pontoCentral_x - aresta;  // x2
                    vertice[3] = pontoCentral_y - aresta;  // y2
                    vertice[4] = pontoCentral_x + aresta;  // x3
                    vertice[5] = pontoCentral_y - aresta;  // y3
                    vertice[6] = pontoCentral_x + aresta;  // x4
                    vertice[7] = pontoCentral_y + aresta;  // y4

                    for (size_t i = 0; i < 8; i++) {
                        if (abs(vertice[i]) > 250.0) {
                            printf(
                                "\n\nParametros invalidos. Favor "
                                "verificar.\n\n");
                            valido = false;
                            break;
                        }
                    }
                    if (valido)
                        return;
                    else
                        break;

                } else if (primitiva == TRIANGULO) {
                    vertice[0] = pontoCentral_x;           // x1
                    vertice[1] = pontoCentral_y + aresta;  // y1
                    vertice[2] = pontoCentral_x - aresta;  // x2
                    vertice[3] = pontoCentral_y - aresta;  // y2
                    vertice[4] = pontoCentral_x + aresta;  // x3
                    vertice[5] = pontoCentral_y - aresta;  // y3

                    for (size_t i = 0; i < 6; i++) {
                        if (abs(vertice[i]) > 250.0) {
                            printf(
                                "\n\nParametros invalidos. Favor "
                                "verificar.\n\n");
                            valido = false;
                            break;
                        }
                    }
                    if (valido)
                        return;
                    else
                        break;
                } else if (
                    primitiva ==
                    HEXAGONO) {  // http://calculo.cc/temas/temas_geometria_analitica/recta/imagenes/problemas/geometrico/p_20_graf.gif
                    vertice[0] = pontoCentral_x - aresta;               // x1
                    vertice[1] = pontoCentral_y + (aresta * sqrt(3));   // y1
                    vertice[2] = pontoCentral_x - (aresta * 2);         // x2
                    vertice[3] = pontoCentral_y;                        // y2
                    vertice[4] = pontoCentral_x - aresta;               // x3
                    vertice[5] = pontoCentral_y - (aresta * sqrt(3));   // y3
                    vertice[6] = pontoCentral_x + aresta;               // x4
                    vertice[7] = pontoCentral_y - (aresta * sqrt(3));   // y4
                    vertice[8] = pontoCentral_x + (aresta * 2);         // x5
                    vertice[9] = pontoCentral_y;                        // y5
                    vertice[10] = pontoCentral_x + aresta;              // x6
                    vertice[11] = pontoCentral_y + (aresta * sqrt(3));  // y6

                    for (size_t i = 0; i < 12; i++) {
                        if (abs(vertice[i]) > 250.0) {
                            printf(
                                "\n\nParametros invalidos. Favor "
                                "verificar.\n\n");
                            valido = false;
                            break;
                        }
                    }
                    if (valido)
                        return;
                    else
                        break;
                }
                break;
            // caso contrario, todas as coordenadas devem ser descritas
            case 2:
                printf(
                    "Digite a seguir as coordenadas do objeto (sentido "
                    "horario)\n");
                printf("x1 = ");
                scanf("%f", &vertice[0]);

                printf("y1 = ");
                scanf("%f", &vertice[1]);

                printf("x2 = ");
                scanf("%f", &vertice[2]);
                printf("y2 = ");
                scanf("%f", &vertice[3]);

                printf("x3 = ");
                scanf("%f", &vertice[4]);
                printf("y3 = ");
                scanf("%f", &vertice[5]);

                for (size_t i = 0; i < 6; i++) {
                    if (abs(vertice[i]) > 250.0) {
                        printf(
                            "\nExistem pontos invalidos. Tente novamente.\n\n");
                        valido = false;
                    }
                    if (i % 2 == 0)
                        for (size_t j = i + 2; j < 6; j += 2) {
                            if ((vertice[i] == vertice[j]) &&
                                (vertice[i + 1] == vertice[j + 1])) {
                                printf(
                                    "\nPontos repetidos nao sao "
                                    "aceitaveis. Tente novamente\n\n");
                                valido = false;
                            }
                        }
                }
                if (!valido) break;

                if (primitiva == QUADRADO || primitiva == HEXAGONO) {
                    printf("x4 = ");
                    scanf("%f", &vertice[6]);
                    printf("y4 = ");
                    scanf("%f", &vertice[7]);
                    for (size_t i = 0; i < 8; i++) {
                        if (abs(vertice[i]) > 250.0) {
                            printf(
                                "\nExistem pontos invalidos. Tente "
                                "novamente.\n\n");
                            valido = false;
                        }
                        if (i % 2 == 0)
                            for (size_t j = i + 2; j < 8; j += 2) {
                                if (vertice[i] == vertice[j] &&
                                    vertice[i + 1] == vertice[j + 1]) {
                                    printf(
                                        "\nPontos repetidos nao sao "
                                        "aceitaveis. Tente novamente\n\n");
                                    valido = false;
                                }
                            }
                    }
                    if (!valido) break;
                }
                if (primitiva == HEXAGONO) {
                    printf("x5 = ");
                    scanf("%f", &vertice[8]);
                    printf("y5 = ");
                    scanf("%f", &vertice[9]);

                    printf("x6 = ");
                    scanf("%f", &vertice[10]);
                    printf("y6 = ");
                    scanf("%f", &vertice[11]);
                    for (size_t i = 0; i < 12; i++) {
                        if (abs(vertice[i]) > 250.0) {
                            printf(
                                "\nExistem pontos invalidos. Tente "
                                "novamente.\n\n");
                            valido = false;
                        }
                        if (i % 2 == 0)
                            for (size_t j = i + 2; j < 12; j += 2) {
                                if (vertice[i] == vertice[j] &&
                                    vertice[i + 1] == vertice[j + 1]) {
                                    printf(
                                        "\nPontos repetidos nao sao "
                                        "aceitaveis. Tente novamente\n\n");
                                    valido = false;
                                }
                            }
                    }
                    if (!valido) break;
                }
                return;
        }
    }
}

// gerenciamento das opcoes de translacao
void MenuTranslacao(int op) {
    // sao quatro opcoes, nos dois eixos:
    // para cima, para baixo, esquerda e direita
    switch (op) {
        case 0:
            y_trans += 20.0;
            break;
        case 1:
            y_trans -= 20.0;
            break;
        case 2:
            x_trans += 20.0;
            break;
        case 3:
            x_trans -= 20.0;
            break;
    }
    glutPostRedisplay();
}

// gerenciamento das opcoes de escala
void MenuEscala(int op) {
    // sao duas opcoes de escala: o dobro ou a metade do
    // tamanho original
    switch (op) {
        case 0:
            escala /= 2;
            break;
        case 1:
            escala *= 2;
    }
}

// gerenciamento das opcoes de rotacao
void MenuRotacao(int op) {
    // sao tres opcoes de angulo, no sentido horario:
    // 30, 45 e 60 graus
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

// gerenciamento das opcoes de reflexao e cisalhamento
void MenuPrincipal(int op) {
    switch (op) {
        case 0:
            // para realizar a reflexao, deve-se somente negativar
            // o valor desta variavel
            reflexao *= -1.0;
            break;
        case 1:
            // o cisalhamento acontece apenas no eixo x, sem retorno
            // somente para exemplificar
            cisalhamento += 0.1;
            break;
    }
}

// Criacao do Menu
void CriaMenu() {
    int menu, submenu1, submenu2, submenu3;

    // opcoes do menu de translacao
    submenu1 = glutCreateMenu(MenuTranslacao);
    glutAddMenuEntry("Acima", 0);
    glutAddMenuEntry("Abaixo", 1);
    glutAddMenuEntry("Direita", 2);
    glutAddMenuEntry("Esquerda", 3);

    // opcoes do menu de escala
    submenu2 = glutCreateMenu(MenuEscala);
    glutAddMenuEntry("x0.5", 0);
    glutAddMenuEntry("x2.0", 1);

    // opcoes do menu de rotacao
    submenu3 = glutCreateMenu(MenuRotacao);
    glutAddMenuEntry("30 graus", 0);
    glutAddMenuEntry("45 graus", 1);
    glutAddMenuEntry("60 graus", 2);

    // menu geral com link para submenus anteriores
    menu = glutCreateMenu(MenuPrincipal);
    glutAddMenuEntry("Reflexao", 0);
    glutAddMenuEntry("Cisalhamento", 1);
    glutAddSubMenu("Translacao", submenu1);
    glutAddSubMenu("Escala", submenu2);
    glutAddSubMenu("Rotacao", submenu3);

    // define abrir menu com botao direito do mouse
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// identifica o clique direito do mouse
void GerenciaMouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) CriaMenu();

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    // exibe menu com primeiras opcoes
    menu();

    glutInit(&argc, argv);
    // double buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    // tamanho e posicao padrao
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    // janela e desenha
    glutCreateWindow("Trabalho 1");
    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);

    glutMouseFunc(GerenciaMouse);

    glutMainLoop();

    return 0;
}