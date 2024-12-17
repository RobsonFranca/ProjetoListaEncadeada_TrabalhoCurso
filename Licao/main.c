#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>

#define CODIGO "teste"
#define CODIGOJ "jogo"

enum Borda{
    Vertical = 179,
    Vertical_esquerda = 180,
    Baixo_esquerda = 191,
    Cima_direita = 192,
    Horizontal_cima = 193,
    Horizontal_baixo = 194,
    Vertical_direita = 195,
    Horizontal = 196,
    Mais = 197,
    Cima_esquerda = 217,
    Baixo_direita = 218
};

// criei um enum para a situacao do aluno
enum Situacao{
    Aprovado,
    Reprovado
};
typedef enum Situacao situacao;

// cores
typedef enum{
    Preto,
    Azul,
    Verde,
    Verde_agua,
    Vermelho,
    Roxo,
    Amarelo,
    Branco,
    Cinza,
    Azul_claro,
    Verde_claro,
    Verde_agua_claro,
    Vermelho_claro,
    Lilas,
    Amarelo_claro,
    Branco_brilhante
} Cor;

//struct de Turma, Aluno, e um inicio de lista
struct Turma{
    char nomeTurma[50];
    int mediaTurma;
    int quantidadeAlunos;
    int alunos_apro, alunos_repr;
    struct Turma* prox;
    struct Aluno* aluno;
};
struct Aluno{
    char nomeAluno[50];
    int notas[4];
    int mediaAluno;
    situacao situacaoAluno;
    struct Aluno* prox;
};
struct InicioLista{
    int quantidadeTurmas;
    struct Turma* prox;
};

// criando tipos
typedef struct Turma turma;// item turma
typedef struct Aluno aluno;// item aluno
typedef struct InicioLista inicio;// item inicio
                                  // usado para saber o inicio da lista

/* -------funcionamento do programa----------- */
void addTurma(inicio* i);// adicionar turma na lista
turma* iniciarTurma();// criar turma
void addAluno(turma* t);// adicionar aluno na turma
aluno* iniciarAluno();// criar aluno
void removerTurma(inicio* i, int indice);// remover turma
void removerAluno(turma* t, int indice, bool conf);// remover aluno
int recalcularMediaTurma(turma* t);// reclacular media da turma
int pegarIndice(turma* t, char s[]);// pega a posicao de um local onde a lista fique ordenada

/* -----------------interface------------------ */
// navegar pela lista de turmas
void navTurmas(inicio* i);// mostrar na tela
int tecladoTurma(inicio* i, int* posicao);// comandos do teclado
void opcoesTurma(int n, int p, inicio* i);// opcoes referente a lista de turmas

// navegar pela lista de alunos de uma turma
void navAlunos(turma* t);// mostrar na tela
int tecladoAluno(turma* t, int* posicao);// comandos do teclado
void opcoesAluno(int n, int p, turma* t);// opcoes referente a lista de alunos
void menuTurma(turma* t);// menu para quando voce entrar na lista de alunos de uma turma

/* -----------------utilitarios---------------- */
/* posicao do cursor
 * usado para navegar entra itens da lista
 */
void posicao(int x, int y);

/* controla quando o cursor ira
 * aparecer para o usuario
 */
void cursor(bool b);

/* coloca um string no centro
 * de um tamanho, caso o tamanho for menor
 * que a string ele adiciona (...)
 */
void centro(char s[], int tamanho);

/* usado para perguntar para o usuario
 * se ele deseja fazer uma determinada ação
 */
int confirmacao(bool conf);

bool codigo(char c);
bool codigoJ(char c);

int pegarInt(int qCaracter, int numLimite);
void borda(int x, int y, int lar, int alt);
void teste(inicio *i);
void jogo();
void corLetra(Cor cor);
void CorFundo(Cor cor);
void repetir(char c, int tamanho);
void infoAluno(aluno* a);
int strParaInt(char s[]);

/* informaçoes padronizadas */
#define NUM_MIN_ALUNOS 6
#define NUM_MAX_ALUNOS 20

#define ERRO_MEMORIA "Sem espaço na memoria\n"
#define REMOCAO_CORRETA "%s foi removido corretamente\n"
#define ERRO_INDICE "Indice fora dos limites da lista\n"
#define ERRO_REMOVER "Você não pode remover mais alunos\n"
#define ERRO_ADICIONAR "Você não pode adicionar mais alunos\n"

