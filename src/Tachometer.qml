import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects
import "logic.js" as Logic
import backend

// Displays engineSpeed

Item {
    id: tachometer
    width: 600
    height: 600
    visible: true
    antialiasing: true

    property int sizeDigits: 30

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
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: one_digit
        x: 10
        y: 327
        text: qsTr("1")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: two_digit
        x: 23
        y: 165
        text: qsTr("2")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: three_digit
        x: 123
        y: 34
        text: qsTr("3")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: four_digit
        x: 285
        y: 0
        text: qsTr("4")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: five_digit
        x: 431
        y: 34
        text: qsTr("5")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: six_digit
        x: 537
        y: 165
        text: qsTr("6")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: seven_digit
        x: 550
        y: 327
        text: qsTr("7")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: eight_digit
        x: 475
        y: 475
        text: qsTr("8")
        font.pointSize: sizeDigits
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Image {
        id: tachometer_indicators
        source: "\Tachometer_Indicators.png"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: DropShadow {
            color: "#000000"
            transparentBorder: false
            horizontalOffset: 1
            verticalOffset: 1
            radius: 2
            source: tachometer_indicators
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
        property int engineSpeed: Dashboard.engineSpeed
        onPaint: {
            var ctx = getContext("2d");

            var grd = ctx.createRadialGradient(250, 250, 225, 250, 250, 275);
            grd.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd.addColorStop(0.5, "rgba(25, 255, 255, 0.5)");
            grd.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();
            ctx.lineWidth = 40;
            ctx.strokeStyle = grd;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-20, Logic.converDegToRad(135), Logic.converDegToRad(Logic.engineSpeedIndicatorTail(Dashboard.engineSpeed)));
            ctx.stroke();

            var grd2 = ctx.createRadialGradient(250, 250, 175, 250, 250, 225);
            grd2.addColorStop(0.0, "rgba(255, 255, 255, 0.0)");
            grd2.addColorStop(0.5, "rgba(255, 255, 255, 0.5)");
            grd2.addColorStop(1.0, "rgba(255, 255, 255, 1.0)");

            ctx.beginPath();

            ctx.font = '60px "RobotoMono Regular"';
            ctx.fillStyle = "#008b8b";   // #008b8b  // "#ffffff"
            Logic.drawTextAlongArc(ctx, "RPM x 1K", cpb1.width/2, cpb1.height/2, cpb1.width/2-10, Logic.converDegToRad(35));

            ctx.lineWidth = 60;
            ctx.strokeStyle = grd2;
            ctx.arc(cpb1.width/2, cpb1.height/2, cpb1.width/2-80, Logic.converDegToRad(135), Logic.converDegToRad(Logic.engineSpeedIndicatorTail(Dashboard.engineSpeed)));
            ctx.stroke();
        }

        function clear()
        {
            var ctx = getContext("2d");
            ctx.reset();
            this.requestPaint();
        }

        onEngineSpeedChanged: clear()

    }

    Image {
        id: seagreen_indicator
        source: "\Seagreen_Indicator.png"
        anchors.centerIn: parent
        rotation: Logic.engineSpeedIndicatorRotation(Dashboard.engineSpeed) // [0 deg = 0km/h] | [270 deg = 280 km/h]
        //Dashboard.onEngineSpeedChanged: console.log("test")
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
        id: driving_mode
        x: 150
        y: 150
        width: 300
        height: 300
        text: String.fromCharCode(Dashboard.driveMode)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 120
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

    Text {
        id: gear_mode
        x: 150+70
        y: 150-40
        width: 300
        height: 300
        text: String.fromCharCode(Dashboard.gear)
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 30
        font.family: "RobotoMono Regular"
        color: "#ffffff"
    }

}
