import RPi.GPIO as GPIO
from time import sleep
import math


# Timings

SHORT_ON    = 400
SHORT_OFF   = 500
LONG_ON     = 850
LONG_OFF    = 950
REST        = 10000


# Timing functions

usleep = lambda x: sleep(x/1000000.0)

def SS(rf_pin):
    GPIO.output(rf_pin, True)
    usleep(SHORT_ON)
    GPIO.output(rf_pin, False)
    usleep(SHORT_OFF)


def SL(rf_pin):
    GPIO.output(rf_pin, True)
    usleep(SHORT_ON)
    GPIO.output(rf_pin, False)
    usleep(LONG_OFF)


def SR(rf_pin):
    GPIO.output(rf_pin, True)
    usleep(SHORT_ON)
    GPIO.output(rf_pin, False)
    usleep(REST)


def LL(rf_pin):
    GPIO.output(rf_pin, True)
    usleep(LONG_ON)
    GPIO.output(rf_pin, False)
    usleep(LONG_OFF)


def LS(rf_pin):
    GPIO.output(rf_pin, True)
    usleep(LONG_ON)
    GPIO.output(rf_pin, False)
    usleep(SHORT_OFF)


def run_timing_func_by_tag(tag, rf_pin):
    if tag == 'SS':
        SS(rf_pin)
    elif tag == 'SL':
        SL(rf_pin)
    elif tag == 'LL':
        LL(rf_pin)
    elif tag == 'LS':
        LS(rf_pin)
    elif tag == 'SR':
        SR(rf_pin)
    else:
        print("Invalid timing function tag provided.")


class FanControl:
    rf_pin = None 
    # Fan remote keys
    remote_keys = (
        ('SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL', 'SL'),
        ('SS', 'LL', 'SL', 'SS', 'LL', 'SS', 'LS', 'LL', 'SS', 'LL', 'SS', 'LS', 'LS', 'LL', 'SL'),
        ('SL', 'SL', 'SL', 'SL', 'SS', 'LL', 'SS', 'LS', 'LS', 'LS', 'LL', 'SS', 'LL', 'SS', 'LS'),
        ('SS', 'LL', 'SS', 'LL', 'SS', 'LL', 'SL', 'SS', 'LS', 'LS', 'LS', 'LS', 'LL', 'SS', 'LS')
    )

    # Fan commands
    power   = ('LL', 'SS', 'LS', 'LS', 'LS', 'LS', 'LL', 'SS', 'LL')
    chime   = ('LL', 'SS', 'LL', 'SL', 'SL', 'SS', 'LL', 'SS', 'LL')
    light   = ('SL', 'SL', 'SS', 'LS', 'LL', 'SS', 'LL', 'SS', 'LL')
    lightd  = ('LL', 'SS', 'LL', 'SS', 'LL', 'SS', 'LL', 'SS', 'LL')
    speed_1 = ('LL', 'SL', 'SS', 'LS', 'LS', 'LS', 'LL', 'SS', 'LL')
    speed_2 = ('LL', 'SS', 'LL', 'SS', 'LS', 'LS', 'LL', 'SS', 'LL')
    speed_3 = ('LL', 'SL', 'SL', 'SS', 'LS', 'LS', 'LL', 'SS', 'LL')
    speed_4 = ('LL', 'SS', 'LS', 'LL', 'SS', 'LS', 'LL', 'SS', 'LL')
    speed_5 = ('LL', 'SL', 'SS', 'LL', 'SS', 'LS', 'LL', 'SS', 'LL')
    speed_6 = ('LL', 'SS', 'LL', 'SL', 'SS', 'LS', 'LL', 'SS', 'LL')
    speed_w = ('LL', 'SL', 'SL', 'SL', 'SS', 'LS', 'LL', 'SS', 'LL')
    summer  = ('LL', 'SS', 'LS', 'LS', 'LL', 'SS', 'LL', 'SS', 'LL')
    winter  = ('LL', 'SS', 'LS', 'LS', 'LL', 'SL', 'SL', 'SS', 'LL')

    def __init__(self, rf_pin):
        self.rf_pin = rf_pin
        GPIO.setmode(GPIO.BOARD)
        GPIO.setup(rf_pin, GPIO.OUT)

    @staticmethod
    def cleanup_gpio():
        GPIO.cleanup()

    def send_single_fan_cmd(self, cmd, remote_key):
        # Send remote key to get fan's attention.
        for tag in remote_key:
            run_timing_func_by_tag(tag, self.rf_pin)
        # Send command
        for tag in cmd:
            run_timing_func_by_tag(tag, self.rf_pin)
        # Send postamble to mark end of command.
        run_timing_func_by_tag('SR', self.rf_pin)

    def send_fan_cmd(self, cmd, remote_key, count=12):
        for i in range(count):
            self.send_single_fan_cmd(cmd, remote_key)

    def turn_on_light_to_brightness(self, brightness, remote_key):
        if brightness <= 0:
            brightness = 0.01
        elif brightness > 1:
            brightness = 1
        send_cnt = 18 * brightness
        for i in range(math.ceil(send_cnt)):
            self.send_fan_cmd(self.lightd, remote_key)

