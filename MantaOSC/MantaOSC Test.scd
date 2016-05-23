~defaultRecvPort = 31416;
~oscSender = NetAddr("localhost", 31417);
~oscSender.sendMsg('/manta/ledcontrol', "padandbutton", 1);

(
OSCdef(\pad, {
    | msg |
    var padnum = msg[1];
    var value = msg[2];
    var color = if(value == 0, "off", if(value < 100, "amber", "red"));
    ~oscSender.sendMsg('manta/led/pad', color, padnum);
}, '/manta/continuous/pad', recvPort: ~defaultRecvPort);

OSCdef(\slider, {
    | msg |
    msg.postln;
}, '/manta/continuous/slider', recvPort: ~defaultRecvPort);

OSCdef(\button, {
    | msg |
    msg.postln;
}, '/manta/continuous/button', recvPort: ~defaultRecvPort);
)