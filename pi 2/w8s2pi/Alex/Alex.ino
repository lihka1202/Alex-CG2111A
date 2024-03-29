#include <serialize.h>
#include "packet.h"
#include "constants.h"
#include <stdarg.h>
#include <math.h>
#include <Arduino.h>

/*
   Alex's configuration constants
*/

// Number of ticks per revolution from the
// wheel encoder.

#define COUNTS_PER_REV      160//to change in lab

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          20.42035225 //to change in lab

// Motor control pins. You need to adjust these till
// Alex moves in the correct direction
#define LF                  5   // Left forward pin
#define LR                  6   // Left reverse pin
#define RF                  11  // Right forward pin
#define RR                  10  // Right reverse pin

#define ALEX_LENGTH 18.0 //to chnage in lab
#define ALEX_BREADTH 12.5 //change in lab
#define PI 3.141592654

#define LEFT_SPEED 255.0
#define RIGHT_SPEED 254.0

//alexDiagonal = sqrt((ALEX_LENGTH * ALEX_LENGTH) + (ALEX_BREADTH * ALEX_BREADTH));
//alexCirc = PI * alexDiagonal;

float alexDiagonal = 21.914607;
float alexCirc = 68.84676837;
/*
      Alex's State Variables
*/
int readSerial(char *buffer);

void sendResponse(TPacket *packet);

// Store the ticks from Alex's left and
// right encoders.
volatile unsigned long leftForwardTicks;
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks;
volatile unsigned long rightReverseTicks;

// Store the revolutions on Alex's left
// and right wheels
volatile unsigned long leftForwardTicksTurns = 0;
volatile unsigned long rightForwardTicksTurns = 0;
volatile unsigned long leftReverseTicksTurns = 0;
volatile unsigned long rightReverseTicksTurns = 0;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

unsigned long deltaDist;
unsigned long newDist;
unsigned long deltaTicks;
unsigned long targetTicks;
unsigned long targetTicksF;

TResult readPacket(TPacket *packet)
{
  // Reads in data from the serial port and
  // deserializes it.Returns deserialized
  // data in "packet".

  char buffer[PACKET_SIZE];
  int len;

  len = readSerial(buffer);

  if (len == 0)
    return PACKET_INCOMPLETE;
  else
    return deserialize(buffer, len, packet);

}

void sendStatus()
{
  // Implement code to send back a packet containing key
  // information like leftTicks, rightTicks, leftRevs, rightRevs
  // forwardDist and reverseDist
  // Use the params array to store this information, and set the
  // packetType and command files accordingly, then use sendResponse
  // to send out the packet. See sendMessage on how to use sendResponse.
  //
  TPacket statusPacket;
  statusPacket.packetType = PACKET_TYPE_RESPONSE;
  statusPacket.command = RESP_STATUS;
  statusPacket.params[0] = leftForwardTicks;
  statusPacket.params[1] = rightForwardTicks;
  statusPacket.params[2] = leftReverseTicks;
  statusPacket.params[3] = rightReverseTicks;
  statusPacket.params[4] = leftForwardTicksTurns;
  statusPacket.params[5] = rightForwardTicksTurns;
  statusPacket.params[6] = leftReverseTicksTurns;
  statusPacket.params[7] = rightReverseTicksTurns;
  statusPacket.params[8] = forwardDist;
  statusPacket.params[9] = reverseDist;
  sendResponse(&statusPacket);
}

void sendMessage(const char *message)
{
  // Sends text messages back to the Pi. Useful
  // for debugging.

  TPacket messagePacket;
  messagePacket.packetType = PACKET_TYPE_MESSAGE;
  strncpy(messagePacket.data, message, MAX_STR_LEN);
  sendResponse(&messagePacket);
}

void dbprint(char *format, ...)
{
  va_list args;
  char buffer[128];

  va_start(args, format);
  vsprintf(buffer, format, args);
  sendMessage(buffer);
}

void sendBadPacket()
{
  // Tell the Pi that it sent us a packet with a bad
  // magic number.

  TPacket badPacket;
  badPacket.packetType = PACKET_TYPE_ERROR;
  badPacket.command = RESP_BAD_PACKET;
  sendResponse(&badPacket);

}

