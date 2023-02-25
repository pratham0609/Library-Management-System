#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "libsys.h"

// Define the global variable of libsys
struct LibsysInfo repo_handle;

//--------------------------------create()---------------------------------
int libsys_create(char *repo_name1, char *repo_name2, char *repo_name3)
{
    // create booksys file pointer to open a file in 'wb' mode
    // handle file pointer error if value is NULL return appropriate error code referring to the error codes
    // close file pointer
    FILE *booksys_file = fopen(repo_name1, "wb");
    if (booksys_file == NULL)
        return LIB_FILE_ERROR;
    fclose(booksys_file);

    // create studsys file pointer to open a file in 'wb' mode
    // handle file pointer error if value is NULL return appropriate error code referring to the error codes
    // close file pointer
    FILE *studsys_file = fopen(repo_name2, "wb");
    if (studsys_file == NULL)
        return LIB_FILE_ERROR;
    fclose(studsys_file);

    // create issuesys file pointer to open a file in 'wb' mode
    // handle file pointer error if value is NULL return appropriate error code referring to the error codes
    // close file pointer
    FILE *issuesys_file = fopen(repo_name3, "wb");
    if (issuesys_file == NULL)
        return LIB_FILE_ERROR;
    fclose(issuesys_file);

    // Open the index file in "wb" mode
    // Initialize index file by store "0" to indicate there are zero entries in index file
    FILE *index_file = fopen("index.ndx", "wb");
    if (index_file == NULL)
        return LIB_FILE_ERROR;
    int i = 0;
    fwrite(&i, sizeof(int), 1, index_file);
    fclose(index_file);

    // return success
    return LIB_SUCCESS;
}
//--------------------------------open()---------------------------------
int libsys_open(char *book_name, char *stud_name, char *issue_name)
{
    if (booksys_open(book_name) != BOOK_SUCCESS)
        return LIB_FILE_ERROR;

    if (studsys_open(stud_name) != BOOK_SUCCESS)
        return LIB_FILE_ERROR;

    if (issuesys_open(issue_name) != BOOK_SUCCESS)
        return LIB_FILE_ERROR;

    FILE *index_file = fopen("index.ndx", "rb+");
    repo_handle.libsys_ndx_fp = index_file;

    int i = fread(&repo_handle.index_count, sizeof(int), 1, repo_handle.libsys_ndx_fp);
    if (i != 1)
        return LIB_FILE_ERROR;

    int j = fread(&repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, repo_handle.libsys_ndx_fp);
    if (j != repo_handle.index_count)
        return LIB_FILE_ERROR;

    fclose(repo_handle.libsys_ndx_fp);
    return LIB_SUCCESS;
}
// call booksys_open()
// call studsys_open()
// call issuesys_open()

// Open the index file in rb+ mode
// Read number of index entries from index file
// Load index_entries array by reading index entries from the index 	file
// Close only the index file

// check status of above functions
//  if all of them return success then return SUCCESS else return ERROR

int booksys_open(char *repo_name)
{
    repo_handle.booksys_data_fp = fopen(repo_name, "rb+");
    if (repo_handle.booksys_data_fp == NULL)
        return LIB_FILE_ERROR;

    strcpy(repo_handle.booksys_name, repo_name);
    repo_handle.book_repo_status = LIB_REPO_OPEN;

    return BOOK_SUCCESS;
}

// 1. assign repo handle a file pointer by opening file in 'rb+' mode
// 2. handle file pointer error if value is NULL return appropriate error code referring to the error codes in libsys.h
// 3. assign values (repo_name) to booksys_repo_handle
// 4. assign value to repo_handle.book_repo_status=LIB_REPO_OPEN;
// 5. return booksys_success

int issuesys_open(char *repo_name)
{
    repo_handle.issuesys_data_fp = fopen(repo_name, "rb+");
    if (repo_handle.issuesys_data_fp == NULL)
        return LIB_FILE_ERROR;

    strcpy(repo_handle.issuesys_name, repo_name);
    repo_handle.issue_repo_status = LIB_REPO_OPEN;

    return BOOK_SUCCESS;
}

// 1. assign repo handle a file pointer by opening file in 'rb+' mode
// 2. handle file pointer error if value is NULL return appropriate error code referring to the error codes in libsys.h
// 3. assign values (repo_name) to issuesys_repo_handle
// 4. assign value to repo_handle.issue_repo_status=LIB_REPO_OPEN;
// 5. return issuesys_success

