// TCRT I.R sensors
#define rightFarSensor     A0
#define rightNearSensor    A1
#define rightCenterSensor  A2
#define leftCenterSensor   A3
#define leftNearSensor     A4
#define leftFarSensor      A5

// obstacle sensor
// whenever the robot encounter any obstacle in front of it it makes a u turn
#define frontSensor 10

//Sensor readings
int leftCenterReading;
int leftNearReading;
int leftFarReading;
int rightCenterReading;
int rightNearReading;
int rightFarReading;
int frontReading = 0;


int replaystage;

// Motors
#define leftMotor1  5
#define leftMotor2  3
#define rightMotor1 6
#define rightMotor2 9



int leftSpeed=235;
int rightSpeed=235;

//Indication
#define led 13
#define buzzer 12

//button
#define button 4
int buttonState = 0;

char path[99] = {};
int pathLength;
int readLength;

int linewidth = 200;

void setup(){

  //Sensors
  pinMode(leftCenterSensor, INPUT);
  pinMode(leftNearSensor, INPUT);
  pinMode(leftFarSensor, INPUT);
  pinMode(rightCenterSensor, INPUT);
  pinMode(rightNearSensor, INPUT);
  pinMode(rightFarSensor, INPUT);
  pinMode(frontSensor, INPUT);
  
  pinMode(button, INPUT);

  //Motors
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);

  //Indication
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(led, HIGH);

  //button
  pinMode(button, INPUT);
  delay(500);
  digitalWrite(led,LOW);
 
}


void loop(){
 
 readSensors();                                                                                    
 
 if(leftFarReading == 0 && rightFarReading == 0 &&
   (leftCenterReading == 1 || rightCenterReading == 1) ){
    straight();                                                                                     
  } else{                                                                                             
    leftHandRule();                                                                                  
}

}


// sensor readings
void readSensors(){
 
  leftCenterReading  = digitalRead(leftCenterSensor);
  leftNearReading    = digitalRead(leftNearSensor);
  leftFarReading     = digitalRead(leftFarSensor);
  rightCenterReading = digitalRead(rightCenterSensor);
  rightNearReading   = digitalRead(rightNearSensor);
  rightFarReading    = digitalRead(rightFarSensor); 
}





// left hand rule
void leftHandRule(){
 
  if( leftFarReading == 1 && rightFarReading == 1){
   
  analogWrite(leftMotor1, leftSpeed);
  analogWrite(leftMotor2,0);
  analogWrite(rightMotor1, rightSpeed);
  analogWrite(rightMotor2,0);
  delay(linewidth);
    readSensors();
   
      if(rightFarReading == 1 && rightNearReading == 1 && rightCenterReading == 1 &&
      leftCenterReading == 1 && leftNearReading == 1 && leftFarReading == 1){
        dryRunDone();
      }
      if(leftFarReading == 0  &&  rightFarReading == 0){
      turnLeft();
    }
   
  }
 
  if(leftFarReading == 1 ){ // if you can turn left then turn left
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(linewidth);
    readSensors();
         if(rightFarReading == 1 && rightNearReading == 1 && rightCenterReading == 1 &&
      leftCenterReading == 1 && leftNearReading == 1 && leftFarReading == 1){
        dryRunDone();
      }
        if(leftFarReading == 0 || rightFarReading == 1){
         turnLeft();
        }
       else if(leftFarReading == 0 && rightFarReading == 0){
        turnLeft();
      }
     
  }
  
  if(rightFarReading == 1){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(30);
    readSensors();
   
    if(leftFarReading == 1){
      delay(linewidth-30);
      readSensors();
     
      if(rightFarReading == 1 && rightNearReading == 1 && rightCenterReading == 1 &&
      leftCenterReading == 1 && leftNearReading == 1 && leftFarReading == 1){
        dryRunDone();
      }
      else{
        turnLeft();
        return;
      }
    }
    delay(linewidth-30);
    readSensors();
    if(
      rightCenterReading == 0 && rightFarReading == 0){
      turnRight();
      return;
    }
    path[pathLength]='S';
    pathLength++;
    if(path[pathLength-2]=='U'){
    calculateShortPath();
    }
    straight();
  }
  readSensors();
  if(leftFarReading == 0 && leftCenterReading == 0 && rightCenterReading ==0
    && rightFarReading == 0 && leftNearReading == 0 && rightNearReading == 0){
    turnAround();
  }

}

// Dry run Done
void dryRunDone(){
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
   replaystage=1;
   path[pathLength]='D';
   pathLength++;
   readSensors();
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
 while(buttonState == LOW)
 {
    buttonState = digitalRead(button); 
 }
 
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
    delay(1000);
   shortestPath();
}


