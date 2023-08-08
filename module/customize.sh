SKIPUNZIP=0

# permission
chmod a+x $MODPATH/inject

if [ "$ARCH" != "arm64" ]; then
	ui_print "Only supports arm64 architecture"
	abort
fi
