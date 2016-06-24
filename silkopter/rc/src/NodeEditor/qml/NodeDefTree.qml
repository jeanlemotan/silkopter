import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.2

TreeView {
    //property var selectedItem;

    id: nodeDefTree

    TableViewColumn {
        title: "Icon"
        role: "Icon"
        width: 32
        delegate: Image {
            source: styleData.value
            anchors.margins: 8
            fillMode: Image.PreserveAspectFit
        }
    }
    TableViewColumn {
       title: "Name"
       role: "Name"
    }

    rowDelegate: Rectangle {
        height: 32
        color: (styleData.selected || styleData.hasActiveFocus) ? "#E75926" : (styleData.alternate ? "#282830" : "black")
    }

    /////////////////////////////////////////////////////////
    //this is needed to fix selection bug on touchscreens
    //ref: https://bugreports.qt.io/browse/QTBUG-47243
    selection: ItemSelectionModel {
        id: selModel
        model: nodeDefTree.model
    }
    onClicked: {
        selModel.clearCurrentIndex();
        selModel.setCurrentIndex(index, 0x0002 | 0x0010);
    }
    /////////////////////////////////////////////////////////

}
