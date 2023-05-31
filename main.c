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


uint32_t get(uint32_t i, dynamic_array *array);

void set(uint32_t i, uint32_t value, dynamic_array *array);

void init_array(dynamic_array *array);


void quicksort(uint32_t *array, int start, int end, int decreasing);

int partition(uint32_t *array, int start, int end, int decreasing);

uint8_t first_parser(const char *cmd);

void swap(uint32_t *a, uint32_t *b);

void add_station(char *line);

void demolish_station(char *line);

void add_car(char *line);

void demolish_car(char *line);

void plan_path(char *line);


char *strtok2(char *line, const char *string);

char *strtok(char *line, const char *string);

void reverse(uint32_t pInt, uint32_t pInt1);

station *stations = NULL;
station *to_add = NULL;
station *to_del = NULL;
FILE *input;
FILE *output;


uint32_t get(uint32_t i, dynamic_array *array) {
    return array->array[i];
}

void set(uint32_t i, uint32_t value, dynamic_array *array) {
    if (array->capacity <= i) {
        array->capacity = i * 2;
        array->array = realloc(array->array, array->capacity * sizeof(uint32_t));
    }
    array->array[i] = value;
}

void init_array(dynamic_array *array) {
    array->capacity = 5;
    array->array = malloc(array->capacity * sizeof(uint32_t));
}


int main() {
//read from stdin and write to stdout
//open input and output files
    input = fopen("prova.txt", "r");
    output = fopen("output.txt", "w");
    char *line = NULL;
    line = malloc(512 * 5 * sizeof(char));
    size_t len = 0;
    ssize_t read;
    while (fgets(line, 512 * 5, input)) {
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
    while (temp->distance != i) {
        temp = temp->next;
    }
    return temp;
}

void reverse(uint32_t start, uint32_t end) {


    uint8_t terminate = 1;
    dynamic_array *result = malloc(sizeof(dynamic_array));
    init_array(result);
    set(0, start, result);
    uint32_t fs = start;
    uint32_t fe = end;
    int i = 1;
    station *temp = get_station(end);
    station *start_station = get_station(start);
    while (terminate) {
        if (end == start) {
            terminate = 0;
            break;
        } else if (start_station->cars != NULL && temp->distance + start_station->cars->autonomy >= start) {
            set(i, temp->distance, result);
            i++;
            start = temp->distance;
            temp = get_station(end);
            continue;
        } else if (start_station->cars == NULL ||
                   start_station->distance - start_station->cars->autonomy > start_station->prev->distance) {
            fputs("nessun percorso\n", output);
            return;
        }
        temp = temp->next;
    }
    quicksort(result->array, 0, i - 1, 1);
    for (int j = 0; j < i; j++) {
        fprintf(output, "%d ", get(j, result));
    }
    fputs("\n", output);
}


void plan_path(char *line) {
    strtokindex = 19;
    char *token = strtok(line, " ");
    uint32_t start = atoi(token);
    uint32_t end = atoi(strtok(NULL, " "));
    uint32_t fs = start;
    uint32_t fe = end;
    int32_t distance = (int32_t) end - (int32_t) start;
    if (distance < 0) {
        reverse(start, end);
        return;
    }


    if (distance == 0) {
        fputs(token, output);
        return;
    }
    uint8_t terminate = 1;
    dynamic_array *result = malloc(sizeof(dynamic_array));
    init_array(result);
    set(0, end, result);
    int i = 1;
    station *temp = get_station(start);
    while (terminate) {
        if (end == start) {
            terminate = 0;
        } else if (temp->cars->autonomy + start >= end) {
            set(i, temp->distance, result);
            i++;
            end = temp->distance;
            temp = get_station(start);
            continue;
        } else if (temp->cars->autonomy + start < temp->next->distance) {
            fputs("nessun percorso\n", output);
            return;
        } else if (temp->cars->autonomy == temp->next->distance) {
            set(i, temp->distance, result);
            i++;
            start = temp->next->distance;
        }
        temp = temp->next;
    }
    quicksort(result->array, 0, i - 1, 0);
    for (int j = 0; j < i; j++) {
        fprintf(output, "%d ", get(j, result));
    }
    fputs("\n", output);
}


void demolish_car(char *line) {
    strtokindex = 13;
    char *token = strtok(line, " ");
    uint32_t distance = atoi(token);
    car *prev = NULL;
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            uint32_t autonomy = atoi(strtok(NULL, " "));
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
                    return;
                }
                if (autonomy > temp2->autonomy) {
                    fputs("non rottamata\n", output);
                    return;
                }
                prev = temp2;
            }

        }
        if (distance < temp->distance) {
            fputs("non demolita\n", output);
            return;
        }
    }
    fputs("non demolita\n", output);

}