void sendBadChecksum()
{
  // Tell the Pi that it sent us a packet with a bad
  // checksum.

  TPacket badChecksum;
  badChecksum.packetType = PACKET_TYPE_ERROR;
  badChecksum.command = RESP_BAD_CHECKSUM;
  sendResponse(&badChecksum);
}

void sendBadCommand()
{
  // Tell the Pi that we don't understand its
  // command sent to us.

  TPacket badCommand;
  badCommand.packetType = PACKET_TYPE_ERROR;
  badCommand.command = RESP_BAD_COMMAND;
  sendResponse(&badCommand);

}

void sendBadResponse()
{
  TPacket badResponse;
  badResponse.packetType = PACKET_TYPE_ERROR;
  badResponse.command = RESP_BAD_RESPONSE;
  sendResponse(&badResponse);
}

void sendOK()
{
  TPacket okPacket;
  okPacket.packetType = PACKET_TYPE_RESPONSE;
  okPacket.command = RESP_OK;
  sendResponse(&okPacket);
}

void sendResponse(TPacket *packet)
{
  // Takes a packet, serializes it then sends it out
  // over the serial port.
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}


/*
   Setup and start codes for external interrupts and
   pullup resistors.

*/
// Enable pull up resistors on pins 2 and 3
void enablePullups()
{
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs.
  //TODO:
  DDRD |= (1 << 2 | 1 << 3);
  PORTD = 0b00001100;
}

// Functions to be called by INT0 and INT1 ISRs.
void leftISR()
{
  switch (dir)
  {
    case FORWARD:
      ++leftForwardTicks;
      forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
      break;
    case BACKWARD:
      ++leftReverseTicks;
      reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
      break;
    case LEFT:
      ++leftReverseTicksTurns;
      break;
    case RIGHT:
      ++leftForwardTicksTurns;
      break;
  }

  /*
    Serial.print("LEFT: ");
    Serial.println(leftTicks);
  */
}

void rightISR()
{
  switch (dir)
  {
    case FORWARD:
      ++rightForwardTicks;
      break;
    case BACKWARD:
      ++rightReverseTicks;
      break;
    case LEFT:
      ++rightForwardTicksTurns;
      break;
      ++rightReverseTicksTurns;
      break;
  }
  /*
    Serial.print("RIGHT: ");
    Serial.println(rightTicks);
  */
}

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered. Remember to enable
  // the INT0 and INT1 interrupts.
  EIMSK |= 0b00000011;
  EICRA |= 0b00001010;
}

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.
ISR (INT0_vect) {
  leftISR();
}

ISR (INT1_vect) {
  rightISR();
}



// Implement INT0 and INT1 ISRs above.

/*
   Setup and start codes for serial communications

*/
// Set up the serial connection. For now we are using
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial()
{
  // To replace later with bare-metal.
  Serial.begin(9600);

  //UBRR0L = 103;
  //UBRR0H = 0;
  //UCSR0C = 0B00000110;
  //UCSR0A = 0;
}

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.

void startSerial()
{
  // Empty for now. To be replaced with bare-metal code
  // later on.
  //UCSR0B = 0B00011000; //ENable USART transmit and receive
}

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid.
// This will be replaced later with bare-metal code.

int readSerial(char *buffer)
{

  int count = 0;

  while (Serial.available())
    buffer[count++] = Serial.read();

  //while ((UCSR0A & 0B10000000) != 0) {
  //  buffer[count++] = UDR0;
  //}

  return count;
}

// Write to the serial port. Replaced later with
// bare-metal code

void writeSerial(const char *buffer, int len)
{
  Serial.write(buffer, len);

  //int i = len;
  //while (i--) {
  //  while ((UCSR0A & 0B00100000) == 0) {
  //    UDR0 = buffer[i - 1];
  //  }

  //}
}

/*
   Alex's motor drivers.

*/

// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.
void setupMotors()
{
  /* Our motor set up is:
        LF 5 OC0B
        LR 6 OC0A
        RF 11 OC2A
        RR 10 OC1B
  */

  //For A1&A2
  //  DDRD |= 0B01100000;
  //  TCCR0A = 0B10100001;
  //  TCCR0B = 0B00000001;
  //  TIMSK0 = 0;
  //
  //  //For B1&B2
  //  DDRB |= 0B00001100;
  //  TCCR1A = 0B00100001;
  //  TCCR1B = 0B00000001;
  //  TCCR2A = 0B10000001;
  //  TCCR2B = 0B00000001;
  //  TIMSK1 = 0;
  //  TIMSK2 = 0;

}

