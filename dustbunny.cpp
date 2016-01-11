//   Photon based rover
//   Written by Scott Beasley - 2016

#include "L9110Driver/L9110Driver.h"

L9110_Motor motor_l (D0, D2);
L9110_Motor motor_r (D1, D3);

/* Globals -------------------------------------------------------------------*/
// Allow for at least 9 servos.
struct usrservos {
   Servo servo;
   int pin;
} userservos[9];

int servosinuse = 0;

/* Function prototypes -------------------------------------------------------*/
int tinkerDigitalRead(String pin);
int tinkerDigitalWrite(String command);
int tinkerAnalogRead(String pin);
int tinkerAnalogWrite(String command);
int tinkerServoOpen(String pin);
int tinkerServoClose(String command);
int tinkerServoSet(String command);
int tinkerServoRead(String command);

/* This function is called once at start up ----------------------------------*/
void setup()
{
	//Setup the Tinker application here

	//Register all the Tinker functions
	Particle.function("digitalread", tinkerDigitalRead);
	Particle.function("digitalwrite", tinkerDigitalWrite);
	Particle.function("analogread", tinkerAnalogRead);
	Particle.function("analogwrite", tinkerAnalogWrite);
	Particle.function("servoOpen", tinkerServoOpen);
	Particle.function("servoClose", tinkerServoClose);
	Particle.function("servoSet", tinkerServoSet);
	Particle.function("servoRead", tinkerServoRead);
	Particle.function("motors", tinkerMotors);

    servosinuse = 0;

    // Mark all servo slots as free on start.
	for (int i = 0; i < 9; i++)
	{
	   userservos[i].pin = -1;
	}

	motor_l.run (BRAKE);
    motor_l.setSpeed (255);
  	motor_r.run (BRAKE);
    motor_r.setSpeed (255);
}

/* This function loops forever --------------------------------------------*/
void loop()
{
	//This will run in a loop
}

/*******************************************************************************
 * Function Name  : tinkerDigitalRead
 * Description    : Reads the digital value of a given pin
 * Input          : Pin
 * Output         : None.
 * Return         : Value of the pin (0 or 1) in INT type
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerDigitalRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		pinMode(pinNumber, INPUT_PULLDOWN);
		return digitalRead(pinNumber);
	}
	else if (pin.startsWith("A"))
	{
		pinMode(pinNumber+10, INPUT_PULLDOWN);
		return digitalRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerDigitalWrite
 * Description    : Sets the specified pin HIGH or LOW
 * Input          : Pin and value
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerDigitalWrite(String command)
{
	bool value = 0;
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(command.substring(3,7) == "HIGH") value = 1;
	else if(command.substring(3,6) == "LOW") value = 0;
	else return -2;

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, value);
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		digitalWrite(pinNumber+10, value);
		return 1;
	}
	else return -3;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogRead
 * Description    : Reads the analog value of a pin
 * Input          : Pin
 * Output         : None.
 * Return         : Returns the analog value in INT type (0 to 4095)
                    Returns a negative number on failure
 *******************************************************************************/
int tinkerAnalogRead(String pin)
{
	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		return -3;
	}
	else if (pin.startsWith("A"))
	{
		return analogRead(pinNumber+10);
	}
	return -2;
}

/*******************************************************************************
 * Function Name  : tinkerAnalogWrite
 * Description    : Writes an analog value (PWM) to the specified pin
 * Input          : Pin and Value (0 to 255)
 * Output         : None.
 * Return         : 1 on success and a negative number on failure
 *******************************************************************************/
int tinkerAnalogWrite(String command)
{
	//convert ascii to integer
	int pinNumber = command.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	String value = command.substring(3);

	if(command.startsWith("D"))
	{
		pinMode(pinNumber, OUTPUT);
		analogWrite(pinNumber, value.toInt());
		return 1;
	}
	else if(command.startsWith("A"))
	{
		pinMode(pinNumber+10, OUTPUT);
		analogWrite(pinNumber+10, value.toInt());
		return 1;
	}
	else return -2;
}

/*******************************************************************************
 * Function Name  : tinkerServoOpen
 * Description    : Assigns a pin to a servo and attaches it.
 * Input          : Pin
 * Output         : None.
 * Return         : The servo element used on success and a negative
 *                : number on failure
 *******************************************************************************/
