#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct car {
    uint32_t autonomy;
    uint32_t quantity;
    struct car *next;
} car;
typedef struct station {
    uint32_t distance;
    struct car *cars;
    struct station *next;
    struct station *prev;
} station;
char *strtokString;
int strtokindex = 0;
char srt_token[30];
uint32_t number_station = 0;
typedef struct dynamic_array {
    uint32_t *array;
    uint32_t capacity;
} dynamic_array;

typedef struct char_array {
    char *array;
    uint32_t capacity;
} char_array;

char get_char(uint32_t i, char_array *array);

void set_char(uint32_t i, char value, char_array *array);

uint32_t get(uint32_t i, dynamic_array *array);

void set(uint32_t i, uint32_t value, dynamic_array *array);

void init_array(dynamic_array *array);

void free_array(dynamic_array *array);

void insertion_sort(uint32_t *array, int start, int end, int decreasing);


void quicksort(uint32_t *array, int start, int end, int decreasing);

int partition(uint32_t *array, int start, int end, int decreasing);

uint8_t first_parser(const char *cmd);

void swap(uint32_t *a, uint32_t *b);

void add_station(char *line);

void demolish_station(char *line);

void add_car(char *line);

void demolish_car(char *line);

void plan_path(char *line);


//char *strtok(char *line, const char *string);

void reverse(uint32_t start, uint32_t end);

void free_cars(struct car *pCar);

station *stations = NULL;
FILE *input;
FILE *output;


uint32_t get(uint32_t i, dynamic_array *array) {
    return array->array[i];
}

void set(uint32_t i, uint32_t value, dynamic_array *array) {
    if (array->capacity <= i) {
        array->capacity = (array->capacity < 100) ? array->capacity * 4 : array->capacity + 100;
        array->array = realloc(array->array, array->capacity * sizeof(uint32_t));
    }
    array->array[i] = value;
}

