#include "dashboard.h"


Dashboard::Dashboard(QObject *parent):
    QObject{parent}
  ,m_driveBackwards{false}
  ,m_engineStarted{false}
  ,m_displayInfo{"Hydrogen Auto \n \n Time to \n save the Planet! \n \n Press 'S' to activate Engine"}
  ,m_gear{' '}
  ,m_driveMode{'P'}
  ,m_instspeed{0}
  ,m_engineSpeed{0}
  ,m_fuelTankMax{10000}
  ,m_fuelTank{m_fuelTankMax}
  ,m_fuelConsumption{0}
  ,m_drivenKilometer{0}
  ,m_gearOld{' '}
  ,m_gearRatio{10}
  ,m_idleSpeed{800}
  ,m_minEngineSpeed{0}
  ,m_maxEngineSpeed{6000}
  ,m_minInstspeed{0}
  ,m_maxInstspeed{280}
{

}

/***
 * assigns value from main.qml as mainWindow_1
*/
void Dashboard::listenTo(QObject *object)
{
    if (!object){
        return;
    }
    object->installEventFilter(this);
}

/***
 * The above listenTo-function gets an object and forwards to event filter.
 * Via filtering events, the function recognizes the
 * pressed key and executes code accordingly.
 *
 * Qt-Documentation:  https://doc.qt.io/qt-5/eventsandfilters.html
 *
 * When the filter object's eventFilter() implementation is called,
 * it can accept or reject the event, and allow or deny further processing
 * of the event. If all the event filters allow further processing of an event
 * (by each returning false), the event is sent to the target object itself.
 * If one event stops processing (by returning true), the target event and any later
 * event filters do not get processed and will not recognize any further events.
 */
bool Dashboard::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "key" << keyEvent->key() << "pressed on" << object;

        if(keyEvent->key() == Qt::Key_S){
            qDebug() << "KeyPressEvent S";
            if(m_engineStarted == false){
                engineStart();
            }
            else {
                engineStop();
            }
        }
        else if(keyEvent->key() == Qt::Key_P){
            qDebug() << "KeyPressEvent P";
            setDriveMode('P');
        }
        else if(keyEvent->key() == Qt::Key_R){
            qDebug() << "KeyPressEvent R";
            setDriveMode('R');
        }
        else if(keyEvent->key() == Qt::Key_N){
            qDebug() << "KeyPressEvent N";
            setDriveMode('N');
        }
        else if(keyEvent->key() == Qt::Key_D){
            qDebug() << "KeyPressEvent D";
            setDriveMode('D');
        }
        else if(keyEvent->key() == Qt::Key_Space){
            qDebug() << "KeyPressEvent Space";
            if (m_fuelTank > 0) {
                accelerate();
                calcDrivenKilometers();
                calcFuelConsumption();
            }
            else {
                setEngineSpeed(m_minEngineSpeed);
                setInstspeed(m_minInstspeed);
                setDriveMode('P');
                engineStop();
            }
        }
        else if(keyEvent->key() == Qt::Key_Down){
            qDebug() << "KeyPressEvent Down";
            brake();
            calcDrivenKilometers();
            setFuelConsumption(0);
        }
        else if(keyEvent->key() == Qt::Key_F){
            qDebug() << "KeyPressEvent F";
            tankGas();
        }
        else if(keyEvent->key() == Qt::Key_T){
            qDebug() << "KeyPressEvent T";
            if (m_driveMode == 'D' && m_instspeed > m_minInstspeed) {
                if (m_fuelTank == 0) {
                    setEngineSpeed(m_minEngineSpeed);
                    setInstspeed(m_minInstspeed);
                    setDriveMode('P');
                    engineStop();
                }
                else {
                    cruiseControl();
                }
            }
        }
        return true;
    }
    return false;
}

void Dashboard::engineStart()
{
    if (m_driveMode == 'P'){
        setDisplayInfo("Engine Activated");
        setEngineStarted(true);
        setFuelConsumption(32);
        setEngineSpeed(m_idleSpeed);
    }
    else {
        setDisplayInfo("Change drive mode\n to [P]");
    }
}

