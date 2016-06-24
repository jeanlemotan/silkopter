import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import Qt.labs.settings 1.0
import com.silk.HAL 1.0
import com.silk.Comms 1.0


Rectangle {
    id: root
    width: 320; height: 480
    color: "#2c3e50"

    TopBar {
        id: topBar
        width: parent.width
        title: "Multirotor Config"
        onBackPressed: s_menus.pop()
    }

    property var config: JSON.parse(s_hal.configJSon);

    onConfigChanged: {
        console.log(s_hal.configJSon)
        console.log(config)
        massValue.text = config.mass
    }

    TextInput {
        id: massValue
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
        focus:true
        validator:DoubleValidator {
            bottom:0;
            top:100;
            decimals: 2
        }
//        text: config.mass
    }

}