// Start the PWM for Alex's motors.
// We will implement this later. For now it is
// blank.
void startMotors()
{
  //  TCNT0 = 0;
  //  OCR0A = 0;
  //  OCR0B = 0;

  //  TCNT1L = 0;
  //  TCNT2 = 0;
  //  OCR1B = 0;
  //  OCR2A = 0;
}

// Convert percentages to PWM values
int pwmVal(float speed, unsigned char motor)
{
  if (speed < 0.0)
    speed = 0;

  if (speed > 100.0)
    speed = 100.0;

  if (motor == 1) {
    return (int) ((speed / 100.0) * RIGHT_SPEED);
  }
  return (int) ((speed / 100.0) * LEFT_SPEED);
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forward(float dist, float speed)
{
  dir = FORWARD;

  if (dist > 0) {
    deltaDist = dist;
  } else {
    deltaDist = 9999999;
  }
  newDist = forwardDist + deltaDist;

  // For now we will ignore dist and move
  // forward indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.

  analogWrite(LF, pwmVal(speed, 2));
  analogWrite(RF, pwmVal(speed, 1));
  analogWrite(LR, 0);
  analogWrite(RR, 0);

  //  OCR0A = 0;                //LR
  //  OCR0B = pwmVal(speed, 2); //LF
  //  OCR1B = 0;                //RR
  //  OCR2A = pwmVal(speed, 1); //RF
}

// Reverse Alex "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Alex will
// continue reversing indefinitely.
void reverse(float dist, float speed)
{

  dir = BACKWARD;

  if (dist > 0) {
    deltaDist = dist;
  } else {
    deltaDist = 9999999;
  }
  newDist = reverseDist + deltaDist;

  // For now we will ignore dist and
  // reverse indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.
  analogWrite(LR, pwmVal(speed, 2));
  analogWrite(RR, pwmVal(speed, 1));
  analogWrite(LF, 0);
  analogWrite(RF, 0);

  //  OCR0A = pwmVal(speed, 2); //LR
  //  OCR0B = 0;                //LF
  //  OCR1B = pwmVal(speed, 1); //RR
  //  OCR2A = 0;                //RF
}

unsigned long computeDeltaTicks(float ang)
{
  unsigned long ticks = (unsigned long) ((ang * alexCirc * COUNTS_PER_REV) / (360.0 * WHEEL_CIRC));
  return ticks;
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void left(float ang, float speed)
{
  dir = LEFT;
  if (ang == 0) {
    deltaTicks = 9999999;
  } else {
    deltaTicks = computeDeltaTicks(ang) / 3.0;
  }
  targetTicks = leftReverseTicksTurns + deltaTicks;
  targetTicksF = rightForwardTicksTurns + deltaTicks;

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn left we reverse the left wheel and move
  // the right wheel forward.
  analogWrite(LR, pwmVal(speed, 2));
  analogWrite(RF, pwmVal(speed, 1));
  analogWrite(LF, 0);
  analogWrite(RR, 0);

  //  OCR0A = pwmVal(speed, 2); //LR
  //  OCR0B = 0;                //LF
  //  OCR1B = 0;                //RR
  //  OCR2A = pwmVal(speed, 1); //RF
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void right(float ang, float speed)
{

  dir = RIGHT;
  if (ang == 0) {
    deltaTicks = 9999999;
  } else {
    deltaTicks = computeDeltaTicks(ang) / 3.0;
  }
  targetTicks = rightReverseTicksTurns + deltaTicks;
  targetTicksF = leftForwardTicksTurns + deltaTicks;

  int val = pwmVal(speed, dir);

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
  analogWrite(RR, pwmVal(speed, 1));
  analogWrite(LF, pwmVal(speed, 2));
  analogWrite(RF, 0);
  analogWrite(LR, 0);

  //  OCR0A = 0;                //LR
  //  OCR0B = pwmVal(speed, 2); //LF
  //  OCR1B = pwmVal(speed, 1); //RR
  //  OCR2A = 0;                //RF
}

// Stop Alex. To replace with bare-metal code later.
void stop()
{
  dir = STOP;
  analogWrite(LF, 0);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
  analogWrite(RR, 0);

  //OCR0A = 0;  //LF
  //OCR0B = 0;  //LR
  //OCR1B = 0;  //RF
  //OCR2A = 0;  //RR
}

/*
   Alex's setup and run codes

*/

// Clears all our counters
void clearCounters()
{
  leftForwardTicks = 0;
  rightForwardTicks = 0;
  leftForwardTicksTurns = 0;
  rightForwardTicksTurns = 0;
  leftReverseTicks = 0;
  rightReverseTicks = 0;
  leftReverseTicksTurns = 0;
  rightReverseTicksTurns = 0;
  forwardDist = 0;
  reverseDist = 0;
}

// Clears one particular counter
void clearOneCounter(int which)
{
  clearCounters();
}
// Intialize Vincent's internal states

void initializeState()
{
  clearCounters();
}

void handleCommand(TPacket *command)
{
  switch (command->command)
  {
    // For movement commands, param[0] = distance, param[1] = speed.
    case COMMAND_STOP:
      sendOK();
      stop();
      break;
    case COMMAND_FORWARD:
      sendOK();
      forward((float) command->params[0], (float) command->params[1]);
      break;
    case COMMAND_REVERSE:
      sendOK();
      reverse((float) command->params[0], (float) command->params[1]);
      break;
    case COMMAND_TURN_LEFT:
      sendOK();
      left((float) command->params[0], (float) command->params[1]);
      break;
    case COMMAND_TURN_RIGHT:
      sendOK();
      right((float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_GET_STATS:
      sendStatus();
      break;

    case COMMAND_CLEAR_STATS:
      clearOneCounter(command->params[0]);
      sendOK();
      break;

    default:
      sendBadCommand();
  }
}

void waitForHello()
{
  int exit = 0;

  while (!exit)
  {
    TPacket hello;
    TResult result;

    do
    {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if (result == PACKET_OK)
    {
      if (hello.packetType == PACKET_TYPE_HELLO)
      {


        sendOK();
        exit = 1;
      }
      else
        sendBadResponse();
    }
    else if (result == PACKET_BAD)
    {
      sendBadPacket();
    }
    else if (result == PACKET_CHECKSUM_BAD)
      sendBadChecksum();
  } // !exit
}

void setup() {
  // put your setup code here, to run once:

  //alexDiagonal = sqrt((ALEX_LENGTH * ALEX_LENGTH) + (ALEX_BREADTH * ALEX_BREADTH));
  //alexCirc = PI * alexDiagonal;

  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupMotors();
  startMotors();
  enablePullups();
  initializeState();
  sei();
}

void handlePacket(TPacket *packet)
{
  switch (packet->packetType)
  {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
      break;

    case PACKET_TYPE_RESPONSE:
      break;

    case PACKET_TYPE_ERROR:
      break;

    case PACKET_TYPE_MESSAGE:
      break;

    case PACKET_TYPE_HELLO:
      break;
  }
}

void loop() {

  // Uncomment the code below for Step 2 of Activity 3 in Week 8 Studio 2

  //forward(0, 100);

  // Uncomment the code below for Week 9 Studio 2


  // put your main code here, to run repeatedly:
  TPacket recvPacket; // This holds commands from the Pi

  TResult result = readPacket(&recvPacket);

  if (result == PACKET_OK)
    handlePacket(&recvPacket);
  else if (result == PACKET_BAD)
  {
    sendBadPacket();
  }
  else if (result == PACKET_CHECKSUM_BAD)
  {
    sendBadChecksum();
  }

  if (deltaDist > 0) {
    if (dir == FORWARD)  {
      if (forwardDist > newDist) {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    } else if (dir == BACKWARD) {
      if (reverseDist > newDist) {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    } else if (dir == STOP) {
      deltaDist = 0;
      newDist = 0;
      stop();
    }
  }

  if (deltaTicks > 0) {
    if (dir == LEFT) {
      if (leftReverseTicksTurns >= targetTicks || rightForwardTicksTurns >= targetTicksF) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } else if (dir == RIGHT) {
      if (rightReverseTicksTurns >= targetTicks || leftForwardTicksTurns >= targetTicksF) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } else if (dir == STOP) {
      deltaTicks = 0;
      targetTicks = 0;
      stop();
    }
  }
}


