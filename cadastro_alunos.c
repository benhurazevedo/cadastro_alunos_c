#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct aluno
{
    char nome[80];
    float nota;
};

struct classe
{
    struct classe* aluno_anterior;
    struct classe* aluno_posterior;
    struct aluno aluno;
};

char opcao[2];
struct classe *alunos;
int alunos_vazio = 1;

void inicializar_classe(void);
void mostrar_opcoes(void);
void preencher_dados_aluno(void);
void adicionar_aluno_classe(void);
void ir_para_ultimo_aluno(void);
void ir_para_primeiro_aluno(void);
void mostrar_alunos(void);
void apagar_alunos(void);
void gravar_alunos(void);
void carregar_alunos(void);

int main()
{
    opcao[0] = '0';
    inicializar_classe();
    carregar_alunos();
    do /*enquanto nao escolher sair*/
    {
        mostrar_opcoes(); /*apresentar opcoes do programa*/
        switch(opcao[0]) /*escolha*/
        {
            case '1':
                ir_para_ultimo_aluno();
                adicionar_aluno_classe();
                preencher_dados_aluno();/*cadastrar aluno*/
                if(alunos_vazio)
                    alunos_vazio = 0;
                break;
            case '2':
                ir_para_primeiro_aluno();
                system("clear");
                if(alunos_vazio)
                    printf("Não há alunos cadastrados.");
                else
                    mostrar_alunos();/*exibir aluno*/
        } 
    }while (opcao[0] != '3');
    gravar_alunos();
    apagar_alunos();
    return 0;
}

void inicializar_classe(void)
{
    if((alunos = malloc(sizeof(struct classe))))
    {
        alunos->aluno_anterior = NULL;
        alunos->aluno_posterior = NULL;
    }
}

void mostrar_opcoes(void) /*exibir as 3 opcoes e aguardar a entrada do usuario*/
{
    system("clear");
    printf("=================================\n");
    printf("        Escolha a opcao\n");
    printf("=================================\n");
    printf("1 - cadastrar aluno;\n");
    printf("2 - exibir alunos;\n");
    printf("3 - sair do programa;\n");
    scanf("%s", opcao);
}

void preencher_dados_aluno(void)
{
    system("clear");
    printf("digite o nome:\n");
    scanf("%s", alunos->aluno.nome);
    printf("digite a nota:\n");
    scanf("%f", &alunos->aluno.nota);
}

void adicionar_aluno_classe(void)
{
    if(!alunos_vazio)
    {
        if(NULL != (alunos->aluno_posterior = malloc(sizeof(struct classe))))
        {
            alunos->aluno_posterior->aluno_anterior = alunos;
            alunos->aluno_posterior->aluno_posterior = NULL;
            alunos = alunos->aluno_posterior;
        }
    }
}

void ir_para_ultimo_aluno(void)
{
    if(alunos->aluno_posterior != NULL)
    {
        alunos = alunos->aluno_posterior;
        ir_para_ultimo_aluno();
    }
}

void ir_para_primeiro_aluno(void)
{
    if(alunos->aluno_anterior != NULL)
    {
        alunos = alunos->aluno_anterior;
        ir_para_primeiro_aluno();
    }
}

void mostrar_alunos(void)
{
    char continuar[2] = "";
    int numero_aluno = 0;
    do 
    {
        if(numero_aluno >0)
            alunos = alunos->aluno_posterior;
        if(strlen(alunos->aluno.nome) > 0)
        {
            printf("aluno %i\n", ++numero_aluno);
            printf("Nome: %s\n", alunos->aluno.nome);
            printf("Nota: %.2f\n\n", alunos->aluno.nota);
        }
    }while(alunos->aluno_posterior != NULL);
    scanf("%s", continuar);
}

void apagar_alunos(void)
{
    if(alunos_vazio)
        return;
    ir_para_ultimo_aluno();
    do
    {
        if(alunos->aluno_anterior != NULL)
        {
            alunos = alunos->aluno_anterior;
            free(alunos->aluno_posterior);
            alunos->aluno_posterior = NULL;
        }
        else 
        {
            free(alunos);
            alunos = NULL;
            return;
        }
    } while (1);
}

void gravar_alunos(void)
{
    FILE *arq;

    if(alunos_vazio)
        return;
    
    remove("data.dat");

    system("touch data.dat");

    arq = fopen("data.dat", "wb");

    if (arq == NULL)
    {
        printf("Problemas na criação do arquivo.");
        exit(0);
    }

    ir_para_primeiro_aluno();
    
    while(1)
    {
        if(strlen(alunos->aluno.nome) != 0)
        {
            fwrite((void*)(&alunos->aluno), sizeof(struct aluno), 1, arq);
            fflush(arq);
        }
        
        if(alunos->aluno_posterior == NULL)
        {
            fclose(arq);
            return;
        }
        else 
            alunos = alunos->aluno_posterior;
    }
}

void carregar_alunos(void)
{
    FILE *arq;

    arq = fopen("data.dat", "rb");

    if (arq == NULL)
        return;
    
    while(!feof(arq))
    {
        if(alunos_vazio)
            alunos_vazio = 0;
        else 
        {
            alunos->aluno_posterior = malloc(sizeof(struct classe));
            if(alunos->aluno_posterior != NULL)
            {
                alunos->aluno_posterior->aluno_anterior = alunos;
                alunos->aluno_posterior->aluno_posterior = NULL;
                alunos = alunos->aluno_posterior;
            }
            else 
            {
                printf("falha de alocacao de memoria!");
                exit(0);
            }
        }
        fread((void*)(&alunos->aluno), sizeof(struct aluno), 1, arq);
    }
    fclose(arq);
}