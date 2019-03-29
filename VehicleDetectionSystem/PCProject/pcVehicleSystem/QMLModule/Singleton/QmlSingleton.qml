pragma Singleton
import QtQuick 2.12


/**
* @brief: qml单例模式
* @author: Fu_Lin
* @date:  2019-03-29
* @description: 主要记录一些公用的数据和方法
*/
QtObject {
    property string blueColor: "blue"
    property int textSize: 20
    property string bgColor:"red"




    //调用此方法，可以移动指定的item
    function moveWindow(item)
    {

    }
}
