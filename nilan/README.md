# Nilan air circulation modbus interface

!!! Currently this compononet will only compile wit DEV version of ESPHome !!!

A configured uart component and a modbus component is required.

Example minimal required config:
```yaml
uart:
  rx_pin: GPIO16
  tx_pin: GPIO17
  parity: EVEN
  baud_rate: 19200
  id: uart_modbus
  stop_bits: 1
  
modbus:
  uart_id: uart_modbus
 
nilan:
  address: 30
  update_interval: 30s
 
sensor:
  - platform: nilan
    temp_t1:
      name: "nilan_temp_t1"
      id: nilan_temp_t1
    target_temp:
      name: "nilan_target_temp"
    speed_mode:
      name: "nilan_speed_mode"

climate:
  - platform: nilan
    name: Nilan
    sensor: controller_temp_t0
```

List of optional sensors:
- temp_t3:
- temp_t4:
- temp_t7:
- temp_t8:
- temp_t15:
- measured_humidity:
- active_alarms:
- speed_mode:
- cool_target_temp:
- min_summer_temp:
- max_summer_temp:
- min_winter_temp:
- max_winter_temp:

TO-DO:
1. Add support for more sensors according to the description (https://fr.nilan.dk/Files//Filer/Download/French/Documentation/Guide%20de%20montage/Modbus%20CTS%20602/MODBUS_CTS-602_2.30_Installation-and-user-guide.pdf)
2. Sometimes sensors get some high values (humidity can be 600). This needs to be looked at
3. Code should be better structured

Kudos:
A bit thanks to ssieb from the ESPHome Discord community for assisting i making this happen! https://github.com/ssieb
