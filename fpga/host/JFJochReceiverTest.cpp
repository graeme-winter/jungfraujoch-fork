// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <bitshuffle/bitshuffle.h>

#include "JFJochReceiverTest.h"
#include "JFJochReceiverService.h"
#include "../../common/ZMQImagePuller.h"
#include "../tests/FPGAUnitTest.h"

extern "C" {
uint32_t bshuf_read_uint32_BE(const void* buf);
uint64_t bshuf_read_uint64_BE(const void* buf);
}

JFJochProtoBuf::JFJochReceiverOutput RunJFJochReceiverTest(std::vector<AcquisitionDevice *> &aq_devices,
                                                           const DiffractionExperiment &x,
                                                           Logger &logger, JungfrauCalibration &calib, size_t &nframes,
                                                           std::vector<uint8_t> &image,
                                                           uint16_t nthreads, bool abort,
                                                           ZMQPreviewPublisher *in_preview_writer) {
    std::atomic<size_t> a_nframes = 0;

    ZMQContext context;
    context.NumThreads(4);
    ZMQImagePuller zmqImagePuller_0(context, "inproc://#1");
    ZMQImagePuller zmqImagePuller_1(context, "inproc://#2");
    ZMQImagePuller zmqImagePuller_2(context, "inproc://#3");

    std::thread puller_thread_0, puller_thread_1, puller_thread_2;
    if (x.GetImageNum() > 0) {
        puller_thread_0 = std::thread([&] {
            std::vector<uint8_t> tmp;
            int64_t msg_size = zmqImagePuller_0.GetImage(tmp);
            while (msg_size > 0) {
                auto image_metadata = (const ImageMetadata *) tmp.data();
                if (image_metadata->frameid == x.GetImageNum() - 1) {
                    image.resize(msg_size - sizeof(ImageMetadata));
                    memcpy(image.data(), tmp.data() + sizeof(ImageMetadata), msg_size - sizeof(ImageMetadata));
                }
                a_nframes++;
                msg_size = zmqImagePuller_0.GetImage(tmp);
            }
        });
        puller_thread_1 = std::thread([&] {
            std::vector<uint8_t> tmp;
            int64_t msg_size = zmqImagePuller_1.GetImage(tmp);
            while (msg_size > 0) {
                auto image_metadata = (const ImageMetadata *) tmp.data();
                if (image_metadata->frameid == x.GetImageNum() - 1) {
                    image.resize(msg_size - sizeof(ImageMetadata));
                    memcpy(image.data(), tmp.data() + sizeof(ImageMetadata), msg_size - sizeof(ImageMetadata));
                }
                a_nframes++;
                msg_size = zmqImagePuller_1.GetImage(tmp);
            }
        });
        puller_thread_2 = std::thread([&] {
            std::vector<uint8_t> tmp;
            int64_t msg_size = zmqImagePuller_2.GetImage(tmp);
            while (msg_size > 0) {
                auto image_metadata = (const ImageMetadata *) tmp.data();
                if (image_metadata->frameid == x.GetImageNum() - 1) {
                    image.resize(msg_size - sizeof(ImageMetadata));
                    memcpy(image.data(), tmp.data() + sizeof(ImageMetadata), msg_size - sizeof(ImageMetadata));
                }
                a_nframes++;
                msg_size = zmqImagePuller_2.GetImage(tmp);
            }
        });
    }

    ZMQSpotPublisher spot_writer(context, "inproc://spot_writer");

    JFJochReceiverService service(aq_devices,context, logger);
    service.PreviewPublisher(in_preview_writer).SpotPublisher(&spot_writer);
    service.NumThreads(nthreads);

    JFJochProtoBuf::JFJochReceiverInput receiver_input;
    *receiver_input.mutable_jungfraujoch_settings() = x;
    *receiver_input.mutable_calibration() = calib;
    receiver_input.add_writer_zmq_address("inproc://#1");
    receiver_input.add_writer_zmq_address("inproc://#2");
    receiver_input.add_writer_zmq_address("inproc://#3");

    JFJochProtoBuf::DataProcessingSettings settings;
    settings.set_enable_3d_spot_finding(false);
    settings.set_signal_to_noise_threshold(2.5);
    settings.set_photon_count_threshold(5);
    settings.set_min_pix_per_spot(3);
    settings.set_max_pix_per_spot(200);
    settings.set_local_bkg_size(5);
    service.SetDataProcessingSettings(nullptr, &settings, nullptr);

    grpc::ServerContext grpc_context;
    service.Start(&grpc_context, &receiver_input, nullptr);

    if (x.GetImageNum() > 0) {
        // Initial communication
        if (abort) {
            std::this_thread::sleep_for(std::chrono::seconds(180));
            service.Cancel(&grpc_context, nullptr, nullptr);
        }
    }

    JFJochProtoBuf::JFJochReceiverOutput receiver_output;
    service.Stop(&grpc_context, nullptr, &receiver_output);

    if (puller_thread_0.joinable())
        puller_thread_0.join();

    if (puller_thread_1.joinable())
        puller_thread_1.join();

    if (puller_thread_2.joinable())
        puller_thread_2.join();

    nframes = a_nframes;
    calib.ImportPedestalAndMask(receiver_output.calibration());

    return receiver_output;
}

