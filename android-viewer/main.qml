import QtQuick 2.5
import com.silkopter.VideoSurface 0.1

Rectangle {
    id: main

    VideoSurface {
        id: videoSurface
        anchors.fill: parent
        //anchors.margins: 1
    }
}
