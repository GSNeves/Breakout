// Nomes: Gustavo Spellmeier Neves e Joao Pedro Kuhn Braun


#include "ArquivosBreakout.h"


short conta_jogadores (JOGADOR jogadores[]){
// Conta a quantidade de jogadores presentes no arranjo passado por referencia, retorna essa quantidade
    short i = 0;
    while (i < MAX_JOGADORES && jogadores[i].nome[0] != '\0' && jogadores[i].pontuacao != 0){ // Jogador valido
        i++;
    }
    return i;
}

void imprime_jogador (JOGADOR jog, int n){
    // Imprime o nome e pontuacao do jogador passado por valor, adicionando um inteiro n a coordenada Y onde serao impressos, sem retorno
    if(jog.pontuacao != 0 && strcmp("\0", jog.nome)){ // Jogador valido
        gotoxy(XFIM+4, YINI+7+n);
        printf("%s - %d                                             ", jog.nome, jog.pontuacao);
    }
}

void imprime_jogadores (JOGADOR jogadores[]){
    // Imprime as informacoes a respeito dos jogadores do arranjo passado para a funcao, por referencia. Sem retorno
    for (int i = 0; i < MAX_JOGADORES; i++)
    {
        imprime_jogador (jogadores[i], i);
    }
}

int abrearquivo (FILE **arqtxt){
    // Abre o arquivo BreakoutJog.txt para leitura, retornando 1 se aberto com exito, ou retornando 0 caso nao foi possivel abrir,
    // passando o arquivo aberto para a funcao que chamou AbreArqJog por referencia
    *arqtxt = fopen("BreakoutJog.txt", "r");
    if (!(*arqtxt)){
        printf("Erro na abertura do arquivo .txt!\n");
        return 0;
    }
    return 1;
}

int LeArqTexto (FILE **arqtxt, JOGADOR jogadores[]){
    // Le o arquivo BreakoutJog.txt, passando o ranking dos jogadores para o programa atraves do arranjo passado como argumento por referencia,
    // e fechando o arquivo, tambem passado por referencia, apos a leitura. Retorna o numero de jogadores lidos do arquivo, e 0 caso ele esteja vazio
    short cont_jogador = 0;
    char c[TAM_NOME + TAM_PONTUACAO + 1]; // Tamanho maximo da linha esperada

    while (!feof(*arqtxt) && cont_jogador < MAX_JOGADORES){ // Controla em qual jogador que os valores que estao sendo lidos serao inseridos

            fgets(c, TAM_NOME + TAM_PONTUACAO + 1, *arqtxt);

            strcpy(jogadores[cont_jogador].nome, strtok(c, "#"));

            jogadores[cont_jogador].pontuacao = atoi(strtok(NULL, "\n"));

            cont_jogador++;
    }

    fclose(*arqtxt);
    return cont_jogador;
}

int esta_na_lista (JOGADOR jog, JOGADOR jogadores[]){
    // Confere se o nome do jogador passado por valor esta em algum dos jogadores do ranking, arranjo de jogadores passado por referencia.
    // Retorna MAX_JOGADORES se o nome nao esta no arranjo, caso contrario, retorna o indice do jogador que ja possui tal nome.
    short cont = 0;

    while (cont < MAX_JOGADORES && strcmp(jog.nome, jogadores[cont].nome) != 0)
    {
        cont++;
    }

    return cont;
}

void insere_ou_nao (JOGADOR jog, JOGADOR jogadores[], short *inseriu){
    // Compara a pontuacao do jogador passado por valor com a pontuacao dos jogadores no arranjo, passado por referencia, substituindo algum jogador no
    // ranking caso a pontuacao do jogador ultrapasse a de outro, fazendo todos os outros jogadores com pontuacoes menores (se tiver) "cairem" uma posicao. Sem retorno
    // altera o valor da variavel short inseriu, passada por referencia, caso tenha inserido o jogador no ranking
    short cont;
    JOGADOR jogador_aux;

    for (cont = 0; cont < MAX_JOGADORES; cont++)
    {
        if (jog.pontuacao > jogadores[cont].pontuacao)
        {
            jogador_aux = jogadores[cont];
            jogadores[cont] = jog;
            jog = jogador_aux;
            *inseriu = 1;
        }
    }
}


void OrdenaListaJogadores (JOGADOR jog, JOGADOR jogadores[], short *inseriu){
    // Insere o jogador passado por valor no arranjo referente ao ranking dos jogadores, passado por referencia, caso o jogador tenha
    // feito uma pontuacao maior que a quinta menor, caso o arranjo nao tenha 5 jogadores e caso o jogador ja nao tenha seu nome no ranking, ou já tenha,
    // mas obteve uma pontuacao maior que a anterior. Sem retorno. Altera o valor de inseriu, passado por referencia, caso insira o jogador no ranking
    short aux;
    aux = esta_na_lista(jog, jogadores);

    if (aux == MAX_JOGADORES) // Jogador nao esta na lista
    {
        insere_ou_nao(jog, jogadores, inseriu); // Testa se deve ser inserido no ranking
    }

    else
    {
        if (jog.pontuacao >= jogadores[aux].pontuacao) // Quando o jogador estiver na lista, compara a sua pontuacao atual com a anterior
        {
            jogadores[aux].pontuacao = -1;
            insere_ou_nao (jog, jogadores, inseriu);
        }
    }
}

