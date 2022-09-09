// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include <bitshuffle/bitshuffle.h>

#include "JFJochReceiverTest.h"
#include "JFJochReceiverService.h"
#include "../../common/ZMQImagePuller.h"
#include "../tests/FPGAUnitTest.h"

#define NPULLERS 4
#define STORAGE_CELL_FOR_TEST 11

extern "C" {
uint32_t bshuf_read_uint32_BE(const void* buf);
uint64_t bshuf_read_uint64_BE(const void* buf);
}

JFJochProtoBuf::JFJochReceiverOutput RunJFJochReceiverTest(std::vector<AcquisitionDevice *> &aq_devices,
                                                           const DiffractionExperiment &x,
                                                           Logger &logger, JFCalibration &calib, size_t &nframes,
                                                           std::vector<uint8_t> &image,
                                                           uint16_t nthreads, bool abort,
                                                           ZMQPreviewPublisher *in_preview_writer,
                                                           int32_t image_number) {

    if (image_number < 0)
        image_number = x.GetImageNum() - 1;

    JFJochProtoBuf::JFJochReceiverInput receiver_input;
    *receiver_input.mutable_jungfraujoch_settings() = x;
    *receiver_input.mutable_calibration() = calib;

    std::atomic<size_t> a_nframes = 0;

    ZMQContext context;
    context.NumThreads(4);

    std::vector<std::unique_ptr<ZMQImagePuller> > zmqImagePuller;
    std::vector<std::thread> puller_thread;

    if (x.GetImageNum() > 0) {
        for (int i = 0; i < NPULLERS; i++) {
            zmqImagePuller.emplace_back(std::make_unique<ZMQImagePuller>(context,
                                                                         "inproc://#" + std::to_string(i)));
            puller_thread.emplace_back([i, &image, &zmqImagePuller, &a_nframes, &image_number] {
                std::vector<uint8_t> tmp;
                int64_t msg_size = zmqImagePuller[i]->GetImage(tmp);
                while (msg_size > 0) {
                    auto image_metadata = (const ImageMetadata *) tmp.data();
                    if (image_metadata->frameid == image_number) {
                        image.resize(msg_size - sizeof(ImageMetadata));
                        memcpy(image.data(), tmp.data() + sizeof(ImageMetadata), msg_size - sizeof(ImageMetadata));
                    }
                    a_nframes++;
                    msg_size = zmqImagePuller[i]->GetImage(tmp);
                }
            });
            receiver_input.add_writer_zmq_address("inproc://#" + std::to_string(i));
        }
    }

    ZMQSpotPublisher spot_writer(context, "inproc://spot_writer");

    JFJochReceiverService service(aq_devices,context, logger);
    service.PreviewPublisher(in_preview_writer).SpotPublisher(&spot_writer);
    service.NumThreads(nthreads);

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

    for (auto &i: puller_thread) {
        if (i.joinable())
            i.join();
    }

    nframes = a_nframes;
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


    // Load gain file
    std::vector<double> gain(3 * RAW_MODULE_SIZE);

    JFCalibration calib(x);

    // Load pedestal files
    std::vector<float> pedestal_g0( RAW_MODULE_SIZE), pedestal_g1(RAW_MODULE_SIZE), pedestal_g2(RAW_MODULE_SIZE);
    LoadBinaryFile(test_path + "tests/test_data/mod5_pedeG0.bin", pedestal_g0.data(), RAW_MODULE_SIZE);
    LoadBinaryFile(test_path + "tests/test_data/mod5_pedeG1.bin", pedestal_g1.data(), RAW_MODULE_SIZE);
    LoadBinaryFile(test_path + "tests/test_data/mod5_pedeG2.bin", pedestal_g2.data(), RAW_MODULE_SIZE);

    for (int m = 0; m < x.GetModulesNum(); m++) {
        calib.Pedestal(m, 0).LoadPedestal(pedestal_g0);
        calib.Pedestal(m, 1).LoadPedestal(pedestal_g1);
        calib.Pedestal(m, 2).LoadPedestal(pedestal_g2);
    }

    for (int s = 1; s < x.GetStorageCellNumber(); s++) {
        for (int m = 0; m < x.GetModulesNum(); m++) {
            for (int i = 0; i < RAW_MODULE_SIZE; i++) {
                pedestal_g0[i] = 3000 + 100 * s + 10 * m + i % 50;
                pedestal_g1[i] = 15000 - 100 * s + 10 * m + i % 50;
                pedestal_g2[i] = 14000 - 100 * s + 10 * m + i % 50;

            }
            calib.Pedestal(m, 0, s).LoadPedestal(pedestal_g0);
            calib.Pedestal(m, 1, s).LoadPedestal(pedestal_g1);
            calib.Pedestal(m, 2, s).LoadPedestal(pedestal_g2);
        }
    }

    for (int d = 0; d < x.GetDataStreamsNum(); d++)
    for (int m = 0; m < x.GetModulesNum(d); m++)
        aq_devices[d]->LoadModuleGain(test_path + "tests/test_data/gainMaps_M049.bin", m);

    std::vector<uint8_t> image;

    size_t nframes = 0;

    size_t image_number = x.GetImageNum() - 1;
    if (x.GetStorageCellNumber() > 1)
        image_number = STORAGE_CELL_FOR_TEST;

    output = RunJFJochReceiverTest(aq_devices, x, logger, calib, nframes, image,
                                                    nthreads, abort, in_preview_writer, image_number);

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
                   size_t storage_cell = 0;
                   if (x.GetStorageCellNumber() > 1)
                       storage_cell = image_number % 16;

                    double result = CheckConversionWithGeomTransform(x, calib, raw_expected_image.data(),
                                                                     decompressed_image,
                                                                     test_path + "tests/test_data/gainMaps_M049.bin",
                                                                     storage_cell);
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
            if (output.device_statistics(i).timestamp(0) != 0xABCDEF) {
                logger.Error("Timestamp recovered incorrectly ");
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
