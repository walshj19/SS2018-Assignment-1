#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern int copy_files();
extern int get_file_names(char **files, int *files_length);

int main ()
{
	char **files;		// holds the list of files to be copied
	int files_length;	// holds the length of the files array

	get_file_names(files, &files_length);

	//print the file names
	for (int i = 0; i < files_length; ++i)
	{
		printf("%s", files[i]);
	}
	// free the memory allocated for the file names
	for (int i = 0; i < files_length; ++i)
	{
		free(files[i]);
	}
	free(files);

	return 0;
}

/* Copy all of the files in the source folder into the destination folder.
 */
int copy_files ()
{
	// use exec to move all of the files
	char *argv[] = {"ls", "./testwww/*.html", "./testbackup", NULL};
	execv("/bin/ls", argv);
	return 0;
}

/* Use ls to get a list of files in the www directory.
 */
int get_file_names (char **files, int *files_length)
{
	FILE *fp;
	int status;
	char path[1024];

	// initialise the 
	*files_length = 0;

	// get a file pointer to the result of the ls command
	fp = popen("ls ./testwww/*.html", "r");

	// // iterate over the file names to count them 
	while(fgets(path, 1024, fp) != NULL)
	{
		//printf("%s", path);
		(*files_length)++;
	}
	printf("%d files in directory\n", *files_length);

	// //allocate memory for the files array
	files = malloc((*files_length)*sizeof(char*));

	// // reset the file pointer
	pclose(fp);
	fp = popen("ls ./testwww/*.html", "r");

	// // iterate over the file names and add them to the array
	for(int i = 0; fgets(path, 1024, fp) != NULL; i++)
	{
		//printf("%s", path);
		// allocate heap memory for the string
		files[i] = malloc(sizeof(path));
		//copy the file name to the array
		strcpy(files[i], path);
	}

	status = pclose(fp);
	return status;
}