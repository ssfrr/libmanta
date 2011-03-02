DEST_DIR=/System/Library/Extensions

FILENAME=MantaDummyDriver.kext

if  [ -d $DEST_DIR/$FILENAME ]
then
    rm -r $DEST_DIR/$FILENAME && echo $FILENAME Removed, please reboot
else
    cp -r $FILENAME $DEST_DIR && \
    chmod -R 755 $DEST_DIR/$FILENAME && \
    chown -R 0:0 $DEST_DIR/$FILENAME && \
    echo $FILENAME Installed, please reboot
fi
