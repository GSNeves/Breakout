// Nomes: Gustavo Spellmeier Neves e Joao Pedro Kuhn Braun

#include <windows.h>
#include <conio.h>
#include <time.h>
#include "ArquivosBreakout.h"

#define TAMANHO_PALHETA 5
#define CHAR_MOLDURA    '#'
#define CHAR_PALHETA    '='
#define CHAR_BOLINHA    '*'
#define CHAR_TIJOLO     '|'
#define ALTURA_TIJOLO_FASE1 3
#define ALTURA_TIJOLO_FASE2 2
#define ALTURA_TIJOLO_FASE3 1
#define LARGURA_TIJOLO  5
#define TAMANHO_PALHETA_FASE1 TAMANHO_PALHETA + 5
#define TAMANHO_PALHETA_FASE2 TAMANHO_PALHETA + 3
#define TAMANHO_PALHETA_FASE3 TAMANHO_PALHETA + 1
#define XINIPAL 30
#define YINIPAL 25
#define XINIBOL XINIPAL + 3
#define YINIBOL YINIPAL - 1
#define VIDAS   3
#define INTERVALO_BONUS 10
#define BONUS_1 1
#define BONUS_2 2
#define BONUS_3 3
#define BONUS_TAMPALHETA 4
#define BONUS_NUMPONTOS 100
#define MEIA_MOLDURA_HORIZONTAL (XFIM - XINI) / 2
#define NUM_MAX_VIDAS 9
#define PONTUACAO_TIJOLO_FASE1 10
#define PONTUACAO_TIJOLO_FASE2 2 * PONTUACAO_TIJOLO_FASE1
#define PONTUACAO_TIJOLO_FASE3 2 * PONTUACAO_TIJOLO_FASE2


void desenha_palheta(PALHETA pal);

void inicia_bonus_tijolos(TIJOLO listatij[], int num_tijolos){
// Essa funcao recebe um arranjo de tijolos por referencia e o tamanho desse arranjo por valor, para alterar os valores de seus bonus. Sem retorno

    for (short cont_tijolos = 0; cont_tijolos < num_tijolos; cont_tijolos++)
    {
        listatij[cont_tijolos].bonus = (int) (rand() / (double) RAND_MAX * (INTERVALO_BONUS + 1));
    }
}

short tem_bonus(TIJOLO tij){
// Checa se o tijolo tem um bonus valido para um efeito especial, retornando 1 caso positivo, caso contrario, retorna 0

    if (tij.bonus >= 1 && tij.bonus <= 3){
        return 1;
    }
    else{
        return 0;
    }
}

void passa_bonus(TIJOLO tij, BOLA *bol){
// Passa o valor do bonus que esta no tijolo para a bolinha que o destruiu
    bol->bonus = tij.bonus;
}

void aplica_bonus(BOLA *bol, PALHETA *pal, JOGADOR *jog, short *teve_bonus){
// Aplica o efeito do bonus caso o jogador tenha pego a bolinha com efeito especial com sucesso
    switch (bol->bonus)
    {
        case BONUS_1:
            jog->pontuacao += BONUS_NUMPONTOS;
            *teve_bonus = 1;
            break;
        case BONUS_2:
            if (jog->vidas < NUM_MAX_VIDAS){
                jog->vidas++;
                *teve_bonus = 1;
            }
            break;
        case BONUS_3:
            if (pal->ponto1.x > MEIA_MOLDURA_HORIZONTAL){
                pal->ponto1.x -= BONUS_TAMPALHETA;
            }
            else{
                pal->ponto2.x += BONUS_TAMPALHETA;
            }
            *teve_bonus = 1;
            break;
    }
    bol->bonus = 0;
}

void atualiza_dados_bonus(PALHETA pal, JOGADOR jog){
// Atualiza os dados que sao possiveis de serem alterados por algum bonus, para o jogador notar a mudanca que o bonus trouxe
    desenha_palheta(pal);
    gotoxy(XFIM+4, YINI+1);
    printf("Pontuacao: %d", jog.pontuacao);
    gotoxy(XFIM+4, YINI+2);
    printf("Vidas: %d", jog.vidas);
}

void inicia_bolinha(BOLA *bol){
// Inicializa a bolinha em suas coordenadas e velocidades iniciais

    bol->posicao.x = XINIBOL;
    bol->posicao.y = YINIBOL;
    bol->velocidade_direcao.x = 1;
    bol->velocidade_direcao.y = -1;
    bol->bonus = 0;
}

void apaga_eixo_palheta(PALHETA pal){
// Apaga todos os caracteres de dentro da moldura do eixo y da palheta
    for (int i = XINI + 1; i < XFIM; i++){
        putchxy(i, pal.ponto1.y, ' ');
    }
}

void gera_tamanho_palheta(PALHETA *pal, int fase){
// Essa funcao decide qual o tamanho da palheta, dependendo da fase
    switch (fase){
        case 0:
            pal->ponto2.x = pal->ponto1.x + TAMANHO_PALHETA_FASE1;
            break;
        case 1:
            pal->ponto2.x = pal->ponto1.x + TAMANHO_PALHETA_FASE1;
            break;
        case 2:
            pal->ponto2.x = pal->ponto1.x + TAMANHO_PALHETA_FASE2;
            break;
        case 3:
            pal->ponto2.x = pal->ponto1.x + TAMANHO_PALHETA_FASE2;
            break;
        case 4:
            pal->ponto2.x = pal->ponto1.x + TAMANHO_PALHETA_FASE3;
            break;
        case 5:
            pal->ponto2.x = pal->ponto1.x + TAMANHO_PALHETA_FASE3;
            break;
    }
}

