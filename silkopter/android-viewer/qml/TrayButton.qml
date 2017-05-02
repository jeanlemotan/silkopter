import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

ToolButton
{
    property string icon
    property color color

    id: root
    Image {
        opacity: root.enabled ? 1 : 0.5
        source: root.icon
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 8
        fillMode: Image.PreserveAspectFit
    }

    style: ButtonStyle {
        background: Rectangle {
            opacity: 0.2
            radius: 4
        }
    }
}
