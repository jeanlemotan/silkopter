import QtQuick 2.5
import com.silkopter.VideoSurface 0.1

Rectangle {
    id: main
    color: "red"

    VideoSurface {
        id: videoSurface
        //color: "blue"
        anchors.fill: parent
        anchors.margins: 20
    }
}