// left turn
void turnLeft(){
 digitalWrite(buzzer, HIGH);
 while(digitalRead(rightCenterSensor) == 1 || digitalRead(leftCenterSensor) == 1){
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, leftSpeed);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    }
  while(digitalRead(rightCenterSensor) == 0){
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, leftSpeed);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
  }
 if(replaystage==0){
    path[pathLength]='L';
    pathLength++;
  if(path[pathLength-2]=='U'){
        calculateShortPath();
      }
  }
}


//right turn
void turnRight(){

  digitalWrite(buzzer, HIGH);
  while(digitalRead(rightCenterSensor) == 1){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, rightSpeed);
  }
   while(digitalRead(rightCenterSensor) == 0){
     digitalWrite(leftMotor1, leftSpeed);
    digitalWrite(leftMotor2, 0);
    digitalWrite(rightMotor1, 0);
    digitalWrite(rightMotor2, leftSpeed);
    }
   while(digitalRead(leftCenterSensor) == 0){
    digitalWrite(leftMotor1, leftSpeed);
    digitalWrite(leftMotor2, 0);
    digitalWrite(rightMotor1, 0);
    digitalWrite(rightMotor2, rightSpeed);
 }
  if(replaystage==0){
  path[pathLength]='R';
  pathLength++;
  if(path[pathLength-2]=='U'){
     calculateShortPath();
    }
  }
 
}


void straight(){
  digitalWrite(buzzer, LOW);

  frontReading = digitalRead(frontSensor);

  if(frontReading == 1)
  {
    readSensors();
    while( leftCenterReading == 1 || rightCenterReading == 1 || rightNearReading == 1)
    {
      readSensors();
      analogWrite(leftMotor1, leftSpeed);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, rightSpeed);
    }
    while( leftCenterReading == 0 && rightCenterReading == 0)
    {
      readSensors();
      analogWrite(leftMotor1, leftSpeed);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, rightSpeed);
    }
    if(replaystage==0){
    path[pathLength]='L';
    pathLength++;
    if(path[pathLength-2]=='U'){
          calculateShortPath();
        }
    }
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, leftSpeed);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, rightSpeed );
    delay(linewidth + 100);
    
    straight();
  }
    if( digitalRead(leftNearSensor) == 1){
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed );
    analogWrite(rightMotor2, 0);
    delay(5);
    return;
  }
    if( digitalRead(rightNearSensor) == 1){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(5);
    return;
  }
  
  if( digitalRead(leftCenterSensor) == 0){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 125 );
    analogWrite(rightMotor2, 0);
    delay(5);
    return;
  }
  if(digitalRead(rightCenterSensor) == 0){
    analogWrite(leftMotor1, 125);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(5);
    return;
  }
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(4);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
}


void turnAround(){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(linewidth);
   while(digitalRead(leftCenterSensor) == 0 || digitalRead(rightCenterSensor) == 0){
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, leftSpeed);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(2);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
    }
  path[pathLength]='U';
  pathLength++;
  straight();
}


void calculateShortPath(){
 int shortDone=0;
  if(path[pathLength-3]=='L' && path[pathLength-1]=='R'){
    pathLength-=3;
    path[pathLength]='U';
    shortDone=1;
  }
  
  if(path[pathLength-3]=='L' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    shortDone=1;
  }
  
  if(path[pathLength-3]=='R' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='U';
    shortDone=1;
  }
 
  
  if(path[pathLength-3]=='S' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='R';
    shortDone=1;
  }
    
  if(path[pathLength-3]=='S' && path[pathLength-1]=='S' && shortDone==0){
    pathLength-=3;
    path[pathLength]='U';
    shortDone=1;
  }
    if(path[pathLength-3]=='L' && path[pathLength-1]=='L' && shortDone==0){
    pathLength-=3;
    path[pathLength]='S';
    shortDone=1;
  }
  path[pathLength+1]='D';
  path[pathLength+2]='D';
  pathLength++;
 
}


void shortestPath(){
   readSensors();
  if(leftFarReading == 0 && rightFarReading == 0){
    straight();
  }
 else{
    if(path[readLength]=='D'){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(linewidth);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
      endMaze();
    }
    if(path[readLength]=='L'){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(linewidth);
      turnLeft();
    }
    if(path[readLength]=='R'){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(linewidth);
    turnRight();
    }
    if(path[readLength]=='S'){
    analogWrite(leftMotor1, leftSpeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, rightSpeed);
    analogWrite(rightMotor2, 0);
    delay(linewidth);
    straight();
    }
   readLength++;
  }
   
  shortestPath();
 
}


void endMaze(){
    digitalWrite(led, HIGH);
    digitalWrite(buzzer,HIGH);
    endMaze();
}


