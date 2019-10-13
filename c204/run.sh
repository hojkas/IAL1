make advanced
./c204-advanced-test >output
echo "TEST BEGIN
----------"
diff output c204-advanced-test.output
echo "----------
TEST END"
rm output
make clean

