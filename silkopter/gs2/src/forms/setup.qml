import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    signal backPressed()
    signal multiConfigPressed()

    TopBar {
        id: topBar
        width: parent.width
        title: "Setup"
        onBackPressed: root.backPressed()
    }

    MenuButton
    {
        id: multiConfigButton
        text: "Multi Config"
        icon: "qrc:/icons/ui/quad.png"
        color: "#bdc3c7"
        onClicked: root.multiConfigPressed()
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: topBar.bottom
        anchors.topMargin: 100
        height: 60
        width: 200
    }

}
