#!/bin/bash
if [ $# -ne 1 ]
then
	echo "usage: bash ./my_backup [backup directory]"
	exit
fi

FILES=*
BACKUP_DIRECTORY=$1

if [ -d "$BACKUP_DIRECTORY" ]
then
    for FILE in $FILES
    do
        if test -f "$BACKUP_DIRECTORY/$FILE"
        then
            DATE_BFILE=$(stat --format %Y "$BACKUP_DIRECTORY/$FILE")
            DATE_FILE=$(stat --format %Y "$FILE")
            if [ "$DATE_FILE" -gt "$DATE_BFILE" ]
            then
                cp -R $FILE "$BACKUP_DIRECTORY/$FILE"
            fi
        else
            cp $FILE "$BACKUP_DIRECTORY/$FILE"
        fi
    done
else
    mkdir "$BACKUP_DIRECTORY"
    cp -R . "$BACKUP_DIRECTORY"
fi