int tinkerServoOpen(String pin)
{
	int servoPin, i;

	//convert ascii to integer
	int pinNumber = pin.charAt(1) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (pinNumber< 0 || pinNumber >7) return -1;

	if(pin.startsWith("D"))
	{
		servoPin = pinNumber;
	}
	else if (pin.startsWith("A"))
	{
		servoPin = pinNumber + 10;
	} else
	{
	    return -2;
	}

	servosinuse++;
    if (servosinuse >= 9)
	{
		return -3; // All servos inuse.
	}

	for (i = 0; i < 9; i++)
	{
		if (userservos[i].pin == -1)
		{
			// Zap any old attachment.
			userservos[i].servo.detach ( );

			userservos[i].pin = servoPin;
			userservos[i].servo.attach (servoPin);
			break;
		}
	}

    // Return the servo array element index used.
    return (i);
}

/*******************************************************************************
 * Function Name  : tinkerServoClose
 * Description    : Unassigns a pin from a servo and detaches it.
 * Input          : servo element
 * Output         : None.
 * Return         : 0
 *******************************************************************************/
int tinkerServoClose(String command)
{
	//convert ascii to integer
	int indexNumber = command.charAt(0) - '0';
	//Sanity check to see if the element numbers are within limits
	if (indexNumber< 0 || indexNumber >8) return -1;

    userservos[indexNumber].pin = -1;
    servosinuse--;
    userservos[indexNumber].servo.detach ( );
    return (0);
}

/*******************************************************************************
 * Function Name  : tinkerServoSet
 * Description    : Unassigns a pin from a servo and detaches it.
 * Input          : servo element and pos value 0-179
 * Output         : None.
 * Return         : 0 on success or a negative number on failure
 *******************************************************************************/
int tinkerServoSet(String command)
{
    int ret = -1;
	//convert ascii to integer
	int indexNumber = command.charAt(0) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (indexNumber< 0 || indexNumber >8)
	{
	   ret = -1;
	}
    else
    {
	   String value = command.substring(2);
	   ret = 0;
	   userservos[indexNumber].servo.write (value.toInt());
    }

    return (ret);
}

/*******************************************************************************
 * Function Name  : tinkerServoRead
 * Description    : Reads the lats value sent to the servo.
 * Input          : servo element
 * Output         : None.
 * Return         : 0 on success or a negative number on failure
 *******************************************************************************/
int tinkerServoRead(String command)
{
	//convert ascii to integer
	int indexNumber = command.charAt(0) - '0';
	//Sanity check to see if the pin numbers are within limits
	if (indexNumber< 0 || indexNumber >8) return -1;

    return (userservos[indexNumber].servo.read ( ));
}

int tinkerMotors(String command)
{
    pinMode(D7, OUTPUT);
    digitalWrite (D7, HIGH);
    delay (1000);
    digitalWrite (D7, LOW);

	//convert ascii to integer
	int mcmd = command.charAt(0) - '0';
	//Sanity check to see if the direction numbers are within limits
	if (mcmd< 0 || mcmd >5) return -1;

    if (mcmd == 0) // Stop
    {
       	motor_l.run (BRAKE);
        motor_r.run (BRAKE);
    }

    else if (mcmd == 1) // Forward
    {
       	motor_l.run (FORWARD | RELEASE);
        motor_r.run (FORWARD | RELEASE);
    }

    else if (mcmd == 2) // Backward
    {
       	motor_l.run (BACKWARD | RELEASE);
        motor_r.run (BACKWARD | RELEASE);
    }

    else if (mcmd == 3) // Left turn
    {
       	motor_l.run (BACKWARD | RELEASE);
        motor_r.run (FORWARD | RELEASE);
    }

    else if (mcmd == 4) // Right turn
    {
       	motor_l.run (FORWARD | RELEASE);
        motor_r.run (BACKWARD | RELEASE);
    }

    else if (mcmd == 5) // Set speed
    {
        String value = command.substring(2);
        int speed = value.toInt();
        if (speed > 255)
           speed = 255;
        if (speed < 0)
           speed = 0;

        motor_l.setSpeed (speed);
        motor_r.setSpeed (speed);
    }

    if (mcmd != 5 && mcmd != 0)
    {
        String value = command.substring(2);
        int delay_time = value.toInt();
        if (delay_time != 0)
        {
           delay (delay_time);
           motor_l.run (BRAKE);
           motor_r.run (BRAKE);
        }
    }

    return (0);
}
