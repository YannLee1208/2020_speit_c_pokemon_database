#include <stdio.h>
#include <memory.h>
#include "stdlib.h"


typedef struct pokemon {
    int id;
    char name[100];
    float height;
    char type1[100];
    char type2[100];
} pokemon, *pokemon_ptr, **pkdatabase;

typedef int (*Comparator)(int a, int b);

pkdatabase pkdb;
int size;
int capacity;
float maxHeight;
float minHeight;

pkdatabase initial_database(int len) {
    maxHeight = 0;
    minHeight = INT32_MAX;

    pkdb = (pkdatabase) malloc(sizeof(pokemon_ptr) * 2 * len);

    capacity = 2 * len;
    size = len;

    if (pkdb == NULL) {
        printf("Not Enough Memory");
        exit(-1);
    }

    int c;
    float height_tmp = 0;

    for (int i = 0; i < len; ++i) {
        pkdb[i] = (pokemon_ptr) malloc(sizeof(pokemon));
        if (pkdb[i] == NULL) {
            printf("Not Enough Memory");
            exit(-1);
        }

        scanf("%d", &(pkdb[i]->id));
        scanf("%s", pkdb[i]->name);
        scanf("%f", &height_tmp);
        pkdb[i]->height = height_tmp;

        if (height_tmp > maxHeight)
            maxHeight = height_tmp;
        if (height_tmp < minHeight)
            minHeight = height_tmp;

        getchar();
        scanf("%[^,\n]", pkdb[i]->type1);
        c = getchar();
        if (c == ',') {
            scanf("%s", pkdb[i]->type2);

            // 排除 type1 == type2
            if  (strcmp(pkdb[i]->type1, pkdb[i]->type2) == 0)
                strcpy(pkdb[i]->type2, "");
        }

        fflush(stdin);
    }

}

void _resize(int new_capacity) {
    capacity = new_capacity;
    pkdatabase pkdb_new = (pkdatabase) malloc(sizeof(pokemon_ptr) * new_capacity);

    for (int i = 0; i < size; ++i) {
        pkdb_new[i] = pkdb[i];
    }

    free(pkdb);
    pkdb = pkdb_new;

}

void _swap(int i, int j){
    pokemon_ptr tmp = pkdb[i];
    pkdb[i] = pkdb[j];
    pkdb[j] = tmp;
}

void _print_pkdb() {
    printf("size = %d, capacity = %d\n", size, capacity);
    for (int i = 0; i < size; ++i) {
        printf("%d, %s, %.3f, %s", pkdb[i]->id, pkdb[i]->name, pkdb[i]->height, pkdb[i]->type1);
        if (pkdb[i]->type2[0] != 0)
            printf(", %s\n", pkdb[i]->type2);
        else
            printf("\n");
    }

    printf("Maxheight = %.3f, Minheight = %.3f \n", maxHeight, minHeight);
    printf("\n");


}

int _find_id(int id){
    for (int i = 0; i < size; ++i) {
        if (pkdb[i]->id == id)
            return i;
    }

    return -1;
}

int insert(int id, char *name, float height, char *type) {

    char *type_tmp = malloc(sizeof(char ) * 100);
    strcpy(type_tmp, type);

    int index_tmp;
    char *tmp;

    if ((index_tmp = _find_id(id)) != -1){
        strcpy(pkdb[index_tmp]->name, name);
        pkdb[index_tmp]->height = height;

        tmp = strtok(type_tmp, ",");

        strcpy(pkdb[index_tmp]->type1, tmp);
        tmp = strtok(NULL, ",");
        if (tmp != NULL && strcmp(tmp, pkdb[index_tmp]->type1) != 0)
            strcpy(pkdb[index_tmp]->type2, tmp);

        return 0;
    }

    int res = 0;
    if (size == capacity) {
        _resize(capacity * 2);
        res = 1;
    }

    pkdb[size] = (pokemon_ptr) malloc(sizeof(pokemon));
    pkdb[size]->id = id;
    strcpy(pkdb[size]->name, name);
    pkdb[size]->height = height;

    if (height > maxHeight)
        maxHeight = height;
    if (height < minHeight)
        minHeight = height;

    tmp = strtok(type_tmp, ",");

    strcpy(pkdb[size]->type1, tmp);
    tmp = strtok(NULL, ",");
    if (tmp != NULL && strcmp(tmp, pkdb[size]->type1) != 0)
        strcpy(pkdb[size]->type2, tmp);

    size++;

    return res;


}