void init_array(dynamic_array *array) {
    array->capacity = 5;
    array->array = malloc(array->capacity * sizeof(uint32_t));
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
        //free(line);
        //exit(0);
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

int main() {
//read from stdin and write to stdout
//open input and output files
    input = fopen("open_111.txt", "r");
    output = fopen("output.txt", "w");
    //input = stdin;
    //output = stdout;
    char *line;

    line = malloc(25 * sizeof(char));
    int line_number = 0;
    while (read_space_end_line(line, input, 25)) {
        line_number++;
        if (line == NULL)
            break;
        /**
     *   aggiungi-stazione 1
     *   demolisci-stazione 2
     *   aggiungi-auto  3
     *   rottama-auto  4
     *   pianifica-percorso  5
     */
        uint8_t cmd = first_parser(line);
        switch (cmd) {
            case 1:
                add_station(line);
                break;
            case 2:
                demolish_station(line);
                break;
            case 3:
                add_car(line);
                break;
            case 4:
                demolish_car(line);
                break;
            case 5:
                plan_path(line);
                break;
        }

    }
    free(line);
    return 0;


}


station *get_station(uint32_t i) {
    station *temp = stations;
    while (temp != NULL && temp->distance != i) {
        temp = temp->next;
    }
    return temp;
}

int get_min_reverse(uint32_t start, uint32_t end) {
    uint8_t terminate = 1;

    int i = 1;
    station *temp = get_station(start);

    while (terminate) {
        if (end == start) {
            terminate = 0;
        } else if ((temp->distance == start && start - temp->cars->autonomy > temp->prev->distance) ||
                   (temp->distance == end)) {
            return 0;
        } else if (temp->distance - temp->cars->autonomy <= end) {
            i++;
            end = temp->distance;
            temp = get_station(start);
            continue;
        } else if (temp->distance == start &&
                   start - temp->cars->autonomy >= temp->prev->prev->distance &&
                   start - temp->cars->autonomy <= temp->prev->distance) {

            i++;
            start = temp->prev->distance;
        }
        temp = temp->prev;
    }
    return i;


}


uint8_t
rec_reverse(uint32_t *array, uint32_t i, uint32_t step, station *ref, station *end, station *start_station) {
    uint8_t true = 1;
    uint8_t false = 0;
    array[i] = ref->distance;
    if (ref == NULL || end == NULL || start_station == NULL) {
        return false;
    }
    i++;
    if (ref->distance == end->distance) {
        return true;
    }
    station *left;
    for (left = end; left != NULL; left = left->next) {
        if (ref->distance - ref->cars->autonomy <= left->distance) {
            break;
        }
    }
    if (step == 1 && left != NULL && left->distance != end->distance) {
        return false;
    }
    for (station *temp = left; (temp != NULL && temp != ref); temp = temp->next) {

        if (rec_reverse(array, i, step - 1, temp, end, start_station)) {
            return true;
        }
    }
    return false;

}


void reverse(uint32_t start, uint32_t end) {

    //set(0, start, result);
    int i;
    station *temp = get_station(end);
    station *start_station = get_station(start);
    if (temp == NULL || start_station == NULL) {
        fputs("nessun percorso\n", output);
        return;
    }

    //get min reverse
    int min_reverse = get_min_reverse(start, end);
    if (min_reverse == 0) {
        fputs("nessun percorso\n", output);
        return;
    }
    uint32_t result[min_reverse];
    i = min_reverse;
    if (!rec_reverse(result, 0, min_reverse, start_station, temp, start_station)) {
        fputs("nessun percorso\n", output);
        return;
    }
    quicksort(result, 0, i - 1, 1);
    //print first element
    fprintf(output, "%d", result[0]);
    for (int j = 1; j < i; j++) {
        fprintf(output, " %d", result[j]);
    }
    fputs("\n", output);
}


void plan_path(char *line) {
    strtokindex = 19;
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    uint32_t start = atoi(token);
    read_space_end_line(token, input, 15);
    uint32_t end = atoi(token);
    int32_t distance = (int32_t) end - (int32_t) start;
    if (distance < 0) {
        reverse(start, end);
        free(token);
        return;
    }


    if (distance == 0) {
        fputs(token, output);
        fputs("\n", output);
        free(token);
        return;
    }
    uint8_t terminate = 1;
    dynamic_array *result = malloc(sizeof(dynamic_array));
    init_array(result);
    set(0, end, result);
    int i = 1;
    station *temp = get_station(start);
    if (temp == NULL) {
        fputs("nessun percorso\n", output);
        free_array(result);
        free(token);
        return;
    }
    while (terminate) {
        if (end == start) {
            terminate = 0;
        } else if ((temp->distance == start && temp->cars->autonomy + start < temp->next->distance) ||
                   (temp->distance == end)) {
            fputs("nessun percorso\n", output);
            free_array(result);
            free(token);
            return;
        } else if (temp->cars->autonomy + temp->distance >= end) {
            set(i, temp->distance, result);
            i++;
            end = temp->distance;
            temp = get_station(start);
            continue;
        } else if (temp->distance == start && temp->distance + temp->cars->autonomy <= temp->next->next->distance &&
                   temp->cars->autonomy + start >= temp->next->distance) {
            set(i, temp->distance, result);
            i++;
            start = temp->next->distance;
        }
        temp = temp->next;
    }
    insertion_sort(result->array, 0, i - 1, 0);
    //print first element
    fprintf(output, "%d", get(0, result));
    for (int j = 1; j < i; j++) {
        fprintf(output, " %d", get(j, result));
    }
    fputs("\n", output);
    free_array(result);
    free(token);
}


void demolish_car(char *line) {
    strtokindex = 13;
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    uint32_t distance = atoi(token);
    read_space_end_line(token, input, 15);
    uint32_t autonomy = atoi(token);
    car *prev = NULL;
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            for (car *temp2 = temp->cars; temp2 != NULL; temp2 = temp2->next) {
                if (temp2->autonomy == autonomy) {
                    temp2->quantity--;
                    if (temp2->quantity == 0) {
                        if (prev == NULL) {
                            temp->cars = temp2->next;
                        } else {
                            prev->next = temp2->next;
                        }
                        free(temp2);
                    }
                    fputs("rottamata\n", output);
                    free(token);
                    return;
                }
                if (autonomy > temp2->autonomy) {
                    fputs("non rottamata\n", output);
                    free(token);
                    return;
                }
                prev = temp2;
            }

        }
        if (distance < temp->distance) {
            fputs("non rottamata\n", output);
            free(token);
            return;
        }
    }
    fputs("non rottamata\n", output);
    free(token);

}

