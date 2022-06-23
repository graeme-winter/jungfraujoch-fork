import json
import zmq
import numpy
import bitshuffle
import sys
import signal
import grpc
import jfjoch_pb2

sys.path.insert(0, "/opt/dectris/albula/4.1/bin")
sys.path.insert(0, "/opt/dectris/albula/4.1/python")

import dectris.albula as albula
from dectris.albula import DNoObject, DDrawingEllipse, DDrawingString, DDrawingLine


TERMINATE = False


def signal_handler(signal, frame):
    global TERMINATE
    TERMINATE = True


signal.signal(signal.SIGINT, signal_handler)

context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.RCVTIMEO = 1000 # in milliseconds
socket.connect("tcp://mx-ic922-1:5400")
socket.setsockopt(zmq.SUBSCRIBE, b"")

OPTIONAL_DATA = albula.DImageOptionalData()

OPTIONAL_DATA.set_x_pixel_size(0.000075)
OPTIONAL_DATA.set_y_pixel_size(0.000075)

albulaMain = albula.openMainFrame()
albulaSubFrame = albulaMain.openSubFrame()


def read_message():
    try:
        msg = socket.recv()
        pbuf = jfjoch_pb2.PreviewFrame()
        pbuf.ParseFromString(msg)
        OPTIONAL_DATA.set_wavelength(pbuf.wavelength_A)
        OPTIONAL_DATA.set_beam_center_x(pbuf.beam_center_x)
        OPTIONAL_DATA.set_beam_center_y(pbuf.beam_center_y)
        OPTIONAL_DATA.set_detector_distance(pbuf.detector_distance_mm * 0.001)
        OPTIONAL_DATA.set_saturation_value(pbuf.saturation_value)
        if pbuf.pixel_depth == 2:
            image_array = numpy.frombuffer(pbuf.data, numpy.int16)
        else:
            image_array = numpy.frombuffer(pbuf.data, numpy.int32)
        image_array = numpy.reshape(image_array, (pbuf.height, pbuf.width))
        print("Received image %d %d %d %d"%(pbuf.image_number, pbuf.height, pbuf.width, len(image_array)))
        print(image_array.shape)
        dimage = albula.DImage(image_array)
        if pbuf.image_number >= 0:
            dimage.setOptionalData(OPTIONAL_DATA)
            albulaSubFrame.loadImage(dimage)
            albulaSubFrame.setTitle("JUNGFRAU PREVIEW Dataset: %s Image: %d" % (pbuf.file_prefix, pbuf.image_number))
        else:
            OPTIONAL_DATA.set_pixel_mask(dimage)
    except:
        pass

while not TERMINATE:
    read_message()
