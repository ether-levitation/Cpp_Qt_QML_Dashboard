import QtQuick
import QtQuick.Shapes
import Qt5Compat.GraphicalEffects
import "logic.js" as Logic
import backend

Item {
    id: fuelTank
    x: 0
    y: 25

    Image {
        id: fuelTankImage
        source: "\Fuel_Tank.png"
        width: 50
        height: 50
    }

    Canvas {
        id: fuelTankProgressBar
        anchors.left: fuelTankImage.right
        width: 350
        height: 50
        property int fuelTank: Dashboard.fuelTank
        onPaint: {
            var ctx = getContext("2d");

            var grd = ctx.createLinearGradient(250, 10, 250, 35);

            var currentFuel = (Dashboard.fuelTank / 10000);

            // Green color parameters
            var RGB_G_00 = "rgba(0, 255, 0, 0.6)",
                RGB_G_05 = "rgba(0, 255, 0, 0.3)",
                RBB_G_10 = "rgba(0, 255, 0, 0.0)";
            // Red color parameters
            var RGB_R_00 = "rgba(255, 0, 0, 0.6)",
                RGB_R_05 = "rgba(255, 0, 0, 0.3)",
                RBB_R_10 = "rgba(255, 0, 0, 0.0)";

            // When tank capacity falls below 20% turn progress bar color from green to red
            if (currentFuel < 0.2) {
                grd.addColorStop(0.0, RGB_R_00);
                grd.addColorStop(0.5, RGB_R_05);
                grd.addColorStop(1.0, RBB_R_10);
            } else {
                grd.addColorStop(0.0, RGB_G_00);
                grd.addColorStop(0.5, RGB_G_05);
                grd.addColorStop(1.0, RBB_G_10);
            }

            // ProgressBar
            var fullFuelTank = fuelTankProgressBar.width-50;

            ctx.fillStyle = grd;
            ctx.fillRect(25, 10, fullFuelTank * currentFuel, 25);
            ctx.fill();

            ctx.strokeStyle = "#ffffff";
            ctx.lineWidth = 2;

            ctx.fillStyle = "#ffffff";
            ctx.font = '30px "RobotoMono Regular"';

            // Zero Digit
            ctx.fillText("0", 0, 35);

            // One Digit
            ctx.fillText("1", fuelTankProgressBar.width-15, 35);

            // White Border
            ctx.beginPath();
            ctx.moveTo(25, 35);
            ctx.lineTo(25, 10);
            ctx.lineTo(fuelTankProgressBar.width-25, 10);
            ctx.lineTo(fuelTankProgressBar.width-25, 35);

            ctx.moveTo((fuelTankProgressBar.width/2) , 10);
            ctx.lineTo((fuelTankProgressBar.width/2) , 35);

            ctx.moveTo((fuelTankProgressBar.width/4+10) , 10);
            ctx.lineTo((fuelTankProgressBar.width/4+10) , 35);

            ctx.moveTo((fuelTankProgressBar.width*3/4-10) , 10);
            ctx.lineTo((fuelTankProgressBar.width*3/4-10) , 35);

            ctx.stroke();
        }
        function clear()
        {
            var ctx = getContext("2d");
            //ctx.reset();
            this.requestPaint();
        }
        onFuelTankChanged: clear()
    }
}