void add_car(char *line) {
    strtokindex = 14;
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    uint32_t distance = atoi(token);
    read_space_end_line(token, input, 15);
    uint32_t autonomy = atoi(token);
    //first we check if we have the station in the list
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            car *prev = NULL;
            //now we check if we have the car in the list TODO possible optimization by postponing addition to plan_path
            for (car *temp2 = temp->cars; temp2 != NULL; temp2 = temp2->next) {
                if (temp2->autonomy == autonomy) {
                    temp2->quantity++;
                    fputs("aggiunta\n", output);
                    free(token);
                    return;
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
                new_car->next = temp->cars;
                temp->cars = new_car;
            } else {
                new_car->next = prev->next;
                prev->next = new_car;
            }
            fputs("aggiunta\n", output);
            free(token);
            return;
        }
        if (distance < temp->distance) {
            fputs("non aggiunta\n", output);
            free(token);
            return;
        }
    }
    fputs("non aggiunta\n", output);
    free(token);

}

void demolish_station(char *line) {
    station *prev = NULL;
    strtokindex = 19;
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    uint32_t distance = atoi(token);
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            if (prev == NULL) {
                stations = temp->next;
            } else {
                prev->next = temp->next;
            }
            if (temp->next != NULL) { temp->next->prev = prev; }

            free_cars(temp->cars);
            free(temp);
            number_station--;
            fputs("demolita\n", output);
            free(token);
            return;
        }
        if (distance < temp->distance) {
            fputs("non demolita\n", output);
            free(token);
            return;
        }
        prev = temp;
    }
    fputs("non demolita\n", output);
    free(token);

}

void free_cars(struct car *pCar) {
    for (car *temp = pCar; temp != NULL;) {
        car *next = temp->next;
        free(temp);
        temp = next;
    }

}


void add_station(char *line) {
    //aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n> 1
    //first  we get_station the distance
    strtokindex = 18;
    //disgard the first token
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    uint32_t distance = atoi(token);
    read_space_end_line(token, input, 15);
    uint16_t num_cars = atoi(token);
    //first we check if we have the station in the list
    station *prev = NULL;
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            fputs("non aggiunta\n", output);
            free(token);
            go_to_next_line(input);
            return;
        }
        if (temp->distance > distance) {
            break;
        }
        prev = temp;
    }

    station *new_station = malloc(sizeof(station));
    if (prev == NULL) {
        new_station->next = stations;
        stations = new_station;
    } else {
        new_station->next = prev->next;
        prev->next = new_station;
    }
    new_station->prev = prev;
    new_station->distance = distance;
    if (new_station->next != NULL)
        new_station->next->prev = new_station;

    uint32_t autonomy[num_cars];
    for (int i = 0; i < num_cars; ++i) {

        read_space_end_line(token, input, 15);
        autonomy[i] = atoi(token);
    }
    if (num_cars > 1)
        quicksort(autonomy, 0, num_cars - 1, 0);
    if (num_cars == 0) {
        autonomy[0] = 0;
        num_cars = 1;

    }
    car *cars = NULL;
    uint32_t prev_autonomy = -1;
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
    number_station++;
    fputs("aggiunta\n", output);
    free(token);
}


//char *strtok(char *line, const char *string) {
//    if (line != NULL) {
//        strtokString = line;
//    }
//    if (strtokString == NULL)
//        return NULL;
//    int8_t i = 0;
//    while (1) {
//        if (strtokString[strtokindex + i] == string[0] || strtokString[strtokindex + i] == '\0') {
//            srt_token[i] = '\0';
//            strtokindex += i + 1;
//            return srt_token;
//        } else {
//            srt_token[i] = strtokString[strtokindex + i];
//            i++;
//        }
//    }
//}




//quicksort array of uint32_t




uint8_t first_parser(const char *cmd) {
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

void swap(uint32_t *a, uint32_t *b) {
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

int partition(uint32_t *array, int start, int end, int decreasing) {
    uint32_t pivot = array[end];
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


void quicksort(uint32_t *array, int start, int end, int decreasing) {
    if (start < end) {
        //hybrid quicksort
//        if (end - start < 10) {
//            insertion_sort(array, start, end, decreasing);
//        } else {
//            int pivot = partition(array, start, end, decreasing);
//         quicksort(array, start, pivot - 1, decreasing);
//       quicksort(array, pivot + 1, end, decreasing);
//        }
        int pivot = partition(array, start, end, decreasing);
        quicksort(array, start, pivot - 1, decreasing);
        quicksort(array, pivot + 1, end, decreasing);
    }

}

void insertion_sort(uint32_t *array, int start, int end, int decreasing) {
    for (int i = start + 1; i <= end; ++i) {
        uint32_t key = array[i];
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


