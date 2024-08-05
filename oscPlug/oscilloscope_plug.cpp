#include "oscilloscope_plug.h"

#include <random>
#include <stddef.h>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<> distr( 0, 1000 );

oscilloscopes::plug::OscPlug::OscPlug( const int& CH_size, const size_t& SR, const size_t& IL ) :
    _CH_size(CH_size)
{
    for( size_t i = 0; i < _CH_size; ++i )
    {
        _oscSignal[i] = oscilloscopes::OscSignal();
        _oscSignal[i]._sampleRate = SR;
        _oscSignal[i]._inputLevel = IL;
    }
}

oscilloscopes::plug::OscPlug::~OscPlug()
{
    for( auto it : _oscSignal )
    {
        if( !it.second._signal.empty() )
            it.second._signal.clear();
    }
    _oscSignal.clear();
}

size_t oscilloscopes::plug::OscPlug::getChannelsSize()
{
    return _CH_size;
}

std::string oscilloscopes::plug::OscPlug::whoAmI()
{
    return std::string("Заглушка");
}
            
void oscilloscopes::plug::OscPlug::setSampleRate( const size_t& SR )
{
    std::cout << "Вызван метод setSampleRate, старое значение = " << _oscSignal[0]._sampleRate;
    for( auto& it : _oscSignal )
        it.second._sampleRate = SR;
    std::cout << " новое = " << _oscSignal[0]._sampleRate << std::endl;
}

size_t oscilloscopes::plug::OscPlug::getSampleRate()
{
    std::cout << "Вызван метод getSampleRate вернул = " << _oscSignal[0]._sampleRate << std::endl;
    return _oscSignal[0]._sampleRate;
}
            
std::vector<size_t> oscilloscopes::plug::OscPlug::getRangeSampleRate()
{
    return _sample;
}

void oscilloscopes::plug::OscPlug::setInputLevel( const uint8_t& CHx, const size_t& IL )
{
    std::cout << "Вызван метод setInputLevel[" << ((int)CHx) << "], старое значение = " << _oscSignal[CHx]._inputLevel;
    _oscSignal[CHx]._inputLevel = IL;
    std::cout << " новое = " << _oscSignal[CHx]._inputLevel << std::endl;
}

size_t oscilloscopes::plug::OscPlug::getInputLevel( const uint8_t &CHx )
{
    std::cout << "Вызван метод getInputLevel вернул = " << _oscSignal[CHx]._inputLevel << std::endl;
    return _oscSignal[CHx]._inputLevel;
}
            
std::vector<size_t> oscilloscopes::plug::OscPlug::getRangeInputLevel()
{
    return _level;
}

oscilloscopes::OscSigframe oscilloscopes::plug::OscPlug::getSignalFrame( const size_t& FS )
{
    std::cout << "Вызван метод getSignalFrame параметр = " << FS;
    for( size_t i = 0; i < _CH_size; ++i )
    {
        if( !_oscSignal[i]._signal.empty() )
            _oscSignal[i]._signal.clear();

        for( size_t j = 0; j < FS; ++j ) 
            _oscSignal[i]._signal.push_back( distr(gen) );

        _oscSignal[i]._signalSize = FS;
    }
    std::cout << "." << std::endl;
    return _oscSignal;
}

std::vector<size_t> oscilloscopes::plug::OscPlug::getRangeSignalFrame()
{
    return _frame;
}