void inicia_palheta(PALHETA *pal, int fase){
// Inicializa a palheta nas coordenadas iniciais e do tamanho referente a cada fase
    pal->ponto1.x = XINIPAL;
    pal->ponto1.y = YINIPAL;
    gera_tamanho_palheta(pal, fase);
}

int roda_fase(FASE fase[], PALHETA *pal, BOLA *bol, TIJOLO tij[FASES_CENARIOS][QUANT_MAX_TIJOLOS], JOGADOR *jog, int posicao_padrao, short carregou);
// Faz com que nao produza erro ao roda_fase ser chamada em menu_inicial

void pausa(PALHETA *pal, BOLA *bol, TIJOLO tij[FASES_CENARIOS][QUANT_MAX_TIJOLOS], JOGADOR *jog){
    // Pausa o jogo e pede para o jogador escrever seu nome, para que o jogo seja salvo
    // Recebe as estruturas PALHETA, BOLA, matriz TIJOLO e JOGADOR, todas por referencia, para salvar o jogo apropriadamente, e nao possui retorno
    short cont_fase, cont_tijolos;
    puts("Insira seu nome, em ate 40 caracteres (incluindo espaco):");
    gets(jog->nome);

    PAUSAJOGO dados;
    dados.jogador = *jog;
    dados.bolinha = *bol;
    dados.palheta = *pal;
    for (cont_fase = 0; cont_fase < FASES_CENARIOS; cont_fase++){
        for (cont_tijolos = 0; cont_tijolos < QUANT_MAX_TIJOLOS; cont_tijolos++)
            dados.listatij[cont_fase][cont_tijolos] = tij[cont_fase][cont_tijolos]; // Preenche matriz de tijolos
    }
    // Passou os dados dos elementos do jogo para a estrutura principal, que tem todos os dados do jogo para ele poder ser salvo

    salva_estado_do_jogo(dados);
}

void instancia_jogo(PAUSAJOGO dados, JOGADOR *jog, TIJOLO tij[FASES_CENARIOS][QUANT_MAX_TIJOLOS], BOLA *bol, PALHETA *pal){
    // Reinstancia as variaveis referentes aos dados do jogo, para caso o jogador carregar um jogo salvo, continuar de onde parou
    // Recebe PAUSAJOGO por valor e JOGADOR, matriz TIJOLO, BOLA e PALHETA por referencia, adequando os valores para o jogo prosseguir dessa maneira. Nao possui retorno
    short cont_fase, cont_tijolos;

    jog->fase_atual = dados.jogador.fase_atual;
    jog->vidas = dados.jogador.vidas;
    jog->pontuacao = dados.jogador.pontuacao;
    strcpy(jog->nome, dados.jogador.nome);

    for (cont_fase = 0; cont_fase < FASES_CENARIOS; cont_fase++){
        for (cont_tijolos = 0; cont_tijolos < QUANT_MAX_TIJOLOS; cont_tijolos++)
            tij[cont_fase][cont_tijolos] = dados.listatij[cont_fase][cont_tijolos];
    }

    *bol = dados.bolinha;
    *pal = dados.palheta;
}

void menu_inicial(char *tecla, PALHETA *pal, BOLA *bol, TIJOLO tij[FASES_CENARIOS][QUANT_MAX_TIJOLOS], JOGADOR *jog, FASE fase[NUMFASES]){
    // Apresenta um menu de opcoes e decide o que fazer dependendo da tecla que o jogador pressionou
    // Recebe um char, e estruturas PALHETA, BOLA, matriz TIJOLO, JOGADOR e arranjo FASE, todos por referencia, para prosseguir com o jogo de maneira adequada. Nao possui retorno
    char nome[TAM_NOME];
    clrscr();
    if(*tecla == P){ // Se o jogador pausou, salva o jogo e pede se o usuario deseja despausar ou sair do jogo
        pausa(pal, bol, tij, jog);
        printf("Pressione P para continuar, ou ESC para sair do jogo\n");
        do{
            *tecla = getch();
        } while (*tecla != P && *tecla != ESC);
        if (*tecla == P)
            roda_fase(fase, pal, bol, tij, jog, 0, 1); // Executa o jogo novamente
    }
    else{
        if(*tecla == C){ // Se o jogador quer carregar um jogo salvo, pede o nome do arquivo a ser carregado
            printf("Entre com o nome do arquivo que deseja carregar:\n");
            gets(nome);
            PAUSAJOGO dados;
            if (le_jogo_salvo(&dados, nome) == 0){ // Se foi lido o arquivo a ser carregado corretamente, pega os dados do novo jogo
                jog->fase_atual = dados.jogador.fase_atual;
                instancia_jogo(dados, jog, tij, bol, pal);
            }
            printf("\nPressione qualquer tecla para voltar ao jogo");
            getch();
            roda_fase(fase, pal, bol, tij, jog, 0, 1); // Executa o jogo novamente, com os dados do mesmo jogo caso houve falha ao carregar jogo salvo
        }

    }

}