void MostraListaJogadores (JOGADOR jogadores[], JOGADOR jog){
    // Mostra para o usuario os nomes nao repetidos dos jogadores do arranjo com suas pontuacoes, incluindo o jogador atual, em ordem
    // decrescente de pontuacao. Sem retorno
    short inseriu = 0;
    short ultimo = conta_jogadores(jogadores);
    short presente_na_lista = esta_na_lista(jog, jogadores);

    JOGADOR jogador_ultimo = jogadores[ultimo-1]; // Guarda o ultimo jogador da lista antes de ser alterada

    OrdenaListaJogadores(jog, jogadores, &inseriu);


    if (inseriu == 0 && presente_na_lista == MAX_JOGADORES){ // Nao foi inserido
        imprime_jogadores(jogadores);
        imprime_jogador(jog, 10);
    }

    else{ // Foi inserido
        imprime_jogadores(jogadores);
        if (ultimo == MAX_JOGADORES && presente_na_lista == MAX_JOGADORES){
            imprime_jogador(jogador_ultimo, 10);
        }
    }

}


int SalvaListaJogadores(FILE *arq, JOGADOR jogadores[]){
    // Sobrescreve o arquivo BreakoutJog.txt com o novo ranking de jogadores, mantendo o mesmo formato. Retorna 1 se salvar os novos
    // jogadores com sucesso, ou retorna 0 caso contrario
    arq = fopen("BreakoutJog.txt", "w");

    if (!arq){
        printf("Erro ao abrir arquivo!\n");
        return 0;
    }

    for (short i = 0; i < conta_jogadores(jogadores); i++){ // Caso nao tenha MAX_JOGADORES jogadores, escreve menos no arquivo
        fprintf(arq, "%s#%d\n", jogadores[i].nome, jogadores[i].pontuacao);
    }

    fclose(arq);
    return 1;
}

int le_jogo_salvo(PAUSAJOGO *dados, char nome[]){
    // Abre um arquivo .bin de nome passado como argumento e pega os dados do jogo salvo nesse arquivo, passando para o jogo por referência
    // Retorna 1 caso a operacao nao foi bem sucedida, 0 caso foi bem sucedida
    FILE *arq;
    if ((arq = fopen(strcat(nome, ".bin"), "rb")) == NULL){
        printf("Erro na abertura do arquivo!\n");  // Se houve erro na abertura para ler o arquivo
        return 1;
    }
    else{
        if (fread(dados, sizeof(PAUSAJOGO), 1, arq) != 1){
            printf("Erro na leitura!\n"); // Se houve erro ao ler o arquivo
            fclose(arq);
            return 1;
        }
        fclose(arq);
        return 0;
    }
}

int salva_estado_do_jogo(PAUSAJOGO dados){
    // Salva os dados do jogo, passados por valor, num arquivo chamado "nome_do_jogador.bin", retornando 1 caso erro e 0 caso bem sucedido
    FILE *arq;
    char nome[TAM_NOME];
    strcpy(nome, dados.jogador.nome);
    if((arq = fopen(strcat(nome, ".bin"), "wb")) == NULL){
        printf("Erro na abertura do arquivo!\n"); // Se houve erro ao abrir o arquivo para escrita
        return 1;
    }
    else {
        if(fwrite(&dados, sizeof(PAUSAJOGO), 1, arq) != 1){
            printf("Erro na escrita dos dados!\n"); // se houve erro ao escrever os dados no arquivo
            fclose(arq);
            return 1;
        }
    }
    fclose(arq);
    return 0;
}

void imprime_informacoes(JOGADOR jog){
    // Cria um menu geral de informacoes para o jogador acompanhar durante a execucao do jogo, por causa do JOGADOR passado como valor
        gotoxy(1, 1);
        printf("Comandos:  Carregar jogo (C)     Pausa (P)     Sair (ESC)");

        gotoxy(XFIM+4, YINI);
        printf("Jogador: %s", jog.nome);
        gotoxy(XFIM+4, YINI+1);
        printf("Pontuacao: %d", jog.pontuacao);
        gotoxy(XFIM+4, YINI+2);
        printf("Vidas: %d", jog.vidas);
        gotoxy(XFIM+4, YINI+3);
        printf("Fase: %d", jog.fase_atual/2 + 1);

        gotoxy(XFIM+4, YFIM-4);
        printf("Disciplina INF1202 - Algoritmos");
        gotoxy(XFIM+4, YFIM-3);
        printf("Semestre: 2020-1");
        gotoxy(XFIM+4, YFIM-2);
        printf("Programadores:");
        gotoxy(XFIM+4, YFIM-1);
        printf("Gustavo Spellmeier Neves");
        gotoxy(XFIM+4, YFIM);
        printf("Joao Pedro Kuhn Braun");
}
