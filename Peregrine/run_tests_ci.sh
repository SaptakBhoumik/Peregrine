# CI TEST RUNNER SCRIPT @ClientCrash 7 - okt - 2021
cd ./tests
pwd
for file in *
do
    ./peregrine compile $file -emit-c
    gcc temp.c -o output.o
    chmod +x ./output.o
    ./output.o
    rm ./output.o
done