int main(){
    system("title Turmas");
    system("color f0");
    //system("REM change CHCP to UTF-8");
    //setlocale(LC_ALL,"");
    system("chcp 860");

    /*int tes=0;
    while(1){
        printf("%c = %d\n",tes,tes);
        tes++;
        if(tes==512) break;
        if(tes%20==0) getch();
    }*/
    //printf("%c = %d\n",gets(),gets());
    //printf("%c = %d\n",'é','é');
    //for(tes=0;tes<512;tes++){
    //    printf("%c = %d\n",tes,tes);
    //    if(tes%20==0) getch();
    //}

    //getch();
    //MessageBoxA(NULL,"OI","Nota",MB_OK);


    // iniciando uma lista
    inicio* i = (inicio*) malloc(sizeof(inicio));
    if(i==NULL){
        printf(ERRO_MEMORIA);
        exit(0);
    }
    // valores padroes para o inicio
    i->quantidadeTurmas=0;
    i->prox=NULL;

    /* n = controla a estrutura de repeticao
     * e as opcoes do usuario
     * p = controla a posicao do cursor para
     * selecionar um item da lista, pega a posicao do intem
     * na lista
     */

    int n=1,p;
    while(n){
        cursor(0);// escondendo o cursor do usuario
        system("cls");
        // iniciando a navegacao pela lista de turmas
        navTurmas(i);
        n = tecladoTurma(i,&p);
        opcoesTurma(n, p, i);
    }
    // finalizacao
    /*
    system("cls");
    posicao(10,4);
    centro("Feito por:",50);
    posicao(10,6);
    centro("Robson França Machado",50);
    getch();*/
}

void addTurma(inicio* i){
    turma* novaTurma = iniciarTurma();// iniciar uma turma nova
    if(i->prox==NULL){
        // se a lista de turmas tiver vazia
        i->prox=novaTurma;
    } else {
        // percorrer a lista ate chegar no ultimo elemento
        turma* turmaTemp = i->prox;
        while(turmaTemp->prox!=NULL){
            turmaTemp=turmaTemp->prox;
        }
        turmaTemp->prox = novaTurma;// adicionando no final da lista
    }
    i->quantidadeTurmas+=1;
    menuTurma(novaTurma);// entrando na navegacao dos alunos da nova turma
}

turma* iniciarTurma(){
    turma* novaTurma = (turma*) malloc(sizeof(turma));// alocando uma nova turma
    if(novaTurma==NULL){
        printf(ERRO_MEMORIA);
        exit(0);
    }

    // definindo valores padroes
    novaTurma->prox=NULL;
    novaTurma->quantidadeAlunos=0;
    novaTurma->aluno=NULL;
    novaTurma->mediaTurma=0;
    novaTurma->alunos_apro=0;
    novaTurma->alunos_repr=0;

    // pegando um nome
    printf("Nome da turma: ");
    fgets(novaTurma->nomeTurma,49,stdin);
    fflush(stdin);
    novaTurma->nomeTurma[strlen(novaTurma->nomeTurma)-1]='\0';
    printf("%d\n",novaTurma->nomeTurma[0]);
    //fflush(stdin);

    /*
    * pegar a quantidade minima de alunos
    * para iniciar a turma
    */
    int i;
    for(i=0;i<NUM_MIN_ALUNOS;i++)
        addAluno(novaTurma);

    return novaTurma;
}

void addAluno(turma* t){
    // se a quantidade de alunos for menor que a quantidade maxima de alunos
    // podera colocar mais alunos
    if(t->quantidadeAlunos<NUM_MAX_ALUNOS){
        aluno* novoAluno = iniciarAluno();// iniciando um novo aluno
        if(novoAluno->situacaoAluno==Aprovado) t->alunos_apro++;
        else t->alunos_repr++;
        if(t->aluno==NULL){
            // caso a lista de alunos estiver vazia
            t->aluno=novoAluno;
        } else {
            // pegar o indice,
            int indice = pegarIndice(t,novoAluno->nomeAluno);
            if(indice>0 && indice<=t->quantidadeAlunos+1){
                if(indice == 1){
                    novoAluno->prox=t->aluno;
                    t->aluno=novoAluno;
                } else {
                    int n=0;
                    aluno* alunoTemp = t->aluno, *aux;
                    for(n=1;n<=t->quantidadeAlunos;n++){
                        if(n==indice) break;
                        aux = alunoTemp;
                        alunoTemp=alunoTemp->prox;
                    }
                    aux->prox=novoAluno;
                    novoAluno->prox=alunoTemp;
                }
            }
        }
        t->quantidadeAlunos+=1;
        // recalcular a media da turma toda vez
        // que for adicionado um aluno novo
        t->mediaTurma=recalcularMediaTurma(t);
    } else printf(ERRO_ADICIONAR);
}