void desenha_moldura(COLORS cor){
    // Imprime a moldura, recebendo sua cor por valor, sem retorno
    int cont;
    textcolor(DARKGRAY);    // Altera as cores do background (dependendo da fase em que o jogador se encontra) e do texto
    textbackground(cor);
    for(cont = XINI; cont<= XFIM; cont++){  // Imprime as bases da moldura
        putchxy(cont, YINI, CHAR_MOLDURA);
        putchxy(cont, YFIM, CHAR_MOLDURA);
    }
    for(cont = YINI + 1; cont< YFIM; cont++){ // Imprime as laterais da moldura
        putchxy(XINI, cont, CHAR_MOLDURA);
        putchxy(XFIM, cont, CHAR_MOLDURA);
    }

    textcolor(LIGHTGRAY);   // Retorna as cores ao normal
    textbackground(BLACK);
}

void desenha_palheta(PALHETA pal){
    // Desenha a palheta, recebendo a estrutura PALHETA por valor, sem retorno
    int cont_largura;
    int cont_altura;
    textbackground(CYAN);   // Muda cor do background

    for(cont_altura = 0; cont_altura <= pal.ponto2.y - pal.ponto1.y; cont_altura++){
        for(cont_largura = 0; cont_largura < pal.ponto2.x - pal.ponto1.x; cont_largura++){  //Imprime a palheta
            putchxy(pal.ponto1.x+cont_largura, pal.ponto1.y+cont_altura, CHAR_PALHETA);
        }
    }
    textbackground(BLACK);  // Retorna para a cor original do background
}

void desenha_bolinha(BOLA bol){
    // Desenha a bolinha, recebendo a estrutura BOLA por valor, sem retorno
    switch (bol.bonus)
    {
        case BONUS_1:
            textbackground(GREEN);
            break;
        case BONUS_2:
            textbackground(RED);
            break;
        case BONUS_3:
            textbackground(BLUE);
            break;
        default:
            textbackground(WHITE);
    }
    putchxy(bol.posicao.x, bol.posicao.y, CHAR_BOLINHA);
    textbackground(BLACK);
}

void desenha_tijolo(TIJOLO tij){
    // Desenha um tijolo, recebendo a estrutura TIJOLO por valor, sem retorno
    int cont_altura;
    int cont_largura;

    switch (tij.estado)
    {
        case 1:
            textbackground(BLUE);
            break;
        case 0:
            textbackground(YELLOW);
            break;
        case -1:
            textbackground(RED);
            break;
        default:
            textbackground(DARKGRAY);
    }

    for(cont_altura = tij.posicao1.y; cont_altura < tij.posicao2.y; cont_altura++){
        for(cont_largura = tij.posicao1.x; cont_largura < tij.posicao2.x; cont_largura++){
            if(cont_largura < XFIM && cont_largura > XINI && cont_altura > YINI && cont_altura < YFIM){   // Desenha apenas a parte do tijolo dentro da moldura
                putchxy(cont_largura, cont_altura, CHAR_TIJOLO);
            }
        }
    }
    textbackground(BLACK);  // Retorna para a cor original do background

}

void desenha_tijolos(TIJOLO tij[], FASE fase){
    // Desenha uma sequencia de tijolos, recebendo a estrutura FASE por valor e um arranjo TIJOLO, sem retorno
    int cont;

    for(cont=0; cont < fase.numTijolos; cont++){
        if(tij[cont].posicao1.x < XFIM && tij[cont].posicao2.x - 1 > XINI && tij[cont].posicao1.y < YFIM && tij[cont].posicao2.y - 1 > YINI && tij[cont].estado < 2){
            desenha_tijolo(tij[cont]);  // Se o tijolo estiver dentro da moldura, desenhe
        }
        else{   // Muda o estado do tijolo para "quebrado" se ele nao se encontra dentro da moldura
            tij[cont].estado = 2;
            tij[cont].posicao1.x = 0;
            tij[cont].posicao1.y = 0;
            tij[cont].posicao2.x = 0;
            tij[cont].posicao2.y = 0;
        }
    }

}

void destroi_tijolo(int indicetijolo, TIJOLO listatijolo[], JOGADOR *jog, BOLA *bol){
    // Destroi o tijolo atingido pela bolinha e atualiza a pontuacao do jogador
    // Recebe o indice do tijolo destruido, por valor, um arranjo TIJOLO e a estrutura JOGADOR, por referencia. Sem retorno
    int cont_altura;
    int cont_largura;

    listatijolo[indicetijolo].estado++;

    if (listatijolo[indicetijolo].estado >= 2){

        jog->pontuacao += listatijolo[indicetijolo].pontos; // Aumenta a pontuacao

        for(cont_altura = listatijolo[indicetijolo].posicao1.y; cont_altura < listatijolo[indicetijolo].posicao2.y; cont_altura++){
            for(cont_largura = listatijolo[indicetijolo].posicao1.x; cont_largura < listatijolo[indicetijolo].posicao2.x; cont_largura++){
                if(cont_largura < XFIM && cont_largura > XINI && cont_altura > YINI && cont_altura < YFIM){ // Para nao apagar a moldura quando o tijolo estiver "colidindo" com a moldura
                    putchxy(cont_largura, cont_altura, ' ');  // Remove o tijolo
                }
            }
        }

        listatijolo[indicetijolo].posicao1.x = 0;  // Zera as coordenadas do tijolo destruido
        listatijolo[indicetijolo].posicao1.y = 0;
        listatijolo[indicetijolo].posicao2.x = 0;
        listatijolo[indicetijolo].posicao2.y = 0;

        if (tem_bonus(listatijolo[indicetijolo])){
            passa_bonus(listatijolo[indicetijolo], bol);
        }
    }
    else{
        desenha_tijolo(listatijolo[indicetijolo]);
    }

    gotoxy(XFIM+4, YINI+1);
    printf("Pontuacao: %d", jog->pontuacao);  // Imprime nova pontuacao
}

