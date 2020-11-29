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

// type of function pointer
typedef int (*Comparator)(int a, int b);

/* Definition of global variables
 * pkdb : pokemon database which saves the pointer to every pokemon record
 * size : the number of records in the database
 * capacity : the capacity (Max length) of database
 * maxHeight : maximum of heights of pokemon in the database
 * minHeight : minimum of heights of pokemon in the database
 */

pkdatabase pkdb;
int size;
int capacity;
float maxHeight;
float minHeight;


pkdatabase initial_database(int len) {
    /*
     * Initialization of pkdb, we assume here all the records are legal, which means ids are different. And if user
     * has inputed a wrong record, he can replace it by insert function.
     *
     * param:
     *    len : length of records user want to input
     */

    maxHeight = 0;
    minHeight = (float) INT32_MAX;

    // We init the database by 2*len to reduce the resize times
    pkdb = (pkdatabase) malloc(sizeof(pokemon_ptr) * 2 * len);


    capacity = 2 * len;
    size = len;

    // Fail to allocate the memories
    if (pkdb == NULL) {
        printf("Not Enough Memory");
        exit(-1);
    }

    // c is used to help judge whether there are two types
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
            // exclude case when type1 == type2
            if (strcmp(pkdb[i]->type1, pkdb[i]->type2) == 0)
                strcpy(pkdb[i]->type2, "");
        }

        // clear buffer zone
        fflush(stdin);
    }

}

void _resize(int new_capacity) {
    /*
     * Function to resize database
     * param:
     *  new_capacity: The new capacity of database, which can be bigger or smaller
     */

    capacity = new_capacity;
    pkdatabase pkdb_new = (pkdatabase) malloc(sizeof(pokemon_ptr) * new_capacity);

    for (int i = 0; i < size; ++i) {
        pkdb_new[i] = pkdb[i];
    }

    free(pkdb);
    pkdb = pkdb_new;

}

