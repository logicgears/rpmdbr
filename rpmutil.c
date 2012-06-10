/* Utility Command to remove a package only from the RPM database
   Compile using "gcc -I/usr/include/rpm -o rpmdbr rpmutil.c -lrpm -lrpmdb -lrpmio"
   Author: Naveen Chandran  --  Last Modified: 28 May 2012 */ 
 
 
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
 
#include <rpmlib.h>
#include <rpmdb.h>
 
int main(int argc, char *argv[])
{
    rpmdbMatchIterator mi;
    int type, count;
    char *name;
    rpmdb db;
    Header h;
    unsigned int offset;
    int found = 0;
 
   if(argc != 2)
   {
       printf("rpmdbr: incorrect number of args\n");
       printf("Usage: rpmdbr <Name_of_the_Package>\n");
       printf("This command removes the specified package from the RPM database.\n");
       exit(1);
   }
 
 
    rpmReadConfigFiles( NULL, NULL );
    // Open Database in Read/Write Mode
    if (rpmdbOpen( "", &db, O_RDWR, 0644 ) != 0) {
        fprintf( stderr, "ERROR: Cannot open RPM database\n");
        exit(1);
    }
 
    mi = rpmdbInitIterator(db, RPMDBI_PACKAGES, NULL, 0);
    while ((h = rpmdbNextIterator(mi))) {
        headerGetEntry(h, RPMTAG_NAME, &type, (void **) &name, &count);
            if ( strcmp( name, argv[1] ) == 0 ) {
                //printf("%s is present\n h is %d\n count is %d\n", name, h, count);
                printf("%s is present\n", name, h, count);
                found = 1;
 
                // Get the offset of the record specified
                offset = rpmdbGetIteratorOffset(mi);
                printf("\nRemoving Package from RPM Database...");
                if ( rpmdbRemove(db, -1 , offset, NULL, NULL) == 0 )
                    printf("\nSuccess!\n");
            }
    }
 
    if (!found)
        printf("Package Not Found in RPM Database\n");
 
    rpmdbFreeIterator(mi);
    rpmdbClose(db);
 
    exit(0);
}