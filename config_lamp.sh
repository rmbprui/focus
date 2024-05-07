#! /bin/bash
mkdir -p $HOME/lamp
sudo docker run -it --name focus-lamp-server -p "80:80" -p "3306:3306" -v ${HOME}/lamp:/app mattrayner/lamp:latest-1804 
