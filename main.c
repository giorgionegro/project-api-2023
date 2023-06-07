#include <stdio.h>
#include <stdlib.h>


#define true 1
#define false 0
#pragma GCC diagnostic ignored "-Wunused-result"
typedef struct car {
    int32_t autonomy;
    int32_t quantity;
    struct car *next;
} car;

void free_cars(struct car *pCar);

typedef struct tree {
    struct station_node *root;
} tree;

typedef struct station_node {
    int32_t distance;
    struct car *cars;
    struct station_node *left;
    struct station_node *right;
    struct station_node *parent;
} station_node;


typedef struct dynamic_array dynamic_array;

int32_t number_station = 0;
typedef struct dynamic_array {
    int32_t *array;
    int32_t capacity;
} dynamic_array;
station_node *cache_station = NULL;


station_node *successor(station_node *node);

station_node *predecessor(station_node *node);

void insert(tree *tree, station_node *node);

void delete(tree *tree, station_node *node);


void transplant(tree *tree, station_node *u, station_node *v);

station_node *search(tree *tree, int32_t distance);

station_node *minimum(station_node *node);

station_node *maximum(station_node *node);

#define tree_max(tree) maximum((tree)->root)
#define tree_min(tree) minimum((tree)->root)


station_node *successor(station_node *node) {
    if (node->right != NULL) {
        return minimum(node->right);
    }
    station_node *parent = node->parent;
    while (parent != NULL && node == parent->right) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

station_node *predecessor(station_node *node) {
    if (node->left != NULL) {
        return maximum(node->left);
    }
    station_node *parent = node->parent;
    while (parent != NULL && node == parent->left) {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

void insert(tree *tree, station_node *node) {
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

void delete(tree *tree, station_node *node) {
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

    free_cars(node->cars);
    if (cache_station == node) {
        cache_station = NULL;
    }
    free(node);

}

void transplant(tree *tree, station_node *u, station_node *v) {
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

station_node *search(tree *tree, int32_t distance) {
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


void free_tree(station_node *node) {
    if (node != NULL) {
        free_tree(node->left);
        free_tree(node->right);
        free_cars(node->cars);
        free(node);
    }
}


int32_t get(int32_t i, dynamic_array *array);

void set(int32_t i, int32_t value, dynamic_array *array);

void init_array(dynamic_array *array);

void free_array(dynamic_array *array);

void insertion_sort(int32_t *array, int start, int end, int decreasing);


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


tree *stations = NULL;
FILE *input;
FILE *output;


int32_t get(int32_t i, dynamic_array *array) {
    return (array->array)[i];
}

void set(int32_t i, int32_t value, dynamic_array *array) {
    if (array->capacity <= i) {
        array->capacity = array->capacity * 2;
        array->array = realloc(array->array, array->capacity * sizeof(int32_t));

    }
    array->array[i] = value;
}

void init_array(dynamic_array *array) {
    array->capacity = 5;
    array->array = malloc(array->capacity * sizeof(int32_t));
}

void free_array(dynamic_array *array) {
    free(array->array);
    free(array);
}


int read_space_end_line(char *line, FILE *inputt, int size) {
    char c;
    int i = 0;
    c = fgetc(inputt);

    if (c == EOF) {
        return 0;
    }
    while (c != '\n' && c != '\r' && c != ' ' && i < size - 2) {
        line[i] = c;
        i++;
        c = fgetc(inputt);

    }
    line[i] = '\0';
    return 1;
}


void go_to_next_line(FILE *file) {
    char c;
    while ((c = fgetc(file)) != '\n' && c != EOF) {}
}


int main(void) {
//read from stdin and write to stdout
//open input and output files
//    input = fopen("open_111.txt", "r");
//    output = fopen("output2.txt", "w");
    input = stdin;
    output = stdout;
    char *line;
    stations = malloc(sizeof(tree));
    stations->root = NULL;
    line = malloc(25 * sizeof(char));
    while (read_space_end_line(line, input, 25)) {


        if (line == NULL)
            break;
        /**
     *   aggiungi-stazione 1
     *   demolisci-stazione 2
     *   aggiungi-auto  3
     *   rottama-auto  4
     *   pianifica-percorso  5
     */
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
    free_tree(stations->root);
    free(stations);
    return 0;


}


typedef struct station_reverse {
    station_node *station;
    struct station_reverse *smaller;
} station_reverse;

station_reverse *add_stationr(station_reverse *lstation, station_node *station) {
    station_reverse *new = malloc(sizeof(station_reverse));
    new->station = station;
    new->smaller = lstation;
    return new;

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
                    start - temp->cars->autonomy > predecessor(temp)->distance) ||
                   (temp->distance == end)) {
            free_stationr(list);
            terminate = 0;
            list = NULL;
        } else if (temp->distance - temp->cars->autonomy <= end) {
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
    int32_t current_d_a = current->distance - current->cars->autonomy;

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
            t_c->smaller = malloc(sizeof(station_reverse));
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
    //first we initialize the buffer
    *integer = 0;
    char c = fgetc(input);
    while (c < '0' || c > '9') {
        c = fgetc(input);
    }
    //now we read the number
    while (c >= '0' && c <= '9') {
        *integer = *integer * 10 + c - '0';
        c = fgetc(input);
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
    dynamic_array *result = malloc(sizeof(dynamic_array));
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
        } else if ((temp->distance == start && temp->cars->autonomy + start < successor(temp)->distance) ||
                   (temp->distance == end)) {
            fputs("nessun percorso\n", output);
            free_array(result);
            return;
        } else if (temp->cars->autonomy + temp->distance >= end) {
            set(i, temp->distance, result);
            i++;
            end = temp->distance;
            temp = search(stations, start);
            continue;
        }

        temp = successor(temp);
    }
    insertion_sort(result->array, 0, i - 1, 0);
    //print first element
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
    car *prev = NULL;
    for (car *temp2 = station->cars; temp2 != NULL; temp2 = temp2->next) {
        if (temp2->autonomy == autonomy) {
            temp2->quantity--;
            if (temp2->quantity == 0) {
                if (prev == NULL) {
                    station->cars = temp2->next;
                } else {
                    prev->next = temp2->next;
                }
                free(temp2);
            }
            fputs("rottamata\n", output);
            return;
        }
        if (autonomy > temp2->autonomy) {
            fputs("non rottamata\n", output);
            return;
        }
        prev = temp2;
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
        goto end;
    }
    car *prev = NULL;
    //now we check if we have the car in the list TODO an heap could be better
    for (car *temp2 = station->cars; temp2 != NULL; temp2 = temp2->next) {
        if (temp2->autonomy == autonomy) {
            temp2->quantity++;
            fputs("aggiunta\n", output);
            goto end;
        }
        if (autonomy > temp2->autonomy) {
            break;
        }
        prev = temp2;
    }

    car *new_car = malloc(sizeof(car));
    new_car->autonomy = autonomy;
    new_car->quantity = 1;
    if (prev == NULL) {
        new_car->next = station->cars;
        station->cars = new_car;
    } else {
        new_car->next = prev->next;
        prev->next = new_car;
    }
    fputs("aggiunta\n", output);
    end:
    return;
}

void demolish_station(void) {
    int32_t distance = 0;
    geti(&distance);
    station_node *station = search(stations, distance);
    if (station == NULL) {
        fputs("non demolita\n", output);
        return;
    }
    delete(stations, station);
    fputs("demolita\n", output);
}

void free_cars(struct car *pCar) {
    for (car *temp = pCar; temp != NULL;) {
        car *next = temp->next;
        free(temp);
        temp = next;
    }

}


void add_station(void) {
    //aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n>
    //first  we get_station the distance
    int32_t distance = 0;
    geti(&distance);

    int32_t num_cars = 0;
    geti(&num_cars);
    //first we check if we have the station in the list
    station_node *new_station = search(stations, distance);
    if (new_station != NULL) {
        fputs("non aggiunta\n", output);
        go_to_next_line(input);
        return;
    }

    new_station = malloc(sizeof(station_node));
    new_station->right = NULL;
    new_station->left = NULL;
    new_station->parent = NULL;
    new_station->cars = NULL;
    new_station->distance = distance;
    int m = num_cars;
    if (num_cars == 0)
        m = 1;

    int32_t autonomy[m];
    for (int i = 0; i < num_cars; ++i) {
        geti(&autonomy[i]);

    }
    if (num_cars > 1)
        quicksort(autonomy, 0, num_cars - 1, 0);
    if (num_cars == 0) {

        autonomy[0] = 0;
        num_cars = 1;

    }

    car *cars = NULL;
    int32_t prev_autonomy = -1;
    for (int i = 0; i < num_cars; ++i) {
        if (prev_autonomy == autonomy[i]) {
            cars->quantity++;
            continue;
        }
        car *new_car = malloc(sizeof(car));
        new_car->autonomy = autonomy[i];
        new_car->quantity = 1;
        new_car->next = cars;
        cars = new_car;
        prev_autonomy = autonomy[i];
    }
    new_station->cars = cars;
    insert(stations, new_station);
    fputs("aggiunta\n", output);
}


int8_t first_parser(const char *cmd) {
    /**
     *   aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n> 1
     *   demolisci-stazione <distanza> 2
     *   aggiungi-auto <distanza> <autonomia> 3
     *   rottama-auto <distanza> <autonomia> 4
     *   pianifica-percorso <distanza-partenza> <distanza-arrivo> 5
     */
    // if
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

int partition(int32_t *array, int start, int end, int decreasing) {
    int32_t pivot = array[end];
    int i = start - 1;

    for (int j = start; j < end; ++j) {
        if ((array[j] < pivot && !decreasing) || (array[j] > pivot && decreasing)) {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[end]);
    return i + 1;
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

void insertion_sort(int32_t *array, int start, int end, int decreasing) {
    for (int i = start + 1; i <= end; ++i) {
        int32_t key = array[i];
        int j = i - 1;
        //decreasing order if decreasing == 1
        if (decreasing) {
            while (j >= start && array[j] < key) {
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = key;
            continue;
        } else {
            while (j >= start && array[j] > key) {
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = key;
        }
    }
}




