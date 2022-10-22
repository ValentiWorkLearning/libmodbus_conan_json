# libmodbus_conan_json

A small utility for creating read\write scenarios for Modbus RTU via json spec. The scenario file structure is given below:

```json
{
  "scenarios": [
    {
      "operation": "read",
      "repitions": 2,
      "title": "read telematics data",
      "registers": [
        [ "0x200F" ],
        [ "0x2010" ],
        [ "0x2011" ],
        [ "0x2012" ],
        [ "0x2013" ]
      ],
      "delay": 2000
    }
  ]
}
```

## Available options list:
```shell
.\modbus_test.exe --help
Allowed options:
  --help                                produce help message
  --serial_path arg (=COM8)             serial device which will be used for
                                        connection
  --debug_mode arg (=0)                 set libmodbus debug mode enabled
  --baudrate arg (=9600)                connection baudrate
  --slave_id arg (=1)                   modbus slave id
  --scenario_path arg (="scenario.json")
                                        path to scenarios file
```

## Possible output:
```shell
PS > .\modbus_test.exe --scenario_path=".\read_registers.json"
[2022-10-23 02:04:37.718] [info] Seial connection path is: COM8
[2022-10-23 02:04:37.718] [info] connection baudrate is: 9600
[2022-10-23 02:04:37.718] [info] Slave ID is: 1
[2022-10-23 02:04:37.718] [info] Debug mode is:false
[2022-10-23 02:04:37.718] [info] [modbus]Created RTU modbus with settings
[2022-10-23 02:04:37.719] [info] [modbus]Serial device path:COM8
[2022-10-23 02:04:37.719] [info] [modbus]Serial baudrate:9600
[2022-10-23 02:04:37.719] [info] [modbus]Modbus slave id:1
[2022-10-23 02:04:37.725] [info] [modbus] Modbus client has connected successfully
[2022-10-23 02:04:37.726] [warning] ***********SCENARIO read telematics data BEGIN********************
[2022-10-23 02:04:37.746] [info] register 0x200f value: 19
[2022-10-23 02:04:37.767] [info] register 0x2010 value: 18
[2022-10-23 02:04:37.787] [info] register 0x2011 value: 12
[2022-10-23 02:04:38.526] [info] register 0x2012 value: 1
[2022-10-23 02:04:38.547] [info] register 0x2013 value: 0
[2022-10-23 02:04:38.568] [info] register 0x200f value: 19
[2022-10-23 02:04:38.588] [info] register 0x2010 value: 18
[2022-10-23 02:04:38.609] [info] register 0x2011 value: 12
[2022-10-23 02:04:38.629] [info] register 0x2012 value: 1
[2022-10-23 02:04:38.650] [info] register 0x2013 value: 0
[2022-10-23 02:04:38.650] [warning] ***********SCENARIO read telematics data END**********************
PS >

```
