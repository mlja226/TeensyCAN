# CanTest

This program is intended to be used to test the [TeensyCan](https://github.com/mlja226/TeensyCAN) program. It was written in python3 and contains test cases to verify TeensyCan's correctness.

## Required Libraries

* argparse
```
pip3 install argparse
```
* pyserial
```
pip3 install pyserial
```

## How to Run

```
usage: python3 can_test.py [-h] serial_port test_file delay

positional arguments:
  serial_port  The port you wish to write/read over.
  test_file    File containing the test CAN data.
  delay        Number of milliseconds between CAN messages.

optional arguments:
  -h, --help   show this help message and exit
```

### Linux / Mac

When running on these systems you need to make sure that you give the program sudo permissions. This is needed in order to write/read from the serial port.

```
sudo python3 can_test.py /dev/ttyUSB0 ./data/battery_voltage_high_1.in 0.1
```

## Test Cases

### Battery
- [ ] Battery Voltage - High
- [ ] Battery Voltage - Low
- [ ] Battery Temperature - High
- [ ] Battery Temperature - Low
- [ ] Battery Current - High
- [ ] Battery Current - Low

### Motor
- [ ]
