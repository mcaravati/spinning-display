firmware=$1

sed -i "s/FIRM/${firmware}/g" Makefile 
make
make install
sed -i "s/${firmware}/FIRM/g" Makefile 
