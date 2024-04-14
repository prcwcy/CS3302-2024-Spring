LINUX_VERSION := linux-6.2.9
LINUX_ARCHIVE := ${LINUX_VERSION}.tar.xz

invalid:
	@echo Invalid Command

init:
	which tar
	which git
	cat ${LINUX_ARCHIVE}.* | tar -k -xJ
	cd ${LINUX_VERSION} && git init && git config --local user.name student && git config --local user.email student@student && git add . && git commit -m initial
	touch init

patch:
	cd ${LINUX_VERSION} && git add . && git diff --cached $$(git rev-list --max-parents=0 HEAD) > ../submission.patch
