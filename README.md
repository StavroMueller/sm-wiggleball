# wiggleball
ploopy trackball nano scroll mode by wiggle gesture

wiggle the ball left/right to switch to a scroll mode, the ball is then simulating a mouse wheel

https://www.youtube.com/watch?v=wvuZpBOGxHc

## Ploopy Nano 2

`keymap.c` is for the Nano 2 (RP2040, QMK target `ploopyco/nano_2/rev2_003`).

- wiggle the ball left/right to toggle scroll mode
- the button is a left click
- caps lock or num lock on also puts the ball in scroll mode

prebuilt firmware: `ploopy_nano_2_wiggleball.uf2`

### flashing

1. unplug the Nano 2
2. hold down its button and plug it in while holding
3. it shows up as a USB drive; drag `ploopy_nano_2_wiggleball.uf2` onto it

if that does not work (bricked firmware), open the case and bridge the two gold bootloader vias with tweezers while plugging in instead. see https://ploopyco.github.io/nano-2-trackball/appendices/programming/

notes:

- this firmware does not include VIA, so the button cannot be remapped in the VIA web app while it is installed
- to go back to stock, flash `nano2_natural_dragscroll_toggle.uf2` from https://github.com/ploopyco/nano-2-trackball/tree/master/firmwares
- Ploopy states that flashing new firmware voids the warranty

### building

```
mkdir -p ~/qmk_firmware/keyboards/ploopyco/nano_2/keymaps/wiggleball
cp keymap.c ~/qmk_firmware/keyboards/ploopyco/nano_2/keymaps/wiggleball/
qmk compile -kb ploopyco/nano_2/rev2_003 -km wiggleball
```

wiggle sensitivity and scroll speed are the `#define`s at the top of `keymap.c`.

## original Ploopy Nano

`ploopy_nano_wiggleball.hex` is the firmware for the original (ATmega32U4) Nano. Do not flash it to a Nano 2. Its source is in the git history of `keymap.c`.

qmk binary: https://github.com/pandrr/wiggleball/raw/refs/heads/main/ploopy_nano_wiggleball.hex.zip