short testa_colisao(int x1Obj1, int x2Obj1, int y1Obj1, int y2Obj1, int x1Obj2, int x2Obj2, int y1Obj2, int y2Obj2){
    // Testa a colisao de 2 objetos, recebendo os valores de suas coordenadas por valor
    // Retorna 1 caso houve colisao, e 0 caso nao houve

    if(x1Obj1 <= x2Obj2 && x2Obj1 >= x1Obj2 && y1Obj1 <= y2Obj2 && y2Obj1 >= y1Obj2){
        return 1;
    }
    else{
        return 0;
    }
}

void move_palheta(char tecla, PALHETA *pal){
    // Move a palheta, recebe um char por valor, para testar o movimento, e uma estrutura PALHETA for referencia, para alterar sua coordenada. Sem retorno


    switch (tecla){
        case SETA_ESQ:    //Se for a seta esquerda, e não houver colisao com as paredes, remove o ultimo caractere e escreve um caractere antes do primeiro
            pal->velocidade_direcao.x = -1;
            if(testa_colisao(pal->ponto1.x-1, pal->ponto2.x, pal->ponto1.y, pal->ponto2.y, XINI, XINI, YINI, YFIM) == 0){
                (pal->ponto1.x) += pal->velocidade_direcao.x;
                (pal->ponto2.x) += pal->velocidade_direcao.x;
                textbackground(CYAN);
                putchxy(pal->ponto1.x, pal->ponto1.y, CHAR_PALHETA);  // Escreve um caractere antes do primeiro
                textbackground(BLACK);
                putchxy(pal->ponto2.x, pal->ponto1.y, ' '); // Remove o ultimo caractere
            }
            break;

        case SETA_DIR:    // Se for a seta direita, e nao houver colisao com as paredes, remove o primeiro caractere e escreve um caractere depois do ultimo
            pal->velocidade_direcao.x = 1;
            if(testa_colisao(pal->ponto1.x, pal->ponto2.x, pal->ponto1.y, pal->ponto2.y, XFIM, XFIM, YINI, YFIM) == 0){
                (pal->ponto1.x) += pal->velocidade_direcao.x;
                (pal->ponto2.x) += pal->velocidade_direcao.x;
                textbackground(CYAN);
                putchxy(pal->ponto2.x-1, pal->ponto1.y, CHAR_PALHETA); // Escreve um caractere depois do ultimo
                textbackground(BLACK);
                putchxy(pal->ponto1.x-1, pal->ponto1.y, ' ');  // Remove o primeiro caractere
            }
            break;
    }


}

int testa_fim_jogo(TIJOLO tij[], JOGADOR jog, FASE fase){
    // Testa se o jogo deve continuar, ou se o jogador venceu/perdeu, recebendo um arranjo TIJOLO, e as estruturas JOGADOR e FASE por valor
    // Retorna 1 caso o jogo acabou, e 0 caso o jogo prossegue
    int cont_tij = 0;

    if(jog.vidas == 0){ // Se acabou as vidas
        gotoxy(XFIM+11, YINI+2);
        printf("Morreu!");
        return 1;
    }

    while(cont_tij < fase.numTijolos){  // Se tem pelo menos 1 tijolo com estado diferente de dois, continua o jogo
        if(tij[cont_tij].estado != 2)
            return 0;
        cont_tij++;
    }

    // Se nao, termina o jogo
    textcolor(CYAN);
    gotoxy(XFIM+4, YINI+3);
    printf("Parabens!!");
    textcolor(LIGHTGRAY);

    return 1;

}

