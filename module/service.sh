MODDIR=${0%/*}

# wait for lmkd start
until pidof lmkd; do
	sleep 1s
done

$MODDIR/inject -p $(pidof lmkd) -so $(realpath $MODDIR/hookLib.so) -symbols hook_lmkd