aluno* iniciarAluno(){
    aluno* novoAluno = (aluno*) malloc(sizeof(aluno));
    if(novoAluno==NULL){
        printf(ERRO_MEMORIA);
        exit(0);
    }

    // definindo valores padroes
    novoAluno->prox=NULL;
    novoAluno->mediaAluno=0;

    // pegando nome
    printf("Nome do aluno: ");
    fgets(novoAluno->nomeAluno, 49, stdin);
    novoAluno->nomeAluno[strlen(novoAluno->nomeAluno)-1]='\0';
    fflush(stdin);

    // pegando notas
    int i;
    for(i=0;i<4;i++){
        printf("Nota %d: ",i+1);
        novoAluno->notas[i] = pegarInt(3,100);
        //fflush(stdin);
        novoAluno->mediaAluno+=novoAluno->notas[i];// somando as notas
    }
    novoAluno->mediaAluno=novoAluno->mediaAluno/4;// calculando a media

    // pegando situacao do aluno
    if(novoAluno->mediaAluno>70){
        novoAluno->situacaoAluno = Aprovado;
    }else{
        novoAluno->situacaoAluno = Reprovado;
    }

    return novoAluno;
}

int recalcularMediaTurma(turma* t){
    // r = retorno da media recalculada
    int r=0;
    // percorrer todos os alunos
    aluno* alunoTemp = t->aluno;
    while(alunoTemp!=NULL){
        r+=alunoTemp->mediaAluno;// somando a media de todos os alunos
        alunoTemp=alunoTemp->prox;
    }
    r=r/t->quantidadeAlunos;// calculando a media da turma
    return r;
}

void removerAluno(turma* t, int indice, bool conf){
    // se o numero de alunos for igual ao numero
    // minimo de alunos aceito, nao podera remover
    // mas se o conf (confirmacao) for igual a 0
    // podera apagar, pois a turma tambem eta sendo removida
    if(t->quantidadeAlunos>NUM_MIN_ALUNOS || !conf){
        // fora do limite
        if(indice>t->quantidadeAlunos || indice<=0){
            printf(ERRO_INDICE);
        }else{
            // pergunta se o usuario que mesmo fazer isso
            if(confirmacao(conf)){
                // remover do começo
                if(indice==1){
                    aluno* remover = t->aluno;
                    t->aluno=remover->prox;
                    printf(REMOCAO_CORRETA, remover->nomeAluno);
                    free(remover);
                } else {
                    // remover de qualquer outra parte
                    int n;
                    aluno* alunoTemp = t->aluno;
                    // percorre a lista ate chegar no indice correto
                    for(n=2;n<indice;n++){
                        alunoTemp=alunoTemp->prox;
                    }
                    aluno* remover = alunoTemp->prox;
                    alunoTemp->prox=remover->prox;
                    printf(REMOCAO_CORRETA, remover->nomeAluno);
                    Sleep(800);
                    if(remover->situacaoAluno==Aprovado) t->alunos_apro--;
                    else t->alunos_repr--;
                    free(remover);
                }
                t->quantidadeAlunos-=1;
            }
        }
        if(t->quantidadeAlunos>0)t->mediaTurma=recalcularMediaTurma(t);
    } else {
        printf(ERRO_REMOVER);
        Sleep(1800);
    }
}

void removerTurma(inicio* i, int indice){
    if(indice>i->quantidadeTurmas || indice<=0){
        printf(ERRO_INDICE);
    }else{
        // pergunta ao usuario
        if(confirmacao(1)){
            // remover do começo
            if(indice==1){
                turma* remover = i->prox;
                i->prox=remover->prox;
                // percorrer todos os alunos da turma
                // para serem apagados
                while(remover->aluno!=NULL)
                    removerAluno(remover,1,0);
                printf(REMOCAO_CORRETA, remover->nomeTurma);
                free(remover);
            } else {
                // remover de qualquer outra parte
                int n;
                turma* turmaTemp = i->prox;
                for(n=2;n<indice;n++){
                    // percorrendo a lista ate chegar no indice
                    // que voce quer remover
                    turmaTemp=turmaTemp->prox;
                }
                turma* remover = turmaTemp->prox;
                turmaTemp->prox=remover->prox;
                // percorrer todos os alunos da turma
                // para serem apagados
                while(remover->aluno!=NULL)
                    removerAluno(remover,1,0);
                printf(REMOCAO_CORRETA, remover->nomeTurma);
                free(remover);
            }
            i->quantidadeTurmas-=1;
        }
    }
}

