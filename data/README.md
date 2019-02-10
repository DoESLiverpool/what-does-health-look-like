# Sensor Data Recordings

This folder holds some recordings of data from experiments made while running the [sensor_mqtt_publish](../sensor_mqtt_publish) code.

Data captured with:
```
  mosquitto_sub -h broker.mqtt-dashboard.com -t "mcqn/#" | tee NAME-OF-LOG-FILE.log
```

## Recordings

 * [2019-02-09-tapdancing.log](2019-02-09-tapdancing.log) - initial tests with [Sole Rebel Tap](http://solerebeltap.co.uk/).  Lines with a single number are from the left-toe sensor, lines with two numbers are `right-heel:right-toe` values
