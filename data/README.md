# Sensor Data Recordings

This folder holds some recordings of data from experiments made while running the [sensor_mqtt_publish](../sensor_mqtt_publish) code.

Data captured with:
```
  mosquitto_sub -h broker.mqtt-dashboard.com -t "mcqn/#" | tee NAME-OF-LOG-FILE.log
```

If you want a quick-and-dirty way to replay any captured data, try:
```
  cat NAME-OF-LOG-FILE.log | mosquitto_pub -h broker.mqtt-dashboard.com -t "mcqn/test"
```

## Recordings

 * [2019-02-09-tapdancing.log](2019-02-09-tapdancing.log) - initial tests with [Sole Rebel Tap](http://solerebeltap.co.uk/).  Lines with a single number are from the left-toe sensor, lines with two numbers are `right-heel:right-toe` values
 * [2019-03-03-running.log](2019-03-03-running.log) - some jogging round the events room, with just one foot instrumented
 * [2019-03-03-walking.log](2019-03-03-walking.log) - walking round the events room, with just one foot instrumented
 * [2019-03-16-pedalling.log](2019-03-16-pedalling.log) - pedalling on an exercise bike, with just one foot instrumented