void move_bolinha(BOLA *bol, PALHETA *pal, TIJOLO tij[], JOGADOR *jog, FASE fase){
    // Move a bolinha, testando as colisoes, e para tal, recebe a estrutura FASE, por valor, e PALHETA, BOLA, JOGADOR e um arranjo TIJOLO, por referencia. Sem retorno
    int prox_xbol;  // Proxima posição x da bolinha
    int prox_ybol;  // Proxima posição y da bolinha
    int cont_tij;
    int meia_palheta = (pal->ponto2.x + pal->ponto1.x) / 2;
    short colidiu_parede = 0;
    short colidiu_lateral_palheta = 0;
    short colidiu_cima_baixo_palheta = 0;
    short teve_bonus = 0;

    prox_xbol = bol->posicao.x + bol->velocidade_direcao.x;
    prox_ybol = bol->posicao.y + bol->velocidade_direcao.y;

    if(!((bol->posicao.x >= pal->ponto1.x && bol->posicao.x < pal->ponto2.x) && (bol->posicao.y == pal->ponto1.y)))   // Para evitar criar buracos na palheta quando ela assuma uma posicao igual a da bolinha ao se mover
        putchxy(bol->posicao.x, bol->posicao.y, ' '); // Apaga o lugar onde a bolinha esta


    if(testa_colisao(prox_xbol, prox_xbol, prox_ybol, prox_ybol, XINI, XFIM, YINI, YINI) == 1){ // Testa colisao com o teto
        bol->velocidade_direcao.y = -bol->velocidade_direcao.y;
        prox_ybol = bol->posicao.y + bol->velocidade_direcao.y;
    }

    if(testa_colisao(prox_xbol, prox_xbol, prox_ybol, prox_ybol, XINI, XFIM, YFIM, YFIM) == 1){  // Testa colisao com o chao
        bol->velocidade_direcao.y = -bol->velocidade_direcao.y;
        (jog->vidas)--;
        gotoxy(XFIM+4, YINI+2);
        printf("Vidas: %d", (jog->vidas));
        prox_ybol = bol->posicao.y + bol->velocidade_direcao.y;
        bol->bonus = 0;
        apaga_eixo_palheta(*pal);
        gera_tamanho_palheta(pal, jog->fase_atual);
        desenha_palheta(*pal);
    }

    if(testa_colisao(prox_xbol, prox_xbol, prox_ybol, prox_ybol, XINI, XINI, YINI, YFIM) == 1){ // Testa colisao com parede da esquerda
        bol->velocidade_direcao.x = -bol->velocidade_direcao.x;
        prox_xbol = bol->posicao.x + bol->velocidade_direcao.x;
        colidiu_parede++;
    }

    if (testa_colisao(prox_xbol, prox_xbol, prox_ybol, prox_ybol, XFIM, XFIM, YINI, YFIM) == 1){ // Testa colisao com parede da direita
        bol->velocidade_direcao.x = -bol->velocidade_direcao.x;
        prox_xbol = bol->posicao.x + bol->velocidade_direcao.x;
        colidiu_parede++;
    }

    // Testa colisao com a metade esquerda da palheta
    if (testa_colisao(prox_xbol, prox_xbol, prox_ybol, prox_ybol, pal->ponto1.x, meia_palheta-1, pal->ponto1.y, pal->ponto2.y) == 1){
        bol->velocidade_direcao.x = -1;
        prox_xbol = bol->posicao.x + bol->velocidade_direcao.x;
        bol->velocidade_direcao.y = -bol->velocidade_direcao.y;
        prox_ybol = bol->posicao.y + bol->velocidade_direcao.y;
        colidiu_cima_baixo_palheta++;
        aplica_bonus(bol, pal, jog, &teve_bonus);
    }

    // Testa colisao com a metade direita da palheta
    if (testa_colisao(prox_xbol, prox_xbol, prox_ybol, prox_ybol, meia_palheta, pal->ponto2.x-1, pal->ponto1.y, pal->ponto2.y) == 1){
        bol->velocidade_direcao.x = 1;
        prox_xbol = bol->posicao.x + bol->velocidade_direcao.x;
        bol->velocidade_direcao.y = -bol->velocidade_direcao.y;
        prox_ybol = bol->posicao.y + bol->velocidade_direcao.y;
        colidiu_cima_baixo_palheta++;
        aplica_bonus(bol, pal, jog, &teve_bonus);
    }

    // Testa colisao lateral com a palheta
    if(bol->posicao.y == pal->ponto1.y && ((bol->posicao.x - bol->velocidade_direcao.x < pal->ponto1.x && bol->posicao.x == pal->ponto1.x-1) || (bol->posicao.x - bol->velocidade_direcao.x > pal->ponto2.x && bol->posicao.x == pal->ponto2.x))){
        bol->velocidade_direcao.x = -bol->velocidade_direcao.x;
        prox_xbol = bol->posicao.x + bol->velocidade_direcao.x;
        colidiu_lateral_palheta++;
        aplica_bonus(bol, pal, jog, &teve_bonus);
    }

    for(cont_tij = 0; cont_tij < fase.numTijolos; cont_tij++){
        // Testa colisao com tijolos
        // Testa colisao lateral com o tijolo
        if(bol->posicao.y >= tij[cont_tij].posicao1.y && bol->posicao.y < tij[cont_tij].posicao2.y && ((bol->posicao.x - bol->velocidade_direcao.x < tij[cont_tij].posicao1.x && bol->posicao.x == tij[cont_tij].posicao1.x-1) || (bol->posicao.x - bol->velocidade_direcao.x > tij[cont_tij].posicao2.x && bol->posicao.x == tij[cont_tij].posicao2.x))){
            bol->velocidade_direcao.x = -bol->velocidade_direcao.x;
            destroi_tijolo(cont_tij, tij, jog, bol);
            prox_xbol = bol->posicao.x + bol->velocidade_direcao.x;
        }
        // Testa colisao com o tijolo (que se nao foi lateral, vai ser vertical)
        else
            if(testa_colisao(prox_xbol, prox_xbol, prox_ybol, prox_ybol, tij[cont_tij].posicao1.x, tij[cont_tij].posicao2.x-1, tij[cont_tij].posicao1.y, tij[cont_tij].posicao2.y-1) == 1){
                bol->velocidade_direcao.y = -bol->velocidade_direcao.y;
                destroi_tijolo(cont_tij, tij, jog, bol);
                prox_ybol = bol->posicao.y + bol->velocidade_direcao.y;
            }

    }
    // Corrige bug da bolinha atravessar a palheta/moldura ao ter colisão simultânea com a palheta e a parede
    if (colidiu_parede && (colidiu_cima_baixo_palheta || colidiu_lateral_palheta))
    {
        bol->velocidade_direcao.x = -bol->velocidade_direcao.x;
    }

    if (teve_bonus){
        atualiza_dados_bonus(*pal, *jog);
    }

    (bol->posicao.x) += bol->velocidade_direcao.x; // Altera as coordenadas da bolinha
    (bol->posicao.y) += bol->velocidade_direcao.y;

    textbackground(BLACK);  // Isso e para quando a bolinha se move ao mesmo tempo que a palheta, senao fica da mesma cor que a palheta
    desenha_bolinha(*bol);  // Coloca a bolinha na proxima posicao
}

