import QtQuick 2.7
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Extras 1.4
import QtQuick.Controls.Styles 1.4
import QtLocation 5.6
import QtPositioning 5.6

import com.silk.VideoSurface 0.1
import com.silk.HUD 1.0

Rectangle {
    readonly property int telemetryFontSize: 10
    readonly property int batteryBarSize: 24
    readonly property int bigMapWidth: 200
    readonly property int bigMapHeight: 200
    readonly property int smallMapWidth: 100
    readonly property int smallMapHeight: 100
    readonly property int mapButtonSize: 32
    readonly property int cameraButtonSize: 32
    readonly property int verticalTicksWidth: 32

    readonly property int modeButtonFontSize: 10
    readonly property int modeButtonHorizontalMargin: 12
    readonly property int modeButtonVerticalMargin: 12
    readonly property var modeButtonBorderColor: "#00FF00"
    readonly property var modeButtonBackgroundColor: "#3300FF00"
    readonly property var modeButtonConfirmedTextColor: "#00FF00"
    readonly property var modeButtonChangedTextColor: "#FFFF00"
    readonly property var modeButtonUnconfirmedTextColor: "#FF3300"
    readonly property var modeButtonChangeTextColor: "#3300FFFF"

    property bool isAltitudeToToHomeValid: s_hud.position.isValid && s_hud.homePosition.isValid
    property real altitudeToToHome: isAltitudeToToHomeValid ? s_hud.position.altitude - s_hud.homePosition.altitude : 0

    property bool isAltitudeToToSeaValid: s_hud.position.isValid
    property real altitudeToToSea: isAltitudeToToSeaValid ? s_hud.position.altitude : 0

    property bool isDistanceToHomeValid: s_hud.homePosition.isValid && s_hud.position.isValid
    property bool distanceToHome: isDistanceToHomeValid ? s_hud.homePosition.distanceTo(s_hud.position) : 0

    property bool isDistanceToMeValid: userPosition.position.isValid && s_hud.position.isValid
    property bool distanceToMe: isDistanceToMeValid ? userPosition.position.distanceTo(s_hud.position) : 0

    function isRadioRxRSSICritical(rssi) {
        return rssi === 0 || rssi <= -90.0; //TODO - add a setting for this
    }
    function isRadioRxRSSIWarning(rssi) {
        return rssi <= -80.0; //TODO - add a setting for this
    }
    function getRadioRxRSSIColor(rssi)
    {
        if (isRadioRxRSSICritical(rssi)) return "red";
        if (isRadioRxRSSIWarning(rssi)) return "yellow";
        return "green";
    }

    function isRadioTxRSSICritical(rssi) {
        return rssi === 0 || rssi <= -90.0; //TODO - add a setting for this
    }
    function isRadioTxRSSIWarning(rssi) {
        return rssi <= -80.0; //TODO - add a setting for this
    }
    function getRadioTxRSSIColor(rssi)
    {
        if (isRadioTxRSSICritical(rssi)) return "red";
        if (isRadioTxRSSIWarning(rssi)) return "yellow";
        return "green";
    }

    function isVideoRxRSSICritical(rssi) {
        return rssi === 0 || rssi <= -80.0; //TODO - add a setting for this
    }
    function isVideoRxRSSIWarning(rssi) {
        return rssi <= -75.0; //TODO - add a setting for this
    }
    function getVideoRxRSSIColor(rssi)
    {
        if (isVideoRxRSSICritical(rssi)) return "red";
        if (isVideoRxRSSIWarning(rssi)) return "yellow";
        return "green";
    }

    function isUAVBatteryVoltageCritical(voltage) {
        return (voltage <= 10.0); //TODO - add a setting for this
    }
    function isUAVBatteryVoltageWarning(voltage) {
        return (voltage <= 10.5); //TODO - add a setting for this
    }
    function getUAVBatteryVoltageColor(voltage)
    {
        if (isUAVBatteryVoltageCritical(voltage)) return "red";
        if (isUAVBatteryVoltageWarning(voltage)) return "yellow";
        return "green";
    }

    function isUAVBatteryLevelCritical(level) {
        return (level <= 0.1);
    }
    function isUAVBatteryLevelWarning(level) {
        return (level <= 0.2);
    }
    function getUAVBatteryLevelColor(level)
    {
        if (isUAVBatteryLevelCritical(level)) return "red";
        if (isUAVBatteryLevelWarning(level)) return "yellow";
        return "green";
    }
    function getUAVBatteryLevelIcon(level) {
        if (isUAVBatteryLevelCritical(level)) return "qrc:/icons/ui/battery-0.png";
        if (level <= 0.25) return "qrc:/icons/ui/battery-25.png";
        if (level <= 0.50) return "qrc:/icons/ui/battery-50.png";
        if (level <= 0.75) return "qrc:/icons/ui/battery-75.png";
        return "qrc:/icons/ui/battery-100.png";
    }

    function getModeText(mode) {
        if (mode === HUD.MODE_IDLE) return "IDLE";
        if (mode === HUD.MODE_FLY) return "FLY";
        if (mode === HUD.MODE_LAND) return "LAND";
        if (mode === HUD.MODE_RETURN_HOME) return "RTH";
        if (mode === HUD.MODE_TAKE_OFF) return "TKOFF";
        return "N/A";
    }
    function getVerticalModeText(mode) {
        if (mode === HUD.VERTICAL_MODE_THRUST) return "THRUST";
        if (mode === HUD.VERTICAL_MODE_ALTITUDE) return "ALT";
        return "N/A";
    }
    function getHorizontalModeText(mode) {
        if (mode === HUD.HORIZONTAL_MODE_ANGLE_RATE) return "RATE";
        if (mode === HUD.HORIZONTAL_MODE_ANGLE) return "ANGLE";
        if (mode === HUD.HORIZONTAL_MODE_POSITION) return "POS";
        return "N/A";
    }
    function getYawModeText(mode) {
        if (mode === HUD.YAW_MODE_ANGLE_RATE) return "RATE";
        if (mode === HUD.YAW_MODE_ANGLE) return "ANGLE";
        return "N/A";
    }

    id: root
    color: "black"

    VideoSurface {
        id: renderer
        //anchors.fill: parent
        anchors.top: root.top
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        Flickable {
            id: gimbalTouchArea
            anchors.fill: parent
            contentHeight: parent.height * 2
            onContentYChanged: {
                s_hud.gimbalPitch = contentY / parent.height;
                console.log("pitch: " + contentY);
            }
        }
    }

/*    Canvas {
        id: hudCanvas
        anchors.fill: parent
        renderTarget: Canvas.FramebufferObject
        renderStrategy: Canvas.Threaded
        onPaint: {
            var ctx = getContext("2d")

            ctx.fillStyle = "white"
            //ctx.fillRect(0, 0, renderer.width, renderer.height )

            ctx.lineWidth = 2;
            ctx.strokeStyle = "red"
            ctx.beginPath()
            var centerX = renderer.x + renderer.width / 2
            var centerY = renderer.y + renderer.height / 2

            var cos = Math.cos(s_hud.localFrameEuler.y)
            var sin = Math.sin(s_hud.localFrameEuler.y)
            var x = cos * 3000
            var y = sin * 3000

            ctx.moveTo(centerX, centerY)
            ctx.lineTo(x, y)

            x = -cos * 3000
            y = -sin * 3000
            ctx.moveTo(centerX, centerY)
            ctx.lineTo(x, y)

            //ctx.closePath()
            ctx.stroke()
        }
        Connections {
            target: s_hud
            onTelemetryChanged: {
                var ctx = hudCanvas.getContext("2d")
                ctx.reset()

                hudCanvas.requestPaint()
            }
        }
    }
*/
    Image {
        id: horizonLine
        anchors.centerIn: parent
        source: "qrc:/images/horizon.png"
        fillMode: Image.PreserveAspectCrop
        width: root.width / 2
        transform: Rotation
        {
            origin.x: horizonLine.width / 2
            origin.y: horizonLine.height / 2
            axis { x: 0; y: 0; z: 1 }
            angle: s_hud.localFrameEuler.y * 180.0 / 3.141592653
        }
    }

    Image {
        id: tickImage
        source: "qrc:/images/vertical_tick.png"
        visible: false
    }

    Rectangle {
        id: verticalTicks
        property real verticalTickOffset: root.altitudeToToHome % 1 * tickImage.height
        color: "transparent"
        clip: true
        x: parent.width / 2 + horizonLine.width / 2
        y: parent.height / 3
        height: parent.height / 3
        width: root.verticalTicksWidth
        Rectangle {
            id: background
            color: "transparent"
            y: -tickImage.height + verticalTicks.verticalTickOffset
            height: parent.height + tickImage.height*2
            anchors.left: parent.left
            anchors.right: parent.right
            Image {
                source: "qrc:/images/vertical_tick.png"
                fillMode: Image.TileVertically
                anchors.fill: parent
                //opacity: 0.3
            }
        }
    }
    GridLayout {
        id: altitudes
        x: verticalTicks.x + verticalTicks.width
        anchors.verticalCenter: parent.verticalCenter
        columns: 2
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: "HOME "
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: root.isAltitudeToToHomeValid ? root.altitudeToToHome.toFixed(1) + " m" : "---"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: "SEA "
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: root.isAltitudeToToSeaValid ? root.altitudeToToSea.toFixed(1) + " m" : "---"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: "VSPD "
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: s_hud.enuVelocity.z.toFixed(1) + " m/s²"
        }
    }


    RowLayout {
        id: topBar
        anchors.top: parent.top
        anchors.right: parent.right
        spacing: 4

        Label {
            TextMetrics {
                font.pointSize: root.telemetryFontSize;
                id: rxRSSIMetrics
                text: "Rx: -999dBm "
            }

            Layout.preferredWidth: rxRSSIMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Rx: " + s_hud.radioRxRSSI + "dBm"
            color: getRadioRxRSSIColor(s_hud.radioRxRSSI)
            SequentialAnimation on opacity {
                running: isRadioRxRSSICritical(s_hud.radioRxRSSI)
                PropertyAnimation { to: 1; duration: 200 }
                PropertyAnimation { to: 0.5; duration: 200 }
            }
        }
        Label {
            TextMetrics {
                font.pointSize: root.telemetryFontSize;
                id: txRSSIMetrics
                text: "Tx: -999dBm "
            }

            Layout.preferredWidth: txRSSIMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Tx: " + s_hud.radioTxRSSI + "dBm"
            color: getRadioTxRSSIColor(s_hud.radioTxRSSI)
            SequentialAnimation on opacity {
                running: isRadioTxRSSICritical(s_hud.radioTxRSSI)
                PropertyAnimation { to: 1; duration: 200 }
                PropertyAnimation { to: 0.5; duration: 200 }
            }
        }
        Label {
            TextMetrics {
                font.pointSize: root.telemetryFontSize;
                id: videoRxRSSIMetrics
                text: "Video: -999dBm "
            }

            Layout.preferredWidth: videoRxRSSIMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: "Video: " + s_hud.videoRxRSSI + "dBm"
            color: getVideoRxRSSIColor(s_hud.videoRxRSSI)
            SequentialAnimation on opacity {
                running: isVideoRxRSSICritical(s_hud.videoRxRSSI)
                PropertyAnimation { to: 1; duration: 200 }
                PropertyAnimation { to: 0.5; duration: 200 }
            }
        }
        Label {
            TextMetrics {
                font.pointSize: root.telemetryFontSize;
                id: ampsMetrics
                text: "999.9A "
            }

            Layout.preferredWidth: ampsMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: s_hud.batteryAverageCurrent.toFixed(1) + "A"
        }
        Label {
            TextMetrics {
                font.pointSize: root.telemetryFontSize;
                id: voltsMetrics
                text: "99.9V "
            }

            Layout.preferredWidth: voltsMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: s_hud.batteryAverageVoltage.toFixed(1) + "V"
            color: getUAVBatteryVoltageColor(s_hud.batteryAverageVoltage)
            SequentialAnimation on opacity {
                running: isUAVBatteryVoltageCritical(s_hud.batteryAverageVoltage)
                PropertyAnimation { to: 1; duration: 200 }
                PropertyAnimation { to: 0.5; duration: 200 }
            }
        }

        Image {
            Layout.preferredHeight: root.batteryBarSize
            Layout.preferredWidth: root.batteryBarSize
            fillMode: Image.PreserveAspectFit
            source: getUAVBatteryLevelIcon(s_hud.batteryCapacityLeft)
            SequentialAnimation on opacity {
                running: isUAVBatteryLevelCritical(s_hud.batteryCapacityLeft)
                PropertyAnimation { to: 1; duration: 200 }
                PropertyAnimation { to: 0.5; duration: 200 }
            }
        }
        Label {
            TextMetrics {
                font.pointSize: telemetryFontSize;
                id: percentMetrics
                text: "100% "
            }

            Layout.preferredWidth: percentMetrics.width
            style: Text.Outline; styleColor: "black"
            font.pointSize: root.telemetryFontSize;
            text: (s_hud.batteryCapacityLeft * 100.0).toFixed(0)  + "%"
            color: getUAVBatteryLevelColor(s_hud.batteryCapacityLeft)
            SequentialAnimation on opacity {
                running: isUAVBatteryLevelCritical(s_hud.batteryCapacityLeft)
                PropertyAnimation { to: 1; duration: 200 }
                PropertyAnimation { to: 0.5; duration: 200 }
            }
        }
    }
    RowLayout {
        id: cameraBar
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        spacing: 4

        Button {
            id: recordButton
            Layout.preferredWidth: root.cameraButtonSize
            Layout.preferredHeight: root.cameraButtonSize
            checkable: true
            opacity: 0.5
            Image {
                source: "qrc:/icons/ui/recording.png";
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                anchors.margins: 4
                opacity: recordButton.checked ? 1.0 : 0.5
            }
            onClicked: {
                s_hud.isRecording = recordButton.checked;
            }
        }
        Button {
            id: streamingQualityButton
            Layout.preferredWidth: root.cameraButtonSize
            Layout.preferredHeight: root.cameraButtonSize
            checkable: true
            opacity: 0.5
            Image {
                source: "qrc:/icons/ui/hq.png";
                fillMode: Image.PreserveAspectFit
                anchors.fill: parent
                anchors.margins: 4
                opacity: streamingQualityButton.checked ? 1.0 : 0.5
            }
            onClicked: {
                s_hud.streamQuality = streamingQualityButton.checked ? HUD.STREAM_QUALITY_HIGH : HUD.STREAM_QUALITY_LOW;
            }
        }
        Item {
            Layout.fillHeight: true
            opacity: 0
        }
    }
    GridLayout {
        id: locationInfoBar
        anchors.top: topBar.bottom
        anchors.right: parent.right
        columns: 2

        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: "HOME "
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: root.isDistanceToHomeValid ? root.distanceToHome.toFixed(1) + " m" : "---"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: "ME "
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: root.isDistanceToMeValid ? root.distanceToMe.toFixed(1) + " m" : "---"
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: "HSPD "
        }
        Label {
            style: Text.Outline; styleColor: "black"
            color: "#00FF00"
            font.pointSize: root.telemetryFontSize;
            text: (s_hud.enuVelocity.toVector2d().length()).toFixed(1) + " m/s²"
        }
    }



    ColumnLayout {
        id: modeBar
        anchors.top: topBar.bottom
        anchors.left: parent.left

        Item {
            id: mode
            width: childrenRect.width
            height: childrenRect.height
            RowLayout {
                Label {
                    id: currentModeText
                    leftPadding: root.modeButtonHorizontalMargin / 2
                    rightPadding: root.modeButtonHorizontalMargin / 2
                    topPadding: root.modeButtonVerticalMargin / 2
                    bottomPadding: root.modeButtonVerticalMargin / 2
                    style: Text.Outline; styleColor: "black"
                    color: s_hud.isTargetModeConfirmed ? root.modeButtonConfirmedTextColor : root.modeButtonUnconfirmedTextColor
                    font.pointSize: root.modeButtonFontSize
                    text: s_hud.isTargetModeConfirmed ? getModeText(s_hud.currentMode) : getModeText(s_hud.currentMode) + "->" + getModeText(s_hud.targetMode)
                    SequentialAnimation on opacity {
                        running: !s_hud.isTargetModeConfirmed
                        loops: Animation.Infinite
                        alwaysRunToEnd: true
                        PropertyAnimation { to: 0.5; duration: 150 }
                        PropertyAnimation { to: 1; duration: 150 }
                    }
                    Connections {
                        target: s_hud
                        onTargetModeConfirmedChanged: {
                            currentModeText.opacity = 1
                        }
                    }

                    background: Rectangle {
                        border.color: root.modeButtonBorderColor
                        color: root.modeButtonBackgroundColor
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            newModes.visible = !newModes.visible
                        }
                    }
                }
                Item {
                    id: newModes
                    width: childrenRect.width
                    height: childrenRect.height
                    visible: false
                    RowLayout {
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getModeText(HUD.MODE_IDLE)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetMode = HUD.MODE_IDLE
                                    newModes.visible = !newModes.visible
                                }
                            }
                        }
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getModeText(HUD.MODE_FLY)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetMode = HUD.MODE_FLY
                                    newModes.visible = !newModes.visible
                                }
                            }
                        }
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getModeText(HUD.MODE_RETURN_HOME)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetMode = HUD.MODE_RETURN_HOME
                                    newModes.visible = !newModes.visible
                                }
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: verticalMode
            width: childrenRect.width
            height: childrenRect.height
            RowLayout {
                Label {
                    id: currentVerticalModeText
                    leftPadding: root.modeButtonHorizontalMargin / 2
                    rightPadding: root.modeButtonHorizontalMargin / 2
                    topPadding: root.modeButtonVerticalMargin / 2
                    bottomPadding: root.modeButtonVerticalMargin / 2
                    style: Text.Outline; styleColor: "black"
                    color: s_hud.isTargetVerticalModeConfirmed ? root.modeButtonConfirmedTextColor : root.modeButtonUnconfirmedTextColor
                    font.pointSize: root.modeButtonFontSize
                    text: s_hud.isTargetVerticalModeConfirmed ? getVerticalModeText(s_hud.currentVerticalMode) : getVerticalModeText(s_hud.currentVerticalMode) + "->" + getVerticalModeText(s_hud.targetVerticalMode)
                    SequentialAnimation on opacity {
                        running: !s_hud.isTargetVerticalModeConfirmed
                        loops: Animation.Infinite
                        alwaysRunToEnd: true
                        PropertyAnimation { to: 0.5; duration: 150 }
                        PropertyAnimation { to: 1; duration: 150 }
                    }
//                    Connections {
//                        target: s_hud
//                        onTargetVerticalModeConfirmedChanged: {
//                            currentVerticalModeText.opacity = 1
//                        }
//                    }

                    background: Rectangle {
                        border.color: root.modeButtonBorderColor
                        color: root.modeButtonBackgroundColor
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            newVerticalModes.visible = !newVerticalModes.visible
                        }
                    }
                }
                Item {
                    id: newVerticalModes
                    width: childrenRect.width
                    height: childrenRect.height
                    visible: false
                    RowLayout {
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getVerticalModeText(HUD.VERTICAL_MODE_THRUST)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetVerticalMode = HUD.VERTICAL_MODE_THRUST
                                    newVerticalModes.visible = !newVerticalModes.visible
                                }
                            }
                        }
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getVerticalModeText(HUD.VERTICAL_MODE_ALTITUDE)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetVerticalMode = HUD.VERTICAL_MODE_ALTITUDE
                                    newVerticalModes.visible = !newVerticalModes.visible
                                }
                            }
                        }
                    }
                }
            }
        }


        Item {
            id: horizontalMode
            width: childrenRect.width
            height: childrenRect.height
            RowLayout {
                Label {
                    id: currentHorizontalModeText
                    leftPadding: root.modeButtonHorizontalMargin / 2
                    rightPadding: root.modeButtonHorizontalMargin / 2
                    topPadding: root.modeButtonVerticalMargin / 2
                    bottomPadding: root.modeButtonVerticalMargin / 2
                    style: Text.Outline; styleColor: "black"
                    color: s_hud.isTargetHorizontalModeConfirmed ? root.modeButtonConfirmedTextColor : root.modeButtonUnconfirmedTextColor
                    font.pointSize: root.modeButtonFontSize
                    text: s_hud.isTargetHorizontalModeConfirmed ? getHorizontalModeText(s_hud.currentHorizontalMode) : getHorizontalModeText(s_hud.currentHorizontalMode) + "->" + getHorizontalModeText(s_hud.targetHorizontalMode)
                    SequentialAnimation on opacity {
                        running: !s_hud.isTargetHorizontalModeConfirmed
                        loops: Animation.Infinite
                        alwaysRunToEnd: true
                        PropertyAnimation { to: 0.5; duration: 150 }
                        PropertyAnimation { to: 1; duration: 150 }
                    }
//                    Connections {
//                        target: s_hud
//                        onTargetHorizontalModeConfirmedChanged: {
//                            currentHorizontalModeText.opacity = 1
//                        }
//                    }

                    background: Rectangle {
                        border.color: root.modeButtonBorderColor
                        color: root.modeButtonBackgroundColor
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            newHorizontalModes.visible = !newHorizontalModes.visible
                        }
                    }
                }
                Item {
                    id: newHorizontalModes
                    width: childrenRect.width
                    height: childrenRect.height
                    visible: false
                    RowLayout {
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getHorizontalModeText(HUD.HORIZONTAL_MODE_ANGLE_RATE)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetHorizontalMode = HUD.HORIZONTAL_MODE_ANGLE_RATE
                                    newHorizontalModes.visible = !newHorizontalModes.visible
                                }
                            }
                        }
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getHorizontalModeText(HUD.HORIZONTAL_MODE_ANGLE)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetHorizontalMode = HUD.HORIZONTAL_MODE_ANGLE
                                    newHorizontalModes.visible = !newHorizontalModes.visible
                                }
                            }
                        }
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getHorizontalModeText(HUD.HORIZONTAL_MODE_POSITION)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetHorizontalMode = HUD.HORIZONTAL_MODE_POSITION
                                    newHorizontalModes.visible = !newHorizontalModes.visible
                                }
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: yawMode
            width: childrenRect.width
            height: childrenRect.height
            RowLayout {
                Label {
                    leftPadding: root.modeButtonHorizontalMargin / 2
                    rightPadding: root.modeButtonHorizontalMargin / 2
                    topPadding: root.modeButtonVerticalMargin / 2
                    bottomPadding: root.modeButtonVerticalMargin / 2
                    style: Text.Outline; styleColor: "black"
                    color: s_hud.isTargetYawModeConfirmed ? root.modeButtonConfirmedTextColor : root.modeButtonUnconfirmedTextColor
                    font.pointSize: root.modeButtonFontSize
                    text: s_hud.isTargetYawModeConfirmed ? getYawModeText(s_hud.currentYawMode) : getYawModeText(s_hud.currentYawMode) + "->" + getYawModeText(s_hud.targetYawMode)
                    SequentialAnimation on opacity {
                        running: !s_hud.isTargetYawModeConfirmed
                        loops: Animation.Infinite
                        alwaysRunToEnd: true
                        PropertyAnimation { to: 0.5; duration: 150 }
                        PropertyAnimation { to: 1; duration: 150 }
                    }
//                    Connections {
//                        target: s_hud
//                        onTargetYawModeConfirmedChanged: {
//                            currentYawModeText.opacity = 1
//                        }
//                    }

                    background: Rectangle {
                        border.color: root.modeButtonBorderColor
                        color: root.modeButtonBackgroundColor
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            newYawModes.visible = !newYawModes.visible
                        }
                    }
                }
                Item {
                    id: newYawModes
                    width: childrenRect.width
                    height: childrenRect.height
                    visible: false
                    RowLayout {
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getYawModeText(HUD.YAW_MODE_ANGLE_RATE)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetYawMode = HUD.YAW_MODE_ANGLE_RATE
                                    newYawModes.visible = !newYawModes.visible
                                }
                            }
                        }
                        Label {
                            leftPadding: root.modeButtonHorizontalMargin / 2
                            rightPadding: root.modeButtonHorizontalMargin / 2
                            topPadding: root.modeButtonVerticalMargin / 2
                            bottomPadding: root.modeButtonVerticalMargin / 2
                            style: Text.Outline; styleColor: "black"
                            color: root.modeButtonUnconfirmedTextColor
                            font.pointSize: root.modeButtonFontSize
                            text: getYawModeText(HUD.YAW_MODE_ANGLE)
                            background: Rectangle {
                                border.color: root.modeButtonBorderColor
                                color: root.modeButtonBackgroundColor
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    s_hud.targetYawMode = HUD.YAW_MODE_ANGLE
                                    newYawModes.visible = !newYawModes.visible
                                }
                            }
                        }
                    }
                }
            }
        }


    }

    PositionSource {
        id: userPosition
        updateInterval: 1000
        active: true

        onPositionChanged: {
            if (map.freeMapPanning === false && map.centerMapOn === 1) {
                var coord = userPosition.position.coordinate;
                map.center = coord;
            }
        }
    }



    Plugin {
        id: osmplugin

        name: "osm"
        PluginParameter { name: "osm.useragent"; value: "silkopter_viewer" }
        PluginParameter { name: "osm.mapping.host"; value: "http://osm.tile.server.address/" }
        PluginParameter { name: "osm.mapping.copyright"; value: "All mine" }
        PluginParameter { name: "osm.routing.host"; value: "http://osrm.server.address/viaroute" }
        PluginParameter { name: "osm.geocoding.host"; value: "http://geocoding.server.address" }
    }
    Plugin {
        id: hereplugin

        name: "here"
        PluginParameter { name: "here.app_id"; value: "Z5ZvL9i1ZOjQhZsGxVR6" }
        PluginParameter { name: "here.token"; value: "owBddrRguz4pZxOyTlc37g" }
        PluginParameter { name: "here.mapping.cache.disk.size"; value: "300" }
        PluginParameter { name: "here.mapping.cache.memory.size"; value: "100" }
        PluginParameter { name: "here.mapping.cache.texture.size"; value: "100" }
    }
    Plugin {
        id: mapboxplugin

        name: "mapbox"
        PluginParameter { name: "mapbox.access_token"; value: "pk.eyJ1IjoiamVhbmxlZmxhbWJldXIiLCJhIjoiY2oyZWl5OThkMDA0bDJ3cGE1Ym9ob3ptaiJ9.Azg-At8Zzd-H98_Tuf4IwA" }
    }

