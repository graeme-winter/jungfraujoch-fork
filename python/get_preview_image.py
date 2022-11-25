# Prepare preview image for calibration with dioptas

import jfjoch_pb2_grpc, jfjoch_pb2, grpc, tifffile, numpy, sys

filename = sys.argv[1]

MAX_MESSAGE_LENGTH=64*1024*1024
channel = grpc.insecure_channel('mx-jungfrau-1:5232',
                                   options=[
        ('grpc.max_send_message_length', MAX_MESSAGE_LENGTH),
        ('grpc.max_receive_message_length', MAX_MESSAGE_LENGTH),
    ])
stub = jfjoch_pb2_grpc.gRPC_JFJochBrokerStub(channel)

pbuf = stub.GetPreviewFrame(jfjoch_pb2.Empty())

image_array = numpy.frombuffer(pbuf.data, numpy.int16)
image_array = numpy.where(image_array<0, -1, image_array)
image_array = numpy.reshape(image_array, (pbuf.height, pbuf.width))
image_array = numpy.flipud(image_array)

tifffile.imwrite('%s.tif'%filename, image_array)
