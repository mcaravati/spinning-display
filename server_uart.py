import numpy as np
import matplotlib.pyplot as plt
from math import pi
import argparse
import serial
import array
import socket
from time import sleep

display_mac_address = ""

# https://blog.kevindoran.co/bluetooth-programming-with-python-3/

def load_image(file_path: str) -> np.array:
    """
    Load an image and convert it to grayscale.

    file_path: str
        Path to the image to load
    
    return: np.array
        Image data
    """
    image = plt.imread(file_path)

    # Invert x and y
    print(image.shape)
    image = np.expand_dims(image, axis=2)
    image = np.transpose(image, (1, 0, 2))
    return np.mean(image, axis=-1)

def convert_image(image: str, nb_slices: int, nb_leds: int) -> dict:
    """
    Convert an image to polar coordinates.

    image: np.array
        Image to convert
    nb_slices: int
        Number of slices, a slice is a radial line from the center of the image
    nb_leds: int
        Number of leds per slice

    return: dict
        Dictionary containing the image data in polar coordinates
    """
    data = {}
    unit = 2 * pi / nb_slices

    for i in range(nb_slices):
        theta = i * unit
        for r in range(nb_leds):
            # Convert to cartesian coordinates
            x = int(np.cos(theta) * r + nb_leds)
            y = int(- np.sin(theta) * r + nb_leds)
            
            # if image[x,y] > 0.0:
            print(image[x,y])

            if(image[x,y] == 255 or image[x,y] == 1.):
                if theta not in data:
                    data[theta] = []
                data[theta].append({
                    "r": r,
                    "value": image[x, y]
                })

    return data

def plot_polar_image(data: dict) -> None:
    """
    Plot a polar image from data. Only plots the points with value 255.

    data: dict
        Dictionary containing the image data
    """
    omega=(2*pi)/10562
    plt.figure()
    for frame in data:
        date = frame[3] | (frame[4] << 8);
        payload = frame[6] | (frame[5] << 8);
        for b in range(16):
            if payload & (1<<(b)):
                plt.polar(date*omega, b,'ro')
    plt.show()

    # for theta in data:
    #     for r in data[theta]:
    #         if r['value'] == 255.:
    #             plt.polar(theta, r['r'], 'ro')

    # plt.show()


def package_data(data: dict, resolution_time=50) -> None:

    frames = []
    for theta in data:
        result = array.array('B', [])
        # Convert theta in ms
        date = ms_to_atmega_time(resolution_time * theta  / (2 * pi))
        word = 0b00_00_00_00_00_00_00_00

        for struct in data[theta]:
            if struct["value"] == 255 or struct["value"] == 1.:
                word |= (0b1 << (struct["r"]))
        # correcting bit direction
        # a = 0
        # for i in range(16):
        #     if word & (1 << i):
        #         a |= (1 << (15 - i))

        # word = a
        # done

        # writing into result
        low_date = date & 0xFF
        high_date = (date >> 8) & 0xFF

        low_word = (word >> 8) & 0xFF
        high_word = word & 0xFF

        result.append(0x69) # 0x6D, 0x67
        result.append(0x6D) #0x67
        result.append(0x67) #

        result.append(low_date)
        result.append(high_date)
        result.append(low_word)
        result.append(high_word)

        frames.append(result.tobytes())

    return frames


def ms_to_atmega_time(time: int, prescaler: int = 64) -> int:
    return int(13000 * time / prescaler)

if __name__ == "__main__":
    # Parse arguments
    parser = argparse.ArgumentParser(description='Controls the POV display')
    parser.add_argument('--image', type=str, help='Path to the image to display', required=True)
    parser.add_argument('--nb_slices', type=int, help='Number of slices', default=200)
    parser.add_argument('--nb_leds', type=int, help='Number of leds per slice', default=16)
    parser.add_argument('--display', default=False, action='store_true', help='Display the image')
    parser.add_argument('--skip_greet', help='Skip bonjour (to use if connexion was established before)', default=False, action='store_true')

    args = parser.parse_args()

    # Load the image
    image_data = load_image(args.image)

    # Convert image to polar coordinates
    polar_data = convert_image(image_data, args.nb_slices, args.nb_leds)
    payload = package_data(polar_data, 46)

    # Print polar image from data
    print(f"len(payload) = {len(payload)}")
    if args.display:
        plot_polar_image(payload)
    # print("display done")

    sock = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
    adapter = "84:7B:57:57:3E:32" # sudo ls /var/lib/bluetooth/
    device = "98:D3:51:FE:05:8E" # POV 12

    sock.connect((device, 1))
    print("Connected")

    # Retrieving banner
    if not args.skip_greet:
        print(sock.recv(20))
    sock.send(array.array('B', [0x6E,0x65,0x77,0x0A,0x0A,0x0A,0x0A]).tobytes()) #send new
    sleep(1)
    print("new sent")
    # Sending image
    packet_size = 1
    packets_amount = len(payload)//packet_size
    for p in range (packets_amount):
        for i in range(packet_size):
            sock.send(payload[i+p*packet_size]) # img\xFF\xFF\xFF\xFF\r\n
            print(f"payload : {payload[i+p*packet_size]}")
        sleep(.001)
    
    for r in range(len(payload)%packet_size):
        sock.send(payload[r+packets_amount*packet_size]) # img\xFF\xFF\xFF\xFF\r\n
    print(f"payload sent")


    # sock.send(array.array('B', [0x6E,0x65,0x77,0x0A,0x0A,0x0A,0x0A]).tobytes()) #send new
    sock.send(array.array('B', [0x73,0x7A,0x0D,0x0A,0x0A,0x0A,0x0A]).tobytes()) #send sz
    print("sent sz")
    try:
        while True:
            data = sock.recv(25)
            print(data)
    except KeyboardInterrupt:
        pass
        
    sock.close()
    