void _reset_max_min_height(int param){
    // if param == 0 reset maxHeight
    // if param == 1 reset minHeight

    switch (param) {
        case 0: {
            float max_height = 0;
            for (int i = 0; i < size; ++i) {
                if (pkdb[i]->height > max_height)
                    max_height = pkdb[i]->height;
            }

            maxHeight = max_height;
        }
        case 1:{
            float min_height = INT32_MAX;
            for (int i = 0; i < size; ++i) {
                if (pkdb[i]->height < min_height)
                   min_height = pkdb[i]->height;
            }

            minHeight = min_height;
        }

    }

}

int delete_id(int id){
    // 记录删除的高度，最大值最小值被删除时重新获取MaxHeight 和 Minheight

    float height_tmp = 0;
    for (int i = 0; i < size; ++i) {
        if (pkdb[i]->id == id){
            height_tmp = pkdb[i]->height;
            free(pkdb[i]);
            pkdb[i] = NULL;
            for (int j = i + 1; j < size; ++j) {
                pkdb[j - 1] = pkdb[j];
            }

            pkdb[--size] = NULL;

            if (size < capacity / 3)
                _resize(capacity / 2);

            if (-0.001 <( height_tmp - maxHeight) && ( height_tmp -maxHeight) < 0.001)
                _reset_max_min_height(0);

            if (-0.001 <( height_tmp - minHeight) && ( height_tmp -minHeight) < 0.001)
                _reset_max_min_height(1);

            printf("Done Successfully\n");
            return 0;
        }
    }

    printf("Pokemon of id %d doesn’t exist in the database.\n", id);
    return 1;
}

void find_height(int param){
    float height_to_find = 0;
    switch (param) {
        case 1:
            height_to_find = maxHeight;
            for (int i = 0; i < size; ++i) {
                if (-0.001 <(pkdb[i]->height - height_to_find) && (pkdb[i]->height - height_to_find) < 0.001)
                    printf("max height name = %s, height = %.3f\n", pkdb[i]->name, pkdb[i]->height);
            }
            break;
        case 2:
            height_to_find = minHeight;
            for (int i = 0; i < size; ++i) {
                if (-0.0001 <(pkdb[i]->height - height_to_find) && (pkdb[i]->height - height_to_find) < 0.0001)
                    printf("min height id = %s, height = %.3f\n", pkdb[i]->name, pkdb[i]->height);
            }
            break;

        default:
            printf("Invalid Parameter");
    }
}

int _partition( int left, int right, Comparator comparator1, Comparator comparator2) {
    int i, j;
    pokemon_ptr pivo = pkdb[ (left+right)/2 ];
    int pivo_id = pivo->id;
    i = left;
    j = right;
    while (1) {
        while (comparator1(pkdb[i]->id, pivo_id)){ i++; }
        while (comparator2(pkdb[j]->id, pivo_id)) { j--; }
        if (i < j)
            _swap( i, j);
        else
            break;
    }
    return i;
}

void _sort_id(int start, int end, Comparator comparator1, Comparator comparator2){
    int i;
    if( end > start )
    {
        i = _partition( start, end, comparator1, comparator2);
        _sort_id( start, i - 1, comparator1, comparator2);
        _sort_id( i + 1, end , comparator1, comparator2);
    }




}

int _ascending (int a, int b) { return b<a; }
int _descending (int a, int b) { return b>a; }