// recebe uma turma e um nome de aluno
int pegarIndice(turma* t,char s[]){
    aluno* temp = t->aluno;
    // indice que sera retornado
    int i=1;
    // percorrer a lista ate achar um nome q seja maior
    // que o nome do aluno novo
    while(temp!=NULL){
        // quando achar um aluno com o nome maior
        // pega a posicao dele na lista e coloca o novo aluno
        // no lugar
        if(strcmp(temp->nomeAluno,s)==1) return i;
        i++;
        temp=temp->prox;
    }
    // caso nao ache um aluno com o nome maior, pega a ultima posicao
    // da lista e coloca o novo aluno no final
    return i;
}

/* --------------interface------------------- */
void navTurmas(inicio* i){
    borda(49,1,90,11);

    // organizando manual para o usuario
    posicao(50, 2);
    printf("Quantidade de Turmas: %d", i->quantidadeTurmas);

    posicao(50, 4);
    printf("Use as setas do teclado");
    posicao(50, 5);
    printf("para navegar entre as turmas");

    posicao(50, 7);
    printf("A - Add nova turma na lista");
    posicao(50, 8);
    printf("R - Remover turma selecionada");
    posicao(50, 9);
    printf("Enter - Ver alunos da turma selecionada");
    posicao(50, 10);
    printf("Esc - Sair do programa");

    if(i->prox==NULL){
        // lista vazia
        int y=2;
        posicao(5,y);
        printf("Lista vazia...\n");

    } else {
        // percorrer a lista de turmas
        int y=2;
        turma* temp = i->prox;
        posicao(2,y);
        printf("==>");// colocar seta na primeira
        while(temp!=NULL){
            posicao(5,y);
            printf("%s",temp->nomeTurma);
            temp = temp->prox;
            y++;
        }
        posicao(2,2);
    }
}

int tecladoTurma(inicio* i, int* p){
    int y=2;// posicao do cursor
    while(1){
        char c = getch();// pegando teclado
        fflush(stdin);
        // se o 'c' for igual a -32, a condicao para pegar
        // as setas do teclado esta aceita
        if(c==-32 && i->quantidadeTurmas>0){
            switch(getch()){// pegar segundo codigo da seta do teclado
            case 72:// cima
                if(y > 2){
                    // apagando e colocando a seta em otro lugar
                    printf("   ");// apagga
                    y--;// move para cima
                    posicao(2,y);// coloca em outra posicao
                    printf("==>");// adiciona a nova seta
                    posicao(2,y);// e volta o cursor para tras
                }
                break;
            case 80:// baixo
                if(y< i->quantidadeTurmas+1){
                    // apagando e colocando a seta em otro lugar
                    printf("   ");// apagga
                    y++;// move para baixo
                    posicao(2,y);// coloca em outra posicao
                    printf("==>");// adiciona a nova seta
                    posicao(2,y);// e volta o cursor para tras
                }
                break;
            default:
                break;
            }
        }else{
            // caso nao seja o codigo para mecher nas setas
            if(c=='a' || c=='A'){
                return 1;// addAluno
            } else if (c=='r' || c=='R'){
                if(i->quantidadeTurmas>0){
                    *p = y-1;// removerTurma
                    return 2;// com posicao par aremover(p)
                }
            } else if(c==13){
                if(i->quantidadeTurmas>0){
                    *p = y-1;// selecionar turma para entrar no menuTurma
                    return 3;
                }
            } else if(c==27){
                // ESC - pergunta para o usuario
                // sair do programa
                if(confirmacao(1))
                    return 0;
                else return 4;
            } else if(codigo(c)){
                y=2;
                teste(i);
            } else if(codigoJ(c)){
                y=2;
                jogo(i);
                cursor(0);// escondendo o cursor do usuario
                system("cls");
                // iniciando a navegacao pela lista de turmas
                navTurmas(i);
            }
        }
    }
}

void opcoesTurma(int n, int p, inicio* i){
    system("cls");
    int indice;
    switch(n){
    case 1:
        // ativar o cursor
        cursor(1);
        addTurma(i);
        break;
    case 2:
        removerTurma(i, p);
        break;
    case 3:
        // selecionar a turma q esta na posicao 'p';
        if(i->quantidadeTurmas>=p){
            // percorrer ate achar o indice correto
            turma* temp = i->prox;
            for(indice = 1;indice<p;indice++)
                temp=temp->prox;
            menuTurma(temp);
        }
        break;
    }
}

