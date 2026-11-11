#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

void clearInput() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

int** createMatrix(int n) {
    int** A = (int**)malloc(n * sizeof(int*));

    if (A == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        A[i] = (int*)calloc(n, sizeof(int));

        if (A[i] == NULL) {
            for (int j = 0; j < i; j++) free(A[j]);
            free(A);
            return NULL;
        }
    }

    return A;
}

void freeMatrix(int** A, int n) {
    if (A == NULL) return;

    for (int i = 0; i < n; i++) free(A[i]);

    free(A);
}

void printMatrix(int** A, int n) {
    printf("    ");
    for (int j = 0; j < n; j++) printf("x%-3d", j + 1);
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("x%-2d ", i + 1);

        for (int j = 0; j < n; j++) printf("%-4d", A[i][j]);
        printf("\n");
    }
}

void randomFill(int** A, int n, double p) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            double r = (double)rand() / (double)RAND_MAX;

            if (r < p) {
                A[i][j] = 1;
                A[j][i] = 1;
            }
        }
    }
}

Node** createAdjacencyList(int** A, int n) {
    Node** list = (Node**)calloc(n, sizeof(Node*));

    if (list == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        Node* last = NULL;

        for (int j = 0; j < n; j++) {
            if (A[i][j] == 1) {
                Node* newNode = (Node*)malloc(sizeof(Node));

                if (newNode == NULL) {
                    for (int k = 0; k < n; k++) {
                        Node* current = list[k];

                        while (current != NULL) {
                            Node* next = current->next;
                            free(current);
                            current = next;
                        }
                    }
                    free(list);
                    return NULL;
                }

                newNode->vertex = j;
                newNode->next = NULL;

                if (list[i] == NULL) list[i] = newNode;
                else last->next = newNode;

                last = newNode;
            }
        }
    }

    return list;
}

void freeAdjacencyList(Node** list, int n) {
    if (list == NULL) return;

    for (int i = 0; i < n; i++) {
        Node* current = list[i];

        while (current != NULL) {
            Node* next = current->next;
            free(current);
            current = next;
        }
    }

    free(list);
}

void printAdjacencyList(Node** list, int n) {
    for (int i = 0; i < n; i++) {
        printf("x%d:", i + 1);

        Node* current = list[i];

        if (current == NULL) printf(" нет смежных вершин");

        while (current != NULL) {
            printf(" x%d", current->vertex + 1);
            current = current->next;
        }

        printf("\n");
    }
}

void dfsMatrix(int** A, int n, int v, int* visited) {
    visited[v] = 1;
    printf("x%d ", v + 1);

    for (int i = 0; i < n; i++) {
        if (A[v][i] == 1 && visited[i] == 0) {
            dfsMatrix(A, n, i, visited);
        }
    }
}

void dfsListRec(Node** list, int v, int* visited) {
    visited[v] = 1;
    printf("x%d ", v + 1);

    Node* current = list[v];

    while (current != NULL) {
        if (visited[current->vertex] == 0) {
            dfsListRec(list, current->vertex, visited);
        }

        current = current->next;
    }
}

void dfsList(Node** list, int n, int start) {
    int* visited = (int*)calloc(n, sizeof(int));

    if (visited == NULL) {
        printf("Не удалось выделить память.\n");
        return;
    }

    printf("Порядок посещения: ");

    clock_t startTime = clock();

    dfsListRec(list, start, visited);

    for (int i = 0; i < n; i++) {
        if (visited[i] == 0) {
            dfsListRec(list, i, visited);
        }
    }

    clock_t endTime = clock();

    double time = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("\nВремя выполнения: %.6f секунд\n", time);

    free(visited);
}


