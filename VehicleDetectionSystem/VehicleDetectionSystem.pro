TEMPLATE = subdirs

CONFIG += ordered

macx {
# mac only
}

ios{
# ios only
}

win32{
# windows only
 SUBDIRS = PCProject/pcVehicleSystem
}

android{
# android only
 SUBDIRS = APPProject/AppVehicleSystem
}

unix:!macx{
# linux only
}
