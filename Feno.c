#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct fardo {
    char cultivar[20];
    char tipoDeFeno; // A, B ou C
    int diametro;
} Fardo;

typedef struct producao {
    int codigo;
    Data dataProducao;
    Fardo tipoDeFardo;
    int qtDeFardos;
    float duracao;
    struct producao *prox;
} Producao;

Producao* criarProducao(int codigo, Data dataProducao, Fardo tipoDeFardo, int qtDeFardos, float duracao, Producao *head) {
    // Código para verificar se o código já existe na lista
    Producao *verificador = head;
    while (verificador != NULL) {
        if (verificador->codigo == codigo) {
            printf("Código %d já existe. Por favor, escolha outro código.\n", codigo);
            return NULL;  // Retorna NULL para indicar que o código já existe
        }
        verificador = verificador->prox;
    }

    Producao *novaProducao = (Producao *)malloc(sizeof(Producao));

    if (novaProducao == NULL) {
        fprintf(stderr, "Erro ao alocar memória para nova produção.\n");
        exit(EXIT_FAILURE);
    }

    novaProducao->codigo = codigo;
    novaProducao->dataProducao = dataProducao;
    novaProducao->tipoDeFardo = tipoDeFardo;
    novaProducao->qtDeFardos = qtDeFardos;
    novaProducao->duracao = duracao;
    novaProducao->prox = NULL;

    return novaProducao;
}

void incluirProducao(Producao **head, Data dataProducao, Fardo tipoDeFardo, int qtDeFardos, float duracao) {
    int codigo;
    
    do {
        printf("Digite o codigo: ");
        scanf("%d", &codigo);

        // Verifica se o código já existe na lista
        Producao *verificador = *head;
        while (verificador != NULL) {
            if (verificador->codigo == codigo) {
                printf("Código %d já existe. Por favor, escolha outro código.\n", codigo);
                codigo = -1;  // Seta para -1 para indicar que o código já existe
                break;
            }
            verificador = verificador->prox;
        }
    } while (codigo == -1);  // Repete enquanto o código já existir

    Producao *novoNo = criarProducao(codigo, dataProducao, tipoDeFardo, qtDeFardos, duracao, *head);
    
    if (novoNo == NULL) {
        // Código já existe, não incluir na lista
        return;
    }

    Producao *atual = *head;

    if (*head == NULL || codigo < (*head)->codigo) {
        novoNo->prox = *head;
        *head = novoNo;
    } else {
        while (atual->prox != NULL && atual->prox->codigo < codigo) {
            atual = atual->prox;
        }
        novoNo->prox = atual->prox;
        atual->prox = novoNo;
    }
}

void consultarPorCultivar(Producao *head, const char *cultivar) {
    Producao *aux = head;
    int fardosEncontrados = 0;
    int fardosA = 0, fardosB = 0, fardosC = 0;

    while (aux != NULL) {
        if (strcmp(aux->tipoDeFardo.cultivar, cultivar) == 0) {
            if (aux->tipoDeFardo.tipoDeFeno == 'A') {
                fardosA += aux->qtDeFardos;
            } else if (aux->tipoDeFardo.tipoDeFeno == 'B') {
                fardosB += aux->qtDeFardos;
            } else if (aux->tipoDeFardo.tipoDeFeno == 'C') {
                fardosC += aux->qtDeFardos;
            }
            fardosEncontrados++;
        }
        aux = aux->prox;
    }

    if (fardosEncontrados == 0) {
        printf("Nenhum registro encontrado para o cultivar especificado.\n");
    } else {
        printf("%s: A - %d\n", cultivar, fardosA);
        printf("%s: B - %d\n", cultivar, fardosB);
        printf("%s: C - %d\n", cultivar, fardosC);
    }
}

void consultarPorData(Producao *head, Data data) {
    Producao *aux = head;
    int fardosEncontrados = 0;

    while (aux != NULL) {
        if (aux->dataProducao.dia == data.dia &&
            aux->dataProducao.mes == data.mes &&
            aux->dataProducao.ano == data.ano) {

            printf("%d/%d/%d: %s - %c - %d\n",
                   aux->dataProducao.dia, aux->dataProducao.mes, aux->dataProducao.ano,
                   aux->tipoDeFardo.cultivar, aux->tipoDeFardo.tipoDeFeno, aux->qtDeFardos);

            fardosEncontrados++;
        }
        aux = aux->prox;
    }

    if (fardosEncontrados == 0) {
        printf("Nenhum registro encontrado para a data especificada.\n");
    }
}

void listarTodos(Producao *inicio) {
    Producao *aux = inicio;

    if (aux == NULL) {
        printf("Nenhum registro de producao encontrado.\n");
        return;
    }

    while (aux != NULL) {
        printf("Código: %d, Data: %d/%d/%d, Cultivar: %s, Tipo de Feno: %c, Quantidade: %d, Duracao: %.2f\n",
               aux->codigo, aux->dataProducao.dia, aux->dataProducao.mes, aux->dataProducao.ano,
               aux->tipoDeFardo.cultivar, aux->tipoDeFardo.tipoDeFeno, aux->qtDeFardos, aux->duracao);
        aux = aux->prox;
    }
}

