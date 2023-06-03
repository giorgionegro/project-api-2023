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



int read_space_end_line(char *line, FILE *inputt,int size) {
    char c;
    int i = 0;
    c = fgetc(inputt);

    if (c==EOF){
        //free(line);
        //exit(0);
        return 0;
    }
    while (c  != '\n'&&c!='\r' && c != ' ' &&i<size-1) {
        line[i] = c;
        i++;
        c = fgetc(inputt);

    }
    line[i] = '\0';
    return 1;
}


int main() {
//read from stdin and write to stdout
//open input and output files
    input = fopen("open_111.txt", "r");
    output = fopen("output.txt", "w");
    //input = stdin;
    //output = stdout;
    char *line ;

    line = malloc(25* sizeof(char));
    int line_number = 0;
    //"aggiungi-stazione 284491582 512 15122301 15503214 15144507 14829110 11874250 15250743 14538797 14882478 12863396 11755164 12952579 11814673 15439119 13550951 12231353 13026524 12428628 14055634 15508189 11533783 15454028 13956501 11727217 13381932 12164047 12866254 14417624 12366516 12390763 12340652 11709379 13087171 14056778 12048466 15110314 13183560 13494284 12283921 12909161 15133258 12569284 13696978 12327935 12244303 14148052 11858455 15429161 12731623 14120711 13101845 13311647 13544051 12664674 12142849 14905575 14669777 14546340 14677221 13790170 15158827 15349245 13741803 11894367 12412053 12214728 12317554 13173815 14405157 12682766 12054076 11863980 12625235 15377589 14820203 13124944 13983425 15470327 13630080 13424970 13611417 12942238 13660174 13286116 14194207 13446056 13833669 13673414 13375502 11994107 14533201 12008789 14032504 12579814 15424076 14636596 11907972 15413566 11619076 11640887 13736207 14907506 14737549 14171179 12771159 12518612 11686886 11522350 11657772 14905812 14720183 13956966 14168101 15102910 13975007 14917270 12763040 12972716 12117558 12539207 12640356 13724966 11577923 15347269 12560302 14106634 14720924 14768554 14560965 14338980 14224682 12191154 15300616 12806827 11596291 12553105 11840075 11631461 12786564 13225601 12229041 11793245 15457579 12911352 12454592 12284631 13167867 12684911 15020928 14240129 13998670 15312452 12692111 13240681 15159686 11689905 13269253 12882161 11968421 15427487 12184040 12516460 15142661 11602097 12540111 14652793 13325172 12225651 14664205 14631297 15024563 14287006 11664551 13903901 12069692 12296866 14645530 12980544 15255171 14998166 13143363 11731813 13795501 14372036 15128456 13659770 12629694 12298766 12626592 15224852 11819678 12587705 13560462 14828688 12669459 11820525 15269691 12918061 14866796 13930637 14299506 14914213 11837272 15513991 14344637 12879170 14567069 11773363 12300437 12417504 11875151 13898500 14332720 15288608 12739556 13376380 12564479 12934847 12749930 15361481 11591775 11724819 13301428 14197754 12425828 14174071 13649392 15392591 12744029 13288145 15468199 11846716 12820909 13222316 11693251 13584930 14803278 14759784 12775638 12340140 14260056 12613371 14545731 12859489 13182198 13843470 12126443 13771595 11641618 12980986 14029338 13879887 14070228 13946336 14628133 12011726 13113661 14951289 14754459 11576447 15145982 12744792 13057814 14611889 13326709 11764220 14949898 13512089 14747701 15306009 13111208 14799583 14924898 14020499 15235127 13630937 12687410 12269307 13922606 13767481 13139436 12202892 15186607 12769142 14329980 11657538 14560208 11915244 13174912 14806237 14887128 12178206 11922013 13066873 11844994 12335154 13526586 13007483 14455048 14865585 13330672 14726849 12153959 11758087 12439821 15410635 11631740 12068616 12428782 12564349 13868970 11691872 11663332 12386623 15047822 14118334 14943186 13125942 12601194 12878058 13307501 12527991 12008312 14942618 13758687 13440366 12953807 12398239 15179630 11490744 11874525 13147986 12894085 11720374 13419360 11823611 12873892 15189937 14278365 14019699 12492453 15047798 14340723 12936447 12301263 12303095 13881146 13223726 12404359 15406599 12113380 13460819 13393352 13327865 11840552 13945289 14555792 14045458 11502727 14505830 11756031 11800552 11868473 12271558 12962867 13184560 11798095 12833030 14959036 14932279 11576374 13667018 15142541 14819639 14906101 15104025 13293018 12456961 15083769 14255720 12649090 13208981 13929705 12355998 12691763 14233640 13578839 14210814 13034547 13674184 15410744 14378394 14153666 13847549 14984273 14897960 13156042 15295846 12501667 12237752 13611082 12389636 11722738 13638946 12773565 13638891 15106001 13836989 15444973 13688049 11622782 14010692 14983364 13478257 13078914 12742503 12404888 13154064 14451149 15334141 15315822 13764148 14331200 11929474 13869348 14438667 13202802 14057595 12739162 14645803 12182416 13163315 14522055 12581682 14791409 14821293 12030267 15040395 12394193 14041401 13381140 15244018 14646541 15330959 13399715 13602639 15312705 13777576 11627739 12888585 14830591 13295639 12631128 12038086 13064395 14185110 14717054 15198801 13832810 15206625 13594491 13456771 14778588 13508246 14838288 14291849 13483986 13235293 12516965 12298957 13439232 11978152 13746167 15129940 12459310 14574790 15003865 12653415 15463500 14311404 14197593 15202371 14288074 14923792 14541904 15206850 14303841 13474367 13911730 13619602 12107977 12287978 14740502 12542732 14094607 13552702 15292789 11927513 11646566 14629389 12518379 14708856 13149833 13027656 11601845 13418385 14437841 14438621 12056342 13303213 14730314 14353776 11786320"
    while (read_space_end_line(line,input,25)) {
        if (line_number == 102320) {
            printf("ciao");
        }
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

void reverse(uint32_t start, uint32_t end) {


    dynamic_array *result = malloc(sizeof(dynamic_array));
    init_array(result);
    set(0, start, result);
    int i = 1;
    station *temp = get_station(end);
    station *start_station = get_station(start);
    if (temp == NULL || start_station == NULL) {
        fputs("nessun percorso\n", output);
        free_array(result);
        return;
    }
    while (1) {
        if (end == start) {
            break;
        } else if (start_station->cars != NULL && temp->distance + start_station->cars->autonomy >= start) {
            set(i, temp->distance, result);
            i++;
            start = temp->distance;
            start_station = get_station(start);
            temp = get_station(end);
            continue;
        } else if (start_station->cars == NULL ||
                   start_station->distance - start_station->cars->autonomy > start_station->prev->distance) {
            fputs("nessun percorso\n", output);
            free_array(result);
            return;
        }
        temp = temp->next;
    }
    quicksort(result->array, 0, i - 1, 1);
    //print first element
    fprintf(output, "%d", get(0, result));
    for (int j = 1; j < i; j++) {
        fprintf(output, " %d", get(j, result));
    }
    fputs("\n", output);
    free_array(result);
}


void plan_path(char *line) {
    strtokindex = 19;
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input,15);
    uint32_t start = atoi(token);
    read_space_end_line(token, input,15);
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
        }if (temp->cars->autonomy + temp->distance >= end) {
            set(i, temp->distance, result);
            i++;
            end = temp->distance;
            temp = get_station(start);
            continue;
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
    read_space_end_line(token, input,15);
    uint32_t distance = atoi(token);
    read_space_end_line(token, input,15);
    uint32_t autonomy =  atoi(token);
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
    read_space_end_line(token, input,15);
    uint32_t distance = atoi(token);
    read_space_end_line(token, input,15);
    uint32_t autonomy =  atoi(token);
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
    read_space_end_line(token, input,15);
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

void go_to_next_line(FILE *file) {
    char c;
    while ((c = fgetc(file)) != '\n' && c != EOF) {}
}


void add_station(char *line) {
    //aggiungi-stazione <distanza> <numero auto> <autonomia-1> <autonomia-2> ... <autonomia-n> 1
    //first  we get_station the distance
    strtokindex = 18;
    //disgard the first token
    char *token = malloc(sizeof(char) * 15);
    read_space_end_line(token, input,15);
    uint32_t distance = atoi(token);
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
    read_space_end_line(token, input,15);
    uint16_t num_cars = atoi(token);
    uint32_t autonomy[num_cars];
    for (int i = 0; i < num_cars; ++i) {

        read_space_end_line(token, input,15);
        autonomy[i] = atoi(token);
    }
    if (num_cars > 1)
        quicksort(autonomy, 0, num_cars - 1, 0);
    if (num_cars==0){
        autonomy[0]=0;
        num_cars=1;
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


