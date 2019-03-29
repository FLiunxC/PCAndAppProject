import QtQuick 2.12
import "../CustomizedComponent"


/**
* @brief: 用于PC端显示的状态栏
* @author: Fu_Lin
* @date:  2019-03-28
* @description: 状态栏可显示放大缩小还原等按钮，中间可显示文本
*/
Rectangle {

    width: 100
    height: 50

    color:"#4876FF"
    QtObject {
        id: id_QObject
        property string maximizationBtn: "qrc:/tab_smaller_w.png" //最大化按钮
        property string maximizationHoverBtn: "qrc:/tab_smaller.png"
        property string minimizeBtn: "qrc:/tab_hide_w.png" //最小化按钮
        property string minimizeBtnHoverBtn: "qrc:/tab_hide.png"
        property string closeBtn: "qrc:/tab_close_w.png" //关闭按钮
        property string closeBtnHoverBtn: "qrc:/tab_close.png"

        //用list包装上按钮，后面直接使用
        property var listHoveBtn: [id_QObject.minimizeBtnHoverBtn, id_QObject.maximizationHoverBtn, id_QObject.closeBtnHoverBtn]
        property var listNormalBtn: [id_QObject.minimizeBtn, id_QObject.maximizationBtn, id_QObject.closeBtn]
    }

    property var mainWindow: null
    //标题栏鼠标区域点击移动整个界面
    MouseArea {
        id: id_titleBarMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        property point clickPos: "0,0"

        onPressed: {
            clickPos = Qt.point(mouse.x, mouse.y)
        }

        onPositionChanged: {
            //计算鼠标偏移量
            var offser = Qt.point(mouse.x - clickPos.x, mouse.y - clickPos.y)

            //如果mainwindow继承自QWidget,用setPos
            mainWindow.setX(mainWindow.x + offser.x)
            mainWindow.setY(mainWindow.y + offser.y)
        }
    }
    Row {
        anchors.right: parent.right
        Repeater {
            model: id_QObject.listNormalBtn

            FImage {
                id: id_fImage
                width: 50
                height: 50
                source: modelData
                MouseArea {
                    id: id_FimageMouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onPressed: {
                        console.log("index", index)
                        if (index == 2) {
                            Qt.quit()
                        }
                    }
                    onEntered: {
                        id_FimageMouseArea.cursorShape = Qt.PointingHandCursor
                        id_fImage.scale = 1.3
                        id_fImage.source = id_QObject.listHoveBtn[index]
                    }
                    onExited: {
                        id_FimageMouseArea.cursorShape = Qt.ArrowCursor
                        id_fImage.scale = 1
                        id_fImage.source = id_QObject.listNormalBtn[index]
                    }
                }
            }
        }
    }
}
