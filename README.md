# Systems Software 2018 Assignment 1

# client requirements
	## internal intranet site separate from the prod site (dev)
	## nightly backups of the site
	## log every change to the site files (user, files changed and timestamp)
	## every night copy dev to prod
		### only copy changed files
		### lock changes during transfer
	## users shouldn't have write access to prod
		### but it should be possible to push changes to prod at any time in an emergency

# technical requirements
	## write a daemon in c to preform the requirements specified by the client
	## logs should be generated as text files stored on the server
	## maintain a separate list of file updates to check when pushing dev to prod
	## backup every night
	## push to prod after the backup
	## lock dev and prod during update
	## it must be possible to trigger backups and pushes at any time
	## A message queue should be setup to allow all processes to report in on completion of a task. (success or failure)
	## a makefile should be used to manage building the project

# context
	## there is only one server
	## there is some folder on the server to which the site can be backed up to
	## the dev and prod sites can be two subfolders of /var/www/html
	## The files are all static html
	## system is debian server
	## users login and make changes to the site files
	## apache is running as the web server
	## /var/www/html is the root of the site

# Deliverables
	## Report on design and operation of the daemon
	## full source code
	## 5 minute video describing and demonstrating the operation of the daemon