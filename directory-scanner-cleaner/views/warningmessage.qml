import QtQuick
import QtQuick.Controls 2.5
import QtQuick.Layouts
import Qt.labs.platform 1.1

Window {
    id: warning_window
    visible: true
    width: 700
    height: 150
    title: qsTr("No such directory")
    color: 'lightgrey'
    GridLayout{
        anchors.fill: parent
        anchors.margins: 20
        rows: 2
        columns: 1
        rowSpacing: 10
        Rectangle{
            Layout.row: 0
            Layout.column: 0
            Layout.fillWidth: true
            height: 24
            color: 'lightgrey'
            Text{
                id: warning_text
                anchors {
                    centerIn: parent
                }

                text: "The directory does not exist or entered wrong. Please check specified path one more time and try again!"
                font {
                    pixelSize: 14
                }
            }
        }

        Button{
            implicitWidth: 90
            Layout.row: 1
            Layout.alignment: Qt.AlignRight
            text: qsTr("Close")
            onClicked: {
                console.log('close button pressed')
                warning_window.close()
            }
        }
    }
}
