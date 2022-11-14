import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects
import "logic.js" as Logic
import backend

// Measures the instantaneous speed of a given moment

Item {
    id: speedometer
    width: 600
    height: 600
    visible: true
    antialiasing: true

    Image {
        id: ellipse_gray_brackground
        source: "/Ellipse_Gray_Background.png"
        anchors.centerIn: parent
    }

    Text {
        id: zero_digit
        x: 90
        y: 475
        text: qsTr("0")
        font.pointSize: 30
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: twohundredeighty_digit
        x: 475
        y: 475
        text: qsTr("280")
        font.pointSize: 30
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Image {
        id: white_indicators
        source: "\White_Indicators.png"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#000000"
            transparentBorder: false
            horizontalOffset: 1
            verticalOffset: 1
            radius: 2
            source: white_indicators
        }
    }

    Image {
        id: ellipse_silver_ring
        source: "/Ellipse_Silver_Ring.png"
        anchors.centerIn: parent
    }

    Canvas {
        id: cpb1
        anchors.centerIn: parent
        width: 500
        height: 500
        property int instspeed: Dashboard.instspeed
        onPaint: {
            var ctx = getContext("2d");

            var grd = ctx.createRadialGradient(250, 250, 225, 250, 250, 275);
            grd.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd.addColorStop(0.5, "rgba(25, 255, 255, 0.5)");
            grd.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();
            ctx.lineWidth = 40;
            ctx.strokeStyle = grd;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-20, Logic.converDegToRad(135), Logic.converDegToRad(Logic.seagreenIndicatorTail(Dashboard.instspeed)));
            ctx.stroke();

            var grd2 = ctx.createRadialGradient(250, 250, 175, 250, 250, 225);
            grd2.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd2.addColorStop(0.5, "rgba(255, 255, 255, 0.5)");
            grd2.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();
            ctx.lineWidth = 60;
            ctx.strokeStyle = grd2;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-80, Logic.converDegToRad(135), Logic.converDegToRad(Logic.seagreenIndicatorTail(Dashboard.instspeed)));
            ctx.stroke();
        }

        function clear()
        {
            var ctx = getContext("2d");
            ctx.reset();
            this.requestPaint();
        }

        onInstspeedChanged: clear()
    }

    Image {
        id: seagreen_indicator
        source: "\Seagreen_Indicator.png"
        anchors.centerIn: parent
        rotation: Logic.SeagreenIndicatorRotation(Dashboard.instspeed) // [0 deg = 0km/h] | [270 deg = 280 km/h]
    }

    Image {
        id: ellipse_seagreen_indicator
        source: "\Ellipse_Seagreen_Indicator.png"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#000000"
            transparentBorder: false
            horizontalOffset: 4
            verticalOffset: 6
            radius: 16
            source: ellipse_seagreen_indicator
        }
    }

    Glow {
        id: glow_ellipse_seagreen_indicator
        anchors.fill: ellipse_seagreen_indicator
        radius: 8
        color: "green"
        source: ellipse_seagreen_indicator
    }

    Text {
        id: speed_unit
        x: 150
        y: 150
        width: 300
        height: 110
        text: qsTr("km/h")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 16
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: speed_value
        x: 150
        y: 150
        width: 300
        height: 300
        text: Dashboard.instspeed
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 120
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

}
