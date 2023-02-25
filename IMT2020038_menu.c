#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "libsys.h"

// Your code must support menu-driven functionality.
// Function calls rollno_libsys.h

int main()
{
    struct Book book;
    struct Student student;
    int rollno, isbn, status;
    char title[25], name[25];

    char *book_repo_name = "book";
    char *stud_repo_name = "stud";
    char *issue_repo_name = "issue";

    libsys_create(book_repo_name, stud_repo_name, issue_repo_name);

    printf("\n1.Add book \n");
    printf(" 2.Add student \n");
    printf(" 3.Delete Book \n");
    printf(" 4.Delete Student \n");
    printf(" 5.Issue Book \n");
    printf(" 6.Search Book by isbn \n");
    printf(" 7.Search Book by title \n");
    printf(" 8.Search student by rollno \n");
    printf(" 9.Search student by name \n");
    printf(" 10.Exit \n ");

    int input;
    scanf("%d", &input);
    while (input != 10)
    {
        switch (input)
        {
        case 1:
            printf(" \n Enter title: ");
            scanf("%s", book.title);
            printf(" \n Enter author: ");
            scanf("%s", book.author);
            printf(" \n Enter isbn: ");
            scanf("%d", &book.isbn);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            put_book_by_isbn(book.isbn, &book);
            libsys_close();
            break;

        case 2:
            printf("\nEnter name: ");
            scanf("%s", student.name);
            printf("\nEnter rollno: ");
            scanf("%d", &student.rollno);
            printf("\nEnter address: ");
            scanf("%s", student.address);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            put_student_by_rollno(student.rollno, &student);
            libsys_close();
            break;

        case 3:
            printf("\nEnter isbn: ");
            scanf("%d", &isbn);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            delete_book_by_isbn(isbn);
            libsys_close();
            break;

        case 4:
            printf("\nEnter rollno: ");
            scanf("%d", &rollno);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            delete_student_by_rollno(rollno);
            libsys_close();
            break;

        case 5:
            printf("\nEnter isbn: ");
            scanf("%d", &isbn);
            printf("\nEnter rollno: ");
            scanf("%d", &rollno);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            issue(rollno, isbn);
            libsys_close();
            break;

        case 6:
            printf("\nEnter isbn: ");
            scanf("%d", &isbn);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_book_by_isbn(isbn, &book);
            if (status == BOOK_SUCCESS)
            {
                printf("\n isbn = %d title = %s author = %s \n", book.isbn, book.title, book.author);
                rollno = issued_by(book.isbn);

                if (rollno == -1)
                    printf("\n Not issued");

                else
                {
                    printf("\n Issued");
                    get_student_by_rollno(rollno, &student);
                    printf("\n rollno = %d name = %s address = %s\n", student.rollno, student.name, student.address);
                }
            }
            else
                printf("\n Book not found");

            libsys_close();
            break;

        case 7:
            printf("\n Enter title: ");
            scanf("%s", title);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_book_by_title(title, &book);
            if (status == BOOK_SUCCESS)
            {
                printf("\n isbn = %d title = %s author = %s \n", book.isbn, book.title, book.author);
                rollno = issued_by(book.isbn);
                if (rollno == -1)
                    printf("\n Not issued ");

                else
                {
                    printf("\n Issued");
                    get_student_by_rollno(rollno, &student);
                    printf("\n rollno = %d name = %s address = %s\n", student.rollno, student.name, student.address);
                }
            }
            else
                printf("\n Book not found\n");

            libsys_close();
            break;

        case 8:
            printf("\n Enter rollno: ");
            scanf("%d", &rollno);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_student_by_rollno(rollno, &student);
            if (status == BOOK_SUCCESS)
                printf("\n rollno = %d name = %s address = %s\n", student.rollno, student.name, student.address);
            else
                printf("\n Student not found \n");
            libsys_close();
            break;

        case 9:
            printf("\n Enter name: ");
            scanf("%s", name);
            libsys_open(book_repo_name, stud_repo_name, issue_repo_name);
            status = get_student_by_name(name, &student);
            if (status == BOOK_SUCCESS)
                printf("\n rollno = %d name = %s address = %s\n", student.rollno, student.name, student.address);
            else
                printf("\n Student not found \n");
            libsys_close();
            break;
        }

        printf("\n1.Add book \n");
        printf(" 2.Add student \n");
        printf(" 3.Delete Book \n");
        printf(" 4.Delete Student \n");
        printf(" 5.Issue Book \n");
        printf(" 6.Search Book by isbn \n");
        printf(" 7.Search Book by title \n");
        printf(" 8.Search student by rollno \n");
        printf(" 9.Search student by name \n");
        printf(" 10.Exit \n ");

        scanf("%d", &input);
    }
}