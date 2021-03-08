These are my dotconfigs
Expected folder structure for run.sh to work properly:

-> folder-with-links/
	-> link1
	-> link2
	-> link3
	-> gather.sh
	-> run.sh

then run.sh will turn that structure into:

-> folder-with-links/
	-> link1
	-> link2
	-> link3
	-> gather.sh
	-> run.sh
	-> dotconfig/
		-> config1
		-> config2
		-> README.md
		-> gather.sh
		-> run.sh

where "dotconfig/" is where github repo will be cloned
so the first time after cloning the repo normally
it's only needed to create the outer "folder-with-links/"
and copy (or move) "gather.sh" and "run.sh" to that folder.

After that, running ./run.sh will clone the repo, copy the files,
add, commit, push (for which you may need to provide credentials),
and ask you if post-deletion of the cloned folder is what you want.

PS: it's not necessary for it to be named "folder-with-links/"