void gameloop(FASE fase, PALHETA *pal, BOLA *bol, TIJOLO tij[], JOGADOR *jog, char *tecla){
    // Funcao responsavel por executar o funcionamento do jogo na fase dada, sem retorno
    // Recebe FASE por valor, e PALHETA, BOLA, arranjo TIJOLO, JOGADOR e um char por referencia para o bom funcionamento do jogo
    *tecla = 0; // Tecla para mover a palheta
    short tempo = 0;  // Para a velocidade da bolinha e da palheta serem diferentes

    // Enquanto o usuario nao executar algum comando e o jogo nao terminar, a fase roda
    while (*tecla != ESC && *tecla != P && *tecla != C && testa_fim_jogo(tij, *jog, fase) != 1)
    {
        tempo++;
        if (kbhit())
        {
            *tecla = getch();
            if (*tecla == ESPECIAL || *tecla == ESPECIAL2)  // Se a tecla for especial, chama a funcao move_palheta
            {
                *tecla = getch();
                move_palheta(*tecla, pal);
            }
        }
        if(tempo % fase.velocidade == 0){   // Move a bolinha
            move_bolinha(bol, pal, tij, jog, fase);
            tempo = 0;  // Reseta o tempo
        }
        testa_fim_jogo(tij, *jog, fase); // Ve se o jogo terminou

        Sleep(20);
    }

}

int roda_fase(FASE fase[], PALHETA *pal, BOLA *bol, TIJOLO tij[FASES_CENARIOS][QUANT_MAX_TIJOLOS], JOGADOR *jog, int posicao_padrao, short carregou){
    // Funcao responsavel por controlar em qual fase o jogador esta, retorna 0 caso o jogador acessou o menu, para evitar bugs e repeticoes desnecessarias do laco
    // Recebe um arranjo FASE, matriz TIJOLO, e PALHETA, BOLA e JOGADOR por referencia
    char tecla = 0;
    char start;
    short cont = jog->fase_atual;
    short cont_fase = jog->fase_atual/2;
    srand((unsigned) time(NULL));
    int aleatorio = (rand() % FASES_ETAPA);
    int cenario_atual;

    if(carregou == 1){
        cenario_atual = cont;
        if(cont % 2 == 1){
            cont--;
        }
    }
    else{
        cenario_atual = cont+aleatorio;
    }

    JOGADOR top5[MAX_JOGADORES] = {};
    FILE *arqtxt;
    if(abrearquivo(&arqtxt))
        LeArqTexto(&arqtxt, top5);

    while (cont_fase < NUMFASES && testa_fim_jogo(tij[cenario_atual], *jog, fase[cont_fase]) != 1 && tecla != ESC && tecla != P && tecla != C){
        // Se o jogador saiu ou se o jogo acabou ou se acabaram as fases, encerra o jogo
        start = 1;
        jog->fase_atual = cenario_atual;
        inicia_bonus_tijolos(tij[cenario_atual], fase[cont_fase].numTijolos);
        clrscr();

        if (posicao_padrao){
            if (jog->vidas < 3){
                jog->vidas = VIDAS;
            }
            inicia_palheta(pal, jog->fase_atual);
            inicia_bolinha(bol);
        }
        posicao_padrao = 1;

        imprime_informacoes(*jog);
        gotoxy(XFIM+9, YINI+6);
        printf("Scoreboard:");
        imprime_jogadores(top5);

        desenha_moldura(fase[cont_fase].cor);
        desenha_tijolos(tij[cenario_atual], fase[cont_fase]);
        desenha_palheta(*pal);
        desenha_bolinha(*bol);

        do{
            if (kbhit()){
                start = getch();
                if (start == ESPECIAL || start == ESPECIAL2){
                    start = getch();
                }
            }
        } while (start != SETA_ESQ && start != SETA_DIR && start != ESPACO);

        gameloop(fase[cont_fase], pal, bol, tij[cenario_atual], jog, &tecla); // Executa o jogo
        if(tecla == P || tecla == C){ // Se uma tecla especifica foi pressionada, chama o menu
            menu_inicial(&tecla, pal, bol, tij, jog, fase);
            return 0;
        }
        cont = cont+2;
        cont_fase++;
        aleatorio = (rand() % FASES_ETAPA);
        cenario_atual = cont+aleatorio;
    }

    gotoxy(XINI, YFIM+1);
    system("pause");
    clrscr();
    if ((jog->vidas == 0 || cont_fase == NUMFASES) && tecla != ESC){
        do{
            gotoxy(1,1);
            printf("Por favor, entre com o seu nome de jogador, com no maximo 40 caracteres (incluindo espaco):\n");
            gets(jog->nome);
        }
        while(jog->nome[0] == '\0');
    }

    gotoxy(XFIM+9, YINI+6);
    if (tecla != ESC){
        printf("Scoreboard:");
        MostraListaJogadores(top5, *jog);
        SalvaListaJogadores(arqtxt, top5);
    }
    return 0;
}

