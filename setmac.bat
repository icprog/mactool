@echo off

adb remount
adb push rtwpriv_arm_64 /system/bin/rtwpriv
adb shell chmod 755 /system/bin/rtwpriv

adb shell rtwpriv wlan0 efuse_set wmap,11a,%1  >  result.log
adb shell rtwpriv wlan0 efuse_set btwmap,3c,%2 >> result.log

