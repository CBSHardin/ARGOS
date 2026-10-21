
#ifndef __ARGOS_API_GPIO_INTERRUPTS_PINS_H__
#define __ARGOS_API_GPIO_INTERRUPTS_PINS_H__

#include "gpio_interrupts.hpp"

#define GpioPinToIntSig(pin)  (pin == 2 ? Argos::GpioInterruptsPHAL::IntSignal_t::int0 :    \
                              (pin == 3 ? Argos::GpioInterruptsPHAL::IntSignal_t::int1 :    \
                              (pin == 21 ? Argos::GpioInterruptsPHAL::IntSignal_t::int2 :   \
                              (pin == 20 ? Argos::GpioInterruptsPHAL::IntSignal_t::int3 :   \
                              (pin == 19 ? Argos::GpioInterruptsPHAL::IntSignal_t::int4 :   \
                              (pin == 18 ? Argos::GpioInterruptsPHAL::IntSignal_t::int5 :   \
                              (pin == 53 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint0 : \
                              (pin == 52 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint1 : \
                              (pin == 51 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint2 : \
                              (pin == 50 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint3 : \
                              (pin == 10 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint4 : \
                              (pin == 11 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint5 : \
                              (pin == 12 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint6 : \
                              (pin == 13 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint7 : \
                              (pin == 0 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint8 :  \
                              (pin == 15 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint9 : \
                              (pin == 14 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint10 :\
                              (pin == 70 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint11 :\
                              (pin == 71 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint12 :\
                              (pin == 72 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint13 :\
                              (pin == 73 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint14 :\
                              (pin == 74 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint15 :\
                              (pin == 62 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint16 :\
                              (pin == 63 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint17 :\
                              (pin == 64 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint18 :\
                              (pin == 65 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint19 :\
                              (pin == 66 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint20 :\
                              (pin == 67 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint21 :\
                              (pin == 68 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint22 :\
                              (pin == 69 ? Argos::GpioInterruptsPHAL::IntSignal_t::pcint23 : Argos::GpioInterruptsPHAL::IntSignal_t::NOT_INTERRUPT\
                              ))))))))))))))))))))))))))))))


#endif
