// Nomes: Gustavo Spellmeier Neves e Joao Pedro Kuhn Braun
#include <stdio.h>
#include <stdlib.h>
#include <conio2.h>
#include <string.h>

#define QUANT_MAX_TIJOLOS   15
#define TAM_NOME 41
#define NUMFASES 3
#define MAX_JOGADORES 5
#define TAM_PONTUACAO 5
#define FASES_ETAPA 2
#define FASES_CENARIOS FASES_ETAPA * NUMFASES

typedef enum  {
XINI = 1,   // Onde comeca a moldura em X
XFIM = 80,  // Onde termina a moldura em X
YINI = 2,   // Onde comeca a moldura em Y
YFIM = 29,  // Onde termina a moldura em Y
}MOLDURA;

typedef enum {
    // Numeros das teclas em ASCII
    ESC      = 27,
    ESPECIAL = -32,
    ESPECIAL2 = 0,
    SETA_ESQ = 75,
    SETA_DIR = 77,
    P = 112,
    C = 99,
    ESPACO = 32,
}TECLAS;

typedef struct{
    // Coordenadas x e y no plano para representar as posicoes dos elementos
    int x;
    int y;
}COORDENADA;

typedef struct{
    // Refere-se a posicao atual da bola, sua velocidade e direcao
    COORDENADA posicao; // Posicao da bola no jogo
    COORDENADA velocidade_direcao; // Direcao e velocidade de deslocamento
    short bonus;
}BOLA;

typedef struct{
    // Refere-se a area que a palheta ocupa no jogo, e sua velocidade e direcao
    COORDENADA velocidade_direcao;
    COORDENADA ponto1; // Coordenada do canto superior esq do envelope
    COORDENADA ponto2; // Coordenada do canto inferior dir do envelope
}PALHETA;

typedef struct{
    // Refere-se ao jogador que esta jogando no momento, tendo como atributos seu nome, quantidade de vidas, fase em que se encontra e sua pontuacao
    char nome[TAM_NOME];
    int vidas;
    int fase_atual;
    int pontuacao;
}JOGADOR;

typedef struct{
    // Refere-se aos atributos de um tijolo, ou seja, a area que ele ocupa no jogo, se existe ou nao, seu valor em pontos e se tem bonus ou nao
    COORDENADA posicao1;
    COORDENADA posicao2;
    short estado;
    int pontos;
    short bonus;
}TIJOLO;

typedef struct{
    // Refere-se aos atributos de uma fase: a cor da moldura durante a fase, a velocidade da bolinha e o numero de tijolos que tera nessa fase
    COLORS cor;
    int velocidade;
    int numTijolos;
}FASE;

typedef struct{
    // Estrutura PAUSAJOGO que salva todos os dados do jogo
    JOGADOR jogador;
    TIJOLO listatij[FASES_CENARIOS][QUANT_MAX_TIJOLOS];
    BOLA bolinha;
    PALHETA palheta;
}PAUSAJOGO;

int le_jogo_salvo(PAUSAJOGO *dados, char nome[]);
    // Abre um arquivo .bin de nome passado como argumento e pega os dados do jogo salvo nesse arquivo, passando para o jogo por referência
    // Retorna 1 caso a operacao nao foi bem sucedida, 0 caso foi bem sucedida


int salva_estado_do_jogo(PAUSAJOGO dados);
    // Salva os dados do jogo, passados por valor, num arquivo chamado "nome_do_jogador.bin", retornando 1 caso erro e 0 caso bem sucedido


void imprime_informacoes(JOGADOR jog);
    // Cria um menu geral de informacoes para o jogador acompanhar durante a execucao do jogo, por causa do JOGADOR passado como valor


short conta_jogadores (JOGADOR jogadores[]);
// Conta a quantidade de jogadores presentes no arranjo passado por referencia, retorna essa quantidade


void imprime_jogador (JOGADOR jog, int n);
// Imprime o nome e pontuacao do jogador passado por valor, adicionando um inteiro n a coordenada Y onde serao impressos, sem retorno


void imprime_jogadores (JOGADOR jogadores[]);
// Imprime as informacoes a respeito dos jogadores do arranjo passado para a funcao, por referencia. Sem retorno


int abrearquivo (FILE **arqtxt);
// Abre o arquivo BreakoutJog.txt, retornando 1 se aberto com exito, ou retornando 0 caso nao foi possivel abrir,
// passando o arquivo aberto para a funcao que chamou AbreArqJog por referencia


int LeArqTexto (FILE **arqtxt, JOGADOR jogadores[]);
// Le o arquivo BreakoutJog.txt, passando o ranking dos jogadores para o programa atraves do arranjo passado como argumento por referencia,
// e fechando o arquivo, tambem passado por referencia, apos a leitura. Retorna o numero de jogadores lidos do arquivo, e 0 caso ele esteja vazio


int esta_na_lista (JOGADOR jog, JOGADOR jogadores[]);
// Confere se o nome do jogador passado por valor esta em algum dos jogadores do ranking, arranjo de jogadores passado por referencia.
// Retorna MAX_JOGADORES se o nome nao esta no arranjo, caso contrario, retorna o indice do jogador que ja possui tal nome.


void insere_ou_nao (JOGADOR jog, JOGADOR jogadores[], short *inseriu);
// Compara a pontuacao do jogador passado por valor com a pontuacao dos jogadores no arranjo, passado por referencia, substituindo algum jogador no
// ranking caso a pontuacao do jogador ultrapasse a de outro, fazendo todos os outros jogadores com pontuacoes menores (se tiver) "cairem" uma posicao. Sem retorno
// altera o valor da variavel short inseriu, passada por referencia, caso tenha inserido o jogador no ranking



void OrdenaListaJogadores (JOGADOR jog, JOGADOR jogadores[], short *inseriu);
// Insere o jogador passado por valor no arranjo referente ao ranking dos jogadores, passado por referencia, caso o jogador tenha
// feito uma pontuacao maior que a quinta menor, caso o arranjo nao tenha 5 jogadores e caso o jogador ja nao tenha seu nome no ranking, ou já tenha,
// mas obteve uma pontuacao maior que a anterior. Sem retorno. Altera o valor de inseriu, passado por referencia, caso insira o jogador no ranking


void MostraListaJogadores (JOGADOR jogadores[], JOGADOR jog);
// Mostra para o usuario os nomes nao repetidos dos jogadores do arranjo com suas pontuacoes, incluindo o jogador atual, em ordem
// decrescente de pontuacao. Sem retorno



int SalvaListaJogadores(FILE *arq, JOGADOR jogadores[]);
// Sobrescreve o arquivo BreakoutJog.txt com o novo ranking de jogadores, mantendo o mesmo formato. Retorna 1 se salvar os novos
// jogadores com sucesso, ou retorna 0 caso contrario




