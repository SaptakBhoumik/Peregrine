# CI TEST RUNNER SCRIPT @ClientCrash 7 - okt - 2021
cd ./tests
pwd
for file in *
do
echo $file ...
../peregrine compile $file -emit-c
gcc temp.c -o output.o | grep 'error'
if [ $? -eq 1]
then
echo "::error GCC FAILED"
exit 1
fi
chmod +x ./output.o
./output.o
rm ./output.o

    
done
exit 0