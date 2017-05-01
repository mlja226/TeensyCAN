import serial, time, io, re, os, queue, sys
from threading import Thread

# Call this like a function to return the current time
getCurrentTime = lambda: int(round(time.time()))

class ThreadedSerialIO(object):

    NO_ERROR = 1
    DISCONNECT_ERROR = 0
    open_ports = []
    start_time = 0

    LOGGING_RAW_DATA = False

    @staticmethod
    def setStartTime( t ):
        ThreadedSerialIO.start_time = t

    def __init__(self, t_port, t_baud_rate, t_read_size, t_full_read, t_data, t_can_timeout):
        # the string port id
        self.port = t_port

        # the output rate of the port
        self.baud_rate = t_baud_rate

        self.connect()

        # controls the loop() method
        self.running = False

        # the name of the decoding type
        self.decode_identifier = 'ascii'

        # Used for logging of raw data
        if ThreadedSerialIO.LOGGING_RAW_DATA:
            self.raw_data_buffer = queue.Queue()

        # the raw data from the serial port
        self.data_buffer = queue.Queue()
        self.data_buffer.put(t_data)
        self.data_buffer_length = 0
        self.input_buffer_index = 0

        # the processed data to be written to the file
        self.write_thread = None
        self.insert_query = queue.Queue()
        self.log_buffer = queue.Queue()
        self.output_buffer_index = 0

        # the main thread
        self.data_thread = None

        # the reader thread
        self.interprate_thread = None
        self.temp_data = ""

        self.can_timeout = t_can_timeout

        # self explanitory
        self.read_data = False

        # number of bytes read per loop(). if -1 then use default readline function
        self.read_size = t_read_size

        self.full_read_toggle = t_full_read

		# used to try and fix this port if possible
        self.error_state = ThreadedSerialIO.NO_ERROR

        self.sql_beginning = ""

    # Read in data from the serial connection
    def loop(self):
        self.curr_timeout = 0
        last_time = getCurrentTime()
        curr_time = getCurrentTime()
        while self.running:
            if( self.error_state == 1 and self.ser.isOpen() ):  #if NO_ERROR
                if self.full_read_toggle or (self.full_read_toggle == False and self.data_buffer_length < 40):
                    try:
                        i = 0
                        line = ""
                        while True:
                            i += 1
                            line = ""

                            try:
                                char = " "
                                raw_line = ""
                                while True:
                                    char = self.ser.read(1).decode(self.decode_identifier)
                                    if char == '\r':
                                        raw_line += "\n"
                                        break
                                    raw_line += char
                                line = raw_line

                            except serial.SerialTimeoutException:
                                print(self.port,"read timeout")

                            #print("read:",line)
                            if line.endswith("'"): line = line[:-1]
                            if line.startswith("b'"): line = line[2:]

                            if line and len(line) > 0: #check if the line is blank
                                # Update the counter
                                last_time = getCurrentTime()

                                # print("data:",line)
                                self.input_buffer_index += 1
                                self.error_state = 1
                                self.curr_timeout = 0
                                self.data_buffer.put(line)

                                # Used to log raw data
                                if ThreadedSerialIO.LOGGING_RAW_DATA:
                                    self.raw_data_buffer.put( str(line) )

                                self.read_data = True

                                if self.full_read_toggle == False:
                                    self.data_buffer_length = self.data_buffer.qsize()
                                    # print(self.data_buffer_length)
                                    if self.data_buffer_length >= 40:
                                        break
                            else:
                                if self.can_timeout:
                                    curr_time = getCurrentTime()
                                    self.curr_timeout = curr_time - last_time

                                    if self.curr_timeout >= 6:
                                        self.error_state = 0
                                        self.running = False
                                        print("\tDevice Timeout on", self.port)
                                    break

                            if i >= 30:
                                break

                    except UnicodeDecodeError:
                        self.tryAnotherBaudRate()
                    except:
                        self.error_state = 0  #ThreadedSerialIO.DISCONNECT_ERROR
                        self.running = False
                        print("\tDevice Disconnected", self.port, sys.exc_info())
            time.sleep(0.1)

    # Connect to a specified port
    def connect(self):
        self.ser = serial.Serial(self.port, baudrate=self.baud_rate, timeout=1, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE)
        self.ser.close()
        self.ser.open()

    # Destroys current connection and recreates it
    def reconnect(self):
        self.ser.close()
        self.connect()

    # Returns the current state of the device
    def isRunning(self):
        return self.running

    # Flushes (or clears) the input and output buffers to this serial port
    def flushPort(self):
        self.ser.flushInput()
        self.ser.flushOutput()

    # Sends configuration code to init some of the special FTDI chips
    def configPort(self):
        # Close the serial port
        self.ser.write("C\r".encode())
        self.ser.flush()
        self.ser.write("S6\r".encode())
        self.ser.flush()
        # Below is the message filtering for the motor controller
        # Config filter to match 0x400-0x4FF messages only
        # self.ser.write("M00008000\r".encode())
        # self.ser.flush()
        # self.ser.write("m00001FF0\r".encode())
        # self.ser.flush()
        self.ser.write("O\r".encode())
        self.ser.flush()

    # Starts up all of the device threads
    def start(self):
        self.flushPort()
        self.configPort()

        self.running = True

        if self.data_thread == None:
            self.data_thread = Thread(target = self.loop)
        self.data_thread.start()

        print("\tStarted Reading Port "+str(self.port)+" with baudrate of "+str(self.baud_rate))

    # Ends all of the device threads
    # NOTE) You can no call this function from within one of the device threads. It must be called from
    #   the main thread.
    def stop(self):
        self.running = False
        self.data_thread.join()
        print("\tStopped Reading Port:"+str(self.port)+" with baudrate of "+str(self.baud_rate))

    # Not used
    def setDecoder(self,decoder_id):
        if self.running:
            print("please call .stop() before calling this method")
        else:
            self.decode_identifier = decoder_id
            self.data_reader = io.TextIOWrapper(io.BufferedRWPair(self.ser,self.ser,1), encoding = self.decode_identifier)

    # Not used
    def setUpdateRate(self,p_rate):
        self.update_rate = p_rate

    # Removes the incorrectly decoded text at the beginning of the data buffer
    def cleanUpData(self):
        self.data = self.data_value.getvalue()
        self.data_value.truncate(0)
        self.data.replace('^'+'@','')
        self.data_value.write(self.data)

    # Returns the number of read or readline commands that have been called
    def getInputBufferIndex(self):
        return self.input_buffer_index

    # Returns the number of times this device has recorded it's state
    def getOutputBufferIndex(self):
        return self.output_buffer_index

    # Returns the number of state records this device is ready to push to either the MySQL database or a log
    def getOutputBufferSize(self):
        if self.insert_query.qsize() > self.log_buffer.qsize():
            return self.insert_query.qsize()
        return self.log_buffer.qsize()

    # Returns the raw data without clearing the raw data buffer
    def viewData(self):
        s_text = ""
        while self.data_buffer.empty() == False:
            s_text += self.data_buffer.get()
        self.data_buffer = queue.Queue()
        self.data_buffer.put(s_text)
        return s_text

    # Returns the raw data and clears the raw data buffer
    def getData(self):
        s_text = ""
        while self.data_buffer.empty() == False:
            s_text += self.data_buffer.get()
        self.data_buffer = queue.Queue()
        return s_text

    # Returns the current number of seconds this has been waiting for input from the device
    def getTimeoutCounter(self):
        return self.curr_timeout

    # Returns true if data has come in over the serial connection
    # NOTE) Also sets the toggle to false so a second call to this function will now return False
    def hasReadData(self):
        temp = self.read_data
        self.read_data = False
        return temp

    # Writes a string over the serial line
    def writeOverSerial(self, p_data):
        try:
            self.ser.write((p_data).encode('ascii'))
            self.ser.flush()
        except: #serial.serialutil.SerialException:
            self.error_state = ThreadedSerialIO.DISCONNECT_ERROR
            self.running = False
            print("Device Disconnected")