bool JFJochReceiverTest(JFJochProtoBuf::JFJochReceiverOutput &output, Logger &logger,
                        std::vector<std::unique_ptr<AcquisitionDevice>> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, const std::string &test_path, bool abort,
                        bool verbose, ZMQPreviewPublisher *in_preview_writer) {
    std::vector<AcquisitionDevice *> tmp_aq_devices;
    for (const auto &i: aq_devices)
        tmp_aq_devices.emplace_back(i.get());

    return JFJochReceiverTest(output, logger, tmp_aq_devices, x, nthreads, test_path,
                              abort, verbose, in_preview_writer);
}

bool JFJochReceiverTest(JFJochProtoBuf::JFJochReceiverOutput &output, Logger &logger,
                        std::vector<AcquisitionDevice *> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, const std::string &test_path, bool abort,
                        bool verbose, ZMQPreviewPublisher *in_preview_writer) {

    std::vector<uint16_t> raw_expected_image(x.GetModulesNum() * RAW_MODULE_SIZE);
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {

        auto nmod_int_pkt_gen = aq_devices[i]->GetInternalPacketGeneratorModuleNum();
        std::vector<uint16_t> input(nmod_int_pkt_gen * RAW_MODULE_SIZE);

        for (int j = 0; j < nmod_int_pkt_gen; j++)
            LoadBinaryFile(test_path + "tests/test_data/mod5_raw" + std::to_string(j) + ".bin",
                           input.data() + j * RAW_MODULE_SIZE, RAW_MODULE_SIZE);

        aq_devices[i]->SetCustomInternalGeneratorFrame((int16_t *) (input.data()),
                                                       nmod_int_pkt_gen * RAW_MODULE_SIZE * sizeof(int16_t));

        uint32_t module0 = x.GetFirstModuleOfDataStream(i);
        for (int m = 0; m < x.GetModulesNum(i); m++) {
            memcpy(raw_expected_image.data() + (module0 + m) * RAW_MODULE_SIZE,
                   input.data() + (m % nmod_int_pkt_gen) * RAW_MODULE_SIZE,
                   RAW_MODULE_SIZE * sizeof(uint16_t));
        }
    }

    // Load pedestal files
    std::vector<float> pedestal(3 * RAW_MODULE_SIZE);
    LoadBinaryFile(test_path + "tests/test_data/mod5_pedeG0.bin", pedestal.data(), RAW_MODULE_SIZE);
    LoadBinaryFile(test_path + "tests/test_data/mod5_pedeG1.bin", pedestal.data() + RAW_MODULE_SIZE, RAW_MODULE_SIZE);
    LoadBinaryFile(test_path + "tests/test_data/mod5_pedeG2.bin", pedestal.data() + 2 * RAW_MODULE_SIZE, RAW_MODULE_SIZE);

    // Load gain file
    std::vector<double> gain(3 * RAW_MODULE_SIZE);

    JungfrauCalibration calib(x), c_out(x);

    for (int m = 0; m < x.GetModulesNum(); m++) {
        calib.LoadModulePedestal(pedestal, m);
    }

    for (int d = 0; d < x.GetDataStreamsNum(); d++)
        for (int m = 0; m < x.GetModulesNum(d); m++)
            aq_devices[d]->LoadModuleGain(test_path + "tests/test_data/gainMaps_M049.bin", m);

    std::vector<uint8_t> image;

    size_t nframes = 0;

    output = RunJFJochReceiverTest(aq_devices, x, logger, calib, nframes, image,
                                                    nthreads, abort, in_preview_writer);

    bool no_errors = true;

    JFJochDecompressor decompressor(x);

    if ((x.GetImageNum() > 0) && (!abort)) {
        if (image.empty()) {
            logger.Error("Image not saved properly");
            no_errors = false;
        } else {
            int16_t *decompressed_image;
            // Image decompression
            try {
                decompressed_image = (int16_t *) decompressor.Decompress((char *) image.data(),
                                                                         image.size(),
                                                                         x.GetPixelsNum(),
                                                                         x.GetPixelDepth());
            } catch (const JFJochException &e) {
                logger.Error(e.what());
                no_errors = false;
            }
            if (no_errors) {
                // Check output

               if (x.GetDetectorMode() == DetectorMode::Conversion) {
                    double result = CheckConversionWithGeomTransform(x, calib, raw_expected_image.data(),
                                                                     decompressed_image,
                                                                     test_path + "tests/test_data/gainMaps_M049.bin");
                    if (result > 0.5) {
                        logger.Error("Mean conversion error (" + std::to_string(result) + ") larger than threshold");
                        no_errors = false;
                    } else
                        logger.Info("Mean conversion error: " + std::to_string(result));
                } else if (x.GetDetectorMode() == DetectorMode::Raw) {
                    if (memcmp(raw_expected_image.data(), decompressed_image, sizeof(uint16_t) * x.GetPixelsNum()) !=
                        0) {
                        logger.Error("Raw data mismatch");
                        no_errors = false;
                    }
                }
            }
        }

        // Final checks
        for (int i = 0; i < x.GetDataStreamsNum(); i++) {
            if (output.device_statistics(i).efficiency() != 1.0) {
                logger.Error("Not all frames were received for data stream ");
                no_errors = false;
            }
        }

        if (nframes != x.GetImageNum()) {
            logger.Error("Wrong frame number from ZeroMQ");
            no_errors = false;
        }
    }

    return no_errors;
}
