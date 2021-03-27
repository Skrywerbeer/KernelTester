import QtQuick
import QtQuick.Window

import cam

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

//    KernelModel {id: kmodel}
    CameraView {
        id: view
        anchors.fill: parent
        captureWidth: 320
        captureHeight: 240
        greyscale: !controls.rgbEnabled
        applyKernel: controls.kernelEnabled
        refreshInterval: 200
        live: controls.liveEnabled
//        captureDelta: true
//        kernel: kmodel
        kernel: kernelModel
        Component.onCompleted: {
            captureFrame()
            updateView()
        }
    }
    ControlDrawer {
        id: controls

        anchors.bottom: parent.bottom
    }

}
