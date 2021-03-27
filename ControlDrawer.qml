import QtQuick

Item {
    id: root

    readonly property alias kernelEnabled: kernelSwitch.toggled
    readonly property alias rgbEnabled: rgbSwitch.toggled
    readonly property alias liveEnabled: liveSwitch.toggled

    width: parent.width
    state: "Closed"

    Rectangle {
        id: background

        anchors.fill: parent
        color: "gainsboro"
        opacity: 0.2
    }
    Rectangle {
        id: handle

        anchors {
            bottom: parent.top
            topMargin: 10
            leftMargin: 10
            rightMargin: 10
            bottomMargin: 10
            horizontalCenter: parent.horizontalCenter
        }
        width: 40
        height: width
        radius: width/2
        color: "gainsboro"
        opacity: 0.2
        border {width: 2; color: "black"}
        layer.enabled: true
        Image {
            anchors {
                fill: parent
                topMargin: 5
                bottomMargin: 5
                leftMargin: 5
                rightMargin: 5
            }
            source: "qrc:/Images/drawerhandle64X64.png"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.state = root.state === "Closed" ? "Opened" : "Closed"
            }
        }
    }
    KernelView {
        anchors {left: parent.left; verticalCenter: parent.verticalCenter}
        width: height
        height: parent.height
        kernel: kernelModel
    }
    Grid {
        anchors {
            top: parent.top
            right: parent.right
            topMargin: 16
        }
        spacing: 8

        ToggleControl {
            id: liveSwitch
            iconSource: "qrc:/Images/live128X128.png"
        }
        ToggleControl {
            id: rgbSwitch
            iconSource: "qrc:/Images/rgb128X128.png"
        }
        ToggleControl {
            id: kernelSwitch
            iconSource: "qrc:/Images/enablekernel128X128.png"
        }
    }
    states: [
        State {
            name: "Closed"
            PropertyChanges {
                target: root
                height: 0
            }
            PropertyChanges {
                target: handle
                rotation: 0
                opacity: 0.2
                color: "gainsboro"
            }
        },
        State {
            name: "Opened"
            PropertyChanges {
                target: root
                height: 200
            }
            PropertyChanges {
                target: handle
                rotation: 180
                opacity: 0.8
                color: "steelblue"
            }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"
            NumberAnimation {
                duration: 300
                properties: "height, rotation, opacity"
            }
            ColorAnimation {duration: 300}
        }
    ]
}
