#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
	char **files;		// holds the list of files to be copied

	get_file_names();
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
int get_file_names ()
{
	FILE *fp;
	int status;
	char path[1024];
	char **files;

	fp = popen("ls ./testwww/*.html", "r");

	while(fgets(path, 1024, fp) != NULL)
	{
		printf("%s", path);
	}

	status = pclose(fp);
	return status;
}