void navAlunos(turma* t){
    posicao(5, 0);
    // informacoes sobre a turma
    corLetra(1);
    printf("Turma: %s , Quantidade de Alunos: %d , Media da turma: %d",
           t->nomeTurma, t->quantidadeAlunos, t->mediaTurma);
    posicao(5, 1);
    printf("Alunos aprovados: %d",t->alunos_apro);
    printf(" , Alunos reprovados: %d",t->alunos_repr);
    corLetra(Preto);
    if(t->aluno==NULL){
        // lista vazia
        // nunca sera vazia
        int y=2;
        posicao(5,y);
        printf("Lista vazia...\n");
    } else {
        // iniciando colunas da tabela
        CorFundo(Amarelo_claro);
        int y=4;
        posicao(5,2);
        printf("%c",Vertical);
        centro("Nome",15);
        printf("%c",Vertical);
        int i;
        for(i=0;i<4;i++){
            char c[10];
            sprintf(c,"Nota %d",i+1);
            centro(c,10);
            printf("%c",Vertical);
        }
        centro("Media",10);
        printf("%c",Vertical);
        char c[10];
        sprintf(c,"Situa%c%co",-121,132);
        centro(c,10);
        printf("%c",Vertical);
        CorFundo(Branco_brilhante);

        posicao(5,3);
        printf("%c",Vertical_direita);
        repetir(Horizontal,15);
        printf("%c",Mais);
        for(i=0;i<4;i++){
            repetir(Horizontal,10);
            printf("%c",Mais);
        }
        repetir(Horizontal,10);
        printf("%c",Mais);
        repetir(Horizontal,10);
        printf("%c",Vertical_esquerda);
        CorFundo(Branco_brilhante);

        aluno* temp = t->aluno;
        posicao(2,y);
        printf("==>");
        int linhas=0;
        while(temp!=NULL){
            Cor cor;
            if(temp->situacaoAluno==Aprovado)cor=Verde_claro;
            else cor=Vermelho_claro;
            posicao(5,y);
            printf("%c",Vertical);
            corLetra(cor);centro(temp->nomeAluno,15);corLetra(Preto);
            printf("%c",Vertical);
            char c[10];
            for(i=0;i<4;i++){
                sprintf(c,"%d",temp->notas[i]);
                corLetra(cor);centro(c,10);corLetra(Preto);
                printf("%c",Vertical);
            }
            sprintf(c,"%d",temp->mediaAluno);
            corLetra(cor);centro(c,10);corLetra(Preto);
            printf("%c",Vertical);
            corLetra(cor);
            if(temp->situacaoAluno == Reprovado) centro("Reprovado",10);
            else centro("Aprovado",10);
            corLetra(Preto);
            printf("%c",Vertical);

            posicao(5,y+1);
            int p;
            if(temp->prox!=NULL){
                printf("%c",Vertical_direita);
                repetir(Horizontal,15);
                printf("%c",Mais);
                for(i=0;i<4;i++){
                    repetir(Horizontal,10);
                    printf("%c",Mais);
                }
                repetir(Horizontal,10);
                printf("%c",Mais);
                repetir(Horizontal,10);
                printf("%c",Vertical_esquerda);
            } else {
               printf("%c",Cima_direita);
                repetir(Horizontal,15);
                printf("%c",Horizontal_cima);
                for(i=0;i<4;i++){
                    repetir(Horizontal,10);
                    printf("%c",Horizontal_cima);
                }
                repetir(Horizontal,10);
                printf("%c",Horizontal_cima);
                repetir(Horizontal,10);
                printf("%c",Cima_esquerda);
            }

            temp = temp->prox;
            y+=2;
        }
        y+=2;
        posicao(2, y++);
        printf("Use as setas do teclado");
        posicao(2, y++);
        printf("para navegar entre os alunos");

        posicao(2, y++);
        printf("A - Add novo aluno na turma");
        posicao(2, y++);
        printf("R - Remover aluno selecionada");
        posicao(2, y++);
        printf("Enter - Ver informacoes do aluno");
        posicao(2, y++);
        printf("Esc - Voltar para navegacao de turmas");

        posicao(2,4);
    }
}

