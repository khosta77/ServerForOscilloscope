#ifndef OSCILLOSCOPE_PLUG_H_
#define OSCILLOSCOPE_PLUG_H_

#include "oscilloscopes.h"

#include <random>
#include <stddef.h>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distr( 0, 1000 );

namespace oscilloscopes
{

    namespace plug
    {

        class OscPlug final : public Oscilloscope
        {
        public:
            // Размер буфера считываемой памяти
            static constexpr size_t _1KB   = 0x00000400;  // 1024 Bytes
            static constexpr size_t _2KB   = 0x00000800;  // 2048 Bytes
            static constexpr size_t _4KB   = 0x00001000;  // 4096 Bytes
            static constexpr size_t _8KB   = 0x00002000;  // 8192 Bytes
            static constexpr size_t _16KB  = 0x00004000;  // 16384 Bytes
            static constexpr size_t _32KB  = 0x00008000;  // 32768 Bytes
            static constexpr size_t _64KB  = 0x00010000;  // 65536 Bytes
            static constexpr size_t _128KB = 0x00020000;  // 131072 Bytes
            //static constexpr size_t _256KB = 0x00040000;  // 262144 Bytes
            //static constexpr size_t _512KB = 0x00080000;  // 524288 Bytes
            //static constexpr size_t _1MB   = 0x00100000;  // 1048576 Bytes

            // Частота сэмплирования
            static constexpr size_t _24MSa  = 0x30;  // 24MSa per channel
            static constexpr size_t _16MSa  = 0x10;  // 16MSa per channel
            static constexpr size_t _8MSa   = 0x08;  // 8MSa per channel
            static constexpr size_t _4MSa   = 0x04;  // 4MSa per channel
            static constexpr size_t _1MSa   = 0x01;  // 1MSa per channel
            static constexpr size_t _500KSa = 0x32;  // 500KSa per channel
            static constexpr size_t _200KSa = 0x14;  // 200KSa per channel
            static constexpr size_t _100KSa = 0x0A;   // 100KSa per channel

            // Масштаб
            static constexpr size_t _10V = 0x01;  // -5V    to 5V
            static constexpr size_t _5V  = 0x02;  // -2.5V  to 2.5V
            static constexpr size_t _2V  = 0x05;  // -1V    to 1V
            static constexpr size_t _1V  = 0x0A;  // -500mv to 500mv

        private:
            OscSigframe _oscSignal;
            int _CH_size;
        public:
            OscPlug( const int& CH_size = 2, const size_t& SR = 10, const size_t& IL = 5 ) :
                _CH_size(CH_size)
            {
                for( size_t i = 0; i < _CH_size; ++i )
                    _oscSignal[i] = oscilloscopes::OscSignal();
                for( size_t i = 0; i < _CH_size; ++i )
                    _oscSignal[i]._sampleRate = SR;
                for( size_t i = 0; i < _CH_size; ++i )
                    _oscSignal[i]._inputLevel = IL;
            }

            ~OscPlug()
            {
                for( auto it : _oscSignal )
                    it.second._signal.clear();
                _oscSignal.clear();
            }

            size_t getChannelsSize() override
            {
                return _CH_size;
            }
            
            void setSampleRate( const size_t& SR ) override
            {
                std::cout << "Вызван метод setSampleRate, старое значение = " << _oscSignal[0]._sampleRate;
                for( size_t i = 0; i < _CH_size; ++i )
                    _oscSignal[i]._sampleRate = SR;
                std::cout << " новое = " << _oscSignal[0]._sampleRate << std::endl;
            }

            size_t getSampleRate() override
            {
                std::cout << "Вызван метод getSampleRate вернул = " << _oscSignal[0]._sampleRate << std::endl;
                return _oscSignal[0]._sampleRate;
            }
            
            std::vector<size_t> getRangeSampleRate() override
            {
                std::cout << "Вызван метод getRangeSampleRate вернул = ";
                std::vector<size_t> range = { _24MSa, _16MSa, _8MSa, _4MSa, _1MSa, _500KSa, _200KSa, _100KSa };
                for( const auto& it : range )
                    std::cout << it << ", ";
                std::cout << std::endl;
                return range;
            }

            void setInputLevel( const uint8_t& CHx, const size_t& IL ) override
            {
                std::cout << "Вызван метод setInputLevel[" << CHx << "], старое значение = " << _oscSignal[CHx]._inputLevel;
                _oscSignal[CHx]._inputLevel = IL;
                std::cout << " новое = " << _oscSignal[CHx]._inputLevel << std::endl;
            }

            size_t getInputLevel( const uint8_t &CHx ) override
            {
                std::cout << "Вызван метод getInputLevel вернул = " << _oscSignal[CHx]._inputLevel << std::endl;
                return _oscSignal[CHx]._inputLevel;                
            }
            
            std::vector<size_t> getRangeInputLevel() override
            {
                std::cout << "Вызван метод getRangeInputLevel вернул = ";
                std::vector<size_t> range = { _10V, _5V, _2V, _1V };
                for( const auto& it : range )
                    std::cout << it << ", ";
                std::cout << std::endl;
                return range;
            }

            OscSigframe getSignalFrame( const size_t& FS ) override
            {
                std::cout << "Вызван метод getSignalFrame параметр = " << FS;
                for( size_t i = 0; i < _CH_size; ++i )
                {
                    _oscSignal[i]._signal.clear();
                    for( size_t j = 0; j < FS; ++j ) 
                    {
                        _oscSignal[i]._signal.push_back( distr(gen) );
                    }
                    _oscSignal[i]._signalSize = FS;
                }
                std::cout << " Вернул значение" << std::endl;
                return _oscSignal;
            }

            std::vector<size_t> getRangeSignalFrame() override
            {
                std::cout << "Вызван метод getRangeSignalFrame вернул = ";
                std::vector<size_t> range = { _1KB, _2KB, _4KB, _8KB, _16KB, _32KB, _64KB, _128KB };
                for( const auto& it : range )
                    std::cout << it << ", ";
                std::cout << std::endl;
                return range; 
            }

        };  // OscPlug

    };  // plug

};  // oscilloscopes

#endif  // OSCILLOSCOPE_PLUG_H_