void dfsIterative(int** A, int n, int start) {
    int* visited = (int*)calloc(n, sizeof(int));
    int* stackVertex = (int*)malloc(n * sizeof(int));
    int* stackNext = (int*)malloc(n * sizeof(int));

    if (visited == NULL || stackVertex == NULL || stackNext == NULL) {
        printf("Не удалось выделить память.\n");
        free(visited);
        free(stackVertex);
        free(stackNext);
        return;
    }

    printf("Порядок посещения: ");

    clock_t startTime = clock();

    for (int k = -1; k < n; k++) {
        int currentStart = (k == -1) ? start : k;

        if (visited[currentStart]) continue;

        int top = 0;
        visited[currentStart] = 1;
        printf("%d ", currentStart + 1);

        stackVertex[top] = currentStart;
        stackNext[top] = 0;

        while (top >= 0) {
            int v = stackVertex[top];
            int j = stackNext[top];

            while (j < n && (A[v][j] == 0 || visited[j])) {
                j++;
            }

            if (j < n) {
                stackNext[top] = j + 1;
                visited[j] = 1;
                printf("%d ", j + 1);

                top++;
                stackVertex[top] = j;
                stackNext[top] = 0;
            }
            else {
                top--;
            }
        }
    }

    clock_t endTime = clock();

    double time = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("\nВремя выполнения: %.6f секунд\n", time);

    free(visited);
    free(stackVertex);
    free(stackNext);
}
void printGraphInfo(int** A, int n, Node** list) {
    printf("Матрица смежности графа G:\n");
    printMatrix(A, n);

    printf("\nСписки смежности графа G:\n");
    printAdjacencyList(list, n);
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(NULL));

    int** A = NULL;
    int n = 0;
    int running = 1;

    while (running) {
        int choice;

        printf("\nГлавное меню:\n");
        printf("1 - Создать граф и вывести матрицу смежности\n");
        printf("2 - Рекурсивный обход в глубину по матрице\n");
        printf("3 - Рекурсивный обход в глубину по спискам смежности (*)\n");
        printf("4 - Нерекурсивный обход в глубину по матрице (*)\n");
        printf("0 - Выход\n");
        printf("Ваш выбор: ");

        if (scanf_s("%d", &choice) != 1) {
            printf("Ошибка ввода.\n");
            clearInput();
            continue;
        }

        switch (choice) {

        case 1: {
            int newN;
            double p;

            printf("Введите количество вершин: ");
            if (scanf_s("%d", &newN) != 1) {
                printf("Ошибка ввода.\n");
                clearInput();
                break;
            }

            printf("Введите вероятность появления ребра (от 0 до 1): ");
            if (scanf_s("%lf", &p) != 1) {
                printf("Ошибка ввода.\n");
                clearInput();
                break;
            }

            if (newN <= 0 || p < 0 || p > 1) {
                printf("Неверные данные.\n");
                break;
            }

            int** B = createMatrix(newN);

            if (B == NULL) {
                printf("Не удалось выделить память.\n");
                break;
            }

            randomFill(B, newN, p);

            freeMatrix(A, n);
            A = B;
            n = newN;

            printf("\nМатрица смежности графа G:\n");
            printMatrix(A, n);

            break;
        }

        case 2:
        case 3:
        case 4: {
            system("cls");

            if (A == NULL) {
                printf("Сначала создайте граф с помощью пункта 1.\n");
                break;
            }

            Node** list = createAdjacencyList(A, n);

            if (list == NULL) {
                printf("Не удалось создать список смежности.\n");
                break;
            }

            printGraphInfo(A, n, list);

            int start;

            printf("\nВведите начальную вершину (1-%d): ", n);

            if (scanf_s("%d", &start) != 1) {
                printf("Ошибка ввода.\n");
                clearInput();
                freeAdjacencyList(list, n);
                break;
            }

            if (start < 1 || start > n) {
                printf("Неверный номер вершины.\n");
                freeAdjacencyList(list, n);
                break;
            }

            if (choice == 2) {
                int* visited = (int*)calloc(n, sizeof(int));

                if (visited == NULL) {
                    printf("Не удалось выделить память.\n");
                }
                else {
                    printf("\nРекурсивный обход в глубину по матрице:\n");
                    printf("Порядок посещения: ");

                    clock_t startTime = clock();

                    dfsMatrix(A, n, start - 1, visited);

                    for (int i = 0; i < n; i++) {
                        if (visited[i] == 0) {
                            dfsMatrix(A, n, i, visited);
                        }
                    }

                    clock_t endTime = clock();

                    double time = (double)(endTime - startTime) / CLOCKS_PER_SEC;

                    printf("\nВремя выполнения: %.6f секунд\n", time);

                    free(visited);
                }
            }
            else if (choice == 3) {
                printf("\nРекурсивный обход в глубину по спискам смежности:\n");
                dfsList(list, n, start - 1);
            }
            else if (choice == 4) {
                printf("\nНерекурсивный обход в глубину по матрице:\n");
                dfsIterative(A, n, start - 1);
            }

            freeAdjacencyList(list, n);


            break;
        }

        case 0:
            running = 0;
            break;

        default:
            printf("Неверный пункт меню.\n");
            break;
        }

        if (running) {
            printf("\nНажмите Enter для возврата в меню...");
            while (getchar() != '\n');
            getchar();
            system("cls");
        }
    }

    freeMatrix(A, n);

    printf("Программа завершена.\n");

    return 0;
}


