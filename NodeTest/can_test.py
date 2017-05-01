#!/bin/python3

from argparse import ArgumentParser
from core.ThreadedSerialIO import ThreadedSerialIO
import time

def printCanMessage( can_message ):
    offset = 5 if can_message[0] == 'T' else 0
    print( "\t"+can_message[0], can_message[1:4+offset], can_message[4+offset:5+offset], can_message[5+offset:9+offset], can_message[9+offset:13+offset], can_message[13+offset:17+offset], can_message[17+offset:21+offset] )

def main():
    # Deal with command line arguments
    parser = ArgumentParser()
    parser.add_argument('serial_port', help='The port you wish to write/read over.')
    parser.add_argument('test_file', help='File containing the test CAN data.')
    parser.add_argument('delay', help='Number of milliseconds between CAN messages.')
    args = parser.parse_args()

    # Open the specified serial port
    port = ThreadedSerialIO( args.serial_port, 115200, 26, True, "", True )
    port.start()

    # Open the specified test file
    with open( args.test_file, "r" ) as input_file:
        for input_line in input_file:
            # Clean up the line of text
            o_data = input_line.replace("\n", "")
            o_data_parts = o_data.split(" ")

            # Write over serial
            port.writeOverSerial( o_data_parts[0]+"\r" )

            # Print out message info
            print( "Out Message..." )
            printCanMessage( o_data_parts[0] )

            # Print out the message parts
            print( "Message Parts..." )
            for i in range(1, len(o_data_parts)):
                print( "\t"+o_data_parts[i] )

            # Print in message info
            i_data = port.getData()
            print( "In Data..." )
            for output_line in i_data.split("\n"):
                if len(output_line) == 0:
                    continue
                if output_line[0] == 't' or output_line[0] == 'T':
                    printCanMessage( output_line )
                else:
                    print( "\t"+output_line )

            # Deliniate between messages
            print()
            print( '-'*80 )

            # Sleep between messages
            time.sleep( float(args.delay) )

    print("Finished running test.")
    port.stop()

if __name__ == '__main__':
    main()
