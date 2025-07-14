#include "person.h"
#include <stdio.h>
#include <string.h>

int create_person(struct person *person, const char *name, unsigned age) {
    if (person == NULL || name == NULL)
        return 1;

    strcpy(person->name, name);
    person->age = age;

    return 0;
}


int update_age(struct person *person, int new_age) {
    if (person != NULL) {
        person->age = new_age;
        return 0;
    }

    return 1;
}

void show_person(struct person *person) {
    if (person != NULL)
        fprintf(stdout, "%s %u\n", person->name, person->age);
}