int studsys_open(char *repo_name)
{
    if (repo_handle.stud_repo_status == LIB_REPO_OPEN)
        return LIB_REPO_ALREADY_OPEN;

    char student_file[25];
    char student_issue_file[25];
    strcpy(student_file, repo_name);
    strcpy(student_issue_file, repo_name);
    strcat(student_file, ".dat");
    strcat(student_issue_file, ".ndx");

    strcpy(repo_handle.studsys_name, repo_name);
    repo_handle.studsys_data_fp = fopen(student_file, "rb+");
    if (repo_handle.studsys_data_fp == NULL)
        return LIB_FILE_ERROR;
    repo_handle.stud_repo_status = LIB_REPO_OPEN;

    repo_handle.studsys_ndx_fp = fopen(student_issue_file, "rb+");
    if (repo_handle.studsys_ndx_fp == NULL)
        return LIB_FILE_ERROR;
    if (fread(&repo_handle.student_index_count, sizeof(int), 1, repo_handle.studsys_ndx_fp) != 1)
        return LIB_FILE_ERROR;
    if (fread(repo_handle.stud_index_entries, sizeof(struct LIB_NdxInfo), repo_handle.student_index_count, repo_handle.studsys_ndx_fp) != repo_handle.student_index_count)
        return LIB_FILE_ERROR;
    fclose(repo_handle.studsys_ndx_fp);
    return LIB_SUCCESS;
}

// 1. assign repo handle a file pointer by opening file in 'rb+' mode
// 2. handle file pointer error if value is NULL return appropriate error code referring to the error codes
// 3. assign values (repo_name) to studsys_repo_handle
// 4. assign value to repo_handle.stud_repo_status=LIB_REPO_OPEN;
// 5. return studsys_success

//------------------------------book:NO change ----------------------------

int get_book_by_isbn(int key, struct Book *rec)
{
    if (repo_handle.repo_status == LIB_REPO_CLOSED)
        return LIB_FILE_ERROR;

    int position = -1;

    for (int i = 0; i < repo_handle.index_count; i++)
    {
        if (repo_handle.index_entries[i].key == key && repo_handle.index_entries[i].flag == RECORD_PRESENT)
            position = repo_handle.index_entries[i].offset;
    }

    if (position == -1)
        return BOOK_FAILURE;

    fseek(repo_handle.libsys_data_fp, position, SEEK_SET);
    int isbn = getw(repo_handle.libsys_data_fp);
    if (key == isbn)
    {
        fread(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp);
        return BOOK_SUCCESS;
    }
    return LIB_REC_NOT_FOUND;
}
// get_rec_by_key
// check repo status
// Search for index entry in index_entries array
//-----use flag to read valid entries
// Seek to the file location based on offset in index entry
// Read the key at the current file location
// Read the record after reading the key

int put_book_by_isbn(int key, struct Book *rec)
{
    if (repo_handle.repo_status == LIB_REPO_CLOSED)
        return LIB_FILE_ERROR;

    for (int i = 0; i < repo_handle.index_count; i++)
    {
        if (repo_handle.index_entries[i].key == key)
        {
            if (repo_handle.index_entries[i].flag == RECORD_DELETED)
            {
                fseek(repo_handle.libsys_data_fp, 0, SEEK_END);
                int offset = ftell(repo_handle.libsys_data_fp);
                int i = putw(key, repo_handle.libsys_data_fp);
                int j = fwrite(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp);

                repo_handle.index_entries[i].flag = RECORD_PRESENT;
                repo_handle.index_entries[i].offset = offset;

                if (i == 0 && j != 0)
                    return BOOK_SUCCESS;

                return LIB_ADD_FAILED;
            }
            else
                return LIB_ADD_FAILED;
        }
    }

    fseek(repo_handle.libsys_data_fp, 0, SEEK_END);
    int offset = ftell(repo_handle.libsys_data_fp);

    repo_handle.index_entries[repo_handle.index_count].key = key;
    repo_handle.index_entries[repo_handle.index_count].offset = offset;
    repo_handle.index_entries[repo_handle.index_count].flag = RECORD_PRESENT;

    repo_handle.index_count++;

    int i = putw(key, repo_handle.libsys_data_fp);

    int j = fwrite(rec, sizeof(struct Book), 1, repo_handle.libsys_data_fp);

    if (i == 0 && j != 0)
        return BOOK_SUCCESS;

    // printf("%d\n",BOOK_FAILURE);
    return LIB_ADD_FAILED;
}
//-----check index file for key
//-----if key already present check for flag [flag=1 : entry is valid]
//-----if key is present but flag is 0 then just add entry at same index i.e update only offset and update flag; return status
//-----if key is present but flag is 1 return failure as data is already present
//-----if key is not prsent then proceed with following steps:

