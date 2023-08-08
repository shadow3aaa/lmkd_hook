MODDIR=${0%/*}

$MODDIR/inject -p $(pidof lmkd) -so $(realpath $MODDIR/hookLib.so) -symbols hook_lmkd
