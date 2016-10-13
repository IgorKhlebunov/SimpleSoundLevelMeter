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
            bottomMargin: 20
        }

        width: parent.width / 8
        height: parent.height / 2
        spacing: 5

        Repeater {
            anchors.fill: parent
                Rectangle {
                    width: parent.width - 4
                    height: (parent.height / 15) - 4
                    radius: 3

                    color: "green"

                    Behavior on color {
                        ColorAnimation {duration: 200}
                    }
                }
            model: 15
        }
    }


}