// Seek to the end of the data file
// Create an index entry with the current data file location using ftell
// Add index entry to array using offset returned by ftell
// Write the key at the current data file location
// Write the record after writing the key
// return status

int delete_book_by_isbn(int key)
{
    if (repo_handle.repo_status == LIB_REPO_CLOSED)
        return LIB_FILE_ERROR;

    for (int i = 0; i < repo_handle.index_count; i++)
    {
        if (repo_handle.index_entries[i].key == key && repo_handle.index_entries[i].flag == RECORD_PRESENT)
        {
            repo_handle.index_entries[i].flag = RECORD_DELETED;
            return LIB_SUCCESS;
        }
    }
    return LIB_REC_NOT_FOUND;
}

//-----delete_rec_by_key
//-----check repo status
//-----Search for index entry in index_entries array
//-----if key matches and flag is 1 then reset flag
//-----if key matches but flag is already reset return status
//-----if key doesn't match then return status

// helper functions --
int isbn_isssued(int rollno) // returns isbn of book issued by student
{
    struct Issue issue;
    if (repo_handle.issue_repo_status == LIB_REPO_OPEN)
    {
        fseek(repo_handle.issuesys_data_fp, 0, SEEK_SET);
        fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
        while (feof(repo_handle.issuesys_data_fp) == 0)
        {
            if (issue.rollno == rollno)
                return issue.isbn;
            fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
        }
    }
    return -1;
}

int issued_by(int isbn) // returns rollno of student who has issued book
{
    struct Issue issue;
    if (repo_handle.issue_repo_status == LIB_REPO_OPEN)
    {
        fseek(repo_handle.issuesys_data_fp, 0, SEEK_SET);
        fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
        while (feof(repo_handle.issuesys_data_fp) == 0)
        {
            if (issue.isbn == isbn)
                return issue.rollno;
            fread(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp);
        }
    }
    return -1;
}

//-----------------------------student: additional code---------------------------
int put_student_by_rollno(int rollno_to_write, struct Student *rec)
{
    if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    int i;
    for (i = 0; i < repo_handle.student_index_count; i++)
    {
        struct LIB_NdxInfo struct_entry = repo_handle.stud_index_entries[i];
        if (struct_entry.key == rollno_to_write)
        {
            if (struct_entry.flag == RECORD_DELETED)
            {
                repo_handle.stud_index_entries[i].flag = RECORD_PRESENT;
                fseek(repo_handle.studsys_data_fp, struct_entry.offset, SEEK_SET);
                fseek(repo_handle.studsys_data_fp, sizeof(int), SEEK_CUR);
                if (fwrite(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp) != 1)
                    return LIB_FILE_ERROR;
                return LIB_SUCCESS;
            }
            else
                return LIB_ADD_FAILED;
        }
    }
    fseek(repo_handle.studsys_data_fp, 0, SEEK_END);
    int loc = ftell(repo_handle.studsys_data_fp);

    if (fwrite(&rollno_to_write, sizeof(int), 1, repo_handle.studsys_data_fp) != 1)
        return LIB_FILE_ERROR;

    if (fwrite(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp) != 1) // add the student
        return LIB_FILE_ERROR;

    struct LIB_NdxInfo index = {rollno_to_write, loc, RECORD_PRESENT};
    repo_handle.stud_index_entries[repo_handle.student_index_count++] = index; // add index entry

    return LIB_SUCCESS;
}
// 1. check if repo status is closed then return return appropriate error code referring to the error codes
// 2. else continue with following action sequence
// 3. set the file pointer to end
// 4. write rollno_to_write
// 5. write Student record
// 6. if both actions are successful then return studsys_success
// 7. else return studsys_add_failed

int get_student_by_rollno(int rollno_to_read, struct Student *rec)
{
    if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    int i;
    int loc = -1;
    for (i = 0; i < repo_handle.student_index_count; i++)
    {
        struct LIB_NdxInfo struct_entry = repo_handle.stud_index_entries[i];
        if (struct_entry.key == rollno_to_read)
        {
            if (struct_entry.flag == RECORD_PRESENT)
            {
                loc = struct_entry.offset;
                break;
            }
            else
                return LIB_REC_NOT_FOUND;
        }
    }
    if (loc == -1)
        return LIB_REC_NOT_FOUND;

    fseek(repo_handle.studsys_data_fp, loc, SEEK_SET);
    int rolln;

    if (fread(&rolln, sizeof(int), 1, repo_handle.studsys_data_fp) != 1)
        return LIB_FILE_ERROR;
    if (rolln != rollno_to_read)
        return LIB_REC_NOT_FOUND;
    if (fread(rec, sizeof(struct Student), 1, repo_handle.studsys_data_fp) != 1)
        return LIB_FILE_ERROR;

    return LIB_SUCCESS;
}
int delete_student_by_rollno(int rollno)
{

    if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    int i;
    for (i = 0; i < repo_handle.student_index_count; i++)
    {
        struct LIB_NdxInfo struct_entry = repo_handle.stud_index_entries[i];
        if (struct_entry.key == rollno)
        {
            if (struct_entry.flag == RECORD_PRESENT)
            {
                repo_handle.stud_index_entries[i].flag = RECORD_DELETED;
                return LIB_SUCCESS;
            }
            else
                return LIB_REC_NOT_FOUND;
        }
    }
    return LIB_REC_NOT_FOUND;
}

