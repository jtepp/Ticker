

# Preparation

Plug in your Arduino Uno WiFi Rev2 via USB. Select
the port and board in the "Tools" menu (you may have
to open the board manager to install the MegaAVR
package to work with the WiFi Rev2 board). In the
"Sketch" menu open the library manager and install
FastLED and WiFiNINA. Under File>Examples>WiFiNINA>
Tools, open FirmwareUpdater and upload it to the
board. While that's running, open the "Tools" menu
and run the WiFiNINA firmware updater. Once that's
complete, you are ready to run the ticker.



## Common Errors
**Error:** `jtag2() something something could not not upload to board something something expected value`
**Cause:** You probably unplugged it while the strips were running.
**Fix:** Unplug all the wires running from the Arduino to the LEDs. Unplug the Arduino from power and press the reset button. Now plug it back in and it should work.
**Next time:** Unplug the Arduino <-> LED power wires before you disconnect the entire Arduino from power.

**Error:** `something something exceeded memory`
**Cause:** You tried using too many LEDs and too many maximum characters in the message for the LEDs tiny memory capabilities. The NUM_LEDS constant will have the biggest effect on this as each increase of 1 will take up about 0.25% of the dynamic memory.
**Fix:** Decrease NUM_LEDS and/or MSG_LENGTH.
**Next time:** Send Arduino an angry letter asking why they gave you the lowest amount of memory you'll ever encounter.

**Error:** repeated `Attempting to connect to SSID`
**Cause:** The LED strips used too much power and didn't leave enough power for the WiFi chip to be able to find a network.
Fix: Lower NUM_LEDS so that each strip powers less LEDs and uses less power.
**Next time:** Use the 3.3V pin instead of the 5V pin. This will result in darker lights, but you can try a higher BRIGHTNESS constant value. Buy a power adaptor. Arduino recommends a 7-12V input, and I would recommend around 60W for a 200*5 LED ticker. (W = V*A so 60W @ 12V = 5A)

**Error:** `No device found`
**Cause:** You tried uploading without plugging the arduino in.
**Fix:** Plug the arduino in and re-upload.
**Next time:** Check all connections before uploading I guess?


**Error:** won't compile when adding more than one LED strip
**Cause:** Honestly no idea, maybe the same thing as the first error
**Fix:** Not sure... maybe try same solution as the first error, and also try compiling with a low NUM_LEDS and compiling with/without the Arduino plugged in
**Next time:** Try the above fix(es) and leave the Arduino unplugged for a while before trying again.

**Error:** something else
**Cause:** sometimes random stuff just happens with cheap hardware like this. It's usually not unfixable/permanent though. Maybe it was your fault, maybe it wasn't.
**Fix:** Google it. If you want you can contact me at https://jacobtepperman.com/contact but I doubt I'll be that helpful.
**Next Time:** Expect disappointment unless you're an electrical engineer and have any idea what you're actually doing with this stuff.
