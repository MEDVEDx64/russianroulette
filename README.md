Linux kernel Russian Roulette
=============================

Got bored while setting up your company's business-critical super-mega-datastorage?
This bastard good module is just for you! Put the bullets in, roll it and make a shot!

Player's Manual
---------------

* Build utilities and linux headers have to be present in your system, then you just type `make` to build
and `sudo insmod russian_roulette.ko` to load the module.

* Create a chardev file with `sudo mknod /dev/rr c X 0`, where X is a *major number* as told you
by `dmesg | tail -n 1` output right after the module is loaded. For your convenience *(what a villain I am!)*
let other users access the device: `sudo chmod a+w /dev/rr`.

* The Roulette device is controlled with a single char typed into it, for example, `echo 1 > /dev/rr`.
These commands are:
  * **1-6**: put bullets into slots
  * **0**: remove all the bullets
  * **r**: roll
  * **s**: shot
