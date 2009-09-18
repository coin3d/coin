#!/bin/bash

SYNCFILES=$(svn st | egrep ^~ | cut -d\  -f2- | sort)

for file in ${SYNCFILES}
do
	echo Processing ${file}
	if [ -e ${file}.bak ]
	then
		echo "Error: \"${file}.bak\" already exists"
		exit 1
	fi 
	mv ${file} ${file}.bak || ( echo "Could not create: \"${file}.bak\""; exit 1)

	svn up ${file}
	find ${file} -type f -and -not -path '*/.svn*' -delete
	cp -r ${file}.bak/* ${file}
	#For now, we are assuming the files that shal be included are included
	#svn st ${file} | egrep ^! | cut -d\  -f2- | xargs svn del
	#svn st ${file} | egrep '^\?' | cut -d\  -f2- | xargs svn add
	rm -rf ${file}.bak
done

if [ -n "${SYNCFILES}" ]
then
	true
	svn ci ${SYNCFILES} -m 'Syncing updated MSVC project files'
fi