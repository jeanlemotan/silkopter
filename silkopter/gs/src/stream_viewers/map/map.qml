import QtQuick 2.5
import QtQuick.Controls 1.4
import QtLocation 5.6
//import QtPositioning 5.5

Rectangle
{
    color: "red"

    //    Map {
    //        id: map
    //        anchors.fill: parent
    //        anchors.leftMargin: 32

    //        zoomLevel: (maximumZoomLevel - minimumZoomLevel)/2
    //        center {
    //            latitude: -27.5796
    //            longitude: 153.1003
    //        }
    //        // Enable pinch gestures to zoom in and out
    //        gesture.flickDeceleration: 3000
    //        gesture.enabled: true
    //    }

    Plugin {
        id: osmplugin

        name: "osm"
        PluginParameter { name: "osm.useragent"; value: "silkopter_gs" }
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
//        PluginParameter { name: "here.mapping.cache.disk.size"; value: "300" }
//        PluginParameter { name: "here.mapping.cache.memory.size"; value: "100" }
//        PluginParameter { name: "here.mapping.cache.texture.size"; value: "100" }
    }

    GroupBox{
        id: mapTypeGroup
        anchors.top: parent.top
        title:"Map Types"
        ComboBox {
            model:map.supportedMapTypes
            textRole: "description"
            onCurrentIndexChanged: map.activeMapType = map.supportedMapTypes[currentIndex]
        }
    }

    Map {
        id: map
        anchors.top: mapTypeGroup.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        plugin: osmplugin
        //plugin: hereplugin
        center {
            latitude: -27
            longitude: 153
        }
        zoomLevel: map.minimumZoomLevel //(map.minimumZoomLevel + map.maximumZoomLevel) / 2
        gesture.enabled: true
    }
}
