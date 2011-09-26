#ifndef _MANTATYPES_H
#define MANTATYPES_H

enum LEDState {
   Off,
   Amber,
   Red,
};
enum LEDControlType {
   PadAndButton,
   Slider,
   Button
};
typedef uint8_t LEDFrame[6];

#endif /* _MANTATYPES_H */
