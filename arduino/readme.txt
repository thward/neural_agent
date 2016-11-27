

Serial via USB
--------------

/dev/ttyACM0

Send instructions using arduino serial monitor tool.
Monitor using arduino serial monitor tool.




Serial via Bluetooth
--------------------

/dev/rfcomm0


Send instructions using c bluetooth program.
Monitor using c bluetooth program printf to command line/log.

Send instructions using BlueSerial app on phone.
Monitor using using BlueSerial app on phone.

Send instructions using arduino serial monitor tool only if USB also connected.
Monitor using arduino serial monitor tool only if USB also connected.

Note: cannot upload sketches via USB if bluetooth module connected.

Note; Sketches cannot be sent via bluetooth; serial port is not available via arduino IDE when USB disconnected.

Note: make sure /etc/bluetooth/rfcomm.conf is not configured or this will hog rfcomm0.


