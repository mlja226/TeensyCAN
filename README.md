# TeensyCAN
  
  This repo contains three PlatformIO project folders, BatteryNode, ArrayNode, and MotorNode. 
  Each of these folders can be opened as a project in PlatformIO and flashed to a Teensy 3.1/3.2 by
  following the instructions below.

## Installation
- Install Atom Text Editor
  1. Visit https://atom.io
  2. Choose the appropriate platform
  3. Download
  4. Run executable

- Install PlatformIO
  1. Open Atom
  2. Go to Settings->Install
  3. In the search bar, enter ‘PlatformIO’
  4. Under the package titled ‘platformio-ide’, select install
- Open the Project
  1. Open Atom
  2. Under the PlatformIO tab, click ‘Open Project’
  3. The project is split into folders for each type of TeensyNode (Battery, Array, Motor). Choose the folder corresponding to the type of node you wish to install.
- Run the Project
  1. Attach the Teensy via USB. 
  2. Click PlatformIO -> Build
  3. Click PlatformIO -> Upload
  4. At the end of the Upload Process, you will be prompted to press the reset button on the Teensy

## Setting up the Network
  1. Things you’ll need.
      - Teensy 3.1 / 3.2 for the program to run on.
      - CAN Transceiver for the Teensy so that it will communicate over differential CAN instead of the built in CAN.
      - CANUSB dongle so that you can test and monitor the program with a normal computer.
  2. Put all of these things on a breadboard and apply power.
      - The Teensy/Transceiver combo will need 5V power.
      - The CANUSB dongle will need 12V power.
  
  
## Testing the network
1. Make sure that your computer has the correct CANUSB serial drivers (may not be necessary for Linux/Mac setups).
    - Official website: http://www.can232.com/?page_id=75

2. Open up the CANUSB’s serial port on your computer.
    - This can be done with programs like CuteCom or Putty.
    - It doesn’t matter what the baud rate is. However, we recommend 115200.

3. Write the following messages over the serial connection (without the quotes). Official CANUSB ASCII Driver documentation: http://www.can232.com/docs/canusb_manual.pdf
    - “S6\r”
        - This tells the CANUSB chip that the CAN network will be running at 500Mbps.
    - “O\r”
        - This tells the CANUSB chip to open up the connection and begin reading.
    - “T0000012380000000000000000”
        - This is a example CAN packet in the CANUSB ASCII format.

4. At this point you should receive “Z\r” back on the serial connection showing that it worked.
    - If you did not get this then you need to debug your setup settings. The network may not be at 500Mbps or may not be set up correctly.

5. Now send the previous “T0000012380000000000000000” message around 20 times.
    - This is to make sure that the Teensy is actually listening on the other end.
    - After about 10 tries if the CANUSB quits printing out “Z\r” then there is something wrong with the Teensy.

## Testing the program

1. Connect your computer to the CANUSB dongle.

2. Navigate to the NodeTest directory inside of this project’s GitHub directory.
    - If you run into problems there is a README giving more information about how to run the program: https://github.com/mlja226/TeensyCAN/blob/master/NodeTest/README.md
    - Notice that there is a directory called data inside of the NodeTest directory. This is where you will find all of the test programs for the project.
    - There is also a gen_test.py file that can be used to generate new test data if necessary.

3. Now start the program.
    - The following command will start the program.
    'sudo python3 can_test.py /dev/ttyUSB0 ./data/battery_voltage_high_1.in 0.1'
    - The first argument is the serial port to open.
    - The second argument is the test data to use. This will be a file containing one ASCII formatted CAN packet per line.
    - The third argument is how many seconds to wait between sending packets. This is so the bus doesn’t get flooded.
    - There is an optional -q/--quiet option that will make the program only print out what it reads from the CAN network.

4. If you see a bunch of “Z” and possibly a few ASCII CAN messages (depending on if your test data contained an error) then it worked!
    - If you do not see a bunch of the letter Z then the network is not setup correctly. Things to check…
        - Are you writing to the correct serial port?
        - Is the CANUSB chip powered?
        - Is the Teensy powered?
        - Have you flashed the correct code onto the Teensy?
        - Is some other program on my computer listening to this port already?
    - If you do not see any error/warning packets when you believe you should then you will need to debug the code that you have on the Teensy.

