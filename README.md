# Breakout (INF1202 - UFRGS)

Implementação em **C** do clássico jogo **Breakout** em modo texto para console Windows, desenvolvida como trabalho final da disciplina **INF1202 - Algoritmos** (UFRGS, 2020/1).

## Funcionalidades

- **3 fases** com **2 cenários possíveis por fase** (sorteados a cada etapa), variando velocidade da bola, tamanho da palheta e resistência/pontuação dos tijolos.
- **Sistema de bônus** ao destruir tijolos (ativados ao rebater a bola colorida com a palheta):
  - Verde: `+100` pontos
  - Vermelho: `+1` vida (até 9 vidas)
  - Azul: aumento temporário da largura da palheta
- **Salvar e carregar partida**: salva o estado completo do jogo em `<nome>.bin`.
- **Scoreboard (Top 5)**: ranking persistido no arquivo `BreakoutJog.txt`.

## Controles

- **Setas `←` / `→` ou `Espaço`**: iniciar a fase
- **Setas `←` / `→`**: mover a palheta
- **`P`**: pausar e salvar o jogo atual
- **`C`**: carregar um jogo salvo
- **`ESC`**: sair do jogo

## Arquivos do Projeto

- `main.c`: loop principal, desenho em console, física/colisões, bônus e configuração das fases.
- `ArquivosBreakout.h` / `ArquivosBreakout.c`: structs, constantes, HUD e persistência do ranking (`BreakoutJog.txt`) e de partidas salvas (`.bin`).
- `BreakoutJog.txt`: arquivo com o placar Top 5 inicial.

## Requisitos e Compilação

- **Sistema Operacional**: Windows (utiliza `<windows.h>`, `<conio.h>` e `<conio2.h>`).
- **Compilador**: GCC/MinGW (por exemplo, via **Code::Blocks**) com a biblioteca `conio2` configurada.

Exemplo de compilação via terminal (com `conio2` instalada no MinGW):

```bash
gcc main.c ArquivosBreakout.c -o breakout.exe -lconio
```

> Execute o binário na mesma pasta de `BreakoutJog.txt` para que o ranking seja carregado e salvo corretamente.

## Autores

- **Gustavo Spellmeier Neves**
- **João Pedro Kuhn Braun**
