import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import "."
import "./QMLModule/CustomizedWidgets"

ApplicationWindow {
    id: id_window

    visible: true
    width: Screen.desktopAvailableWidth / 2
    height: Screen.desktopAvailableHeight / 2

    title: qsTr("VehicleDetectionSystem")

    flags: Qt.Window | Qt.FramelessWindowHint //去掉标题栏

    //最上方的标题栏，只允许点击标题栏拖动
    header: TitleBar {
        id: id_titleBar

        width: parent.width
        height: 50
        mainWindow: id_window
    }

    //显示的内容区域
    Rectangle {
        id: id_container

        //anchors.fill: parent
        color: "yellow"
    }
}
