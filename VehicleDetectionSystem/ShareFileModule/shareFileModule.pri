QT += widgets
SOURCES += $$PWD/Help/HelpClass.cpp \
    $$PWD/Help/ToolTip/ToolTip.cpp \
    $$PWD/Help/GraphicsDropShadowEffect/QNGraphicsEffect.cpp



HEADERS += $$PWD/Help/HelpClass.h \
    $$PWD/Singleton/Singleton.h \
    $$PWD/Logger/MessageHandler.h \
    $$PWD/Help/ToolTip/ToolTip.h \
    $$PWD/Help/GraphicsDropShadowEffect/QNGraphicsEffect.h

INCLUDEPATH += $$PWD/Help \
               $$PWD/Logger \
               $$PWD/Singleton \
               $$PWD/Help/ToolTip \
               $$PWD/Help/GraphicsDropShadowEffect \

# 自定义操作如下：
#PROFILEPATH = $$PWD/Debug
build_pass:CONFIG(debug, debug|release) {
   #debug编译
#PROFILEPATH = $$PWD/Debug
} else {
   #release编译
#PROFILEPATH = $$PWD/Release
}
#生成目标应用程序名称
#TARGET = testPro
# 指定生成的应用程序放置的目录
#DESTDIR += $$PROFILEPATH/bin

#下面暂时用不到，屏蔽掉
#指定uic命令将.ui文件转化成ui_*.h文件的存放的目录
#UI_DIR += $$PROFILEPATH/fromsUi
#指定moc命令将含Q_OBJECT的头文件转换成标准.h文件的存放目录
#MOC_DIR += $$PROFILEPATH/tmp/Moc
#指定目标文件(obj)的存放目录
#OBJECTS_DIR += $$PROFILEPATH/tmp/Object

#头文件包含路径
INCLUDEPATH += .
