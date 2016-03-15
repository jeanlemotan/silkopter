import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: page
    width: 320; height: 480
    color: "#ecf0f1"

    signal sig_start()

    Text {
        id: title
        color: "#2980b9"
        text: "Silkopter"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 42; font.bold: true
    }

    ToolButton  {
        id: startButton
        anchors.horizontalCenter: parent.horizontalCenter
        y: title.y + 100
        //width: startText.width + 16
        //height: startText.height + 8
        iconSource: "qrc:/icons/ui/fly.png"

        style: ButtonStyle {
            background: Rectangle {
                color: "#C5EFF7"
                radius: 8

                SequentialAnimation on color {
                    loops: Animation.Infinite
                    ColorAnimation { from: "#C5EFF7"; to: "#22A7F0"; duration: 500 }
                    ColorAnimation { from: "#22A7F0"; to: "#C5EFF7"; duration: 500 }
                }
            }
        }


        onClicked: {
            parent.color = "#FF00FF";
            page.sig_start()
        }
    }
}
