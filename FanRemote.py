from FanControl import FanControl
import sys

debug_dim = False

if len(sys.argv) < 3:
    sys.exit()

# Get fan remote key
try:
    remote_id = int(sys.argv[1])
    if remote_id < 0 or remote_id > len(FanControl.remote_keys):
        print('Invalid remote id.')
        sys.exit()
    remote_key = FanControl.remote_keys[remote_id]
except ValueError:
    print('Remote id must be an integer.')
    sys.exit()

# Get fan command
if sys.argv[2] == 'light':
    cmd = FanControl.light
elif sys.argv[2] == 'lightd':
    cmd = FanControl.lightd
    if not debug_dim and len(sys.argv) >= 4:
        try:
            count = int(sys.argv[3])
        except ValueError:
            print('Invalid cmd multiplier.')
            sys.exit()
elif sys.argv[2] == 'lightset':
    if len(sys.argv) != 4:
        print('Missing brightness level.')
        sys.exit()
    try:
        brightness = float(sys.argv[3])
    except ValueError:
        print('Invalid brightness level.')
        sys.exit()
    cmd = None
elif sys.argv[2] == 'power':
    cmd = FanControl.power
elif sys.argv[2] == 'chime':
    cmd = FanControl.chime
elif sys.argv[2].startswith('speed_'):
    if sys.argv[2][6:] == '1':
        cmd = FanControl.speed_1
    elif sys.argv[2][6:] == '2':
        cmd = FanControl.speed_2
    elif sys.argv[2][6:] == '3':
        cmd = FanControl.speed_3
    elif sys.argv[2][6:] == '4':
        cmd = FanControl.speed_4
    elif sys.argv[2][6:] == '5':
        cmd = FanControl.speed_5
    elif sys.argv[2][6:] == '6':
        cmd = FanControl.speed_6
    elif sys.argv[2][6:] == 'w':
        cmd = FanControl.speed_w
    else:
        print('Invalid speed command.')
        sys.exit()
elif sys.argv[2] == 'summer':
    cmd = FanControl.summer
elif sys.argv[2] == 'winter':
    cmd = FanControl.winter
else:
    print('Invalid command.')
    sys.exit()

print('Remote id: {0}\nCommand: {1}'.format(remote_id, sys.argv[2]))

fan_ctrl = FanControl(40)
if (cmd == FanControl.lightd and debug_dim):
    try:
        num = 0
        while True:
            if num < 220:
                fan_ctrl.send_fan_cmd(cmd, remote_key)
            else:
                break
            num = num + 1
            print(num)
    except KeyboardInterrupt:
        pass
elif sys.argv[2] == 'lightset':
    fan_ctrl.turn_on_light_to_brightness(brightness, remote_key)
else:
    try:
        fan_ctrl.send_fan_cmd(cmd, remote_key, count)
    except NameError:
        fan_ctrl.send_fan_cmd(cmd, remote_key)
fan_ctrl.cleanup_gpio()
