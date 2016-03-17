import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: root
    width: 800; height: 480
    color: "#2c3e50"

    signal startPressed()

    Text {
        id: title
        color: "#ecf0f1"
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
                    ColorAnimation { from: "#3498db"; to: "#7f8c8d"; duration: 500 }
                    ColorAnimation { from: "#7f8c8d"; to: "#3498db"; duration: 500 }
                }
            }
        }

        onClicked: root.startPressed()
    }
}
