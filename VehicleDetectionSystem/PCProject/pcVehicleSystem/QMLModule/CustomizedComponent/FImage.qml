import QtQuick 2.12


/**
* @brief: 不用每次使用Image时都去定制这些属性
* @author: Fu_Lin
* @date:  2019-03-28
* @description: 描述主要实现的功能
*/
Image {
    id: id_Fimage
    asynchronous: true
    fillMode: Image.PreserveAspectFit
    sourceSize: Qt.size(id_Fimage.width, id_Fimage.height)

    onWidthChanged: {
        id_Fimage.sourceSize.width = id_Fimage.width
    }

    onHeightChanged: {
        id_Fimage.sourceSize.height = id_Fimage.height
    }
}
