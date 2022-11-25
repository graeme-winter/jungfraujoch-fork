// Copyright (2019-2022) Paul Scherrer Institute
// SPDX-License-Identifier: GPL-3.0-or-later

#include "JFJochReceiverTest.h"
#include "JFJochReceiverService.h"
#include "../../common/ZMQImagePusher.h"
#include "../../common/TestImagePusher.h"

#define STORAGE_CELL_FOR_TEST 11

JFJochProtoBuf::ReceiverOutput RunJFJochReceiverTest(std::vector<AcquisitionDevice *> &aq_devices, ImagePusher &pusher,
                                                           const DiffractionExperiment &x,
                                                           Logger &logger, JFCalibration &calib,
                                                           uint16_t nthreads, bool abort,
                                                           ZMQPreviewPublisher *in_preview_writer) {
    JFJochProtoBuf::ReceiverInput receiver_input;
    *receiver_input.mutable_jungfraujoch_settings() = x;
    *receiver_input.mutable_calibration() = calib;

     ZMQContext context;
    context.NumThreads(4);

    ZMQSpotPublisher spot_writer(context, "inproc://spot_writer");

    JFJochReceiverService service(aq_devices, logger, pusher);
    service.PreviewPublisher(in_preview_writer).SpotPublisher(&spot_writer);
    service.NumThreads(nthreads);

    JFJochProtoBuf::DataProcessingSettings settings;
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

    JFJochProtoBuf::ReceiverOutput receiver_output;
    service.Stop(&grpc_context, nullptr, &receiver_output);

    return receiver_output;
}

bool JFJochReceiverTest(JFJochProtoBuf::ReceiverOutput &output, Logger &logger,
                        std::vector<std::unique_ptr<AcquisitionDevice>> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, bool abort,
                        bool verbose, ZMQPreviewPublisher *in_preview_writer) {
    std::vector<AcquisitionDevice *> tmp_aq_devices;
    for (const auto &i: aq_devices)
        tmp_aq_devices.emplace_back(i.get());

    return JFJochReceiverTest(output, logger, tmp_aq_devices, x, nthreads,
                              abort, verbose, in_preview_writer);
}

static JFCalibration GeneratePedestalCalibration(const DiffractionExperiment &x) {
    JFCalibration ret(x);

    std::vector<float> pedestal_g0( RAW_MODULE_SIZE), pedestal_g1(RAW_MODULE_SIZE), pedestal_g2(RAW_MODULE_SIZE);

    for (int s = 0; s < x.GetStorageCellNumber(); s++) {
        for (int m = 0; m < x.GetModulesNum(); m++) {
            for (int i = 0; i < RAW_MODULE_SIZE; i++) {
                pedestal_g0[i] = 3000 + 100 * s + 10 * m + i % 50;
                pedestal_g1[i] = 15000 - 100 * s + 10 * m + i % 50;
                pedestal_g2[i] = 14000 - 100 * s + 10 * m + i % 50;

            }
            ret.Pedestal(m, 0, s).LoadPedestal(pedestal_g0);
            ret.Pedestal(m, 1, s).LoadPedestal(pedestal_g1);
            ret.Pedestal(m, 2, s).LoadPedestal(pedestal_g2);
        }
    }

    return ret;
}

bool JFJochReceiverTest(JFJochProtoBuf::ReceiverOutput &output, Logger &logger,
                        std::vector<AcquisitionDevice *> &aq_devices, const DiffractionExperiment &x,
                        uint16_t nthreads, bool abort,
                        bool verbose, ZMQPreviewPublisher *in_preview_writer) {

    std::vector<uint16_t> raw_expected_image(RAW_MODULE_SIZE * x.GetModulesNum());
    for (int i = 0; i < x.GetDataStreamsNum(); i++) {
        uint32_t module0 = x.GetFirstModuleOfDataStream(i);
        for (int m = 0; m < x.GetModulesNum(i); m++) {
            memcpy(raw_expected_image.data() + (module0 + m) * RAW_MODULE_SIZE,
                   aq_devices[i]->GetInternalGeneratorFrame().data(), RAW_MODULE_SIZE * sizeof(uint16_t));
        }
    }

    JFCalibration calib = GeneratePedestalCalibration(x);
    JFModuleGainCalibration gain_calib;

    for (int d = 0; d < x.GetDataStreamsNum(); d++)
        for (int m = 0; m < x.GetModulesNum(d); m++)
            aq_devices[d]->LoadModuleGain(gain_calib, m);

    int64_t image_number = x.GetImageNum() - 1;

    if (x.GetStorageCellNumber() > 1)
        image_number = STORAGE_CELL_FOR_TEST;
    if (image_number < 0)
        image_number = 0;

    TestImagePusher pusher(x.GetImageLocationInFile(image_number));

    output = RunJFJochReceiverTest(aq_devices, pusher, x, logger, calib,  nthreads, abort,
                                   in_preview_writer);

    bool no_errors = true;

    if ((x.GetImageNum() > 0) && (!abort)) {
        no_errors = pusher.CheckImage(x, raw_expected_image, calib, gain_calib, logger);

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
        if (!pusher.CheckSequence()) {
            logger.Error("Wrong sequence of operations for pusher");
            no_errors = false;
        }
        if (pusher.GetCounter() != x.GetImageNum()) {
            logger.Error("Wrong frame number from pusher");
            no_errors = false;
        }
    }

    return no_errors;
}
