#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"


#define true 1
#define false 0


#pragma GCC diagnostic ignored "-Wunused-result"


typedef struct car {
    int32_t autonomy;
    int32_t quantity;
} car;

typedef struct car_heap {
    car *cars;
    int32_t size;
    int32_t capacity;
} car_heap;

void init_car_heap(car_heap *heap, int32_t capacity);

void insert_car(car_heap *heap, int32_t autonomy);


void max_heapify(car_heap *heap, int32_t index);


void free_car_heap(car_heap *heap);

void init_car_heap(car_heap *heap, int32_t capacity) {
    heap->cars = malloc(sizeof(car) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
}

int32_t search_car(car_heap *heap, int32_t autonomy) {//search in heap
    for (int32_t i = 0; i < heap->size; i++) {
        if (heap->cars[i].autonomy == autonomy) {
            return i;
        }

    }

    return -1;

}

void insert_car(car_heap *heap, int32_t autonomy) {
    if (heap->size == heap->capacity) {
        // Heap is full, perform resizing if necessary
        heap->capacity *= 2;
        heap->cars = realloc(heap->cars, sizeof(car) * heap->capacity);
    }

    // Check if a car with the same autonomy already exists
    int32_t index = search_car(heap, autonomy);
    if (index != -1) {
        // Car already exists, increment the quantity
        heap->cars[index].quantity += 1;
        return;
    }

    // Insert the car at the last position
    index = heap->size;
    heap->cars[index].autonomy = autonomy;
    heap->cars[index].quantity = 1;

    // Restore the max heap property
    while (index > 0 && heap->cars[index].autonomy > heap->cars[(index - 1) / 2].autonomy) {
        // Swap the car with its parent if it has a higher autonomy
        car temp = heap->cars[index];
        heap->cars[index] = heap->cars[(index - 1) / 2];
        heap->cars[(index - 1) / 2] = temp;

        // Move up to the parent index
        index = (index - 1) / 2;
    }

    // Increment the heap size
    heap->size++;
}

void insert_car_without_check(car_heap *heap, int32_t autonomy) {
    if (heap->size == heap->capacity) {
        // Heap is full, perform resizing if necessary
        heap->capacity *= 2;
        heap->cars = realloc(heap->cars, sizeof(car) * heap->capacity);
    }
    // Insert the car at the last position
    int32_t index = heap->size;
    index = heap->size;
    heap->cars[index].autonomy = autonomy;
    heap->cars[index].quantity = 1;

    // Restore the max heap property
    while (index > 0 && heap->cars[index].autonomy > heap->cars[(index - 1) / 2].autonomy) {
        // Swap the car with its parent if it has a higher autonomy
        car temp = heap->cars[index];
        heap->cars[index] = heap->cars[(index - 1) / 2];
        heap->cars[(index - 1) / 2] = temp;

        // Move up to the parent index
        index = (index - 1) / 2;
    }

    // Increment the heap size
    heap->size++;
}

void swap_cars(car *car1, car *car2) {
    car temp = *car1;
    *car1 = *car2;
    *car2 = temp;
}

void remove_car_helper(car_heap *heap, int32_t index) {
    // Swap the car with the last car in the array
    swap_cars(&heap->cars[index], &heap->cars[heap->size - 1]);

    // Decrement the heap size
    heap->size--;

    // Restore the max heap property
    max_heapify(heap, index);
}

int8_t remove_car(car_heap *heap, int32_t autonomy) {
    int32_t index = search_car(heap, autonomy);

    if (index == -1) {
        return false; // Car not found
    }
    // Check if the car has a quantity greater than 2
    if (heap->cars[index].quantity > 1) {
        heap->cars[index].quantity--;
        return true; // Car removed
    }
    remove_car_helper(heap, index);
    return true; // Car removed
}


void max_heapify(car_heap *heap, int32_t index) {
    int32_t left = 2 * index + 1;
    int32_t right = 2 * index + 2;
    int32_t largest = index;

    if (left < heap->size && heap->cars[left].autonomy > heap->cars[largest].autonomy) {
        largest = left;
    }

    if (right < heap->size && heap->cars[right].autonomy > heap->cars[largest].autonomy) {
        largest = right;
    }

    if (largest != index) {
        // Swap the car with the largest autonomy with the current car
        car temp = heap->cars[index];
        heap->cars[index] = heap->cars[largest];
        heap->cars[largest] = temp;

        // Recursively max-heapify the affected subtree
        max_heapify(heap, largest);
    }
}


void free_car_heap(car_heap *heap) {
    free(heap->cars);
}


void insertion_sort(int32_t *array, int start, int end, int decreasing);


typedef struct station_tree {
    struct station_node *root;
} station_tree;

typedef struct station_node {
    int32_t distance;
    struct car_heap *cars;
    struct station_node *left;
    struct station_node *right;
    struct station_node *parent;
} station_node;


int cmp_int(const void *a, const void *b) {

    return *(int *) a - *(int *) b;

//	const int l = *(const int *)a;
//	const int r = *(const int *)b;

//	return l - r;
//	return l > r;
//	return (l > r) - (l < r);
}


typedef struct dynamic_array dynamic_array;

typedef struct dynamic_array {
    int32_t *array;
    int32_t capacity;
} dynamic_array;
station_node *cache_station = NULL;


station_node *successor(station_node *node);


station_node *predecessor(station_node *node);

void insert(station_tree *tree, station_node *node);

void aDelete(station_tree *tree, station_node *node);


void transplant(station_tree *tree, station_node *u, station_node *v);

station_node *search(station_tree *tree, int32_t distance);

station_node *minimum(station_node *node);

station_node *maximum(station_node *node);


__asm__("\t.globl\tsuccessor\n"
        "\t.type\tsuccessor, @function\n"
        "successor:\n"
        ".LFB2a:\n"
        "\t.cfi_startproc\n"
        "\tmovq\t24(%rdi), %rcx\t# node_6(D)->right, node\n"
        "\ttestq\t%rcx, %rcx\t# node\n"
        "\tje\t.L282123sdea\t#,\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L23asd:\n"
        "\tmovq\t%rcx, %rax\t# node, <retval>\n"
        "\tmovq\t16(%rcx), %rcx\t# node_7->left, node\n"
        "\ttestq\t%rcx, %rcx\t# node\n"
        "\tjne\t.L23asd\t#,\n"
        "\tret\t\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L282123sdea:\n"
        "\tmovq\t32(%rdi), %rax\t# node_6(D)->parent, <retval>\n"
        "\ttestq\t%rax, %rax\t# <retval>\n"
        "\tjne\t.L15asdasd\t#,\n"
        "\tret\t\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L2967:\n"
        "\tmovq\t32(%rax), %rdx\t# parent_4->parent, parent\n"
        "\tmovq\t%rax, %rdi\t# <retval>, node\n"
        "\ttestq\t%rdx, %rdx\t# parent\n"
        "\tje\t.L28asdasdvd\t#,\n"
        "\tmovq\t%rdx, %rax\t# parent, <retval>\n"
        ".L15asdasd:\n"
        "\tcmpq\t24(%rax), %rdi\t# parent_4->right, node\n"
        "\tje\t.L2967\t#,\n"
        "\tret\t\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L28asdasdvd:\n"
        "\txorl\t%eax, %eax\t# <retval>\n"
        "\tret\t\n"
        "\t.cfi_endproc");

__asm__(".LFB2b:\n"
        "\t.size\tsuccessor, .-successor\n"
        "\t.p2align 4\n"
        "\t.globl\tpredecessor\n"
        "\t.type\tpredecessor, @function\n"
        "predecessor:\n"
        ".LFB2c:\n"
        "\t.cfi_startproc\n"
        "\tmovq\t16(%rdi), %rcx\t# node_6(D)->left, node\n"
        "\ttestq\t%rcx, %rcx\t# node\n"
        "\tje\t.L124sadw\t#,\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L1340asd:\n"
        "\tmovq\t%rcx, %rax\t# node, <retval>\n"
        "\tmovq\t24(%rcx), %rcx\t# node_7->right, node\n"
        "\ttestq\t%rcx, %rcx\t# node\n"
        "\tjne\t.L1340asd\t#,\n"
        "\tret\t\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L124sadw:\n"
        "\tmovq\t32(%rdi), %rax\t# node_6(D)->parent, <retval>\n"
        "\ttestq\t%rax, %rax\t# <retval>\n"
        "\tjne\t.L154lksadjhklih\t#,\n"
        "\tret\t\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L16432432sdf:\n"
        "\tmovq\t32(%rax), %rdx\t# parent_4->parent, parent\n"
        "\tmovq\t%rax, %rdi\t# <retval>, node\n"
        "\ttestq\t%rdx, %rdx\t# parent\n"
        "\tje\t.L1741234sdfsdf\t#,\n"
        "\tmovq\t%rdx, %rax\t# parent, <retval>\n"
        ".L154lksadjhklih:\n"
        "\tcmpq\t16(%rax), %rdi\t# parent_4->left, node\n"
        "\tje\t.L16432432sdf\t#,\n"
        "\tret\t\n"
        "\t.p2align 4\n"
        "\t.p2align 3\n"
        ".L1741234sdfsdf:\n"
        "\txorl\t%eax, %eax\t# <retval>\n"
        "\tret\t\n"
        "\t.cfi_endproc");

void insert(station_tree *tree, station_node *node) {
    station_node *y = NULL;
    station_node *x = tree->root;
    while (x != NULL) {
        y = x;
        if (node->distance < x->distance) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    node->parent = y;
    if (y == NULL) {
        tree->root = node;
    } else if (node->distance < y->distance) {
        y->left = node;
    } else {
        y->right = node;
    }
    cache_station = node;
}

void aDelete(station_tree *tree, station_node *node) {
    if (node->left == NULL) {
        transplant(tree, node, node->right);
    } else if (node->right == NULL) {
        transplant(tree, node, node->left);
    } else {
        station_node *y = minimum(node->right);
        if (y->parent != node) {
            transplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
    }

    free_car_heap(node->cars);
    free(node->cars);
    if (cache_station == node) {
        cache_station = NULL;
    }
    free(node);

}

void transplant(station_tree *tree, station_node *u, station_node *v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
    u->parent = NULL;
}

station_node *search(station_tree *tree, int32_t distance) {
    if (cache_station != NULL && cache_station->distance == distance) {
        return cache_station;
    }
    station_node *node = tree->root;
    while (node != NULL && distance != node->distance) {
        if (distance < node->distance) {
            node = node->left;
        } else {
            node = node->right;
        }
    }
    cache_station = node;
    return node;
}

station_node *minimum(station_node *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

station_node *maximum(station_node *node) {
    while (node->right != NULL) {
        node = node->right;
    }
    return node;
}

int32_t get(int32_t i, dynamic_array *array);

void set(int32_t i, int32_t value, dynamic_array *array);

void init_array(dynamic_array *array);

void free_array(dynamic_array *array);

void insertion_sort(int32_t *array, int start, int end, int decreasing);

//define max of station as max_car(station->cars->root)->autonomy
#define max(s) ((s->cars)->cars[0].autonomy)

void quicksort(int32_t *array, int start, int end, int decreasing);

int partition(int32_t *array, int start, int end, int decreasing);

int8_t first_parser(const char *cmd);

void swap(int32_t *a, int32_t *b);

void add_station(void);

void demolish_station(void);

void add_car(void);

void demolish_car(void);

void plan_path(void);

typedef struct station_reverse station_reverse;


void reverse(int32_t start, int32_t end);


station_tree *stations = NULL;
FILE *input;
FILE *output;


int32_t get(int32_t i, dynamic_array *array) {
    return (array->array)[i];
}

void set(int32_t i, int32_t value, dynamic_array *array) {
    if (array->capacity <= i) {
        array->capacity = array->capacity * 3;
        array->array = (int32_t *) realloc(array->array, array->capacity * sizeof(int32_t));
    }
    array->array[i] = value;
}

void init_array(dynamic_array *array) {
    array->capacity = 5;
    array->array = (int32_t *) malloc(array->capacity * sizeof(int32_t));
}

void free_array(dynamic_array *array) {
    free(array->array);
    free(array);
}


int read_space_end_line(char *line, FILE *inputt, int size) {
    char c;
    int i = 0;
    c = fgetc_unlocked(inputt);
    if (c == EOF) {
        return 0;
    }
    while (c != '\n' && c != '\r' && c != ' ' && i < size - 2) {
        line[i] = c;
        i++;
        c = fgetc_unlocked(inputt);
    }
    line[i] = '\0';
    return 1;
}


void go_to_next_line(FILE *file) {
    char c;
    while ((c = fgetc_unlocked(file)) != '\n' && c != EOF) {}
}


int main(void) {
//    input = fopen("open_111.txt", "r");
//    output = fopen("output2.txt", "w");
    input = stdin;
    output = stdout;
    char *line;
    stations = (station_tree *) malloc(sizeof(station_tree));
    stations->root = NULL;
    line = (char *) malloc(25 * sizeof(char));
    while (read_space_end_line(line, input, 25)) {
        if (line == NULL)
            break;
        int8_t cmd = first_parser(line);
        switch (cmd) {
            case 1:
                add_station();
                break;
            case 2:
                demolish_station();
                break;
            case 3:
                add_car();
                break;
            case 4:
                demolish_car();
                break;
            case 5:
                plan_path();
                break;
        }
    }
    free(line);
    return 0;
}


typedef struct station_reverse {
    station_node *station;
    struct station_reverse *smaller;
} station_reverse;

station_reverse *add_stationr(station_reverse *lstation, station_node *station) {
    station_reverse *new_station = (station_reverse *) malloc(sizeof(station_reverse));
    new_station->station = station;
    new_station->smaller = lstation;
    return new_station;
}


void free_stationr(station_reverse *lstation) {
    if (lstation == NULL)
        return;
    for (station_reverse *temp = lstation; temp != NULL;) {
        station_reverse *temp_station = temp->smaller;
        free(temp);
        temp = temp_station;
    }
}

station_reverse *get_first_pass(int32_t start, int32_t end) {
    int8_t terminate = 1;
    station_reverse *list = NULL;
    station_node *starts = search(stations, start);
    if (starts == NULL)
        return NULL;
    station_node *ends = search(stations, end);
    if (ends == NULL)
        return NULL;
    list = add_stationr(list, ends);
    station_node *temp = starts;
    while (terminate) {
        if (end == start) {
            terminate = 0;
        } else if ((temp->distance == start &&
                    start - max(temp) > predecessor(temp)->distance) ||
                   (temp->distance == end)) {
            free_stationr(list);
            terminate = 0;
            list = NULL;
        } else if (temp->distance - max(temp) <= end) {
            list = add_stationr(list, temp);
            end = temp->distance;
            temp = starts;
            continue;
        }
        temp = predecessor(temp);
    }
    return list;
}


int8_t c(station_node *current, station_reverse *best, station_reverse *currentNavigated, station_reverse *end) {
    if (best == end) {
        return 1;
    }
    int32_t current_d_a = current->distance - max(current);
    station_node *left = currentNavigated->station;
    if (current_d_a > left->distance) {
        if (current_d_a <= best->station->distance)
            return true;
        return false;
    }
    for (; (current_d_a <= left->distance); left = predecessor(left)) {
        if (c(left, best->smaller, currentNavigated->smaller, end))
            best->station = left;
        currentNavigated->station = left;
    }
    if (left == NULL)
        return true;
    return true;
}


void reverse(int32_t start, int32_t end) {
    station_node *temp2 = search(stations, end);
    station_node *start_station = search(stations, start);
    if (temp2 == NULL || start_station == NULL) {
        fputs("nessun percorso\n", output);
        return;
    }
    station_reverse *best = get_first_pass(start, end);
    if (best == NULL) {
        fprintf(output, "nessun percorso\n");
    } else {
        station_reverse *end_station;
        for (end_station = best; end_station->smaller != NULL; end_station = end_station->smaller);
        station_reverse *currentNavigated = NULL;
        currentNavigated = add_stationr(currentNavigated, NULL);
        station_reverse *t_c = currentNavigated;
        station_reverse *temp = best->smaller;
        while (temp != NULL) {
            t_c->smaller = (struct station_reverse *) malloc(sizeof(station_reverse));
            t_c = t_c->smaller;
            t_c->smaller = NULL;
            t_c->station = successor(temp->station);
            temp = temp->smaller;
        }
        c(best->station, best->smaller, currentNavigated->smaller, end_station);
        fprintf(output, "%d", best->station->distance);
        for (station_reverse *temp5 = best->smaller; temp5 != NULL; temp5 = temp5->smaller) {
            fprintf(output, " %d", temp5->station->distance);
        }
        fprintf(output, "\n");
        free_stationr(currentNavigated);
        free_stationr(best);
    }

}


void geti(int32_t *integer) {
    *integer = 0;
    char c = fgetc_unlocked(input);
    while (c < '0' || c > '9') {
        c = fgetc_unlocked(input);
    }
    while (c >= '0' && c <= '9') {
        *integer = *integer * 10 + c - '0';
        c = fgetc_unlocked(input);
    }
}


void plan_path(void) {
    int32_t start = 0;
    geti(&start);
    int32_t end = 0;
    geti(&end);
    int32_t distance = (int32_t) end - (int32_t) start;
    if (distance < 0) {
        reverse(start, end);
        return;
    }
    if (distance == 0) {
        fprintf(output, "%d\n", start);
        return;
    }
    int8_t terminate = 1;
    dynamic_array *result = (dynamic_array *) malloc(sizeof(dynamic_array));
    init_array(result);
    set(0, end, result);
    int i = 1;
    station_node *temp = search(stations, start);
    if (temp == NULL) {
        fputs("nessun percorso\n", output);
        free_array(result);
        return;
    }
    while (terminate) {
        if (end == start) {
            terminate = 0;
        } else if ((temp->distance == start && max(temp) + start < successor(temp)->distance) ||
                   (temp->distance == end)) {
            fputs("nessun percorso\n", output);
            free_array(result);
            return;
        } else if (max(temp) + temp->distance >= end) {
            set(i, temp->distance, result);
            i++;
            end = temp->distance;
            temp = search(stations, start);
            continue;
        }
        temp = successor(temp);
    }
    quicksort(result->array, 0, i - 1, 0);
    fprintf(output, "%d", get(0, result));
    for (int j = 1; j < i; j++) {
        fprintf(output, " %d", get(j, result));
    }
    fputs("\n", output);
    free_array(result);
}


void demolish_car(void) {

    int32_t distance = 0;
    geti(&distance);
    int32_t autonomy = 0;
    geti(&autonomy);
    station_node *station = search(stations, distance);
    if (station == NULL) {
        fputs("non rottamata\n", output);
        return;
    }
    if (remove_car(station->cars, autonomy) == true) {
        fputs("rottamata\n", output);
    } else {
        fputs("non rottamata\n", output);
    }

}

void add_car(void) {
    int32_t distance = 0;
    geti(&distance);
    int32_t autonomy = 0;
    geti(&autonomy);
    station_node *station = search(stations, distance);
    if (station == NULL) {
        fputs("non aggiunta\n", output);
        return;
    }
    insert_car(station->cars, autonomy);
    fputs("aggiunta\n", output);
}

void demolish_station(void) {
    int32_t distance = 0;
    geti(&distance);
    station_node *station = search(stations, distance);
    if (station == NULL) {
        fputs("non demolita\n", output);
        return;
    }
    aDelete(stations, station);
    fputs("demolita\n", output);
}
//pragma ignore -Wincompatible-pointer-types gcc

void add_station(void) {
    int32_t distance = 0;
    geti(&distance);
    int32_t num_cars = 0;
    geti(&num_cars);
    station_node *new_station = search(stations, distance);
    if (new_station != NULL) {
        fputs("non aggiunta\n", output);
        go_to_next_line(input);
        return;
    }
    new_station = (station_node *) malloc(sizeof(station_node));
    new_station->right = NULL;
    new_station->left = NULL;
    new_station->parent = NULL;
    new_station->cars = NULL;
    new_station->distance = distance;
    new_station->cars = (dynamic_array *) malloc(sizeof(dynamic_array));
    init_car_heap(new_station->cars, num_cars + 5);

    int m = num_cars;
    if (num_cars == 0)
        m = 1;
    int32_t autonomy[m];
    for (int i = 0; i < num_cars; ++i) {
        geti(&autonomy[i]);
    }
    if (num_cars == 0) {
        autonomy[0] = 0;
        num_cars = 1;
    }
    //generate cars and insert them in the heap
    for (int i = 0; i < num_cars; ++i) {
        insert_car_without_check(new_station->cars, autonomy[i]);
    }

    insert(stations, new_station);
    fputs("aggiunta\n", output);
}


int8_t first_parser(const char *cmd) {
    if (cmd[7] == 'c')
        return cmd[0] == 'd' ? 2 : 5;
    else if (cmd[0] == 'a') {
        return cmd[9] == 's' ? 1 : 3;
    }
    return 4;
}

void swap(int32_t *a, int32_t *b) {
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

int median_of_three(int32_t *array, int start, int end) {
    int mid = start + (end - start) / 2;

    if (array[start] > array[mid])
        swap(&array[start], &array[mid]);

    if (array[start] > array[end])
        swap(&array[start], &array[end]);

    if (array[mid] > array[end])
        swap(&array[mid], &array[end]);

    return mid;
}

int partition(int32_t *array, int start, int end, int decreasing) {
    int pivotIndex = median_of_three(array, start, end);
    int32_t pivot = array[pivotIndex];
    int i = start - 1;

    swap(&array[pivotIndex], &array[end]);

    for (int j = start; j < end; ++j) {
        if ((array[j] < pivot && !decreasing) || (array[j] > pivot && decreasing)) {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[end]);
    return i + 1;
}

void insertion_sort(int32_t *array, int start, int end, int decreasing) {
    for (int i = start + 1; i <= end; ++i) {
        int32_t key = array[i];
        int j = i - 1;

        if (decreasing) {
            while (j >= start && array[j] < key) {
                array[j + 1] = array[j];
                j--;
            }
        } else {
            while (j >= start && array[j] > key) {
                array[j + 1] = array[j];
                j--;
            }
        }

        array[j + 1] = key;
    }
}

void quicksort(int32_t *array, int start, int end, int decreasing) {
    while (start < end) {
        if (end - start + 1 < 10) {
            insertion_sort(array, start, end, decreasing);
            break;
        } else {
            int pivot = partition(array, start, end, decreasing);

            if (!decreasing) {
                if (pivot - start < end - pivot) {
                    quicksort(array, start, pivot - 1, decreasing);
                    start = pivot + 1;
                } else {
                    quicksort(array, pivot + 1, end, decreasing);
                    end = pivot - 1;
                }
            } else {
                if (pivot - start > end - pivot) {
                    quicksort(array, start, pivot - 1, decreasing);
                    start = pivot + 1;
                } else {
                    quicksort(array, pivot + 1, end, decreasing);
                    end = pivot - 1;
                }
            }
        }
    }
}




