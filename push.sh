cd docs
doxygen Doxyfile
cd latex
make

echo -n "Please provide a commit message: "
read COMMIT_MSG

cd ../..

git add *
git commit -m "${COMMIT_MSG}"

read -p "Want to push with commit message: '${COMMIT_MSG}'? [(Y)es or (N)o] " userInput
case $userInput in
    [Yy]* )
        git push
        ;;
    [Nn]* )
        echo "Push cancelled!"
        ;;
    * ) 
        echo "Please answer yes or no."
        ;;
esac
