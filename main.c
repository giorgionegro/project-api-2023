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
} station;
char *strtokString;
int strtokindex = 0;
char srt_token[30];
uint32_t number_station = 0;
void quicksort(uint32_t *array, int start, int end);

int partition(uint32_t *array, int start, int end);

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
    char str2[] = "aggiungi-auto 10 100";
    add_car(str2);
    char str3[] = "aggiungi-auto 10 200";
    add_car(str3);
    char str4[] = "aggiungi-auto 10 300";
    add_car(str4);
    char str5[] = "aggiungi-auto 10 400";
    add_car(str5);
    return 0;

}


station get(uint32_t i)
{
    station *temp = stations;
    while(temp->distance != i)
    {
        temp = temp->next;
    }
    return *temp;
}

void reverse(uint32_t start, uint32_t end) {




    uint8_t terminate = 1;
    uint32_t array[number_station+1];

//    int i = 0;
//    station temp = get(end);
//    while(terminate)
//    {
//        if(temp.cars->autonomy+start >= end)
//        {
//            array[i] = temp.distance;
//            i++;
//            array[i] = get(end).distance;
//            i++;
//            terminate = 0;
//        }
//        else if(temp.cars->autonomy+start < temp.next->distance)
//        {
//            puts("nessun percorso\n");
//            return;
//        }
//        else if(temp.cars->autonomy==temp.next->distance)
//        {
//            array[i]=temp.distance;
//            i++;
//            start = temp.next->distance;
//        }
//        temp=*temp.next;
//    }
//    quicksort(array, 0, i-1);
//    for(int j = 0; j<i; j++)
//    {
//        printf("%d ", array[j]);
//    }
//    puts("\n");
}



void plan_path(char *line) {
    strtokindex = 18;
    char *token = strtok(line, " ");
    uint32_t start = atoi(token);
    uint32_t end = atoi(strtok(NULL, " "));
    int32_t distance = (int32_t)end - (int32_t)start;
    if (distance < 0) {
        reverse(start, end);
        return;
    }




if(distance == 0)
    {
        puts(token);
        return;
    }
    uint8_t terminate = 1;
    uint32_t array[number_station+1];

    int i = 0;
    station temp = get(start);
    while(terminate)
    {
        if(temp.cars->autonomy+start >= end)
        {
            array[i] = temp.distance;
            i++;
            array[i] = get(end).distance;
            i++;
            terminate = 0;
        }
        else if(temp.cars->autonomy+start < temp.next->distance)
        {
            puts("nessun percorso\n");
            return;
        }
        else if(temp.cars->autonomy==temp.next->distance)
        {
            array[i]=temp.distance;
            i++;
            start = temp.next->distance;
        }
        temp=*temp.next;
    }
    quicksort(array, 0, i-1);
    for(int j = 0; j<i; j++)
    {
        printf("%d ", array[j]);
    }
    puts("\n");
}





void demolish_car(char *line) {
    strtokindex = 12;
    char *token = strtok(line, " ");
    uint32_t distance = atoi(token);
    car *prev = NULL;
    for(station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            uint32_t autonomy = atoi(strtok(NULL, " "));
            for (car *temp2 = temp->cars; temp2 != NULL; temp2 = temp2->next)
            {
                if (temp2->autonomy == autonomy)
                {
                    temp2->quantity--;
                    if (temp2->quantity ==0)
                    {
                        if (prev == NULL)
                        {
                            temp->cars = temp2->next;
                        }
                        else {
                            prev->next = temp2->next;
                        }
                        free(temp2);
                    }
                    puts("rottamata\n");
                    return;
                }
                if (autonomy > temp2->autonomy)
                {
                    puts("non rottamata\n");
                    return;
                }
            }

        }
        if (distance > temp->distance) {
            puts("non demolita\n");
            return;
        }
    }

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
                    puts("aggiunta");
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
            puts("aggiunta");
            return;
        }
    }


}

void demolish_station(char *line) {
    station *prev = NULL;
    strtokindex = 18;
    char *token = strtok(line, " ");
    uint32_t distance = atoi(token);
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            if (prev == NULL) {
                stations = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            number_station--;
            puts("demolita\n");
            return;
        }
        if (distance > temp->distance) {
            puts("non demolita\n");
        }
        prev = temp;
    }



}

void add_station(char *line) {
    //aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n> 1
    //first  we get the distance
    strtokindex = 18;
    //disgard the first token
    char *token = strtok(line, " ");
    uint32_t distance = atoi(token);
    //first we check if we have the station in the list
    station *prev = NULL;
    for (station *temp = stations; temp != NULL; temp = temp->next) {
        if (temp->distance == distance) {
            puts("non aggiunta\n");
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
    new_station->distance = distance;
    token = strtok(NULL, " ");
    uint16_t num_cars = atoi(token);
    uint32_t autonomy[num_cars];
    for (int i = 0; i < num_cars; ++i) {
        token = strtok(NULL, " ");
        autonomy[i] = atoi(token);
    }
    if (num_cars > 1)
        quicksort(autonomy, 0, num_cars - 1);
    car *cars = NULL;
    for (int i = 0; i < num_cars; ++i) {
        car *new_car = malloc(sizeof(car));
        new_car->autonomy = autonomy[i];
        new_car->quantity = 1;
        new_car->next = cars;
        cars = new_car;
    }
    new_station->cars = cars;
    number_station++;
    puts("aggiunta\n");
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
    if (cmd[7] == 'r')
        return cmd[0] == 'd' ? 2 : 4;
    else if (cmd[0] == 'a') {
        return cmd[9] == 's' ? 1 : 3;
    }
    return 5;
}

void swap(uint32_t *a, uint32_t *b) {
    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

int partition(uint32_t *array, int start, int end) {
    uint32_t pivot = array[end];
    int i = start - 1;
    for (int j = start; j < end; ++j) {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[end]);
    return i + 1;
}

void insertion_sort(uint32_t *array, int start, int end) {
    for (int i = start + 1; i <= end; ++i) {
        uint32_t key = array[i];
        int j = i - 1;
        while (j >= start && array[j] > key) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = key;
    }
}

void quicksort(uint32_t *array, int start, int end) {
    if (start < end) {
        //hybrid quicksort
        if (end - start < 10) {
            insertion_sort(array, start, end);
        } else {
            int pivot = partition(array, start, end);
            quicksort(array, start, pivot - 1);
            quicksort(array, pivot + 1, end);
        }
    }
}


