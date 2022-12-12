import zmq
import numpy
import sys
import signal
import base64

sys.path.insert(0, "/opt/dectris/albula/4.1/bin")
sys.path.insert(0, "/opt/dectris/albula/4.1/python")

import dectris.albula as albula

TERMINATE = False


def signal_handler(signal, frame):
    global TERMINATE
    TERMINATE = True


signal.signal(signal.SIGINT, signal_handler)

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.RCVTIMEO = 1000  # in milliseconds
socket.connect("tcp://mx-ic922-1:5400")
socket.setsockopt(zmq.SUBSCRIBE, b"")

OPTIONAL_DATA = albula.DImageOptionalData()

OPTIONAL_DATA.set_x_pixel_size(0.000075)
OPTIONAL_DATA.set_y_pixel_size(0.000075)

albulaMain = albula.openMainFrame()
albulaSubFrame = albulaMain.openSubFrame()


def read_message():
    try:
        msg = socket.recv_json()
        OPTIONAL_DATA.set_wavelength(msg["wavelength_A"])
        OPTIONAL_DATA.set_beam_center_x(msg["beam_x_pxl"])
        OPTIONAL_DATA.set_beam_center_y(msg["beam_center_y"])
        OPTIONAL_DATA.set_detector_distance(msg["detector_distance_mm"] * 0.001)
        data_decoded = base64.decodebytes(msg["data"])
        if msg["pixel_depth"] == 2:
            image_array = numpy.frombuffer(data_decoded, numpy.int16)
        else:
            image_array = numpy.frombuffer(data_decoded, numpy.int32)
        image_array = numpy.reshape(image_array, (msg["height"], msg["width"]))
        print("Received image %d %d %d %d" % (msg["image_number"], msg["height"], msg["width"], len(image_array)))
        print(image_array.shape)
        dimage = albula.DImage(image_array)
        if msg["image_number"] >= 0:
            OPTIONAL_DATA.set_saturation_value(msg["saturation_value"])
            dimage.setOptionalData(OPTIONAL_DATA)
            albulaSubFrame.loadImage(dimage)
            albulaSubFrame.setTitle(
                "JUNGFRAU PREVIEW Dataset: %s Image: %d" % (msg["file_prefix"], msg["image_number"]))
        else:
            OPTIONAL_DATA.set_pixel_mask(dimage)
    except:
        pass


while not TERMINATE:
    read_message()
