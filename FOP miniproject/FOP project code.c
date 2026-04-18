#include <stdio.h>

// Structure to store book details
struct library {
    int book_id;          // unique book id
    char book_name[50];   // name of book
    char author[50];      // author name
    int status;           // 0 = Available, 1 = Issued
};

int main() {

    struct library b;   // structure variable for one book record

    FILE *fp;           // main file pointer (library.txt)
    FILE *temp;         // temporary file pointer (used for update/delete)

    int choice;         // stores menu choice entered by user
    int id;             // used to search book by id
    int found;          // flag variable (0 = not found, 1 = found)

    // loop until user chooses exit
    do {

        // ===== MENU =====
        printf("\n===== LIBRARY MENU =====");
        printf("\n1. Add Book");
        printf("\n2. Display Books");
        printf("\n3. Search Book");
        printf("\n4. Issue Book");
        printf("\n5. Return Book");
        printf("\n6. Delete Book");
        printf("\n7. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch(choice) {

        // ================= ADD BOOK =================
        case 1:
            fp = fopen("library.txt", "ab");   // open file in append mode

            printf("Enter ID: ");
            scanf("%d", &b.book_id);

            printf("Enter Name: ");
            scanf(" %[^\n]", b.book_name);

            printf("Enter Author: ");
            scanf(" %[^\n]", b.author);

            b.status = 0;   // new book is available by default

            fwrite(&b, sizeof(b), 1, fp);  // write structure to file
            fclose(fp);

            printf("Book Added Successfully!\n");
            break;


        // ================= DISPLAY BOOKS =================
        case 2:
            fp = fopen("library.txt", "rb");   // open file in read mode

            printf("\n----- Book List -----\n");

            // read all records one by one
            while(fread(&b, sizeof(b), 1, fp)) {

                printf("\nID: %d\nName: %s\nAuthor: %s\n",
                       b.book_id, b.book_name, b.author);

                // display book status
                if(b.status == 0)
                    printf("Status: Available\n");
                else
                    printf("Status: Issued\n");
            }

            fclose(fp);
            break;


        // ================= SEARCH BOOK =================
        case 3:
            printf("Enter ID to search: ");
            scanf("%d", &id);

            fp = fopen("library.txt", "rb");
            found = 0;   // initially not found

            // search record
            while(fread(&b, sizeof(b), 1, fp)) {

                if(b.book_id == id) {
                    printf("\nBook Found!");
                    printf("\nName: %s\nAuthor: %s\n", b.book_name, b.author);

                    if(b.status == 0)
                        printf("Status: Available\n");
                    else
                        printf("Status: Issued\n");

                    found = 1;   // record found
                }
            }

            if(!found)
                printf("Book not found!\n");

            fclose(fp);
            break;


        // ================= ISSUE BOOK =================
        case 4:
            printf("Enter ID to issue: ");
            scanf("%d", &id);

            fp = fopen("library.txt", "rb");    // read old file
            temp = fopen("temp.txt", "wb");     // write updated data

            // copy records and update selected record
            while(fread(&b, sizeof(b), 1, fp)) {

                if(b.book_id == id && b.status == 0) {
                    b.status = 1;   // mark as issued
                    printf("Book Issued Successfully!\n");
                }

                fwrite(&b, sizeof(b), 1, temp);
            }

            fclose(fp);
            fclose(temp);

            remove("library.txt");             // delete old file
            rename("temp.txt", "library.txt"); // rename temp file
            break;


        // ================= RETURN BOOK =================
        case 5:
            printf("Enter ID to return: ");
            scanf("%d", &id);

            fp = fopen("library.txt", "rb");
            temp = fopen("temp.txt", "wb");

            while(fread(&b, sizeof(b), 1, fp)) {

                if(b.book_id == id && b.status == 1) {
                    b.status = 0;   // mark as available
                    printf("Book Returned Successfully!\n");
                }

                fwrite(&b, sizeof(b), 1, temp);
            }

            fclose(fp);
            fclose(temp);

            remove("library.txt");
            rename("temp.txt", "library.txt");
            break;


        // ================= DELETE BOOK =================
        case 6:
            printf("Enter ID to delete: ");
            scanf("%d", &id);

            fp = fopen("library.txt", "rb");
            temp = fopen("temp.txt", "wb");

            found = 0;

            // copy all except deleted record
            while(fread(&b, sizeof(b), 1, fp)) {

                if(b.book_id != id)
                    fwrite(&b, sizeof(b), 1, temp);
                else
                    found = 1;
            }

            fclose(fp);
            fclose(temp);

            remove("library.txt");
            rename("temp.txt", "library.txt");

            if(found)
                printf("Book Deleted Successfully!\n");
            else
                printf("Book not found!\n");

            break;

        }

    } while(choice != 7);   // repeat menu until exit

    return 0;
}
