#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef struct car {
    int32_t autonomy;
    int32_t quantity;
    struct car *next;
} car;
typedef struct station {
    int32_t distance;
    struct car *cars;
    struct station *next;
    struct station *prev;
} station;

typedef struct station_update {
    struct station *station;
    int32_t distance;
    int32_t sautonomy;
    int32_t eautonomy;
    struct station_update *next;
} station_update;


typedef struct dynamic_array dynamic_array;

typedef struct dynamic_array_station dynamic_array_station;


int get_all_station_reachable(dynamic_array_station *array, station *station1);


uint32_t number_station = 0;
typedef struct dynamic_array {
    uint32_t *array;
    uint32_t capacity;
} dynamic_array;

typedef struct dynamic_array_station {
    station **array;
    uint32_t capacity;
} dynamic_array_station;

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

typedef struct station_reverse station_reverse;


void reverse(uint32_t start, uint32_t end);

void free_cars(struct car *pCar);

void set_station(int i, station *value, dynamic_array_station *pArray);

void init_array_station(dynamic_array_station *station);

void get_all_station_reachable_from_null(int32_t distance, int32_t sautonomy, int32_t eautonomy);

station *stations = NULL;
FILE *input;
FILE *output;


uint32_t get(uint32_t i, dynamic_array *array) {
    return (array->array)[i];
}

void set(uint32_t i, uint32_t value, dynamic_array *array) {
    if (array->capacity <= i) {
        uint32_t old_capacity = array->capacity;
        array->capacity = (array->capacity < 100) ? array->capacity * 4 : array->capacity + 100;
        array->array = realloc(array->array, array->capacity * sizeof(uint32_t));
        // init the new array to -1 with memset
        memset(array->array + old_capacity, -1, (array->capacity - old_capacity) * sizeof(uint32_t));
    }
    array->array[i] = value;
}

