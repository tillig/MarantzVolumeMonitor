# Marantz Volume Monitor

Arduino-based volume monitor for Marantz receivers.

---

My Marantz receiver sits in a cabinet, which is fine, but when handling 4K content the on-screen display showing volume and input information doesn't come up. This seems to be a fairly well-known issue.

My solution to this is a small Arduino-based volume monitor that uses the Marantz HTTP API to get info about the receiver and display it on an LCD.

- [Blog entry with additional details](http://www.paraesthesia.com/archive/2017/03/27/arduino-volume-monitor-for-marantz-receiver/)
- [YouTube video with the monitor in action](https://www.youtube.com/watch?v=8WN-ZNZASWc)

# Software

I used the [Visual Micro](http://www.visualmicro.com/) Arduino integration for Visual Studio when building this so you'll see VS solution and project files in there, but you shouldn't need that to use the code.

Note I have tried to retain project structure compatibility with the standard Arduino IDE, but that's not a priority; given I use Visual Micro, I give priority to the dev environment that I work in.

# Hardware

This is what I used, so the pins and such are set to use these items. If you use a different set of shields or hardware, you may have to adjust accordingly.

- [Arduino Uno R3](http://amzn.to/2moYxUe)
- [DFRobot 1602 LCD Shield](http://amzn.to/2mwXbGw) ([Reference](https://www.dfrobot.com/product-51.html))
- [SunFounder Ethernet Shield W5100](http://amzn.to/2miekaT)