void _print_pkdb() {
    /*
     * Auxiliary function to print all the records in database
     */

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


int _find_id(int id) {
    /*
     * Auxiliary function used to find whether the id is recorded in the database
     * param :
     *  id : id to find
     *
     * return :
     *  if exists, return index of the record
     *  else, return -1
     */

    for (int i = 0; i < size; ++i) {
        if (pkdb[i]->id == id)
            return i;
    }

    return -1;
}

int insert(int id, char *name, float height, char *type) {
    /*
     * Insert a record into database
     *
     * return:
     *   0 if successful. Return 1 if the max size of the database is exceeded.
     */

    // copy of type because it can be a const char*, which fail to be used in strtok function
    char *type_tmp = malloc(sizeof(char) * 100);
    strcpy(type_tmp, type);

    int index_tmp;
    char *tmp;

    // If the id has existed, we replace the old record by new one.
    if ((index_tmp = _find_id(id)) != -1) {
        strcpy(pkdb[index_tmp]->name, name);
        pkdb[index_tmp]->height = height;

        tmp = strtok(type_tmp, ",");

        strcpy(pkdb[index_tmp]->type1, tmp);
        tmp = strtok(NULL, ",");
        if (tmp == NULL || strcmp(tmp, pkdb[index_tmp]->type1) == 0) {
            strcpy(pkdb[index_tmp]->type2, "");
            return 0;
        }

        strcpy(pkdb[index_tmp]->type2, tmp);
        printf("You have replaced the record of id = %d\n", id);
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

    // update maxHeight and minHeight
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

void _reset_max_min_height(int param) {
    /*
     * Auxiliary function to reset max or min height when deleting record
     * param :
     *  0 : reset maxHeight
     *  1 : reset minHeight
     */

    switch (param) {
        case 0: {
            float max_height = 0;
            for (int i = 0; i < size; ++i) {
                if (pkdb[i]->height > max_height)
                    max_height = pkdb[i]->height;
            }

            maxHeight = max_height;
        }
        case 1: {
            float min_height = INT32_MAX;
            for (int i = 0; i < size; ++i) {
                if (pkdb[i]->height < min_height)
                    min_height = pkdb[i]->height;
            }

            minHeight = min_height;
        }

    }

}

int delete_id(int id) {
    /*
     * Function to delete a record by it's id
     * param:
     *  id : id of record needing to be deleted
     *
     * return :
     *  0 : if deleted successfully
     *  1 : if not
     */

    // Save the height of deleted record in case it's the maximum or minimum
    float height_tmp = 0;

    for (int i = 0; i < size; ++i) {
        if (pkdb[i]->id == id) {
            height_tmp = pkdb[i]->height;
            free(pkdb[i]);
            pkdb[i] = NULL;
            for (int j = i + 1; j < size; ++j) {
                pkdb[j - 1] = pkdb[j];
            }

            pkdb[--size] = NULL;

            // if size is too small, we resize the database
            if (size < capacity / 3)
                _resize(capacity / 2);

            if (-0.001 < (height_tmp - maxHeight) && (height_tmp - maxHeight) < 0.001)
                _reset_max_min_height(0);

            if (-0.001 < (height_tmp - minHeight) && (height_tmp - minHeight) < 0.001)
                _reset_max_min_height(1);

            printf("Done Successfully\n");
            return 0;
        }
    }

    printf("Pokemon of id %d doesn’t exist in the database.\n", id);
    return 1;
}

void find_height(int param) {
    /*
     * funtion to find pokemons with maxHeight or minHeight
     * param :
     *  1 : maxHeight
     *  2 : minHeight
     */

    switch (param) {
        case 1:
            printf("max height is %.3f, pokemon name =", maxHeight);
            for (int i = 0; i < size; ++i) {
                if (-0.001 < (pkdb[i]->height - maxHeight) && (pkdb[i]->height - maxHeight) < 0.001)
                    printf(" %s", pkdb[i]->name);
            }
            printf("\n");
            break;
        case 2:
            printf("min height is %.3f, , pokemon name =", minHeight);
            for (int i = 0; i < size; ++i) {
                if (-0.0001 < (pkdb[i]->height - minHeight) && (pkdb[i]->height - minHeight) < 0.0001)
                    printf(" %s", pkdb[i]->name);
            }
            printf("\n");
            break;

        default:
            printf("Invalid Parameter\n");
    }
}

void _swap(int i, int j) {
    /*
     * Auxiliary function to swap record
     * param:
     *  i, j : index of records need to swap
     */
    pokemon_ptr tmp = pkdb[i];
    pkdb[i] = pkdb[j];
    pkdb[j] = tmp;
}

int _partition(int left, int right, Comparator comparator1, Comparator comparator2) {
    int i, j;
    pokemon_ptr pivo = pkdb[(left + right) / 2];
    int pivo_id = pivo->id;
    i = left;
    j = right;
    while (1) {
        while (comparator1(pkdb[i]->id, pivo_id)) { i++; }
        while (comparator2(pkdb[j]->id, pivo_id)) { j--; }
        if (i < j)
            _swap(i, j);
        else
            break;
    }
    return i;
}

void _sort_id(int start, int end, Comparator comparator1, Comparator comparator2) {
    /*
     * sort function
     */

    int i;
    if (end > start) {
        i = _partition(start, end, comparator1, comparator2);
        _sort_id(start, i - 1, comparator1, comparator2);
        _sort_id(i + 1, end, comparator1, comparator2);
    }


}

int _ascending(int a, int b) { return b < a; }

int _descending(int a, int b) { return b > a; }

void sort_id_plus(int param) {
    /*
     * function to sort id
     * param :
     *  1 : ascending way
     *  2 : descending way
     */

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

void _free_source() {
    /*
     * Auxiliary function to free all the memories we allocate
     */

    for (int i = 0; i < size; ++i) {
        free(pkdb[i]);
    }

    free(pkdb);
}

void _comparae_type(char *type1, char *type2, int type_num) {
    /*
     * Auxiliary funtion to compare the type of records to type1 and type2
     * param:
     *  type_num: 1 means only compare type1
     *            2 means we need to compare two types
     */

    if (type_num == 1) {
        for (int i = 0; i < size; ++i) {
            if (!(strcmp(pkdb[i]->type1, type1)) || !(strcmp(pkdb[i]->type2, type1)))
                printf(" %s, ", pkdb[i]->name);
        }
        printf("\n");
    } else {
        for (int i = 0; i < size; ++i) {
            if (!(strcmp(pkdb[i]->type1, type1))) {
                if (!(strcmp(pkdb[i]->type2, type2)))
                    printf(" %s", pkdb[i]->name);
                else continue;
            } else {
                if (!(strcmp(pkdb[i]->type1, type2))) {
                    if (!(strcmp(pkdb[i]->type2, type1)))
                        printf(" %s", pkdb[i]->name);
                    else continue;
                } else continue;
            }
        }
        printf("\n");

    }
}

void group(char *type) {
    /*
     * function to group pokemon by types, print all the pokemons contain this type.
     * param :
     *   type to group, which can by 1 type or 2, the order doesn't matter
     *
     */

    char *type_tmp = malloc(sizeof(char) * 100);
    char *tmp1;
    char *tmp2;
    strcpy(type_tmp, type);

    int type_num = 1;
    tmp1 = strtok(type_tmp, ",");
    tmp2 = strtok(NULL, ",");
    if (tmp2 != NULL)
        type_num += 1;

    printf("%s : ", type);
    _comparae_type(tmp1, tmp2, type_num);

    free(type_tmp);

}

int main() {

    printf("Enter record number you want to input :\n");
    int db_len = 0;
    int command_len = 0;
    scanf("%d", &db_len);
    initial_database(db_len);

    _print_pkdb();
    printf("Initialization Done, please enter your command number : \n");
    scanf("%d", &command_len);
    printf("Now please enter you command\n"
           "1 represents insert a pokemon and you need to input it's id, name, height, type in order\n"
           "2 represents delete a pokemon and you need to input it's id\n"
           "3 represents find pokemons with max height or min, and 3 1 represents max,3 2 min\n"
           "4 represents sorting the database with id-ascending order"
           "5 represents sorting the database with id-ascending order (param=1) or id-descending order (param=2)\n"
           "6 represents printing the name of all the pokemons belonging to the given type.\n");

    int command_id = 0;
    int command_paramater = 0;
    int id_tmp = 0;
    char *name = malloc(sizeof(char) * 100);
    char *type = malloc(sizeof(char) * 100);
    float height = 0;
    for (int i = 0; i < command_len; ++i) {
        scanf("%d", &command_id);
        switch (command_id) {
            case 1:{
                scanf("%d", &id_tmp);
                scanf("%s", name);
                scanf("%f", &height);
                scanf("%s", type);
                insert(id_tmp, name, height, type);
                fflush(stdin);
//                _print_pkdb();
                break;
            }
            case 2: {
                scanf("%d", &command_paramater);
                delete_id(command_paramater);
//                _print_pkdb();
                break;
            }
            case 3:
                scanf("%d", &command_paramater);
                find_height(command_paramater);
                break;
            case 4:
                sort_id_plus(1);
//                _print_pkdb();
                break;
            case 5:
                scanf("%d", &command_paramater);
                sort_id_plus(command_paramater);
//                _print_pkdb();
                break;
            case 6:
                scanf("%s", type);
                group(type);
                break;
        }
    }

    free(name);
    free(type);
    _free_source();

}
