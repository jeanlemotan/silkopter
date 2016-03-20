import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.silk.UAVNodeEditor 1.0

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

    MenuButton {
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

    MenuButton {
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

    UAVNodeEditor {
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.bottom: parent.bottom
        fillColor: "green"

        TreeView {
            anchors.fill: parent
            model: parent.nodeDefModel
            TableViewColumn {
               title: "Name"
               role: "Name"
            }

            itemDelegate: Item {
                property string name: styleData.value
                onNameChanged: {
                    icon.source = "qrc:/icons/nodes/" + name + ".png"
                }

                Image {
                    id: icon
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.margins: 8
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    anchors.left: icon.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.margins: 4
                    color: styleData.textColor
                    elide: styleData.elideMode
                    text: name
                }
             }


            rowDelegate: Rectangle {
                height: 32
                color: styleData.selected ? "#E75926" : (styleData.alternate ? "#282830" : "black")
            }

//            style: TreeViewStyle {
//                alternateBackgroundColor: "#282830"
//            }
        }
    }
}