void Dashboard::engineStop()
{
    if (m_driveMode == 'P'){
        setDisplayInfo("Engine Halted \n \n Tank is empty! \n \n To refuel press [F] and hold");
        setEngineStarted(false);
        setFuelConsumption(0);
        setEngineSpeed(m_minEngineSpeed);
    }
    else {
        setDisplayInfo("Change drive mode\n to [P]");
    }
}

/***
 * Function increases the driven kilometers.
 * To achieve an optimum prototype, this function increases the drivenKilometer
 * depending on the instspeed-range.
 *
*/
void Dashboard::calcDrivenKilometers()
{
    if (m_driveMode == 'D') {
        if (m_instspeed > m_minInstspeed && m_instspeed <= 40) {
            setDrivenKilometer(m_drivenKilometer += 5);
        }
        else if (m_instspeed > 40 && m_instspeed <= 80) {
            setDrivenKilometer(m_drivenKilometer += 15);
        }
        else if (m_instspeed > 80 && m_instspeed <= 120) {
            setDrivenKilometer(m_drivenKilometer += 25);
        }
        else if (m_instspeed > 120 && m_instspeed <= 160) {
            setDrivenKilometer(m_drivenKilometer += 35);
        }
        else if (m_instspeed > 160 && m_instspeed <= 200) {
            setDrivenKilometer(m_drivenKilometer += 50);
        }
        else if (m_instspeed > 200 && m_instspeed <= 240) {
            setDrivenKilometer(m_drivenKilometer += 60);
        }
        else if (m_instspeed > 240 && m_instspeed <= m_maxInstspeed) {
            setDrivenKilometer(m_drivenKilometer += 70);
        }

        emit drivenKilometerChanged();
    }
}

/***
 * Function sets current fuel consumption and decreases fuel level accordingly.
*/
void Dashboard::calcFuelConsumption()
{
    if (m_driveMode != 'P') {

        if (m_instspeed == m_minInstspeed) {
            setFuelConsumption(32);
        }
        else if (m_instspeed > m_minInstspeed && m_instspeed <= 40) {
            setFuelConsumption(50);
            setFuelTank(m_fuelTank -= 2);
        }
        else if (m_instspeed > 40 && m_instspeed <= 80) {
            setFuelConsumption(70);
            setFuelTank(m_fuelTank -= 4);
        }
        else if (m_instspeed > 80 && m_instspeed <= 120) {
            setFuelConsumption(110);
            setFuelTank(m_fuelTank -= 6);
        }
        else if (m_instspeed > 120 && m_instspeed <= 160) {
            setFuelConsumption(150);
            setFuelTank(m_fuelTank -= 8);
        }
        else if (m_instspeed > 160 && m_instspeed <= 200) {
            setFuelConsumption(200);
            setFuelTank(m_fuelTank -= 10);
        }
        else if (m_instspeed > 200 && m_instspeed <= 240) {
            setFuelConsumption(280);
            setFuelTank(m_fuelTank -= 12);
        }
        else if (m_instspeed > 240 && m_instspeed <= m_maxInstspeed) {
            setFuelConsumption(360);
            setFuelTank(m_fuelTank -= 14);
        }
        if (m_fuelTank < 2000) {
            setDisplayInfo("Tank is almost empty.\n Seek a green \n Hydrogen station!");
        }
        if (m_fuelTank < 0) {
            setFuelTank(0);
        }

        qDebug() << "Tank: " << m_fuelTank;
        emit fuelTankChanged();
        emit fuelConsumptionChanged();
    }
}

/***
 * Function refills the tank and is invoked by key-Event "F".
*/
void Dashboard::tankGas()
{
    if (m_fuelTank < m_fuelTankMax && m_instspeed == m_minInstspeed) {
        setFuelTank(m_fuelTank + 100);
        setDisplayInfo("Filling tank with Hydrogen");
    }
    else if (m_fuelTank >= m_fuelTankMax) {
        setFuelTank(m_fuelTankMax);
        setDisplayInfo("Tank is full. \n Enjoy your ride!");
    }
    else {
        setDisplayInfo("Take a stretch \n after filling \n up the tank");
    }

    emit fuelTankChanged();
}

