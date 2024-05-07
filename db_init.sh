#! /bin/bash
echo -n "Enter the database user: "
read DB_USER
echo -n "Enter the database password: "
read -s DB_PASS
echo ""

mysql -h 127.0.0.1 -P 3306 -u ${DB_USER} -p${DB_PASS} < focus_db.sql