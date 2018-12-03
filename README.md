# C Mosquitto Example

## Requirements

One needs the Mosquitto library and headers to compile this source. Ubuntu instructions follow.


```
sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa
sudo apt-get update
sudo apt-get install mosquitto-dev
```

## Compiling

A simple Makefile is included. Just run `make` inside the project directory.

## Running

1. First, start a Mosquitto broker locally (there is no configuration file yet).


```
mosquitto &
```

2. Then, run a Mosquitto subscriber to test the publisher example.


```
mosquitto_sub -t topic
```

3. Finally, run the publisher example, which sends `0`'s for 10 seconds with 1 second delay to the local Mosquitto broker in the `topic` topic.

```
./bin/pub
```