void sort_id_plus(int param){

    switch (param) {
        case 1:
            _sort_id(0, size - 1, &_descending, &_ascending);
            for (int j = 0; j < size; ++j) {
                printf("%s ", pkdb[j]->name);
            }
            printf("\n");
            break;
        case 2:
            _sort_id(0, size - 1, &_ascending, &_descending);
            for (int j = 0; j < size; ++j) {
                printf("%s ", pkdb[j]->name);
            }
            printf("\n");
            break;
        default:
            printf("Wrong Parameter, you can only enter 1 or 2, but given %d\n", param);
    }
}

void _free_source(){
    for (int i = 0; i < size; ++i) {
        free(pkdb[i]);
    }

    free(pkdb);
}

void _comparae_type(char *type1, char* type2, int type_num){
    if (type_num == 1) {
        for (int i = 0; i < size; ++i) {
            if (!(strcmp(pkdb[i]->type1, type1)) || !(strcmp(pkdb[i]->type2, type1)))
                printf("%s, ", pkdb[i]->name);
        }
        printf("\n");
    }


    else {
        for (int i = 0; i < size; ++i) {
            if (!(strcmp(pkdb[i]->type1, type1))) {
                if (!(strcmp(pkdb[i]->type2, type2)))
                    printf("%s ", pkdb[i]->name);
                else continue;
            } else {
                if (!(strcmp(pkdb[i]->type1, type2))) {
                    if (!(strcmp(pkdb[i]->type2, type1)))
                        printf("%s ", pkdb[i]->name);
                    else continue;
                } else continue;
            }
        }
        printf("\n");

    }
}

void group(char* type){
    char *type_tmp = malloc(sizeof(char) * 100);
    char *tmp1;
    char *tmp2;
    strcpy(type_tmp, type);

    int type_num = 1;
    tmp1 = strtok(type_tmp, ",");
    tmp2 = strtok(NULL, ",");
    if (tmp2 != NULL)
        type_num += 1;

    printf("%s :", type );
    _comparae_type(tmp1, tmp2, type_num);

    free(type_tmp);

}

int main() {

    printf("Enter Number:\n");
    int db_len = 0;
    int command_len = 0;
    scanf("%d", &db_len);
    initial_database(db_len);

    printf("Initialization Done, please enter your command number : \n");
    scanf("%d", &command_len);
    int command_id = 0;
    int command_paramater = 0;
    char *type = malloc(sizeof(char)* 100);
    for (int i = 0; i < command_len; ++i) {
        scanf("%d", &command_id);
        switch (command_id) {
            case 2:{
                scanf("%d", &command_paramater);
                delete_id(command_paramater);
                break;
            }
            case 3:
                scanf("%d", &command_paramater);
                find_height(command_paramater);
                break;
            case 4:
                sort_id_plus(1);
                break;
            case 5:
                scanf("%d", &command_paramater);
                sort_id_plus(command_paramater);
                break;
            case 6:
                scanf("%s", type);
                group(type);
                break;
        }
    }

    free(type);
    _free_source();

//    insert(10, "gg", 0.03, "eat,fly");
//    insert(13, "dddd", 0.9, "eat,eat");
//    insert(17, "hh", 0.01, "eat");
//    insert(9, "ff", 0.20, "eat,poison");
//    insert(20, "mm", 0.30, "drink");
//    insert(70, "qq", 30, "fly,drink");

//    _print_pkdb();
//
//    sort_id_plus(1);
//
//    _print_pkdb();
//
//    group("eat");
//
//    group("eat,fly");
//    group("drink,fly");
////    sort_id_plus(2);
////
////    _print_pkdb();
//
//    delete_id(17);
////
//    _print_pkdb();
////
//    delete_id(9);
////
//    _print_pkdb();
//
//    delete_id(70);
//
//    _print_pkdb();
//
//    delete_id(10);
//    _print_pkdb();
//
//    delete_id(13);
//    _print_pkdb();
//
//    find_height(1);
//    find_height(2);
//
//
//    _free_source();



}
