make advanced
./c206-advanced-test >output
echo "TEST BEGIN
----------"
diff output c206-advanced-test.output
echo "----------
TEST END"
rm output
make clean

