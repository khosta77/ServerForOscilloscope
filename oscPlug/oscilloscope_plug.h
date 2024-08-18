#ifndef OSCILLOSCOPE_PLUG_H_
#define OSCILLOSCOPE_PLUG_H_

#include "oscilloscopes.h"

namespace oscilloscopes
{

    namespace plug
    {

        class OscPlug final : public Oscilloscope
        {
        private:
            OscSigframe _oscSignal;
            int _CH_size;

            const std::vector<size_t> _sample = { 1, 2, 3, 4, 5 };
            const std::vector<size_t> _level = { 6, 7, 8, 9 };
            const std::vector<size_t> _frame = { 10, 11, 12, 13 };

        public:
            OscPlug( const int& CH_size = 2, const size_t& SR = 10, const size_t& IL = 5 );
            ~OscPlug();

            const size_t getChannelsSize() const override;
            const std::string whoAmI() const override;
            
            size_t setSampleRate( const size_t& SR ) override;
            const size_t getSampleRate() override;
            const std::vector<size_t> getRangeSampleRate() const override;

            size_t setInputLevel( const uint8_t& CHx, const size_t& IL ) override;
            const size_t getInputLevel( const uint8_t &CHx ) override;
            const std::vector<size_t> getRangeInputLevel() const override;

            OscSigframe getSignalFrame( const size_t& FS ) override;
            const std::vector<size_t> getRangeSignalFrame() const override;

            OscSignal getSignalFromTrigger( const uint8_t&, const float&, const uint8_t& ) override;
            const void onTrigger() override;
            const void offTrigger() override;

        };  // OscPlug

    };  // plug

};  // oscilloscopes

#endif  // OSCILLOSCOPE_PLUG_H_
