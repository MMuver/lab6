#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int** vydelit_matritsu(int n)
{
    int** m = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        m[i] = (int*)calloc(n, sizeof(int));
    return m;
}

void osvobodit_matritsu(int** m, int n)
{
    if (!m)
        return;
    for (int i = 0; i < n; i++)
        free(m[i]);
    free(m);
}

void vyvesti_matritsu(int** a, int n, const char* imya, const int* metki)
{
    printf("Матрица смежности %s (%dx%d):\n     ", imya, n, n);
    for (int j = 0; j < n; j++)
    {
        int v_num = (metki != NULL) ? metki[j] : (j + 1);
        printf("v%-3d", v_num);
    }
    printf("\n    +");
    for (int j = 0; j < n; j++)
        printf("----");
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        int r_num = (metki != NULL) ? metki[i] : (i + 1);
        printf("v%-2d | ", r_num);
        for (int j = 0; j < n; j++)
            printf("%-4d", a[i][j]);
        printf("\n");
    }
}

void vyvesti_spiski(int** a, int n, const char* imya, const int* metki)
{
    printf("Списки смежности для %s:\n", imya);
    for (int i = 0; i < n; i++)
    {
        int u_num = (metki != NULL) ? metki[i] : (i + 1);
        printf("v%d: ", u_num);
        int has = 0;
        for (int j = 0; j < n; j++)
        {
            if (a[i][j] == 1)
            {
                int v_num = (metki != NULL) ? metki[j] : (j + 1);
                printf("-> v%d ", v_num);
                has = 1;
            }
        }
        if (!has)
            printf("(нет смежных вершин)");
        printf("\n");
    }
}

void generirovat_graf(int** a, int n, int prob)
{
    int loop_prob = prob / 4;
    for (int i = 0; i < n; i++)
    {
        a[i][i] = (rand() % 100 < loop_prob) ? 1 : 0;
        for (int j = i + 1; j < n; j++)
        {
            int edge = (rand() % 100 < prob) ? 1 : 0;
            a[i][j] = edge;
            a[j][i] = edge;
        }
    }
}

void otozhdestvit_vershiny(int** a, int n, int u, int v)
{
    u--;
    v--;
    if (u > v)
    {
        int t = u;
        u = v;
        v = t;
    }

    int new_n = n - 1;
    int** res = vydelit_matritsu(new_n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (a[i][j])
            {
                int ni = (i == v) ? u : (i > v ? i - 1 : i);
                int nj = (j == v) ? u : (j > v ? j - 1 : j);
                res[ni][nj] = 1;
            }
        }
    }

    vyvesti_matritsu(res, new_n, "после отождествления", NULL);
    printf("\n");
    vyvesti_spiski(res, new_n, "после отождествления", NULL);
    osvobodit_matritsu(res, new_n);
}

void styanut_rebro(int** a, int n, int u, int v)
{
    u--;
    v--;
    if (a[u][v] == 0)
    {
        printf("Ребро между v%d и v%d отсутствует. Стягивание невозможно.\n", u + 1, v + 1);
        return;
    }

    if (u > v)
    {
        int t = u;
        u = v;
        v = t;
    }

    int new_n = n - 1;
    int** res = vydelit_matritsu(new_n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (a[i][j] && !((i == u && j == v) || (i == v && j == u)))
            {
                int ni = (i == v) ? u : (i > v ? i - 1 : i);
                int nj = (j == v) ? u : (j > v ? j - 1 : j);
                res[ni][nj] = 1;
            }
        }
    }

    vyvesti_matritsu(res, new_n, "после стягивания ребра", NULL);
    printf("\n");
    vyvesti_spiski(res, new_n, "после стягивания ребра", NULL);
    osvobodit_matritsu(res, new_n);
}

void rasshepit_vershinu(int** a, int n, int v)
{
    v--;
    int new_n = n + 1;
    int** res = vydelit_matritsu(new_n);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            res[i][j] = a[i][j];

    int new_v = new_n - 1;
    res[v][new_v] = 1;
    res[new_v][v] = 1;

    for (int j = 0; j < n; j++)
    {
        if (j != v && a[v][j])
        {
            res[new_v][j] = 1;
            res[j][new_v] = 1;
        }
    }
    if (a[v][v])
    {
        res[new_v][new_v] = 1;
    }

    vyvesti_matritsu(res, new_n, "после расщепления вершины", NULL);
    printf("\n");
    vyvesti_spiski(res, new_n, "после расщепления вершины", NULL);
    osvobodit_matritsu(res, new_n);
}

