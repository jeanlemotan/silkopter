import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Rectangle {
    id: root
    width: 800; height: 600
    color: "#2c3e50"

    TopBar {
        id: topBar
        width: parent.width
        title: "Multi Config"
        onBackPressed: s_menus.pop()
    }

    MenuButton
    {
        id: quadxButton
        text: "Quad X"
        icon: "qrc:/icons/ui/quadx.png"
        //color: "#bdc3c7"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.topMargin: 100
        height: 60
        width: 200
    }

    MenuButton
    {
        id: quadplusButton
        text: "Quad X"
        icon: "qrc:/icons/ui/quadx.png"
        //color: "#bdc3c7"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.topMargin: 100
        height: 60
        width: 200
    }

}
