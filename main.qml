import QtQuick 2.7
import QtQuick.Controls 1.5

Rectangle {
    id: root

    ComboBox {
        id: inputDevices

        anchors {
            top: parent.top
            left: parent.left

            margins: 20
        }

        width: 200
        model: wrapper.inputDevices
        enabled: !startButton.isStart

        onCurrentIndexChanged: {
            wrapper.changeInputDevice(currentText)
        }
    }

    Button {
        id: updateButton

        anchors {
            top: parent.top
            left: inputDevices.right
            margins: 20
        }

        enabled: !startButton.isStart

        text: "Update"

        onClicked: {
            wrapper.updateInputDevices()
        }
    }

    Button {
        id: startButton

        anchors {
            top: parent.top
            left: updateButton.right
            margins: 20
        }

        property bool isStart:  false

        text: isStart ? "Stop" : "Start"

        onClicked: {
            isStart = !isStart

            if (isStart) {
                wrapper.start()
            }
            else {
                wrapper.stop()
            }
        }
    }

    Label {
        id: dbValue
        anchors {
            left: soundMeter.right
            verticalCenter: soundMeter.verticalCenter
            leftMargin: 20
        }

        font {
            pixelSize: 25
        }

        text: Math.floor(((wrapper.db * 10) + 100))
        color: "red"
    }

    Column {
        id: soundMeter
        anchors {
            top: inputDevices.bottom
            left: parent.left
            leftMargin: 20
            topMargin: 20
        }

        width: parent.width / 10
        height: parent.height / 3
        spacing: 5

        Repeater {
            id: blockRepeater
            anchors.fill: parent
                Rectangle {
                    width: parent.width - 4
                    height: (parent.height / 10) - 4
                    radius: 3

                    color: "green"

                    Behavior on color {
                        ColorAnimation {duration: 200}
                    }
                }
            model: 10
        }
    }


    TextArea {
        id: terminal

        anchors {
            top: soundMeter.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom

            margins: 20
        }

        readOnly: true

    }

    Connections {
        target: wrapper

        onDbChanged: {
            var value = Math.floor(Math.abs(level))
            for(var i = 0; i < blockRepeater.count; ++i) {
                blockRepeater.itemAt(i).color = (i > value) ? "red" : "green"
            }
        }
    }

    Connections {
        target: server

        onConsoleMessage: {
            terminal.append(message)
        }
    }
}