void add_car(char *line) {
    strtokindex = 14;
    char *token = strtok(line, " ");
    uint32_t distance = atoi(token);
    //first we check if we have the station in the list
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            uint32_t autonomy = atoi(strtok(NULL, " "));
            car *prev = NULL;
            //now we check if we have the car in the list TODO possible optimization by postponing addition to plan_path
            for (car *temp2 = temp->cars; temp2 != NULL; temp2 = temp2->next) {
                if (temp2->autonomy == autonomy) {
                    temp2->quantity++;
                    fputs("aggiunta\n", output);
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
            return;
        }
    }


}

void demolish_station(char *line) {
    station *prev = NULL;
    strtokindex = 19;
    char *token = strtok(line, " ");
    uint32_t distance = atoi(token);
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            if (prev == NULL) {
                stations = temp->next;
            } else {
                prev->next = temp->next;
            }
            temp->next->prev = prev;
            free(temp);
            number_station--;
            fputs("demolita\n", output);
            return;
        }
        if (distance < temp->distance) {
            fputs("non demolita\n", output);
            return;
        }
        prev = temp;
    }
    fputs("non demolita\n", output);


}

void add_station(char *line) {
    //aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n> 1
    //first  we get_station the distance
    strtokindex = 18;
    //disgard the first token
    char *token = strtok(line, " ");
    uint32_t distance = atoi(token);
    //first we check if we have the station in the list
    station *prev = NULL;
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            fputs("non aggiunta\n", output);
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
    token = strtok(NULL, " ");
    uint16_t num_cars = atoi(token);
    uint32_t autonomy[num_cars];
    for (int i = 0; i < num_cars; ++i) {
        token = strtok(NULL, " ");
        autonomy[i] = atoi(token);
    }
    if (num_cars > 1)
        quicksort(autonomy, 0, num_cars - 1, 0);
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
}


char *strtok(char *line, const char *string) {
    if (line != NULL) {
        strtokString = line;
    }
    if (strtokString == NULL)
        return NULL;
    int8_t i = 0;
    while (1) {
        if (strtokString[strtokindex + i] == string[0] || strtokString[strtokindex + i] == '\0') {
            srt_token[i] = '\0';
            strtokindex += i + 1;
            return srt_token;
        } else {
            srt_token[i] = strtokString[strtokindex + i];
            i++;
        }
    }
}




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
        //decreasing order if decreasing == 1
        if (array[j] < pivot && decreasing) {
            i++;
            swap(&array[i], &array[j]);
        }
        if (array[j] > pivot && !decreasing) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[end]);
    return i + 1;
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

void quicksort(uint32_t *array, int start, int end, int decreasing) {
    if (start < end) {
        //hybrid quicksort
        if (end - start < 10) {
            insertion_sort(array, start, end, decreasing);
        } else {
            int pivot = partition(array, start, end, decreasing);
            quicksort(array, start, pivot - 1, decreasing);
            quicksort(array, pivot + 1, end, decreasing);
        }
    }
}


