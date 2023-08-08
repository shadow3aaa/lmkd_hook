MODDIR=${0%/*}

$MODDIR/Inject -p $(pidof lmkd) -so $(realpath $MODDIR/hookLib.so) -symbols hook_lmkd
