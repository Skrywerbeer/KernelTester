import QtQuick

Item {
    id: root

    property alias kernel: view.model

    BorderImage {
        id: border
//        height: childrenRect.height + 60
//        width: childrenRect.width + 80
        anchors.fill: parent
        source: "qrc:/Images/kernelborder1024X1024.png"
        TableView {
            id: view
            anchors {
                fill: parent
//                centerIn: parent
                leftMargin: 20
                rightMargin: 20
                topMargin: 20
                bottomMargin: 20
            }
            rowHeightProvider: function() {return view.height/view.rows}
            columnWidthProvider: function() {return view.width/view.columns}
            delegate: elementDelegate
            onWidthChanged: forceLayout()
            onHeightChanged: forceLayout()
        }
    }
    Component {
        id: elementDelegate

        Rectangle {
            id: delegateBackground

            border {width: 1; color: "white"}
//            color: "royalblue"
            state: "Inactive"
            TextEdit {
                id: delegateEdit
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 16
                text: element
                onEditingFinished: element = text
                Keys.onReturnPressed: {
                    element = text
                }
            }

            states: [
                State {
                    name: "Inactive"
                    when: !delegateEdit.cursorVisible
                    PropertyChanges {
                        target: delegateBackground
                        color: "transparent"
                    }
                },
                State {
                    name: "Active"
                    when: delegateEdit.cursorVisible
                    PropertyChanges {
                        target: delegateBackground
                        color: "royalblue"
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
    }
}
