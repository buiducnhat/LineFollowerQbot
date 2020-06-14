#include<Arduino.h>
#include <WMHead.h>
#include <WMBoard.h>

//uncomment this block if need to use distance module (hc sr04)
//#define ECHOPIN 4
//#define TRIGPIN 3

//init ifra sensor (l1 is left sensor, l2 is right sensor)
WMLineFollower lineFollower1(11,12);
//set pin for motors, motor1 is left, motor2 is right
WMDCMotor motor1(8);
WMDCMotor motor2(7);

// data type to represent for direction
enum direct{
    ahead, left, right, none
};

//variable to get data from ifra sensors.
int leftSensor = 0;
int rightSensor = 0;
//variable to get direct to go, and the last direct
direct curDirect = ahead, lastDirect = ahead;

//function to get distance if necessary:
/*int getDistance(){
    unsigned long duration;
    int distance = 0;
    digitalWrite(TRIGPIN,0);   
    delayMicroseconds(2);
    digitalWrite(TRIGPIN,1);   
    delayMicroseconds(5);   
    digitalWrite(TRIGPIN,0);   
    duration = pulseIn(ECHOPIN, HIGH);  
    distance = int(duration/2/29.412);
    return distance;
}*/

void setup() {
    /*
    pinMode(ECHOPIN, INPUT);
    pinMode(TRIGPIN, OUTPUT);
    */
}

void loop() {
    curDirect = getDirect();
    if (curDirect == ahead){
        runAhead();
        lastDirect = ahead;
    }
    else if (curDirect == left){
        turnLeft();
        delay(10);
        lastDirect = left;
    }
    else if (curDirect == right){
        turnRight();
        delay(10);
        lastDirect = right;
    }
    else{
        if (lastDirect == ahead){
            runSlow();
        }
        else if (lastDirect == left){
            turnLeft();
        }
        else if (lastDirect == right){
            turnRight();
        }
    }
}

direct getDirect(){
    leftSensor = lineFollower1.readSensor1Status();
    rightSensor = lineFollower1.readSensor2Status();
    
    //if both sensors get value 0 => go head
    if (leftSensor == 0 && rightSensor == 0){
        return ahead;
    }
    else if (leftSensor == 0 && rightSensor == 1){
        return right;
    }
    else if (leftSensor == 1 && rightSensor == 0){
        return left;
    }
    else{
        return none;
    }
}

//set suitable speed for motors
void runAhead(){
    motor1.reverseRun(70);
    motor2.reverseRun(70);
}
void turnLeft(){
    motor1.reverseRun(30);
    motor2.reverseRun(80);
}
void turnRight(){
    motor1.reverseRun(80);
    motor2.reverseRun(30);
}
void runSlow(){
    motor1.reverseRun(30);
    motor2.reverseRun(30);
}
