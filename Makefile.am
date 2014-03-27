SUBDIRS		= o2on
ACLOCAL_AMFLAGS = -I m4

dist-hook: changelog

changelog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog