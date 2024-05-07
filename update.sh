#! /bin/bash

echo -n "Enter the admin user: "
read USER
echo -n "Enter the admin password: "
read -s PASS

su - ${USER} << !
${PASS}

echo ${PASS} | sudo -S dpkg --configure -a
echo ${PASS} | sudo -S apt update
echo ${PASS} | sudo -S apt upgrade -y
echo ${PASS} | sudo -S apt dist-upgrade -y
echo ${PASS} | sudo -S apt autoremove -y
echo ${PASS} | sudo -S apt autoclean -y
echo ${PASS} | sudo -S apt install libtbb-dev build-essential gcc make cmake cmake-gui cmake-curses-gui libssl-dev -y
echo ${PASS} | sudo -S apt install doxygen graphviz libcppunit-dev catch libclang-common-8-dev qtbase5-private-dev -y
echo ${PASS} | sudo -S apt install libmysqlcppconn-dev libmysqlclient-dev libssl-dev mysql-client libmysql++-dev libqt5sql5-mysql libclang-common-8-dev -y
echo ${PASS} | sudo -S rm -rf ~/repositories
mkdir -p ~/repositories
cd repositories
git clone https://code.qt.io/qt/qtmqtt.git
cd qtmqtt
mkdir build
cd build
qmake qmake -r ..
make
echo ${PASS} | sudo -S make install
echo ${PASS} | sudo -S ldconfig
echo ${PASS} | sudo -S updatedb
echo "Finished updating the machine!"