/***
 * Function sets the gears according to instspeed.
 * Each selected gear has a different gearRatio
 * to emulate changing gears and increases with engineSpeed.
*/
void Dashboard::automaticGear(const int instspeed)
{
    m_gearOld = m_gear;

    if(m_driveMode == 'D'){
        if (instspeed <= 25) {
            setGear('1');
            m_gearRatio = 10;
        }
        if (instspeed >= 25 && instspeed <= 34) {
            setGear('2');
            m_gearRatio = 9;
        }
        else if (instspeed >= 35 && instspeed <= 49) {
            setGear('3');
            m_gearRatio = 7;
        }
        else if (instspeed >= 50 && instspeed <= 79) {
            setGear('4');
            m_gearRatio = 6;
        }
        else if (instspeed >= 80 && instspeed <= 99) {
            setGear('5');
            m_gearRatio = 4;
        }
        else if (instspeed >= 100) {
            setGear('6');
            m_gearRatio = 2;
        }
        if (m_gearOld != m_gear) {
            if (m_gear != ' ' && static_cast<int>(m_gear) > static_cast<int>(m_gearOld)) {
                setEngineSpeed(m_engineSpeed - 1000);
            }
        }
    }
    else{
        setGear(' ');
    }
}

/***
 *  Acceleration is dependent on selected driveMode.
 *  driveMode 'D' is required before auto can accelerate.
 *  engineSpeed is increased according to gear selection.
*/
void Dashboard::accelerate()
{
    switch (m_driveMode) {
    case 'P':
        if (!m_engineStarted)
            setDisplayInfo("Activate engine");
        else
            setDisplayInfo("Select [D] gear to drive");
        break;
    case 'R':
        setGear(' ');
        if (m_instspeed < 30) {
            setEngineSpeed(m_engineSpeed + 20);
            setInstspeed(m_instspeed + 1);
        }
        break;
    case 'N':
        if (m_engineSpeed < m_maxEngineSpeed) {
            setEngineSpeed(m_engineSpeed + 20);
        }
        break;
    case 'D':
        if (m_instspeed < m_maxInstspeed) {
            setEngineSpeed(m_engineSpeed + 10 * m_gearRatio);
            setInstspeed(m_instspeed + 1);
            automaticGear(m_instspeed);
        }
        break;
    }
    qDebug() << "engineSpeed: " << m_engineSpeed;
}

/***
 * If 'Key_Down' keyEvent is invoked this function decreases the instspeed
 * and engineSpeed.
*/
void Dashboard::brake()
{
    if (m_instspeed > 3) {
        setInstspeed(m_instspeed -= 3);
        automaticGear(m_instspeed);
    }
    else {
        setInstspeed(m_minInstspeed);
    }

    if (m_engineSpeed > m_idleSpeed) {
        setEngineSpeed(m_engineSpeed -= 80);
    }
    else if (m_engineSpeed <= m_idleSpeed) {
        setEngineSpeed(m_idleSpeed);
    }

    emit engineSpeedChanged();
    emit instspeedChanged();
}

/***
 * If 'Key_T' keyEvent is invoked, cruiseControl-function continues increasing the
 * drivenKilometers and empties the fuelTank, relative to current instspeed.
*/
void Dashboard::cruiseControl()
{
    setDisplayInfo("Cruise control active");
    calcFuelConsumption();
    calcDrivenKilometers();
}

