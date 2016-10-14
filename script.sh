#!/bin/bash

mkdir ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks

for i in QtCore QtQuick QtQml QtNetwork QtGui QtWidgets QtMultimedia
do
    cp -R ~/Qt5.7.0/5.7/clang_64/lib/$i.framework ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks
    install_name_tool -id @executable_path/../Frameworks/$i.framework/Versions/5/$i ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks/$i.framework/Versions/5/$i
    install_name_tool -change ~/Qt5.7.0/5.7/clang_64/lib/$i.framework/Versions/5/$i @executable_path/../Frameworks/$i.framework/Versions/5/$i
    
    if [ $i != QtCore ]
    then
	install_name_tool -change ~/Qt5.7.0/5.7/clang_64/lib/QtCore.framework/Versions/5/QtCore  @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks/$i.framework/Versions/5/$i
    fi
done


install_name_tool -change ~/Qt5.7.0/5.7/clang_64/lib/QtGui.framework/Versions/5/QtGui  @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -change ~/Qt5.7.0/5.7/clang_64/lib/QtQml.framework/Versions/5/QtQml  @executable_path/../Frameworks/QtQml.framework/Versions/5/QtQml ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks/QtQuick.framework/Versions/5/QtQuick
install_name_tool -change ~/Qt5.7.0/5.7/clang_64/lib/QtNetwork.framework/Versions/5/QtNetwork  @executable_path/../Frameworks/QtNetwork.framework/Versions/5/QtNetwork ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks/QtQuick.framework/Versions/5/QtQuick
install_name_tool -change ~/Qt5.7.0/5.7/clang_64/lib/QtNetwork.framework/Versions/5/QtNetwork  @executable_path/../Frameworks/QtNetwork.framework/Versions/5/QtNetwork ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks/QtQml.framework/Versions/5/QtQml
install_name_tool -change ~/Qt5.7.0/5.7/clang_64/lib/QtNetwork.framework/Versions/5/QtNetwork  @executable_path/../Frameworks/QtNetwork.framework/Versions/5/QtNetwork ~/bin/build-SimpleSoundLevelMeter-Desktop_Qt_5_7_0_clang_64bit-Release/SimpleSoundLevelMeter.app/Contents/Frameworks/QtMultimedia.framework/Versions/5/QtMultimedia