// 1. check if repo status is closed then  return appropriate error code referring to the error codes
// 2. else continue with following action sequence

// 3.1 read rollno
// 3.2 check if rollno is equal to the rollno_to_read
// 3.3 if yes then read entire record of a Student and return studsys_success
// 3.4 else skip the record and read next rollno of the Student
// 4. Repeat step 4.1 to 4.4 till end of file
// 5. Return record not found : appropriate error code referring to the error codes

//--------------------------------issue()---------------------------------
int issue(int rollno, int isbn)
{
    if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;
    if (repo_handle.book_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;
    if (repo_handle.issue_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;
    if (issued_by(isbn) != -1 || isbn_isssued(rollno) != -1)
        return BOOK_FAILURE;

    struct Student std;
    int student_1 = get_student_by_rollno(rollno, &std);
    if (student_1 != BOOK_SUCCESS)
        return student_1;
    struct Book bk;
    int student_2 = get_book_by_isbn(isbn, &bk);
    if (student_2 != BOOK_SUCCESS)
        return student_2;

    struct Issue issue = {rollno, isbn};
    fseek(repo_handle.issuesys_data_fp, 0, SEEK_END);
    if (fwrite(&issue, sizeof(struct Issue), 1, repo_handle.issuesys_data_fp) != 1)
        return LIB_ADD_FAILED;

    return LIB_SUCCESS;
}

// check if book repo status is closed then return return appropriate error code referring to the error codes
// else continue with following action sequence

// check if student repo status is closed then return return appropriate error code referring to the error codes
// else continue with following action sequence

// check if issue repo status is closed then return return appropriate error code referring to the error codes
// else continue with following action sequence

// declare student and book variables

// get book by isbn and store status in status1
// get student by rollno and store status in status1

// if status1 and status2 are successful then continue with following action sequence else return error

// create Issue object and assign rollno and isbn
// set the file pointer to end
// write issue record

// if both actions are successful then return success
// else return failed

//----------------------------------close()---------------------------------
int libsys_close()
{
    int x = booksys_close();
    if (x != LIB_SUCCESS)
        return x;
    int y = studsys_close();
    if (y != LIB_SUCCESS)
        return y;
    int z = issuesys_close();
    if (z != LIB_SUCCESS)
        return z;
    return LIB_SUCCESS;
}

// call booksys_close()
// call studsys_close()
// call issuesys_close()

// Open the index file in wb mode (write mode, not append mode)
// Write number of index entries at the beginning of index file
// Unload the index array into the index file (overwrite the entire index file)
// Close the index file and data file

// check status of above functions
//  if all of them return success then return SUCCESS else return ERROR

int booksys_close()
{
    if (repo_handle.book_repo_status == LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;

    char issue_file[25];
    strcpy(issue_file, repo_handle.booksys_name);
    strcat(issue_file, ".ndx");

    repo_handle.booksys_ndx_fp = fopen(issue_file, "wb");
    if (repo_handle.booksys_ndx_fp == NULL)
        return LIB_FILE_ERROR;
    if (fwrite(&repo_handle.index_count, sizeof(int), 1, repo_handle.booksys_ndx_fp) != 1)
        return LIB_NDX_SAVE_FAILED;
    if (fwrite(repo_handle.index_entries, sizeof(struct LIB_NdxInfo), repo_handle.index_count, repo_handle.booksys_ndx_fp) != repo_handle.index_count)
        return LIB_NDX_SAVE_FAILED;

    fclose(repo_handle.booksys_ndx_fp);
    repo_handle.booksys_ndx_fp = NULL;
    repo_handle.index_count = 0;
    strcpy(repo_handle.booksys_name, "");
    repo_handle.book_repo_status = LIB_REPO_CLOSED;
    fclose(repo_handle.booksys_data_fp);
    repo_handle.booksys_data_fp = NULL;

    return LIB_SUCCESS;
}

// 1. check if repo status is closed then  return appropriate error code referring to the error codes in libsys.h
// 2. else continue with following action sequence
// 3. close file pointer
// 4. set booksys_name as ""
// 5. set book_repo_status=LIB_REPO_CLOSED
// 6. return LIB_SUCCESS;

int studsys_close()
{
    if (repo_handle.stud_repo_status == LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;

    char student_issue_file[25];
    strcpy(student_issue_file, repo_handle.studsys_name);
    strcat(student_issue_file, ".ndx");

    repo_handle.studsys_ndx_fp = fopen(student_issue_file, "wb");
    if (repo_handle.studsys_ndx_fp == NULL)
        return LIB_FILE_ERROR;
    if (fwrite(&repo_handle.student_index_count, sizeof(int), 1, repo_handle.studsys_ndx_fp) != 1)
        return LIB_NDX_SAVE_FAILED;
    if (fwrite(repo_handle.stud_index_entries, sizeof(struct LIB_NdxInfo), repo_handle.student_index_count, repo_handle.studsys_ndx_fp) != repo_handle.student_index_count)
        return LIB_NDX_SAVE_FAILED;

    fclose(repo_handle.studsys_ndx_fp);
    repo_handle.studsys_ndx_fp = NULL;
    repo_handle.student_index_count = 0;
    strcpy(repo_handle.studsys_name, "");
    repo_handle.stud_repo_status = LIB_REPO_CLOSED;
    fclose(repo_handle.studsys_data_fp);
    repo_handle.studsys_data_fp = NULL;

    return LIB_SUCCESS;
}

// 1. check if repo status is closed then  return appropriate error code referring to the error codes
// 2. else continue with following action sequence
// 3. close file pointer
// 4. set studsys_name as ""
// 5. set stud_repo_status=LIB_REPO_CLOSED
// 6. return LIB_SUCCESS;

int issuesys_close()
{
    if (repo_handle.issue_repo_status == LIB_REPO_CLOSED)
        return LIB_REPO_NOT_OPEN;
    strcpy(repo_handle.issuesys_name, "");
    repo_handle.issue_repo_status = LIB_REPO_CLOSED;
    fclose(repo_handle.issuesys_data_fp);
    repo_handle.issuesys_data_fp = NULL;
    return LIB_SUCCESS;
}

// 1. check if repo status is closed then  return appropriate error code referring to the error codes
// 2. else continue with following action sequence
// 3. close file pointer
// 4. set issuesys_name as ""
// 5. set issue_repo_status=LIB_REPO_CLOSED
// 6. return LIB_SUCCESS;

//---------------------------------------------------Additional Functions -----------------------
// write code for int get_book_by_title(char *title, struct Book *rec)
int get_book_by_title(char *title, struct Book *rec)
{
    int isbn;
    struct Book book;
    if (repo_handle.book_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    fseek(repo_handle.booksys_data_fp, 0, SEEK_SET);
    fread(&isbn, sizeof(int), 1, repo_handle.booksys_data_fp);
    fread(&book, sizeof(struct Book), 1, repo_handle.booksys_data_fp);

    while (feof(repo_handle.booksys_data_fp) == 0)
    {
        if (strcmp(book.title, title) == 0)
            return get_book_by_isbn(book.isbn, rec);

        fread(&isbn, sizeof(int), 1, repo_handle.booksys_data_fp);
        fread(&book, sizeof(struct Book), 1, repo_handle.booksys_data_fp);
    }
    return LIB_REC_NOT_FOUND;
}
// write code for get_student_by_name(char *name, struct Student *rec)
int get_student_by_name(char *name, struct Student *rec)
{
    int rolln;
    struct Student st;

    if (repo_handle.stud_repo_status != LIB_REPO_OPEN)
        return LIB_REPO_NOT_OPEN;

    fseek(repo_handle.studsys_data_fp, 0, SEEK_SET);
    fread(&rolln, sizeof(int), 1, repo_handle.studsys_data_fp);
    fread(&st, sizeof(struct Student), 1, repo_handle.studsys_data_fp);

    while (feof(repo_handle.studsys_data_fp) == 0)
    {
        if (strcmp(st.name, name) == 0)
            return get_student_by_rollno(st.rollno, rec);

        fread(&rolln, sizeof(int), 1, repo_handle.studsys_data_fp);
        fread(&st, sizeof(struct Student), 1, repo_handle.studsys_data_fp);
    }
    return LIB_REC_NOT_FOUND;
}