/***
 * keyEvent function checks the validity of emulation in
 * correspondance to selected DriverMode.
 * If the new driveMode selection is permitted, action will be allowed,
 * otherwise a warning is displayed.
*/
void Dashboard::setDriveMode(char newDriveMode)
{
    if (m_driveMode == newDriveMode)
        return;

    if(newDriveMode == 'P' || newDriveMode == 'R' || newDriveMode == 'N' || newDriveMode == 'D'){
        if (m_engineStarted) {
            if (newDriveMode == 'D'){
                setGear('1');
                setEngineSpeed(m_idleSpeed);
            }

            if(m_instspeed > m_minInstspeed){
                if(newDriveMode == 'P'){
                    setDisplayInfo(" [P] gear \n is permitted \n when stationary");
                    return;
                }
                else if(newDriveMode == 'R'){
                    setDisplayInfo(" [R] gear \n is permitted \n when stationary");
                    return;
                }
                else if(newDriveMode == 'N'){
                    setDisplayInfo(" [N] gear \n is permitted \n when stationary");
                    return;
                }
            }
            else {
                if (newDriveMode == 'N') {
                    setGear(' ');
                    setFuelConsumption(32);
                }
                if (newDriveMode == 'P') {
                    setGear(' ');
                    setFuelConsumption(32);
                }
            }

            if(newDriveMode == 'R'){
                setDriveBackwards(true);
            }
            else{
                setDriveBackwards(false);
            }

            setDisplayInfo("");
            m_driveMode = newDriveMode;
            emit driveModeChanged();
        }
        else {
            setDisplayInfo("Press [S] to activate engine");
        }
    }
}

/***
 * Relevant QProperty Objects are listed.
 * ( getters and setters )
 *
*/
bool Dashboard::getEngineStarted() const
{
    return m_engineStarted;
}
void Dashboard::setEngineStarted(bool newEngineStarted)
{
    if (m_engineStarted == newEngineStarted)
        return;
    m_engineStarted = newEngineStarted;
    emit engineStartedChanged();
}

int Dashboard::getInstspeed() const
{
    return m_instspeed;
}
void Dashboard::setInstspeed(int newInstspeed)
{
    if (m_instspeed == newInstspeed)
        return;
    m_instspeed = newInstspeed;
    emit instspeedChanged();
}

int Dashboard::getEngineSpeed() const
{
    return m_engineSpeed;
}
void Dashboard::setEngineSpeed(int newEngineSpeed)
{
    if (m_engineSpeed == newEngineSpeed)
        return;
    m_engineSpeed = newEngineSpeed;
    emit engineSpeedChanged();
}

int Dashboard::getFuelConsumption() const
{
    return m_fuelConsumption;
}
void Dashboard::setFuelConsumption(int newFuelConsumption)
{
    if (m_fuelConsumption == newFuelConsumption)
        return;
    m_fuelConsumption = newFuelConsumption;
    emit fuelConsumptionChanged();
}

int Dashboard::getDrivenKilometer() const
{
    return m_drivenKilometer;
}
void Dashboard::setDrivenKilometer(int newDrivenKilometer)
{
    if (m_drivenKilometer == newDrivenKilometer)
        return;
    m_drivenKilometer = newDrivenKilometer;
    emit drivenKilometerChanged();
}

bool Dashboard::getDriveBackwards() const
{
    return m_driveBackwards;
}
void Dashboard::setDriveBackwards(bool newDriveBackwards)
{
    if (m_driveBackwards == newDriveBackwards)
        return;
    m_driveBackwards = newDriveBackwards;
    emit driveBackwardsChanged();
}

const QString &Dashboard::getDisplayInfo() const
{
    return m_displayInfo;
}
void Dashboard::setDisplayInfo(const QString &newDisplayInfo)
{
    if (m_displayInfo == newDisplayInfo)
        return;
    m_displayInfo = newDisplayInfo;
    emit displayInfoChanged();
}

char Dashboard::getDriveMode() const
{
    return m_driveMode;
}

int Dashboard::getFuelTank() const
{
    return m_fuelTank;
}
void Dashboard::setFuelTank(int newFuelTank)
{
    if (m_fuelTank == newFuelTank)
        return;
    m_fuelTank = newFuelTank;
    emit fuelTankChanged();
}

char Dashboard::getGear() const
{
    return m_gear;
}
void Dashboard::setGear(char newGear)
{
    if (m_gear == newGear)
        return;
    m_gear = newGear;
    emit gearChanged();
}
