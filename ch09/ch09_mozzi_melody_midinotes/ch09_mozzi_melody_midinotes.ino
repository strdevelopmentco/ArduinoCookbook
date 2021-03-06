/*
 * Mozzi Melody
 * Play the Chimes of Big Ben
 */

#include <MozziGuts.h>
#include <Oscil.h> // oscillator template
#include <tables/sin2048_int8.h> // sine table for oscillator
#include <EventDelay.h>
#include <mozzi_midi.h>

#define CONTROL_RATE 64 // Control rate in Hz, use powers of 2

enum notes 
{
  E3 = 52, B4 = 59, E4 = 64, F4S = 66, G4S = 68, REST = 0
};

int score[] = { E4, G4S, F4S, B4, REST,
                E4, F4S, G4S, E4, REST,
                G4S, E4, F4S, B4, REST,
                B4, F4S, G4S, E4, REST,
                E3, REST, E3, REST, E3, REST, E3, REST };
const byte scoreLen = sizeof(score) / sizeof(score[0]);

byte beats[scoreLen] = {2, 2, 2, 2, 2, 
                        2, 2, 2, 2, 2, 
                        2, 2, 2, 2, 2,
                        2, 2, 2, 2, 10, 
                        4, 4, 4, 4, 4, 4, 4, 10};
unsigned int beat_ms = 60000 / 180; // the time in ms for 1/8 note

const int pauseTime = 200; // pause between notes
int currNote = 0; // index of the note we're playing
bool pausing = false; // Mini-rest between beats

Oscil <SIN2048_NUM_CELLS, AUDIO_RATE> aSin(SIN2048_DATA); // Sine wave
EventDelay kChangeNoteDelay; // Delay object

void setup() {
  startMozzi(CONTROL_RATE);
  kChangeNoteDelay.start();
}

void updateControl() {
  
  if (kChangeNoteDelay.ready()) { // Is the delay up?
    pausing = !pausing; // Toggle rest state
    if (pausing) {
      aSin.setFreq(0); // set the frequency
      kChangeNoteDelay.set(pauseTime); // Hold the rest for 200ms
    } else {
      if (currNote >= scoreLen) {
        currNote = 0; // Go back to the beginning when done
      }
      int duration = beats[currNote] * beat_ms; // use beats array to determine duration
      kChangeNoteDelay.set(duration - pauseTime); // Set the note duration
      aSin.setFreq(mtof(score[currNote])); // set the frequency
      currNote++;
    }
    kChangeNoteDelay.start();
  }
}

int updateAudio() {
  return aSin.next(); 
}

void loop() {
  audioHook(); // You must have this in your loop
}
