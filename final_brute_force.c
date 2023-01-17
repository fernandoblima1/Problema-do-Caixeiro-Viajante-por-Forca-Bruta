#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define INT_MAX 100000

int N; // quantidade de nós no grafo
int *best_route;
int **graph;

int min_distance = INT_MAX;
int vertex, edges, weight;
int x = 0;

void generateRandomGraph()
{
    int i, j;
    srand(time(NULL));

    graph = (int **)malloc(N * sizeof(int *));

    for (i = 0; i < N; i++)
        graph[i] = (int *)malloc(N * sizeof(int));

    vertex = N;
    edges = (vertex * (vertex - 1)) / 2;

    for (i = 0; i < vertex; i++)
    {
        for (j = 0; j < vertex; j++)
        {
            graph[i][j] = INT_MAX;
            if (i == j)
            {
                graph[i][j] = 0;
                continue;
            }
            weight = rand() % 10 + 1;
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
    }
}

void buildGraph()
{
    FILE *arq;
    int i, j, peso = 0, max = 0;
    arq = fopen("input.txt", "r");
    if (arq == NULL)
    {
        printf("Nao foi possivel abrir o arquivo\n");
        exit(0);
    }

    while (fscanf(arq, "%d %d %d", &i, &j, &peso) != EOF)
    {
        if (max < i)
        {
            max = i;
        }
        if (max < j)
        {
            max = j;
        }
    }

    N = max + 1;

    // criação dinamica da matriz graph
    graph = (int **)malloc(N * sizeof(int *));

    for (i = 0; i < N; i++)
        graph[i] = (int *)malloc(N * sizeof(int));

    fclose(arq);

    arq = fopen("input.txt", "r");

    while (fscanf(arq, "%d %d %d", &i, &j, &peso) != EOF)
    {
        graph[i][j] = peso;
        graph[j][i] = peso;
    }
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (graph[i][j] == 0 && i != j)
            {
                graph[i][j] = INT_MAX;
            }
            if (i == j)
            {
                graph[i][j] = 0;
            }
        }
    }
    fclose(arq);
}

void showGraph()
{
    int i, j;
    printf("Grafo inserido = [\n");
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
    printf("]\n");
}

// função para calcular a distância total de uma rota
int calc_distance(int route[N])
{
    int distance = 0;
    int i;

    for (i = 0; i < N - 1; i++)
    {
        distance += graph[route[i]][route[i + 1]]; // Adiciona os pesos das arestas da rota gerada
    }
    distance += graph[route[N - 1]][route[0]]; // adicionando a volta para o nó inicial para formar o peso do ciclo
    return distance;
}

// função para gerar as permutações
void generate_permutations(int route[], int index)
{
    int i;
    // Condição de parada
    if (index == N)
    {
        int distance = calc_distance(route);
        if (distance < min_distance)
        {
            min_distance = distance;
            for (i = 0; i < N; i++)
            {
                best_route[i] = route[i];
            }
        }
        x++;
        return;
    }
    // Calcula as combinações das rotas para testar caso a caso
    for (i = index; i < N; i++)
    {
        int temp = route[index];
        route[index] = route[i];
        route[i] = temp;
        // Chama a função recursivamente para cálcular as permutações
        generate_permutations(route, index + 1);
        temp = route[index];
        route[index] = route[i];
        route[i] = temp;
    }
}

// O(n!)

int main()
{
    int choice;
    clock_t begin, end;
    printf("========= MENU =========");
    printf("\n");
    printf("1 - GRAFO ALEATORIO\n");
    printf("2 - INSERIR GRAFO PRONTO");
    printf("\n");
    printf("========================");
    scanf("%d", &choice);

    if (choice < 1 || choice > 2)
    {
        printf("Opção inválida");
    }
    else if (choice == 1)
    {
        printf("INSIRA O NUMERO DE VERTICES: ");
        scanf("%d", &N);
        while (N <= 0)
        {
            printf("\nEntrada inválida, insira novamente: ");
                scanf("%d", &N);
        }
        
        generateRandomGraph();
    }
    else
    {
        buildGraph();
    }
    showGraph();
    best_route = (int *)malloc(N * sizeof(int));

    int i;
    int route[N]; // 1 2 8 0 ... 1
    // Cáculo das posições para permuta
    for (i = 0; i < N; i++)
    {
        // 0 1 2 3 4 5 6
        route[i] = i;
    }
    begin = clock();
    generate_permutations(route, 1);
    end = clock();
    // Print da melhor rota
    printf("Melhor rota: [ ");

    for (i = 0; i < N; i++)
    {
        printf("%d", best_route[i]);
        printf(", ");
    }
    printf("0 ]\n");
    printf("Programa executado em %.lf ms\n",((double)end-begin)/((CLOCKS_PER_SEC/1000)));
    printf("Distancia: %d\n", min_distance);
    free(graph);
    free(best_route);
    return 0;
}