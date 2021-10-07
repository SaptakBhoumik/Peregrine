# CI TEST RUNNER SCRIPT @ClientCrash 7 - okt - 2021
cd ./tests
pwd
for file in *
do
echo $file ...
../peregrine compile $file -emit-c
gcc temp.c -o output.o 
if test -f "output.o";then
chmod +x ./output.o
./output.o
rm ./output.o
else
echo "Error: gcc failed"
exit 1
fi


    
done
exit 0