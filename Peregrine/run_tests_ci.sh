# CI TEST RUNNER SCRIPT @ClientCrash 7 - okt - 2021
cd ./tests
pwd
for file in *
do
echo $file ...
../peregrine compile $file -emit-c
if gcc temp.c -o output.o then
echo "::error GCC FAILED"
exit
fi
chmod +x ./output.o
./output.o
rm ./output.o

    
done