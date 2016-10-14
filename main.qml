import QtQuick 2.7
import QtQuick.Controls 1.5

Rectangle {
    color: "white"
    Label {
        id: dbValue
        font {
            pixelSize: 25
        }

        text: wrapper.db
        color: "red"
        anchors.centerIn: parent
    }

    Label {
        anchors {
            top: dbValue.bottom
            horizontalCenter: parent.horizontalCenter

            topMargin: 20
        }

        font {
            pixelSize: 25
        }

        text: Math.floor(Math.abs(wrapper.db))
        color: "red"
    }

    Column {
        anchors {
            left: parent.left
            bottom: parent.bottom
            leftMargin: 10
            bottomMargin: 10
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

    Connections {
        target: wrapper

        onDbChanged: {
            var items = Math.floor(Math.abs(level + 2))
            for(var i = 0; i < blockRepeater.count; ++i) {
                blockRepeater.itemAt(i).color = (i > items) ? "red" : "green"
            }
        }
    }
}
