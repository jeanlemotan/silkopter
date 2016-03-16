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
        height: parent.height
        anchors.left: parent.left
        anchors.margins: 4
        fillMode: Image.PreserveAspectFit
    }

    style: ButtonStyle {
        background: Rectangle {
            color: root.color
            opacity: root.enabled ? 1 : 0.5
            radius: 8
        }
    }
}
