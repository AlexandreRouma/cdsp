#include <stdio.h>
#include <Windows.h>
#include <hackrf.h>
#include <math.h>
#include <cdsp/types.h>
#include <cdsp/stream.h>
#include <cdsp/modulation.h>
#include <cdsp/math.h>
#include <cdsp/file.h>
#include <cdsp/generator.h>
#include <cdsp/resampling.h>
#include <cdsp/hackrf.h>
#include <cdsp/demodulation.h>
#include <cdsp/filter.h>
#include <complex>

/*
        ==================== PLEASE NOTE ====================

        This file is for testing of the library
        The library itself is in the cdsp/ directory
        
        =====================================================
*/

cdsp::HackRFSink* sink;

void sig_hndl() {
    sink->stop();
    hackrf_exit();
    exit(0);
}

int main(int argc, char *argv[]) {
    hackrf_init();
    hackrf_device_list_t* list = hackrf_device_list();
    hackrf_device* dev;
    int err = hackrf_device_list_open(list, 0, &dev);
    if (err != 0) {
        printf("Error while opening HackRF: %d\n", err);
        return -1;
    }
    hackrf_set_freq(dev, 85000000);
    hackrf_set_txvga_gain(dev, 10);
    hackrf_set_sample_rate(dev, 8000000);

    printf("HackRF Open!\n");

    std::vector<float> taps;
    taps.push_back(0.000918f);
    taps.push_back(0.006427f);
    taps.push_back(0.017513f);
    taps.push_back(0.032128f);
    taps.push_back(0.044902f);
    taps.push_back(0.050000f);
    taps.push_back(0.044902f);
    taps.push_back(0.032128f);
    taps.push_back(0.017513f);
    taps.push_back(0.006427f);
    taps.push_back(0.000918f);

    int test = sizeof(std::complex<float>);
    int testb = sizeof(cdsp::complex_t);

    cdsp::RawFileSource src(argv[1], 100);
    //cdsp::RandomSource src(0, 0, 100);
    cdsp::Interpolator interp(&src.output, 167, 16700);
    cdsp::FMModulator fmmod(&interp.output, 100000, 8000000, 16700);
    cdsp::HackRFSink hackrf(dev, 16700, &fmmod.output);
    sink = &hackrf;
    src.start();
    interp.start();
    fmmod.start();
    hackrf.start();

    // cdsp::HackRFSource hackrf(dev, 64000);
    // cdsp::ComplexSineSource src(-1000000, 8000000, 64000);
    // cdsp::Multiplier mul(&hackrf.output, &src.output, 64000);
    // cdsp::IQInterpolator interp(&mul.output, 1.0f / 20.0f, 3200);
    // cdsp::FMDemodulator demod(&interp.output, 100000, 400000, 3200);
    // cdsp::Interpolator audioInterp(&demod.output, 0.1f, 320);
    // cdsp::RawFileSink file("output.raw", &audioInterp.output, 320);

    // sink = &hackrf;

    // hackrf.start();
    // src.start();
    // mul.start();
    // interp.start();
    // demod.start();
    // audioInterp.start();
    // file.start();

    

    SetConsoleCtrlHandler( (PHANDLER_ROUTINE) sig_hndl, TRUE);

    while (true) {
        Sleep(100);
    }
    return 0;
}