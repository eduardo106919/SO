#ifndef PERSON_H
#define PERSON_H

#define NAME_SIZE 30

struct person {
    char name[NAME_SIZE];
    unsigned age;
};

/* Creates a person with name and age */
int create_person(struct person *person, const char *name, unsigned age);

/* Updates a person's age */
int update_age(struct person *person, int new_age);

/* Shows a person to stdout */
void show_person(struct person *person);



#endif
