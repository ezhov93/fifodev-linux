#!/bin/bash

make disclean
make

insmod ./iodev.ko debug=1 mode=0
echo "*************************************************"
echo "*** MODE = 0 ************************************"
echo "*************************************************"
echo ""
echo "*** TERM ****************************************"
echo "123456789" >/dev/iodev
cat /dev/iodev
echo ""
echo "*** TEST_DEVICE *********************************"
./test_iodev
echo ""
echo "*** DMESG ***************************************"
dmesg | tail -n10
rmmod /dev/iodev
echo ""
echo "*************************************************"
echo "*** MODE = 1 ************************************"
echo "*************************************************"
echo ""
insmod ./iodev.ko debug=1 mode=1
echo "*** TERM ****************************************"
echo ""
echo "123456789" >/dev/iodev
cat /dev/iodev
echo ""
echo "*** TEST_DEVICE *********************************"
echo ""
./test_iodev
echo ""
echo "*** DMESG ***************************************"
echo ""
dmesg | tail -n10
rmmod /dev/iodev
echo ""
echo "*************************************************"
echo "*** MODE = 3 ************************************"
echo "*************************************************"
echo ""
insmod ./iodev.ko debug=1 mode=2
echo ""
echo "*** TERM ****************************************"
echo ""
echo "123456789" >/dev/iodev
cat /dev/iodev
cat /dev/iodev
echo ""
echo "*** TEST_DEVICE *********************************"
echo ""
./test_iodev
echo ""
echo "*** DMESG ***************************************"
echo ""
dmesg | tail -n10
rmmod /dev/iodev

make clean
make disclean
