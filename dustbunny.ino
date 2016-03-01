// This #include statement was automatically added by the Particle IDE.
#include "L9110Driver/L9110Driver.h"

// Various time delays used for driving autonomously
#define TURNDELAY 475
#define BACKUPDELAY 400
#define MAXFORWARDSPEED 225

int rovMotors (String command);

// Globals
L9110_Motor motor_l (D0, D2);
L9110_Motor motor_r (D1, D3);
int speed = 200;

/* This function is called once at start up ----------------------------------*/
void setup ( )
{
    Serial.begin (9600);

    //Register the rover functions
    Particle.function ("motors", rovMotors);

  	rovMotors ("0,0"); // Make sure the motors are off at startup
}

/* This function loops forever --------------------------------------------*/
void loop ( )
{

}

int rovMotors (String command)
{
	//convert ascii to integer
	int mcmd = command.charAt(0) - '0';
	//Sanity check to see if the direction numbers are within limits
	if (mcmd < 0 || mcmd > 5)
	    return -1;

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
        speed = value.toInt();
        if (speed > 255)
           speed = 255;
        if (speed < 0)
           speed = 0;
    }

    if (mcmd != 5 && mcmd != 0)
    {
        String value = command.substring (2);
        motor_l.setSpeed (speed);
        motor_r.setSpeed (speed);
        int delay_time = value.toInt ( );
        if (delay_time != 0)
        {
           delay (delay_time);
           motor_l.run (BRAKE);
           motor_r.run (BRAKE);
        }
    }

    return (0);
}