void alterarProducao(Producao *head, int codigo) {
    Producao *atual = head;

    while (atual != NULL && atual->codigo != codigo) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Código %d não encontrado.\n", codigo);
        return;
    }

    int opcao;
    printf("Escolha o que deseja alterar para o código %d:\n", codigo);
    printf("1. Data de Producao\n");
    printf("2. Cultivar\n");
    printf("3. Tipo de Feno\n");
    printf("4. Diametro\n");
    printf("5. Quantidade de Fardos\n");
    printf("6. Duracao\n");
    printf("Escolha a opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("Digite a nova data de producao (dia mes ano): ");
            scanf("%d %d %d", &atual->dataProducao.dia, &atual->dataProducao.mes, &atual->dataProducao.ano);
            break;
        case 2:
            printf("Digite o novo cultivar: ");
            scanf("%s", atual->tipoDeFardo.cultivar);
            break;
        case 3:
            printf("Digite o novo tipo de feno (A, B ou C): ");
            scanf(" %c", &atual->tipoDeFardo.tipoDeFeno);
            break;
        case 4:
            printf("Digite o novo diametro: ");
            scanf("%d", &atual->tipoDeFardo.diametro);
            break;
        case 5:
            printf("Digite a nova quantidade de fardos: ");
            scanf("%d", &atual->qtDeFardos);
            break;
        case 6:
            printf("Digite a nova duracao: ");
            scanf("%f", &atual->duracao);
            break;
        default:
            printf("Opcao invalida. Nenhum dado alterado.\n");
            break;
    }
}

void excluirProducao(Producao **head, int codigo) {
    Producao *atual = *head;
    Producao *anterior = NULL;

    // Encontrar o nó com o código correspondente
    while (atual != NULL && atual->codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf("Código %d não encontrado.\n", codigo);
        return;
    }

    // Remover o nó da lista
    if (anterior == NULL) {
        // Remover o primeiro nó
        *head = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    // Liberar memória do nó removido
    free(atual);

    printf("Producao com codigo %d excluida com sucesso.\n", codigo);
}

void liberarMemoria(Producao *head) {
    Producao *atual = head;
    while (atual != NULL) {
        Producao *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
}

void exibirMenu() {
    printf("\nMenu:\n");
    printf("1. Incluir Producao\n");
    printf("2. Alterar\n");
    printf("3. Consultar por Cultivar\n");
    printf("4. Listar Todos\n");
    printf("5. Excluir\n");
    printf("6. Consultar por Data\n");
    printf("7. Sair\n");
    printf("Escolha a opcao: ");
}

int main() {
    int opcao;
    Producao *producaoHead = NULL; //inicializa o ponteiro inicial

    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Incluir produção
                Data dataProducao;
                Fardo tipoDeFardo;
                int qtDeFardos;
                float duracao;

                printf("Digite a data de producao (dia mes ano): ");
                scanf("%d %d %d", &dataProducao.dia, &dataProducao.mes, &dataProducao.ano);

                printf("Digite o cultivar: ");
                scanf("%s", tipoDeFardo.cultivar);

                printf("Digite o tipo de feno (A, B ou C): ");
                scanf(" %c", &tipoDeFardo.tipoDeFeno);

                printf("Digite o diametro: ");
                scanf("%d", &tipoDeFardo.diametro);

                printf("Digite a quantidade de fardos: ");
                scanf("%d", &qtDeFardos);

                printf("Digite a duracao: ");
                scanf("%f", &duracao);

                incluirProducao(&producaoHead, dataProducao, tipoDeFardo, qtDeFardos, duracao);
                break;
            }
            case 2: {
                // Alterar produção
                int codigo;
                printf("Digite o codigo da producao que deseja alterar: ");
                scanf("%d", &codigo);
                alterarProducao(producaoHead, codigo);
                break;
            }
            case 3: {
                // Consultar por cultivar
                char cultivar[20];
                printf("Digite o cultivar para consulta: ");
                scanf("%s", cultivar);
                consultarPorCultivar(producaoHead, cultivar);
                break;
            }
            case 4:
                // Listar todas as produções
                listarTodos(producaoHead);
                break;
            case 5: {
                // Excluir produção
                int codigo;
                printf("Digite o codigo da producao que deseja excluir: ");
                scanf("%d", &codigo);
                excluirProducao(&producaoHead, codigo);
                break;
            }
            case 6: {
                // Consultar por data
                Data dataConsulta;
                printf("Digite a data para consulta (dia mes ano): ");
                scanf("%d %d %d", &dataConsulta.dia, &dataConsulta.mes, &dataConsulta.ano);
                consultarPorData(producaoHead, dataConsulta);
                break;
            }
            case 7:
                // Finalizar o programa
                liberarMemoria(producaoHead);
                printf("Programa finalizado.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 7);

    return 0;
}