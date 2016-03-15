import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: page
    width: 320; height: 480
    color: "#ecf0f1"

    signal sig_back()
    signal sig_startSetup()
    signal sig_startHUD()

    Rectangle  {
        id: titleBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: titleText.height + 16
        color: "#34495e"
        Text {
            id: titleText
            color: "#2980b9"
            text: "Silkopter"
            x: 8
            anchors.verticalCenter: parent.verticalCenter
            font.pointSize: 30; font.bold: true
        }

        ToolButton {
            id: backButton
            x: parent.width - width - 8;
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height - 16
            width: height
            Image {
                source: "qrc:/icons/ui/back.png"
                anchors.fill: parent
                anchors.margins: 4
                fillMode: Image.PreserveAspectFit
            }

            style: ButtonStyle {
                background: Rectangle {
                    color: "#C5EFF7"
                    radius: 4
                }
            }
            onClicked: {
                page.sig_back()
            }
        }
    }

}
