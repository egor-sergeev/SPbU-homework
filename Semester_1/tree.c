#include <stdio.h>
#include <stdlib.h>

struct tree {
    int num;
    int localFreq;
    int totalFreq;
    struct tree *left;
    struct tree *right;
};

void quickSort(int *a, int arraySize, int *f);
void swapIntegers(int *a, int *b);

void clearTree(struct tree *t);

void genTreePicture(struct tree *tree, int isRoot, int totalNum, FILE *output);

/*
 * Основные свойства:
 * 1. a -- отсортированный массив, тогда если a[i] -- корень дерева, то элементы 1..(i-1) лежат в левом поддереве,
 * а (i+1)..n в правом.
 * 2. На оптимальность дерева с фиксированным корнем влияет только оптимальность левого и правого поддеревьев.
 * 3. Вес узла = сумма его частоты и веса поддеревьев (левого и правого).
 *
 * Описание алгоритма:
 * 1. p(x) -- количество запросов к элементам меньшим х.
 * 2. left, right -- левая и правая границы рассматриваемого дерева соответственно.
 * 3. Для оптимальности дерева вес левого и правого поддеревьев его корня должен минимально различаться.
 * 4. Вес поддерева, включающего в себя элементы l..r равен p(r)-p(l).
 * 5. Значит корнем поддерева станет элемент minRoot такой, что разность веса поддеревьев (l..(m-1) и (m+1)..r) будет
 * минимальной по модулю. Обозначим эту разность за diff(left, right, minRoot)=abs((p(m-1)-p(l)) - (p(r)-p(m+1))=
 * =abs((p(m-1)+p(m+1)) - (p(l)+p(r))).
 * 6. После выбора корня рекурсивно переходим на левое и правое поддеревья.
 */

struct tree *buildTree(int *a, int *f, int *p, int l, int r, int numTotal);

int main() {
    FILE *inputFile = fopen("/home/drundolet/homeworks-course1/tree.txt", "r");
    if (!inputFile) {
        printf("Can't open file to read\n");
        exit(1);
    }

    int numTotal, requestsTotal = 0;
    fscanf(inputFile, "%d", &numTotal);
    int a[numTotal];
    int f[numTotal];

    for (int i = 0; i < numTotal; i++) {
        fscanf(inputFile, "%d - %d", &a[i], &f[i]);
        requestsTotal += f[i];
    }
    //Sorting:
    quickSort(a, numTotal, f);

    int p[numTotal + 1];//p[numTotal] == requestsTotal
    p[0] = 0;
    for (int i = 1; i < numTotal + 1; i++) {
        p[i] = p[i - 1] + f[i - 1];
    }

    struct tree *finalTree = buildTree(a, f, p, 0, numTotal - 1, numTotal);

    FILE *outputFile = fopen("/home/drundolet/homeworks-course1/output.dot", "w");
    if (!outputFile) {
        printf("Can't open file to read\n");
        exit(1);
    }

    genTreePicture(finalTree, 1, numTotal, outputFile);
    fprintf(outputFile, "}");

    clearTree(finalTree);

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}

struct tree *buildTree(int *a, int *f, int *p, int l, int r, int numTotal) {
    if (l > r) return NULL;

    if (l == r) {
        struct tree *leaf = malloc(sizeof(struct tree));
        leaf->left = NULL;
        leaf->right = NULL;
        leaf->localFreq = leaf->totalFreq = f[r];
        leaf->num = a[r];
        return leaf;
    }


    int min = abs((p[l - 1] + p[l + 1]) - (p[r + 1] + p[l - 1]));
    int minPos = l;

    for (int i = l; i < r; i++) {
        if (abs((p[i - 1] + p[i + 1]) - (p[r + 1] + p[l - 1])) < min) {
            min = abs((p[i - 1] + p[i + 1]) - (p[r + 1] + p[l - 1]));
            minPos = i;
        }
    }

    struct tree *node = malloc(sizeof(struct tree));
    node->num = a[minPos];
    node->localFreq = f[minPos];
    node->totalFreq = p[r + 1] - p[l];
    node->left = buildTree(a, f, p, l, minPos - 1, numTotal);
    node->right = buildTree(a, f, p, minPos + 1, r, numTotal);
    return node;
}

void genTreePicture(struct tree *tree, int isRoot, int totalNum, FILE *output) {
    if (isRoot) {
        fprintf(output, "digraph {\n");
        isRoot = 0;
    }

    if (tree->left) {
        fprintf(output, "\"%d\\n%d\" -> \"%d\\n%d\"\n",
                tree->num, tree->localFreq, tree->left->num, tree->left->localFreq);
        genTreePicture(tree->left, isRoot, totalNum, output);
    }

    if (tree->right) {
        fprintf(output, "\"%d\\n%d\" -> \"%d\\n%d\"\n",
                tree->num, tree->localFreq, tree->right->num, tree->right->localFreq);
        genTreePicture(tree->right, isRoot, totalNum, output);
    }
}

void clearTree(struct tree *t){
    if (t->left) clearTree(t->left);
    if (t->right) clearTree((t->right));
    free(t);
}

void swapIntegers(int *a, int *b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
void quickSort(int *a, int arraySize, int *f) {
    if (arraySize < 2) return;

    int base = a[arraySize / 2];
    int i, j;
    for (i = 0, j = arraySize - 1;; i++, j--) {
        while (a[i] < base) i++;
        while (a[j] > base) j--;

        if (i >= j) break;

        swapIntegers(a + i, a + j);
        swapIntegers(f + i, f + j);
    }
    quickSort(a, i, f);
    quickSort(a + i, arraySize - i, f + i);
}