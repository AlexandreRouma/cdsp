#pragma once
#include <thread>
#include <cdsp/stream.h>
#include <cdsp/types.h>

namespace cdsp {
    class Interpolator {
    public:
        Interpolator(stream<float>* in, float interpolation, int bufferSize) : output(bufferSize * 2) {
            _input = in;
            _interpolation = interpolation;
            _bufferSize = bufferSize;
        }

        void start() {
            _workerThread = std::thread(_worker, this);
        }

        stream<float> output;

    private:
        static void _worker(Interpolator* _this) {
            float* inBuf = new float[(int)((float)_this->_bufferSize / _this->_interpolation)];
            float* outBuf = new float[_this->_bufferSize];
            while (true) {
                _this->_input->read(inBuf, (int)((float)_this->_bufferSize / _this->_interpolation));
                for (int i = 0; i < _this->_bufferSize; i++) {
                    outBuf[i] = inBuf[(int)((float)i / _this->_interpolation)];
                }
                _this->output.write(outBuf, _this->_bufferSize);
            }
        }

        stream<float>* _input;
        int _bufferSize;
        float _interpolation;
        std::thread _workerThread;
    };


    class IQInterpolator {
    public:
        IQInterpolator(stream<complex_t>* in, float interpolation, int bufferSize) : output(bufferSize * 2) {
            _input = in;
            _interpolation = interpolation;
            _bufferSize = bufferSize;
        }

        void start() {
            _workerThread = std::thread(_worker, this);
        }

        stream<complex_t> output;

    private:
        static void _worker(IQInterpolator* _this) {
            complex_t* inBuf = new complex_t[(int)((float)_this->_bufferSize / _this->_interpolation)];
            complex_t* outBuf = new complex_t[_this->_bufferSize];
            while (true) {
                _this->_input->read(inBuf, (int)((float)_this->_bufferSize / _this->_interpolation));
                for (int i = 0; i < _this->_bufferSize; i++) {
                    outBuf[i] = inBuf[(int)((float)i / _this->_interpolation)];
                }
                _this->output.write(outBuf, _this->_bufferSize);
            }
        }

        stream<complex_t>* _input;
        int _bufferSize;
        float _interpolation;
        std::thread _workerThread;
    };
};