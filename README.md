# temperatures

## Summary

A small personal project bringing together some hype components.
The initial goal was to build the cheapest wifi connected thermometer.

The temperature is collected from a DS1820 by an ESP-01.
The data is then sent by wifi to a raspberry pi relay (which I already owned and was used as a print server).
The raspberry send the datas collected from all the sensors to a influxdb/grafana instance.

![Project diagram](images/Temperatures.png)

## Raspberry code

The raspberry code is based on a [Flask](http://flask.pocoo.org/) microservice which relays the data to the configured influxdb server.
The microservice lifecycle is managed with [Supervisord](http://supervisord.org/)

## ESP8266 code

The ESP8266 code brings together the onewire protocol to read data from the DS1820 and the ESP8266WiFi code to send a HTTP request.
My ESP-01 is powered from 2 AA batteries, and customized according like [this](http://tim.jagenberg.info/2015/01/18/low-power-esp8266/) to lower power consumption.

## BOM

* ESP-01 - < 2$
* DS1820 - < 1$
* AAA Holder - < 1$
* Resistors ( 1x 4.7k & 3x 2.2k) - < 1$