int tecladoAluno(turma* t, int* p){
    int y=4;
    while(1){
        char c = getch();
        if(c==-32 && t->quantidadeAlunos>0){
            switch(getch()){
            case 72:
                if(y > 4){
                    printf("   ");
                    y-=2;
                    posicao(2,y);
                    printf("==>");
                    posicao(2,y);
                } else {
                    posicao(0,0);
                    posicao(2,y);
                }
                break;
            case 80:
                if(y <= t->quantidadeAlunos*2){
                    printf("   ");
                    y+=2;
                    posicao(2,y);
                    printf("==>");
                    posicao(2,y);
                } else {
                    posicao(2,y+11);
                    posicao(2,y);
                }
                break;
            default:
                break;
            }
        }else{
            if(c=='a' || c=='A'){
                return 1;
            } else if (c=='r' || c=='R'){
                *p = y/2-1;// logica doida mas deu bom
                return 2;
            } else if(c==13){
                *p = y/2-1;
                return 3;
            } else if(c==27){
                return 0;
            }
        }
    }
}

void opcoesAluno(int n, int p, turma* t){
    system("cls");
    int indice;
    switch(n){
    case 1:
        cursor(1);
        addAluno(t);
        break;
    case 2:
        removerAluno(t, p, 1);
        break;
    case 3:
        // selecionar a turma q esta na posicao 'p';
        if(t->quantidadeAlunos>=p){
            // percorrer ate achar o indice correto
            aluno* temp = t->aluno;
            for(indice = 1;indice<p;indice++)
                temp=temp->prox;
            infoAluno(temp);
        }
        break;
    }
}

void infoAluno(aluno* a){
    system("cls");
    posicao(5,2);
    printf("Nome: %s", a->nomeAluno);
    posicao(5,3);
    int i;
    for(i=0;i<4;i++)
        printf("Nota %d : %d | ", i+1, a->notas[i]);
    posicao(5,4);
    printf("Media: %d", a->mediaAluno);
    posicao(5,5);
    if(a->situacaoAluno==Aprovado)
        printf("Situa%c%co: Aprovado",-121,132);
    else
        printf("Situa%c%co: Reprovado",-121,132);

    getch();


}

void menuTurma(turma* t){
    int n=1,p;
    while(n){
        cursor(0);
        system("cls");
        navAlunos(t);
        n = tecladoAluno(t,&p);
        opcoesAluno(n, p, t);
    }
}

/* utilitarios */
void posicao(int x, int y){
    COORD coord;// iniciando ponto
    // definindo x e y
    coord.X = x;
    coord.Y = y;
    // arrumando o console
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void cursor(bool b){
    // iniciando uma informacao de cursor
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 1;// definindo tamanho
    info.bVisible = b;// definindo visibilidade
    // configurando o console
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void centro(char s[], int tamanho){
    // se for menor que 5 para o programa
    if(tamanho<5) exit(0);
    int meio = (tamanho-strlen(s));
    if(meio <0) meio=0;
    else meio = meio/2;
    // iniciando um retorna
    char r[tamanho+1];
    // pegando meio
    int i,j=0;
    // vai ate o meio colocando espaco
    for(i=0;i<meio;i++)
        r[i]=' ';
    // escreve a palavra
    for(i=i;i<strlen(s)+meio;i++){
        r[i]=s[j++];
        if(i==tamanho)break;
    }
    // caso nao de ppra colocar tudo
    // adiciona (...)
    if(i==tamanho){
        r[tamanho]='\0';
        r[tamanho-1]='.';
        r[tamanho-2]='.';
        r[tamanho-3]='.';
        printf(r);
        return;
    }
    // acaba colocando mais espacoes
    for(i=i;i<tamanho;i++)
        r[i]=' ';
    // finalizando string
    r[tamanho]='\0';
    // escrever
    printf(r);
}

void repetir(char c, int tamanho){
    int i;
    for(i=0;i<tamanho;i++){
        printf("%c",c);
    }
}

int confirmacao(bool conf){
    system("cls");
    // caso queira perguntar
    if(conf){
        cursor(0);
        posicao(5,2);
        printf("Tem certeza que deseja fazer isso?");
        posicao(5,4);
        centro("S/N",34);
        // só para se for precionado s ou n
        while(1){
            switch(getch()){
            case 's':
                return 1;
                break;
            case 'S':
                return 1;
                break;
            case 'n':
                return 0;
                break;
            case 'N':
                return 0;
                break;

            }
        }
    } else return 1;
}

// teste
void corLetra(Cor cor){
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)){
        WORD wCor = (csbi.wAttributes & 0xF0)+(cor & 0x0F);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wCor);
    }
}

void CorFundo(Cor cor){
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)){
        WORD wCor = (csbi.wAttributes & 0x000F)+(cor << 4);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wCor);
    }
}