void menu(){

    srand(time(0));

    JOGADOR jogador1 = {"jogador1", VIDAS, 0, 0};

    PALHETA pal = {{0, 0}, {XINIPAL, YINIPAL}, {pal.ponto1.x+TAMANHO_PALHETA_FASE1, pal.ponto1.y}};
    BOLA bol = {{XINIBOL, YINIBOL}, {1, -1}, 0};

    TIJOLO tij1 = {{XINI+6, YINI+3}, {tij1.posicao1.x + LARGURA_TIJOLO, tij1.posicao1.y + ALTURA_TIJOLO_FASE1}, 1, PONTUACAO_TIJOLO_FASE1, 0};
    TIJOLO tij2 = {{XFIM/2-1, YINI+3}, {tij2.posicao1.x + LARGURA_TIJOLO, tij2.posicao1.y + ALTURA_TIJOLO_FASE1}, 1, PONTUACAO_TIJOLO_FASE1, 0};
    TIJOLO tij3 = {{XFIM-10, YINI+3}, {tij3.posicao1.x + LARGURA_TIJOLO, tij3.posicao1.y + ALTURA_TIJOLO_FASE1}, 1, PONTUACAO_TIJOLO_FASE1, 0}; // Tijolos fase 1
    TIJOLO tij4 = {{XINI+6, YINI+10}, {tij4.posicao1.x + LARGURA_TIJOLO, tij4.posicao1.y + ALTURA_TIJOLO_FASE1}, 1, PONTUACAO_TIJOLO_FASE1, 0};
    TIJOLO tij5 = {{XFIM/2-1, YINI+10}, {tij5.posicao1.x + LARGURA_TIJOLO, tij5.posicao1.y + ALTURA_TIJOLO_FASE1}, 1, PONTUACAO_TIJOLO_FASE1, 0};
    TIJOLO tij6 = {{XFIM-10, YINI+10}, {tij6.posicao1.x + LARGURA_TIJOLO, tij6.posicao1.y + ALTURA_TIJOLO_FASE1}, 1, PONTUACAO_TIJOLO_FASE1, 0};

    TIJOLO tij1_1 = {{XINI+22, YINI+3}, {tij1_1.posicao1.x + LARGURA_TIJOLO, tij1_1.posicao1.y + ALTURA_TIJOLO_FASE1}, 0, PONTUACAO_TIJOLO_FASE1, 0};
    TIJOLO tij1_2 = {{XINI+54, YINI+3}, {tij1_2.posicao1.x + LARGURA_TIJOLO, tij1_2.posicao1.y + ALTURA_TIJOLO_FASE1}, 0, PONTUACAO_TIJOLO_FASE1, 0};
    TIJOLO tij1_3 = {{XINI+22, YINI+10}, {tij1_3.posicao1.x + LARGURA_TIJOLO, tij1_3.posicao1.y + ALTURA_TIJOLO_FASE1}, 0, PONTUACAO_TIJOLO_FASE1, 0};
    TIJOLO tij1_4 = {{XINI+54, YINI+10}, {tij1_4.posicao1.x + LARGURA_TIJOLO, tij1_4.posicao1.y + ALTURA_TIJOLO_FASE1}, 0, PONTUACAO_TIJOLO_FASE1, 0};

    TIJOLO tij7 = {{XINI+6, YINI+3}, {tij7.posicao1.x + LARGURA_TIJOLO, tij7.posicao1.y + ALTURA_TIJOLO_FASE2}, 0, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij8 = {{XFIM/2-1, YINI+3}, {tij8.posicao1.x + LARGURA_TIJOLO, tij8.posicao1.y + ALTURA_TIJOLO_FASE2}, -1, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij9 = {{XFIM-10, YINI+3}, {tij9.posicao1.x + LARGURA_TIJOLO, tij9.posicao1.y + ALTURA_TIJOLO_FASE2}, 0, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij10 = {{XINI+20, YINI+8}, {tij10.posicao1.x + LARGURA_TIJOLO, tij10.posicao1.y + ALTURA_TIJOLO_FASE2}, 1, PONTUACAO_TIJOLO_FASE2, 0}; // Tijolos fase 2
    TIJOLO tij11 = {{XINI+55, YINI+8}, {tij11.posicao1.x + LARGURA_TIJOLO, tij11.posicao1.y + ALTURA_TIJOLO_FASE2}, 1, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij12 = {{XINI+6, YINI+13}, {tij12.posicao1.x + LARGURA_TIJOLO, tij12.posicao1.y + ALTURA_TIJOLO_FASE2}, 0, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij13 = {{XFIM/2-1, YINI+13}, {tij13.posicao1.x + LARGURA_TIJOLO, tij13.posicao1.y + ALTURA_TIJOLO_FASE2}, -1, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij14 = {{XFIM-10, YINI+13}, {tij14.posicao1.x + LARGURA_TIJOLO, tij14.posicao1.y + ALTURA_TIJOLO_FASE2}, 0, PONTUACAO_TIJOLO_FASE2, 0};

    TIJOLO tij2_1 = {{XINI+20, YINI+3}, {tij2_1.posicao1.x + LARGURA_TIJOLO, tij2_1.posicao1.y + ALTURA_TIJOLO_FASE2}, 1, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij2_2 = {{XINI+55, YINI+3}, {tij2_2.posicao1.x + LARGURA_TIJOLO, tij2_2.posicao1.y + ALTURA_TIJOLO_FASE2}, 1, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij2_3 = {{XFIM/2-1, YINI+8}, {tij2_3.posicao1.x + LARGURA_TIJOLO, tij2_3.posicao1.y + ALTURA_TIJOLO_FASE2}, 0, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij2_4 = {{XINI+20, YINI+13}, {tij2_4.posicao1.x + LARGURA_TIJOLO, tij2_4.posicao1.y + ALTURA_TIJOLO_FASE2}, 1, PONTUACAO_TIJOLO_FASE2, 0};
    TIJOLO tij2_5 = {{XINI+55, YINI+13}, {tij2_5.posicao1.x + LARGURA_TIJOLO, tij2_5.posicao1.y + ALTURA_TIJOLO_FASE2}, 1, PONTUACAO_TIJOLO_FASE2, 0};


    TIJOLO tij15 = {{XINI+6, YINI+3}, {tij15.posicao1.x + LARGURA_TIJOLO, tij15.posicao1.y + ALTURA_TIJOLO_FASE3}, -2, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij16 = {{XINI+27, YINI+3}, {tij16.posicao1.x + LARGURA_TIJOLO, tij16.posicao1.y + ALTURA_TIJOLO_FASE3}, -1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij17 = {{XINI+48, YINI+3}, {tij17.posicao1.x + LARGURA_TIJOLO, tij17.posicao1.y + ALTURA_TIJOLO_FASE3}, -1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij18 = {{XFIM-10, YINI+3}, {tij18.posicao1.x + LARGURA_TIJOLO, tij18.posicao1.y + ALTURA_TIJOLO_FASE3}, -2, PONTUACAO_TIJOLO_FASE3, 0};

    TIJOLO tij19 = {{XINI+6, YINI+6}, {tij19.posicao1.x + LARGURA_TIJOLO, tij19.posicao1.y + ALTURA_TIJOLO_FASE3}, 0, PONTUACAO_TIJOLO_FASE3, 0}; // Tijolos fase 3
    TIJOLO tij20 = {{XFIM-10, YINI+6}, {tij20.posicao1.x + LARGURA_TIJOLO, tij20.posicao1.y + ALTURA_TIJOLO_FASE3}, 0, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij21 = {{XINI+6, YINI+9}, {tij21.posicao1.x + LARGURA_TIJOLO, tij21.posicao1.y + ALTURA_TIJOLO_FASE3}, 0, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij22 = {{XFIM-10, YINI+9}, {tij22.posicao1.x + LARGURA_TIJOLO, tij22.posicao1.y + ALTURA_TIJOLO_FASE3}, 0, PONTUACAO_TIJOLO_FASE3, 0};

    TIJOLO tij23 = {{XINI+6, YINI+12}, {tij23.posicao1.x + LARGURA_TIJOLO, tij23.posicao1.y + ALTURA_TIJOLO_FASE3}, -2, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij24 = {{XINI+27, YINI+12}, {tij24.posicao1.x + LARGURA_TIJOLO, tij24.posicao1.y + ALTURA_TIJOLO_FASE3}, -1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij25 = {{XINI+48, YINI+12}, {tij25.posicao1.x + LARGURA_TIJOLO, tij25.posicao1.y + ALTURA_TIJOLO_FASE3}, -1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij26 = {{XFIM-10, YINI+12}, {tij26.posicao1.x + LARGURA_TIJOLO, tij26.posicao1.y + ALTURA_TIJOLO_FASE3}, -2, PONTUACAO_TIJOLO_FASE3, 0};

    TIJOLO tij3_1 = {{XINI+16, YINI+3}, {tij3_1.posicao1.x + LARGURA_TIJOLO, tij3_1.posicao1.y + ALTURA_TIJOLO_FASE3}, 1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij3_2 = {{XINI+37, YINI+3}, {tij3_2.posicao1.x + LARGURA_TIJOLO, tij3_2.posicao1.y + ALTURA_TIJOLO_FASE3}, 1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij3_3 = {{XINI+58, YINI+3}, {tij3_3.posicao1.x + LARGURA_TIJOLO, tij3_3.posicao1.y + ALTURA_TIJOLO_FASE3}, 1, PONTUACAO_TIJOLO_FASE3, 0};

    TIJOLO tij3_4 = {{XINI+16, YINI+12}, {tij3_4.posicao1.x + LARGURA_TIJOLO, tij3_4.posicao1.y + ALTURA_TIJOLO_FASE3}, 1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij3_5 = {{XINI+37, YINI+12}, {tij3_5.posicao1.x + LARGURA_TIJOLO, tij3_5.posicao1.y + ALTURA_TIJOLO_FASE3}, 1, PONTUACAO_TIJOLO_FASE3, 0};
    TIJOLO tij3_6 = {{XINI+58, YINI+12}, {tij3_6.posicao1.x + LARGURA_TIJOLO, tij3_6.posicao1.y + ALTURA_TIJOLO_FASE3}, 1, PONTUACAO_TIJOLO_FASE3, 0};


    TIJOLO listatij[FASES_CENARIOS][QUANT_MAX_TIJOLOS] = {{tij2, tij3, tij4, tij5, tij1_2, tij1_3}, {tij1, tij2, tij5, tij6, tij1_1, tij1_4}, {tij8, tij10, tij11, tij12, tij13, tij14, tij2_3, tij2_4, tij2_5}, {tij7, tij8, tij9, tij10, tij11, tij13, tij2_1, tij2_2, tij2_3}, {tij16, tij17, tij19, tij20, tij21, tij22, tij23, tij24, tij25, tij26, tij3_1, tij3_2, tij3_3, tij3_4, tij3_6}, {tij15, tij16, tij17, tij18, tij19, tij20, tij21, tij22, tij24, tij25, tij3_1, tij3_3, tij3_4, tij3_5, tij3_6}};
    // Lista de tijolos

    FASE fases[NUMFASES] = {{1, 5, 6}, {2, 4, 9}, {3, 3, 15}};
    // Lista de fases

    roda_fase(fases, &pal, &bol, listatij, &jogador1, 1, 0);   // Roda o jogo
    gotoxy(XINI, YFIM+1);
    system("pause");
    clrscr();   // Quando terminar o jogo, limpa a tela e encerra o programa
    gotoxy(1, 1);
    system("pause");
}

int main(){
    //Funcao principal do jogo
    menu();
    return 0;
}