//    GroupBox {
//        id: mapTypeGroup
//        anchors.top: parent.top
//        title: "Map Types"
//        ComboBox {
//            //width: 150
//            model: map.supportedMapTypes
//            textRole: "description"
//            onCurrentIndexChanged: map.activeMapType = map.supportedMapTypes[currentIndex]
//        }
//    }

    Item {
        id: mapContainer
        //visible: false
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: root.bigMapWidth
        height: root.bigMapWidth

        Map {
            id: map
            anchors.fill: parent
            //opacity: 0.7
            plugin: mapboxplugin
            zoomLevel: 10//map.minimumZoomLevel //(map.minimumZoomLevel + map.maximumZoomLevel) / 2
            gesture.enabled: true
            copyrightsVisible: false

            property int centerMapOn: 0;
            property bool freeMapPanning: false;

            Component.onCompleted: {
                var count = map.supportedMapTypes.length
                console.log('Number of supported map types: ' + count)
                console.log('Supported map types:')
                for (var i = 0; i < count; i++) {
                    console.log('style: ' + map.supportedMapTypes[i].style
                                + ', name: ' + map.supportedMapTypes[i].name
                                + ', desc: ' + map.supportedMapTypes[i].description
                                + ', mobile: ' + map.supportedMapTypes[i].mobile)
                    if (map.supportedMapTypes[i].style === MapType.SatelliteMapDay ||
                            map.supportedMapTypes[i].style === MapType.SatelliteMapNight)
                    {
                        console.log("    satellite map type found, setting as active");
                        map.activeMapType = map.supportedMapTypes[i];
                    }
                }
            }

            MapPolyline {
                id: history
                line.width: 4
                line.color: "#60FF00FF"
            }

            Connections {
                target: s_hud
                onPathPointAdded: {
                    history.addCoordinate(point);
                }
                onPathCleared: {
                    history = [];
                }
                onTelemetryChanged: {
                    if (map.freeMapPanning === false && map.centerMapOn === 0) {
                        map.center = s_hud.position;
                    }
                }
            }
            Connections {
                target: map.gesture
                //unlock the centering when panning
                onPanStarted: {
                    map.freeMapPanning = true;
                }
            }

            MapCircle {
                id: userPositionIndicator
                center: userPosition.position.coordinate
                radius: 2.0
                SequentialAnimation on color {
                    loops: Animation.Infinite
                    ColorAnimation { from: "#60FF0000"; to: "white"; duration: 1000 }
                    ColorAnimation { from: "white"; to: "#60FF0000"; duration: 1000 }
                }
                border.width: 2
            }
            MapCircle {
                id: homePositionIndicator
                center: s_hud.homePosition
                radius: 3.0
                SequentialAnimation on color {
                    loops: Animation.Infinite
                    ColorAnimation { from: "blue"; to: "white"; duration: 1000 }
                    ColorAnimation { from: "white"; to: "blue"; duration: 1000 }
                }
                border.width: 2
                visible: s_hud.homePosition.isValid
            }
            MapCircle {
                id: positionIndicator
                center: s_hud.position
                radius: 2.0
                SequentialAnimation on color {
                    loops: Animation.Infinite
                    ColorAnimation { from: "green"; to: "white"; duration: 1000 }
                    ColorAnimation { from: "white"; to: "green"; duration: 1000 }
                }
                border.width: 2
            }

            function getCenterMapOnIcon() {
                if (map.centerMapOn == 0) return "qrc:/icons/ui/uav-multirotor.png";
                if (map.centerMapOn == 1) return "qrc:/icons/ui/location.png";
                return ""
            }

            function showBigMap(show) {
                changeMapCenteringButton.visible = show;
                openBigMapButton.visible = !show;
                closeBigMapButton.visible = show;

                if (show === true) {
                    mapContainer.width = root.bigMapWidth;
                    mapContainer.height = root.bigMapHeight;
                } else {
                    mapContainer.width = root.smallMapWidth;
                    mapContainer.height = root.smallMapHeight;
                }
            }

            Button {
                id: changeMapCenteringButton
                anchors.right: map.right
                anchors.top: map.top
                width: root.mapButtonSize
                height: width
                opacity: 0.5

                Image {
                    opacity: map.freeMapPanning === false ? 1.0 : 0.3;
                    source: map.getCenterMapOnIcon();
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    anchors.margins: 8
                }

                onClicked: {
                    if (map.freeMapPanning === true) {
                        map.freeMapPanning = false;
                    } else {
                        map.centerMapOn = map.centerMapOn + 1;
                        if (map.centerMapOn > 1) {
                            map.centerMapOn = 0;
                        }
                    }

                    if (map.centerMapOn === 0) {
                        map.center = s_hud.position
                    } else if (map.centerMapOn === 1) {
                        map.center = userPosition.position.coordinate
                    }
                }
            }
            Button {
                id: closeBigMapButton
                anchors.right: map.right
                anchors.top: changeMapCenteringButton.bottom
                width: root.mapButtonSize
                height: width
                opacity: 0.5

                Image {
                    source: "qrc:/icons/ui/back.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    anchors.margins: 8
                }
                onClicked: {
                    map.showBigMap(false);
                }
            }
            MouseArea {
                id: openBigMapButton
                anchors.fill: parent
                onClicked: {
                    map.showBigMap(true);
                }
            }
        }
    }

//    RadialGradient {
//        id: mapGradientMask
//        anchors.fill: mapContainer
//        gradient: Gradient {
//            GradientStop { position: 0.0; color: "#FF000000" }
//            GradientStop { position: 0.45; color: "#FF000000" }
//            GradientStop { position: 0.5; color: "#00000000" }
//        }
//        visible: false
//    }

//    Item {
//        id: mapGradientMask
//        anchors.fill: mapContainer
//        RectangularGlow {
//            id: __maskRectGlow
//            anchors.fill: __maskRect
//            glowRadius: 10
//            spread: 0.2
//            color: "#FFFFFFFF"
//        }
//        Rectangle {
//            id: __maskRect
//            color: "#FFFFFFFF"
//            anchors.centerIn: parent
//            width: parent.width - 20
//            height: parent.height - 20
//        }
//    }

//    OpacityMask {
//        anchors.fill: mapContainer
//        source: mapContainer
//        maskSource: mapGradientMask
//    }
}
