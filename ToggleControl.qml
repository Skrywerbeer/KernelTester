import QtQuick

Rectangle {
    id: root

    property bool toggled
    property real buttonSize: 40
    property alias iconSource: icon.source

    width: buttonSize
    height: buttonSize
    radius: buttonSize/2
    border {width: 2; color: "black"}
    state: "Off"

    Image {
        id: icon
        anchors.centerIn: parent
        width: root.width/2
        height: root.height/2
        fillMode: Image.PreserveAspectFit
    }
    MouseArea {
        anchors.fill: parent
        onClicked: toggled = !toggled
    }

    states: [
        State {
            name: "Off"
            when: !toggled
            PropertyChanges {
                target: root
                color: "beige"
            }
        },
        State {
            name: "On"
            when: toggled
            PropertyChanges {
                target: root
                color: "steelblue"
            }
        }
    ]
    transitions: [
        Transition {
            from: "*"
            to: "*"
            ColorAnimation {duration: 200}
        }
    ]
}
