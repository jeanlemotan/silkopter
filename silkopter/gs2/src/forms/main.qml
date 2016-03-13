import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle {
    id: page
    width: 320; height: 480
    color: "#ecf0f1"

    Text {
        id: title
        color: "#2980b9"
        text: "Silkopter"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 42; font.bold: true
    }

    Rectangle  {
        id: start
        anchors.horizontalCenter: parent.horizontalCenter
        y: title.y + 100
        width: startText.width + 16
        height: startText.height + 8
        color: "#34495e"

        Text {
            text: "Start"
            id: startText
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pointSize: 22; font.bold: true

            SequentialAnimation on color {
                loops: Animation.Infinite
                ColorAnimation { from: "#ecf0f1"; to: "#7f8c8d"; duration: 500 }
                ColorAnimation { from: "#7f8c8d"; to: "#ecf0f1"; duration: 500 }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: parent.color = "#FF00FF"
        }
    }
}