/* pegar numero inteiro do usuario
 * recebe a quantidade de caracter que o
 * numero tera e um numero limite
 */
int pegarInt(int qCaracter, int numLimite){
    char n[qCaracter+1];
    int i;
    for(i=0;i<=qCaracter;i++)
        n[i]='\0';
    i=0;
    while(1){
        char c=getch();
        if(c>=48 && c<=57){
            if(i<qCaracter){
                n[i++]=c;
                n[i]='\0';
                if(strParaInt(n)>numLimite){
                    i--;
                    n[i]='\0';
                } else printf("%c",c);

            }
        } else if(c==8){
            if(i>0){
                i--;
                n[i]='\0';
                printf("\b \b");
            }
        }else if(c==13){
            if(n[0]=='\0')
                printf("0\n");
            else printf("\n");
            return strParaInt(n);
        }
    }
}

// converter string em inteiro
int strParaInt(char s[]){
    int n=0;
    int tamanho = strlen(s);

    int i,j=1;
    for(i=tamanho-1;i>=0;i--){
        int aux = s[i]-48;
        aux = aux*j;
        j = j*10;
        n+=aux;
    }
    return n;
}

void addTurmaAuto(inicio* i, turma* novaTurma){
    if(i->prox==NULL){
        // se a lista de turmas tiver vazia
        i->prox=novaTurma;
    } else {
        // percorrer a lista ate chegar no ultimo elemento
        turma* turmaTemp = i->prox;
        while(turmaTemp->prox!=NULL){
            turmaTemp=turmaTemp->prox;
        }
        turmaTemp->prox = novaTurma;// adicionando no final da lista
    }
    i->quantidadeTurmas+=1;
    //menuTurma(novaTurma);// entrando na navegacao dos alunos da nova turma
}

void addAlunoAuto(turma* t, aluno* novoAluno){
    if(t->aluno==NULL){
        // caso a lista de alunos estiver vazia
        t->aluno=novoAluno;
    } else {
         aluno* alunoTemp = t->aluno;
         while(alunoTemp->prox!=NULL){
            alunoTemp=alunoTemp->prox;
         }
         alunoTemp->prox=novoAluno;
    }
    t->quantidadeAlunos+=1;
    // recalcular a media da turma toda vez
    // que for adicionado um aluno novo
    t->mediaTurma=recalcularMediaTurma(t);
}

void teste(inicio *i){
    system("cls");
    printf("Modo teste ativado\nColocando turmas e alunos teste\n");
    getch();
    system("cls");

    int t, a;
    char ct = 'A';
    for(t=0;t<3;t++){
        turma* nova = (turma*) malloc(sizeof(turma));
        sprintf(nova->nomeTurma,"Turma teste - %c",ct);
        nova->aluno=NULL;
        nova->mediaTurma=0;
        nova->prox=NULL;
        nova->quantidadeAlunos=0;
        nova->alunos_apro=0;
        nova->alunos_repr=0;
        srand(time(NULL));
        int q = (rand()%15)+6;
        for(a=0;a<q;a++){
            aluno* novo = (aluno*) malloc(sizeof(aluno));
            novo->prox=NULL;
            novo->mediaAluno=0;
            sprintf(novo->nomeAluno,"Aluno %d da turma %c",a+1,ct);
            int n;
            for(n=0;n<4;n++){
                novo->notas[n]=rand()%51+50;
                novo->mediaAluno+=novo->notas[n];
            }
            novo->mediaAluno=novo->mediaAluno/4;
            if(novo->mediaAluno>=70) {
                novo->situacaoAluno=Aprovado;
                nova->alunos_apro++;
            } else{
                novo->situacaoAluno=Reprovado;
                nova->alunos_repr++;
            }
            addAlunoAuto(nova,novo);
            printf("aluno %s adicionado na turma %s\n",novo->nomeAluno, nova->nomeTurma);
            Sleep(200);
        }
        addTurmaAuto(i,nova);
        printf("Turma %s adicionada a lista\n\n",nova->nomeTurma);
        ct++;
    }
    getch();
    system("cls");
    navTurmas(i);
}

bool codigo(char c){
    if(c==CODIGO[0]){
        int i;
        for(i=1;i<strlen(CODIGO);i++){
            if(getch()!=CODIGO[i]){
                break;
                return false;
            }
        }
        if(i==strlen(CODIGO))
            return true;
    }
    return false;
}