void init_array(dynamic_array *array) {
    array->capacity = 5;
    array->array = malloc(array->capacity * sizeof(uint32_t));
    //init all the array to -1 with memset
    memset(array->array, -1, array->capacity * sizeof(uint32_t));

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

station *cache_station = NULL;
station_update *to_update = NULL;
station_update *to_remove = NULL;

int main() {
//read from stdin and write to stdout
//open input and output files
    input = fopen("open_111.txt", "r");
    //output = fopen("output.txt", "w");
    //input = stdin;
    output = stdout;
    char *line;

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
        uint8_t cmd = first_parser(line);
        switch (cmd) {
            case 1:
                cache_station = NULL;
                add_station(line);
                break;
            case 2:
                cache_station = NULL;
                demolish_station(line);
                break;
            case 3:
                add_car(line);
                break;
            case 4:
                demolish_car(line);
                break;
            case 5:
                cache_station = NULL;
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


typedef struct station_reverse {
    station *station;
    struct station_reverse *smaller;
    struct station_reverse *bigger;
} station_reverse;

station_reverse *add_stationr(station_reverse *lstation, uint32_t autonomy, uint32_t distance, station *station) {

    station_reverse *prev = NULL;
    if (lstation == NULL) {
        station_reverse *new = malloc(sizeof(station_reverse));
        new->station = station;
        new->smaller = NULL;
        new->bigger = NULL;
        lstation = new;
        return lstation;
    }
    for (station_reverse *temp = lstation; temp != NULL; temp = temp->smaller) {
        if (temp->station->distance == distance) {
            return NULL;
        }
        if (temp->station->distance > distance) {
        }
    }

    station_reverse *new_station = malloc(sizeof(station_reverse));
    if (prev == NULL) {
        new_station->smaller = lstation;
        lstation = new_station;
    } else {
        new_station->smaller = prev->smaller;
        prev->smaller = new_station;
    }
    new_station->bigger = prev;
    new_station->station->distance = distance;
    if (new_station->smaller != NULL)
        new_station->smaller->bigger = new_station;

    new_station->station->cars->autonomy = autonomy;
    new_station->station = station;
    return lstation;

}

void swaps(station *a, uint32_t distance, uint32_t autonomy) {
    a->distance = distance;
    a->cars->autonomy = autonomy;
}

uint32_t count = 0;


void free_stationr(station_reverse *lstation) {

    for (station_reverse *temp = lstation; temp != NULL;) {
        station_reverse *temp_station = temp->smaller;
        free(temp);
        temp = temp_station;

    }

}

station_reverse *get_first_pass(uint32_t start, uint32_t end) {
    uint8_t terminate = 1;
    station_reverse *list = NULL;
    int i = 1;
    station *starts = get_station(start);
    list = add_stationr(list, get_station(end)->cars->autonomy, end, get_station(end));
    station *temp = starts;
    while (terminate) {
        if (end == start) {
            terminate = 0;
        } else if ((temp->distance == start &&
                    start - temp->cars->autonomy > temp->prev->distance) ||
                   (temp->distance == end)) {
            free_stationr(list);
            return NULL;
        } else if (temp->distance - temp->cars->autonomy <= end) {
            list = add_stationr(list, temp->cars->autonomy, temp->distance, temp);
            i++;
            end = temp->distance;
            temp = starts;
            continue;
        } else if (temp->distance == start &&
                   start - temp->cars->autonomy >= temp->prev->prev->distance &&
                   start - temp->cars->autonomy <= temp->prev->distance) {
            list = add_stationr(list, temp->cars->autonomy, temp->distance, temp);
            i++;
            start = temp->next->distance;
            starts = temp->next;
        }
        temp = temp->prev;
    }
    count = i;
    return list;
}

/*char *print_station(station_reverse *list) {
    char *string = malloc(1000 * sizeof(char));
    sprintf(string, "%d", list->station->distance);
    for (station_reverse *temp5 = list->smaller; temp5 != NULL; temp5 = temp5->smaller) {
        sprintf(string, " %d", temp5->station->distance);
    }
    return string;
}*/




void init_array_station(dynamic_array_station *station1) {
    station1->array = malloc(sizeof(station) * 10);
    station1->capacity = 10;
}

//
//bool c(current, best, currentNavigated){
//    if(best==NULL) return true
//    left = leftmost after currentNavigated in range of current
//    if(left == NULL){
//        if(best in range of current) return true
//        return false
//    }
//    for( ; left in range of current; left = left -> smaller){
//        if(c(left, best->smaller, currentNavigated-> smaller)){
//            best->station = left->station
//        }
//        currentNavigated->station = left->station
//    }
//    return true
//}



uint8_t c(station *current, station_reverse *best, station_reverse *currentNavigated) {

    if (best == NULL) {
        return 1;
    }
    int32_t current_d_a = current->distance - current->cars->autonomy;
    station *left = currentNavigated->station->prev;
    if (left != NULL && current_d_a <= left->distance) {
    } else {
        return 0;
    }

    if (left == NULL) {
        if (current_d_a <= best->station->distance) {
            return 1;
        }
        return 0;
    }
    for (station *temp = left; temp != NULL && current_d_a <= temp->distance; temp = temp->prev) {
        if (c(temp, best->smaller, currentNavigated->smaller)) {
            best->station = temp;
        }
        currentNavigated->station = temp;
    }
    return 1;
}


/*
 * findShortestReverse(bigger, smaller){

best = shortestPath(bigger, smaller)

if(best!=NULL){
navigated = foreach of best the one before (the first of navigated should have NULL station and reference to next)

c(best, best, navigated);

best is the best
}else{
NESSUN PERCORSO
}
}
 */










void reverse(uint32_t start, uint32_t end) {
    //update statio based on to update and to remove
    station_reverse *best = get_first_pass(start, end);
    if (best == NULL) {
        fprintf(output, "nessun percorso\n");
        return;
    }
    station_reverse *currentNavigated = malloc(sizeof(station_reverse));
    station_reverse *temp = best;
    station_reverse *prev = NULL;
    while (temp != NULL) {
        currentNavigated->station = temp->station->next;
        currentNavigated->smaller = prev;
        if (prev != NULL) {
            prev->bigger = currentNavigated;
        }
        temp = temp->smaller;
    }
    c(best->station, best, currentNavigated);


    fprintf(output, "%d", best->station->distance);
    for (station_reverse *temp5 = best->smaller; temp5 != NULL; temp5 = temp5->smaller) {
        fprintf(output, " %d", temp5->station->distance);
    }
    fprintf(output, "\n");
    free_stationr(best);

}


void plan_path(char *line) {
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
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    int32_t distance = atoi(token);
    read_space_end_line(token, input, 15);
    int32_t autonomy = atoi(token);
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
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    int32_t distance = atoi(token);
    read_space_end_line(token, input, 15);
    int32_t autonomy = atoi(token);
    station *start = stations;
    if (cache_station != NULL && cache_station->distance == distance) {
        start = cache_station;
    }
    //first we check if we have the station in the list
    for (station *temp = start; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            cache_station = temp;
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
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    int32_t distance = atoi(token);
    station *start = stations;
    if (cache_station != NULL && cache_station->distance == distance) {
        start = cache_station;
    }
    for (station *temp = start; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            cache_station = temp;
            if (prev == NULL) {
                stations = temp->next;
            } else {
                prev->next = temp->next;
            }
            if (temp->next != NULL) { temp->next->prev = prev; }
            //see if in to_update and remove
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
    //aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n>
    //first  we get_station the distance
    //disgard the first token
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input, 15);
    int32_t distance = atoi(token);
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
    int m = num_cars;
    if (num_cars == 0)
        m = 1;

    int32_t autonomy[m];
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


int get_all_station_reachable(dynamic_array_station *array, station *station1) {
//dynamic_array of stations reachable
    station *temp = station1;
    int i = 0;
    while (temp != NULL && station1->distance - station1->cars->autonomy < temp->distance) {
        set_station(i, station1, array);
        i++;
        temp = temp->prev;
    }
    return i;
}

void set_station(int i, station *value, dynamic_array_station *pArray) {
    if (pArray->capacity <= i) {
        pArray->capacity = (pArray->capacity < 100) ? pArray->capacity * 4 : pArray->capacity + 100;
        pArray->array = realloc(pArray->array, pArray->capacity * sizeof(uint32_t));
    }
    (pArray->array)[i] = value;

}


