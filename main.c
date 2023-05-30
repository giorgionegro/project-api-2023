#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

typedef struct car {
    uint32_t autonomy;
    struct car *next;
} car;
typedef struct station {
    uint32_t distance;
    struct car *cars;
    struct station *next;
} station;

void quicksort(uint32_t *array, int start, int end);

int partition(uint32_t *array, int start, int end);

uint8_t first_parser(const char *cmd);

void swap(uint32_t *a, uint32_t *b);

void add_station(char *line);

void demolish_station(char *line);

void add_car(char *line);

void demolish_car(char *line);

void plan_path(char *line);

station *stations = NULL;
station *to_add = NULL;
station *to_del = NULL;

int main() {
//read from stdin and write to stdout
//    char * line = NULL;
//    size_t len = 0;
//    ssize_t read;
//    while (1) {
//        char * line =gets(STDIN_FILENO);
//        if (line == NULL)
//            break;
//        uint8_t cmd = first_parser(line);
//        switch (cmd) {
//            case 1: add_station(line);
//            break;
//            case 2: demolish_station(line);
//            break;
//            case 3: add_car(line);
//            break;
//            case 4: demolish_car(line);
//            break;
//            case 5: plan_path(line);
//            break;
//        }
//
//    }
//    free(line);
//    return 0;

    char str[] = "aggiungi-stazione 10 3 200 100 300";
    add_station(str);


}

void plan_path(char *line) {

}

void demolish_car(char *line) {

}

void add_car(char *line) {

}

void demolish_station(char *line) {

}

void add_station(char *line) {
    //aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n> 1
    //first  we get the distance
    char *token = strtok(line, " ");
    //disgard the first token
    token = strtok(NULL, " ");
    uint32_t distance = atoi(token);
    station *new_station = malloc(sizeof(station));
    new_station->distance = distance;
    //get the number of cars
    token = strtok(NULL, " ");
    uint16_t num_cars = atoi(token);
    //for each car we get the autonomy and we add it to a array, in the end we sort the array and we add it to the station
    //we can use vla array
    uint32_t autonomy[num_cars];
    for (int i = 0; i < num_cars; ++i) {
        token = strtok(NULL, " ");
        autonomy[i] = atoi(token);
    }

    quicksort(autonomy, 0, num_cars - 1);
    car *cars = NULL;
    for (int i = 0; i < num_cars; ++i) {
        car *new_car = malloc(sizeof(car));
        new_car->autonomy = autonomy[i];
        new_car->next = cars;
        cars = new_car;
    }
    new_station->cars = cars;
    to_add = new_station;


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
    if (cmd[0] == 'r')
        return 4;
    else if (cmd[0] == 'a') {
        return cmd[9] == 's' ? 1 : 3;
    } else if (cmd[0] == 'd')
        return 2;
    return 5;
}

void swap(uint32_t *a, uint32_t *b) {
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

int partition(uint32_t *array, int start, int end) {
    int pivot = array[end];
    int i = start - 1;
    for (int j = start; j < end; ++j) {
        if (array[j] > pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[end]);
    return i + 1;
}

void quicksort(uint32_t *array, int start, int end) {
    if (start > end) {
        int pivot = partition(array, start, end);
        quicksort(array, start, pivot - 1);
        quicksort(array, pivot + 1, end);
    }
}


