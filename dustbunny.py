#!/usr/bin/env python

from flask import Flask, render_template # request
import time
import requests
import meinheld

DEVICE_ID = '<YOUR DEV ID>'
ACCESS_TOKEN = '<YOUR ACCESS TOKEN>'

API_URL = 'https://api.spark.io/v1/devices'
DEVICE_URL = API_URL+'/'+DEVICE_ID

# Speed and drive control variables
last_direction = -1
speed_offset = 84
turn_tm = 166
run_time = 750

application = Flask (__name__, static_url_path = "", static_folder = "images")

@application.route("/")
def index ( ):
   return render_template ('index.html', name=None)

@application.route ("/forward")
def forward ( ):
   global run_time

   print "Forward"
   motors ("1", run_time)

   # sleep one second
   time.sleep (1)
   return "ok" # keep flask happy with a return.

@application.route ("/backward")
def backward ( ):
   global run_time

   print "Backward"
   motors ("2", run_time)

   # sleep one second
   time.sleep (1)
   return "ok"

@application.route ("/left")
def left ( ):
   global speed_offset, last_direction, turn_tm

   print "Left"
   motors ("3", 500 - (turn_tm / 2))

   # sleep one second
   time.sleep (1)
   return "ok"

@application.route ("/right")
def right ( ):
   global speed_offset, last_direction, turn_tm

   print "Right"
   motors ("4", 500 - (turn_tm / 2))

   # sleep one second
   time.sleep (1)
   return "ok"

@application.route ("/ltforward")
def ltforward ( ):
   global speed_offset, last_direction, turn_tm

   print "Left 45deg turn"
   motors ("3", 250 - (turn_tm / 2))

   # sleep one second

   time.sleep (1)
   return "ok"

@application.route ("/rtforward")
def rtforward ( ):
   global speed_offset, last_direction, turn_tm

   print "Right 45deg turn"
   motors ("4", 250 - (turn_tm / 2))

   # sleep one second
   time.sleep (1)
   return "ok"

@application.route ("/stop")
def stop ( ):
   print "Stop"
   motors ("0", "0")

   # sleep one second
   time.sleep (1)
   return "ok"

@application.route ("/speed_low")
def speed_low ( ):
   global speed_offset, last_direction, turn_tm

   speed_offset = 42
   turn_tm = 83

   setspeed (150)
   # sleep 1 second
   time.sleep (01)
   return "ok"

@application.route ("/speed_mid")
def speed_mid ( ):
   global speed_offset, last_direction, turn_tm

   speed_offset = 84
   turn_tm = 166

   setspeed (195)
   # sleep 1 second
   time.sleep (1)
   return "ok"

@application.route ("/speed_hi")
def speed_hi ( ):
   global speed_offset, last_direction, turn_tm

   speed_offset = 126
   turn_tm = 332

   setspeed (250)
   # sleep 1 second
   time.sleep (1)
   return "ok"

@application.route ("/continuous")
def continuous ( ):
   global run_time

   print "Continuous run"
   run_time = 0

   # sleep 1 second
   time.sleep (1)
   return "ok"

@application.route ("/mid_run")
def mid_run ( ):
   global run_time

   print "Mid run"
   run_time = 750

   # sleep 1 second
   time.sleep (1)
   return "ok"

@application.route ("/short_time")
def short_time ( ):
   global run_time

   print "Short run"
   run_time = 300

   # sleep 1 second
   time.sleep (1)
   return "ok"

def motors (direction, run_time):
    r = requests.post(DEVICE_URL+'/motors',
        data={'access_token':ACCESS_TOKEN, 'args':direction+','+str(run_time)})
    ret_json = r.json ( )
    return_val = ret_json['return_value']
    return int(return_val)

def setspeed (speed):
    r = requests.post(DEVICE_URL+'/speed',
        data={'access_token':ACCESS_TOKEN, 'args':str(speed)})
    ret_json = r.json ( )
    return_val = ret_json['return_value']
    return int(return_val)

def digitalWrite (pin, state):
    r = requests.post(DEVICE_URL+'/digitalwrite',
        data={'access_token':ACCESS_TOKEN, 'args':pin+','+state})
    ret_json = r.json ( )
    return_val = ret_json['return_value']
    return int(return_val)

def digitalRead (pin):
    r = requests.post(DEVICE_URL+'/digitalread',
        data={'access_token':ACCESS_TOKEN, 'args':pin})
    ret_json = r.json ( )
    return_val = ret_json['return_value']
    return int(return_val)

if __name__ == "__main__" :
   #application.run (host='0.0.0.0', port=8080, debug=False)
   #application.run (host='0.0.0.0')
   meinheld.listen(("0.0.0.0"))
   meinheld.run(app)
