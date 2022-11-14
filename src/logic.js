/**
 * CONVERT DEGREE VALUE TO RADIANS
 */
function converDegToRad(degree) {
    var factor = Math.PI / 180;
    return degree * factor;
}

/**
 * DRAWING TEXT ALONG ARC
 *   Parsing 6 Input Parameters in total
 *      Context:    Related to Canvas onPaint Context
 *      str:        String will be printed within the Canvas
 *      centerX:    Defines x-Component Center-Position
 *      centerY:    Defines y-Component Center-Position
 *      radius:     Defines the center to the Curve Edge Distance
 *      angle:      Defines starting angle
 */
function drawTextAlongArc(context, str, centerX, centerY, radius, angle)
{
    context.save();
    context.translate(centerX, centerY);
    context.rotate(angle - converDegToRad(2));

    for (var n = 0; n < str.length; n++)
    {
        if (str[n-1] === "1")
        {
            context.rotate((-1) * (angle) / (str.length*0.99));
        }
        else if (str[n-1] === "M")
        {
            context.rotate((-1) * (angle) / (str.length*0.45));
        }
        else
        {
            context.rotate((-1) * (angle) / (str.length*0.65));
        }

        context.save();
        context.translate(0, radius);

        var char1 = str[n];

        context.fillText(char1, 0, 0);
        context.restore();
    }

    context.restore();

}

/**
 *  ROTATION VELOCITY INDICATOR
 *  Parsed value to determine current instspeed
 *  indicator position correlates to min / max value (0km/h) (280km/h)
 */
function SeagreenIndicatorRotation(instspeed)
{
    var maxInstspeed = 280;
    var percentage = instspeed / maxInstspeed;

    return percentage * 270;
}

/**
 *  ENGINESPEED ROTATION INDICATOR
 *  Parsed engine-speed to determine current value
 *  indicator position correlates to min / max value (0rpm) (8000rpm)
 */
function engineSpeedIndicatorRotation(engineSpeed)
{
    var maxEngineSpeed = 8000;
    var percentage = engineSpeed / maxEngineSpeed;

    return percentage * 270;
}

/**
 *  INITIAL VELOCITY POSITION INDICATOR
 *  Parsed instspeed value to determine start position
 */
function seagreenIndicatorTail(instspeed)
{
    var offset = 135; // at 135 deg => 0 km/h
    var percentage = instspeed / 280;

    return percentage * 270 + offset;
}

/**
 *  ENGINE-SPEED INITIAL POSITION INDICATOR
 *  Parsed engine-speed to determine initial value
 */
function engineSpeedIndicatorTail(engineSpeed)
{
    var offset = 135; // at 135 deg => 0 RPM
    var percentage = engineSpeed / 8000;

    return percentage * 270 + offset;
}

