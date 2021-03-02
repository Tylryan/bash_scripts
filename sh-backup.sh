#! /usr/bin/bash

update_readme=$1

if [ $update_readme == '-u' ];then
    # Pull before you send

    cd ~/bash_repository

    git pull &&
    
    # Update the readme 
    
    nano README.md ;

    # Copy files from ~/bin to ~/bash_repository

    cp ~/bin/* ~/bash_repository &&

    # Git push

    git add . && git commit -m "Backing up my sh files" && git push
else
    # Pull before you send

    cd ~/bash_repository

    git pull &&

    # Copy files from ~/bin to ~/bash_repository

    cp ~/bin/* ~/bash_repository &&

    # Git push

    git add . && git commit -m "Backing up my sh files" && git push

fi




