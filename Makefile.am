SUBDIRS = o2on

dist-hook: changelog

changelog: configure.ac
	git log --stat --name-only --date=short --abbrev-commit > ChangeLog