void udalit_vershinu(int** a, int n, int v)
{
    v--;
    int new_n = n - 1;
    int** res = vydelit_matritsu(new_n);

    for (int i = 0; i < n; i++)
    {
        if (i == v)
            continue;
        int ni = (i > v) ? i - 1 : i;
        for (int j = 0; j < n; j++)
        {
            if (j == v)
                continue;
            int nj = (j > v) ? j - 1 : j;
            res[ni][nj] = a[i][j];
        }
    }

    vyvesti_matritsu(res, new_n, "после удаления вершины", NULL);
    printf("\n");
    vyvesti_spiski(res, new_n, "после удаления вершины", NULL);
    osvobodit_matritsu(res, new_n);
}

void udalit_rebro(int** a, int n, int u, int v)
{
    u--;
    v--;
    if (a[u][v] == 0)
    {
        printf("Ребро между v%d и v%d отсутствует. Удаление невозможно.\n", u + 1, v + 1);
        return;
    }

    int** res = vydelit_matritsu(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            res[i][j] = a[i][j];

    res[u][v] = 0;
    res[v][u] = 0;

    vyvesti_matritsu(res, n, "после удаления ребра", NULL);
    printf("\n");
    vyvesti_spiski(res, n, "после удаления ребра", NULL);
    osvobodit_matritsu(res, n);
}

void obedinenie_grafov(int** a, int n1, int** b, int n2)
{
    int new_n = (n1 > n2) ? n1 : n2;
    int** res = vydelit_matritsu(new_n);

    for (int i = 0; i < new_n; i++)
    {
        for (int j = 0; j < new_n; j++)
        {
            int edge_a = (i < n1 && j < n1) ? a[i][j] : 0;
            int edge_b = (i < n2 && j < n2) ? b[i][j] : 0;
            res[i][j] = edge_a || edge_b;
        }
    }

    printf("\nОбъединение графов G1 U G2 (%dx%d):\n", new_n, new_n);
    vyvesti_matritsu(res, new_n, "G1 U G2", NULL);
    printf("\n");
    vyvesti_spiski(res, new_n, "G1 U G2", NULL);
    osvobodit_matritsu(res, new_n);
}

void peresechenie_grafov(int** a, int n1, int** b, int n2)
{
    int new_n = (n1 < n2) ? n1 : n2;
    int** res = vydelit_matritsu(new_n);

    for (int i = 0; i < new_n; i++)
    {
        for (int j = 0; j < new_n; j++)
        {
            res[i][j] = a[i][j] && b[i][j];
        }
    }

    printf("\nПересечение графов G1 и G2 (%dx%d):\n", new_n, new_n);
    vyvesti_matritsu(res, new_n, "G1 и G2", NULL);
    printf("\n");
    vyvesti_spiski(res, new_n, "G1 и G2", NULL);
    osvobodit_matritsu(res, new_n);
}

void koltsevaya_summa(int** a, int n1, int** b, int n2)
{
    int max_n = (n1 > n2) ? n1 : n2;
    int** x_mat = vydelit_matritsu(max_n);

    for (int i = 0; i < max_n; i++)
    {
        for (int j = 0; j < max_n; j++)
        {
            int edge_a = (i < n1 && j < n1) ? a[i][j] : 0;
            int edge_b = (i < n2 && j < n2) ? b[i][j] : 0;
            x_mat[i][j] = edge_a ^ edge_b;
        }
    }

    printf("\nКольцевая сумма G1 (+) G2 (матрица XOR):\n");
    vyvesti_matritsu(x_mat, max_n, "G1 (+) G2 (с изолированными вершинами)", NULL);

    int isolated = 0;
    int* deg = (int*)calloc(max_n, sizeof(int));
    for (int i = 0; i < max_n; i++)
    {
        for (int j = 0; j < max_n; j++)
        {
            if (x_mat[i][j] == 1)
                deg[i]++;
        }
        if (deg[i] == 0)
            isolated++;
    }

    if (isolated > 0 && isolated < max_n)
    {
        int new_n = max_n - isolated;
        int* labels = (int*)malloc(new_n * sizeof(int));
        int idx = 0;
        printf("\nИзолированные вершины, исключенные из кольцевой суммы: ");
        for (int i = 0; i < max_n; i++)
        {
            if (deg[i] == 0)
                printf("v%d ", i + 1);
            else
                labels[idx++] = i + 1;
        }
        printf("\n");

        int** res_ring = vydelit_matritsu(new_n);
        for (int i = 0; i < new_n; i++)
            for (int j = 0; j < new_n; j++)
                res_ring[i][j] = x_mat[labels[i] - 1][labels[j] - 1];

        printf("\nРезультирующий граф кольцевой суммы без изолированных вершин:\n");
        vyvesti_matritsu(res_ring, new_n, "G1 (+) G2", labels);
        printf("\n");
        vyvesti_spiski(res_ring, new_n, "G1 (+) G2", labels);

        osvobodit_matritsu(res_ring, new_n);
        free(labels);
    }
    else if (isolated == max_n)
    {
        printf("\nВсе вершины изолированы (графы идентичны). Кольцевая сумма пуста.\n");
    }
    else
    {
        printf("\nВ кольцевой сумме нет изолированных вершин. Граф сохраняет %d вершин:\n", max_n);
        vyvesti_spiski(x_mat, max_n, "G1 (+) G2", NULL);
    }

    free(deg);
    osvobodit_matritsu(x_mat, max_n);
}

void dekartovo_proizvedenie(int** a, int n1, int** b, int n2)
{
    int total_n = n1 * n2;
    printf("\nДекартово произведение графов G1 x G2 (%d вершин):\n", total_n);
    int** prod = vydelit_matritsu(total_n);

    for (int u1 = 0; u1 < n1; u1++)
    {
        for (int v1 = 0; v1 < n2; v1++)
        {
            int idx1 = u1 * n2 + v1;
            for (int u2 = 0; u2 < n1; u2++)
            {
                for (int v2 = 0; v2 < n2; v2++)
                {
                    int idx2 = u2 * n2 + v2;
                    if ((u1 == u2 && b[v1][v2]) || (v1 == v2 && a[u1][u2]))
                        prod[idx1][idx2] = 1;
                }
            }
        }
    }

    vyvesti_matritsu(prod, total_n, "G1 x G2", NULL);
    printf("\nСоответствие индексов парам вершин (u в G1, v в G2):\n");
    for (int u = 0; u < n1; u++)
        for (int v = 0; v < n2; v++)
            printf("Вершина %2d: (u%d, v%d)\n", u * n2 + v + 1, u + 1, v + 1);

    printf("\n");
    vyvesti_spiski(prod, total_n, "G1 x G2", NULL);
    osvobodit_matritsu(prod, total_n);
}

int main(void)
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    srand((unsigned int)time(NULL));

    int n1 = 0;
    int n2 = 0;
    int** g1 = NULL;
    int** g2 = NULL;

    while (1)
    {
        int op = 0;
        printf("\nМеню операций над графами:\n");
        printf("1 - Сгенерировать графы\n");
        printf("2 - Отождествление\n");
        printf("3 - Стягивание\n");
        printf("4 - Расщепление\n");
        printf("5 - Удаление вершины\n");
        printf("6 - Удаление ребра\n");
        printf("7 - Объединение\n");
        printf("8 - Пересечение\n");
        printf("9 - Кольцевая сумма\n");
        printf("10 - Декартово произведение\n");
        printf("0 - Выход из программы\n");
        printf("Ваш выбор: ");
        fflush(stdout);
        if (scanf("%d", &op) != 1 || op == 0)
            break;

        if (op == 1)
        {
            osvobodit_matritsu(g1, n1);
            osvobodit_matritsu(g2, n2);
            g1 = g2 = NULL;

            printf("Введите количество вершин для графа G1 (n1 >= 2): ");
            fflush(stdout);
            if (scanf("%d", &n1) != 1 || n1 < 2)
            {
                printf("Некорректный размер G1!\n");
                n1 = 0;
                continue;
            }

            printf("Введите количество вершин для графа G2 (n2 >= 2): ");
            fflush(stdout);
            if (scanf("%d", &n2) != 1 || n2 < 2)
            {
                printf("Некорректный размер G2!\n");
                n2 = 0;
                continue;
            }

            int prob;
            printf("Введите общую вероятность появления ребра в процентах (0-100): ");
            fflush(stdout);
            if (scanf("%d", &prob) != 1 || prob < 0 || prob > 100)
            {
                printf("Некорректная вероятность!\n");
                n1 = n2 = 0;
                continue;
            }

            g1 = vydelit_matritsu(n1);
            g2 = vydelit_matritsu(n2);
            generirovat_graf(g1, n1, prob);
            generirovat_graf(g2, n2, prob);

            printf("\nСгенерированные графы:\n");
            vyvesti_matritsu(g1, n1, "G1", NULL);
            printf("\n");
            vyvesti_matritsu(g2, n2, "G2", NULL);

            printf("\nСписки смежности:\n");
            vyvesti_spiski(g1, n1, "G1", NULL);
            printf("\n");
            vyvesti_spiski(g2, n2, "G2", NULL);
        }
        else if (op == 2)
        {
            if (g1 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            int u, v;
            printf("Введите номера двух вершин для отождествления (от 1 до %d): ", n1);
            fflush(stdout);
            if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n1 || v < 1 || v > n1 || u == v)
            {
                printf("Некорректные номера вершин!\n");
                continue;
            }
            otozhdestvit_vershiny(g1, n1, u, v);
        }
        else if (op == 3)
        {
            if (g1 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            int u, v;
            printf("Введите номера вершин ребра для стягивания (от 1 до %d): ", n1);
            fflush(stdout);
            if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n1 || v < 1 || v > n1 || u == v)
            {
                printf("Некорректные номера вершин!\n");
                continue;
            }
            styanut_rebro(g1, n1, u, v);
        }
        else if (op == 4)
        {
            if (g1 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            int v;
            printf("Введите номер вершины для расщепления (от 1 до %d): ", n1);
            fflush(stdout);
            if (scanf("%d", &v) != 1 || v < 1 || v > n1)
            {
                printf("Некорректный номер вершины!\n");
                continue;
            }
            rasshepit_vershinu(g1, n1, v);
        }
        else if (op == 5)
        {
            if (g1 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            int v;
            printf("Введите номер вершины для удаления (от 1 до %d): ", n1);
            fflush(stdout);
            if (scanf("%d", &v) != 1 || v < 1 || v > n1)
            {
                printf("Некорректный номер вершины!\n");
                continue;
            }
            udalit_vershinu(g1, n1, v);
        }
        else if (op == 6)
        {
            if (g1 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            int u, v;
            printf("Введите номера вершин ребра для удаления (от 1 до %d): ", n1);
            fflush(stdout);
            if (scanf("%d %d", &u, &v) != 2 || u < 1 || u > n1 || v < 1 || v > n1 || u == v)
            {
                printf("Некорректные номера вершин!\n");
                continue;
            }
            udalit_rebro(g1, n1, u, v);
        }
        else if (op == 7)
        {
            if (g1 == NULL || g2 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            obedinenie_grafov(g1, n1, g2, n2);
        }
        else if (op == 8)
        {
            if (g1 == NULL || g2 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            peresechenie_grafov(g1, n1, g2, n2);
        }
        else if (op == 9)
        {
            if (g1 == NULL || g2 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            koltsevaya_summa(g1, n1, g2, n2);
        }
        else if (op == 10)
        {
            if (g1 == NULL || g2 == NULL)
            {
                printf("Сначала сгенерируйте графы (пункт 1)!\n");
                continue;
            }
            dekartovo_proizvedenie(g1, n1, g2, n2);
        }
        else
        {
            printf("Неверный пункт меню!\n");
        }
    }

    osvobodit_matritsu(g1, n1);
    osvobodit_matritsu(g2, n2);

    return 0;
}
