#include <stdio.h>
#include <string.h>

struct Contact {
    char name[50];
    char phone[15];
    char email[50];
};

void addContact(FILE *file) {
    struct Contact contact;

    printf("Enter Name: ");
    scanf(" %[^\n]", contact.name);
    printf("Enter Phone: ");
    scanf(" %[^\n]", contact.phone);
    printf("Enter Email: ");
    scanf(" %[^\n]", contact.email);

    fwrite(&contact, sizeof(struct Contact), 1, file);
    printf("Contact added!\n");
}

void viewContacts(FILE *file) {
    struct Contact contact;
    rewind(file); // Move to the start of the file

    printf("\nContacts List:\n");
    while (fread(&contact, sizeof(struct Contact), 1, file)) {
        printf("Name: %s\nPhone: %s\nEmail: %s\n", contact.name, contact.phone, contact.email);
    }
}

void editContact(FILE *file) {
    struct Contact contact;
    char name[50];
    int found = 0;

    printf("Enter Name to Edit: ");
    scanf(" %[^\n]", name);

    rewind(file);
    while (fread(&contact, sizeof(struct Contact), 1, file)) {
        if (strcmp(contact.name, name) == 0) {
            found = 1;
            printf("Enter New Name: ");
            scanf(" %[^\n]", contact.name);
            printf("Enter New Phone: ");
            scanf(" %[^\n]", contact.phone);
            printf("Enter New Email: ");
            scanf(" %[^\n]", contact.email);

            fseek(file, -sizeof(struct Contact), SEEK_CUR);
            fwrite(&contact, sizeof(struct Contact), 1, file);
            printf("Contact updated!\n");
            break;
        }
    }
    if (!found) {
        printf("Contact not found!\n");
    }
}

void deleteContact(FILE *file) {
    struct Contact contact;
    FILE *tempFile = fopen("temp.dat", "wb");
    char name[50];
    int found = 0;

    printf("Enter Name to Delete: ");
    scanf(" %[^\n]", name);

    rewind(file);
    while (fread(&contact, sizeof(struct Contact), 1, file)) {
        if (strcmp(contact.name, name) == 0) {
            found = 1;
            printf("Contact deleted!\n");
        } else {
            fwrite(&contact, sizeof(struct Contact), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);
    remove("contacts.dat");
    rename("temp.dat", "contacts.dat");

    if (!found) {
        printf("Contact not found!\n");
    }
}

int main() {
    FILE *file = fopen("contacts.dat", "rb+");
    if (file == NULL) {
        file = fopen("contacts.dat", "wb+");
    }

    int choice;
    do {
        printf("\n1. Add Contact\n2. View Contacts\n3. Edit Contact\n4. Delete Contact\n5. Exit\nChoose: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addContact(file); break;
            case 2: viewContacts(file); break;
            case 3: editContact(file); break;
            case 4: deleteContact(file); break;
            case 5: fclose(file); printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 5);

    return 0;
}