bool codigoJ(char c){
    if(c==CODIGOJ[0]){
        int i;
        for(i=1;i<strlen(CODIGOJ);i++){
            if(getch()!=CODIGOJ[i]){
                break;
                return false;
            }
        }
        if(i==strlen(CODIGOJ))
            return true;
    }
    return false;
}

void borda(int x, int y, int lar, int alt){
    int x_b=0, y_b=0;
    for(x_b=x;x_b<=lar;x_b++){
        for(y_b=y;y_b<=alt;y_b++){
            if((x_b==x || x_b==lar) || ((y_b==y || y_b==alt))){
                posicao(x_b,y_b);
                if(x_b==x&&y_b==y)
                    printf("%c",Baixo_direita);
                else if(y_b==y&&x_b==lar)
                    printf("%c",Baixo_esquerda);
                else if(y_b==alt&&x_b==x)
                    printf("%c",Cima_direita);
                else if(y_b==alt&&x_b==lar)
                    printf("%c",Cima_esquerda);
                else if(x_b==x||x_b==lar)
                    printf("%c",Vertical);
                else if(y_b==y||y_b==alt)
                    printf("%c",Horizontal);
                else printf("#");
            }
        }
    }
}

struct Parte{
    int x,y;
    char c;
    struct Parte *prox;
};
typedef struct Parte parte;

void addParte(parte* p);

void jogo(){
    system("cls");
    int xdir=2;
    int ydir=0;
    int tamanho = 3;
    int velocidade = 10;
    borda(1,1,39,20);
    parte* p = (parte*) malloc(sizeof(parte));
    parte* comida = (parte*) malloc(sizeof(parte));
    srand(time(NULL));
    comida->c='x';
    comida->prox=NULL;
    comida->x=rand()%18*2+2;
    comida->y=rand()%16+2;
    posicao(comida->x,comida->y);
    printf("%c",comida->c);

    p->prox=NULL;

    addParte(p);
    addParte(p);
    addParte(p);

    parte* temp1 = p;
    int x=22;
    while(temp1!=NULL){
        temp1->x=x;
        temp1->y=10;
        temp1->c='o';
        x+=2;
        temp1=temp1->prox;
    }
    int rodando = 1;
    while(rodando){
        parte* temp = p;
        parte auxP;
        auxP.x=temp->x;
        auxP.y=temp->y;
        //posicao(temp->x,temp->y);
        //printf(" ");

        temp->x+=xdir;
        temp->y+=ydir;

        if(temp->x<1) temp->x=38;
        if(temp->x>38) temp->x=2;
        if(temp->y<2) temp->y=19;
        if(temp->y>19) temp->y=2;

        posicao(temp->x,temp->y);
        printf("%c",temp->c);

        temp = temp->prox;
        // passando as posicoes para o proximo
        while(temp->prox!=NULL){
            parte auxP2;
            auxP2.x = temp->x;
            auxP2.y = temp->y;
            posicao(temp->x,temp->y);
            printf(" ");
            temp->x=auxP.x;
            temp->y=auxP.y;
            posicao(temp->x,temp->y);
            printf("%c",temp->c);
            auxP.x = auxP2.x;
            auxP.y = auxP2.y;
            temp=temp->prox;
        }

        if (p->x == comida->x && p->y == comida->y){
            addParte(p);
            comida->x=rand()%18*2+2;
            comida->y=rand()%16+2;
            tamanho++;
            velocidade+=4;
            if (velocidade > 50){
                velocidade = 50;
            }
        }
        posicao(comida->x,comida->y);
        printf("%c",comida->c);

        temp = p->prox;
        while(temp->prox!=NULL){
            if(p->x == temp->x && p->y == temp->y && tamanho > 4){
                rodando = 0;
            }
            temp = temp->prox;
        }


        Sleep(150-velocidade);
        if(kbhit()){
            switch(_getch()){
            case 77:
                if (xdir != -2){
                    xdir=2;
                    ydir=0;
                }
                break;
            case 75:
                if (xdir != 2){
                    xdir=-2;
                    ydir=0;
                }
                break;
            case 72:
                if (ydir != 1){
                    xdir=0;
                    ydir=-1;
                }
                break;
            case 80:
                if (ydir != -1){
                    xdir=0;
                    ydir=1;
                }
                break;
            default:
                break;
            }
        }
    }
}

void addParte(parte* p){
    // ciar parte
    parte* pt = (parte*) malloc(sizeof(parte));
    pt->x=0;
    pt->y=0;
    pt->c='o';
    pt->prox=p->prox;

    p->prox=pt;
}
