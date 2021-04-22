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
    temp_t0:
      name: "Controller temperature"
      id: nilan_controller_temp_t0
    temp_t3: #Optional
      name: "Exhaust temperature"
      id: nilan_exhaust_temp_t3
    temp_t4: #Optional
      name: "Outlet temperature"
      id: nilan_outlet_temp_t4
    temp_t7: #Optional
      name: "Inlet temperature"
      id: nilan_inlet_temp_t7
    temp_t8: #Optional
      name: "Outdoor temperature"
      id: nilan_outdoor_temp_t8
    temp_t15: #Optional
      name: "Room temperature"
      id: nilan_room_temp_t15
    measured_humidity: #Optional
      name: "Humidity"
      id: nilan_humidity
    active_alarms: #Optional
      name: "Active alarms"
      id: nilan_active_alarms
    speed_mode: #Optional
      name: "Speed mode"
      id: nilan_speed_mode
    cool_target_temp: #Optional
      name: "Cooling set temperature"
      id: nilan_cooling_set_temp
    min_summer_temp: #Optional
      name: "Min summer temperature"
      id: nilan_min_summer_temp
    max_summer_temp: #Optional
      name: "Max summer temperature"
      id: nilan_max_summer_temp
    min_winter_temp: #Optional
      name: "Min winter temperature"
      id: nilan_min_winter_temp
    max_winter_temp: #Optional
      name: "Max winter temperature"
      id: nilan_max_winter_temp
    heat_exchange_efficiency: #Optional
      name: "Krydsveksler effektivitet"
      id: nilan_heat_exchange_efficiency
    control_state: #Optional
      name: "Kontroltilstand"
      id: nilan_control_state
      
binary_sensor:
  - platform: nilan
    is_summer: #Optional
      name: "Sommerdrift"
      id: nilan_is_summer
    filter_ok: #Optional
      name: "Filter OK"
      id: nilan_filter_ok
    on_off_state: #Optional
      name: "On/Off tilstand"
      id: nilan_on_off_state
    bypass_on_off: #Optional
      name: "Bypass On"
      id: nilan_bypass_on
    door_open: #Optional
      name: "Door open"
      id: nilan_door_open

climate:
  - platform: nilan
    name: Nilan
    sensor: nilan_controller_temp_t0
    
```

TO-DO:
1. Add support for more sensors according to the description (https://fr.nilan.dk/Files//Filer/Download/French/Documentation/Guide%20de%20montage/Modbus%20CTS%20602/MODBUS_CTS-602_2.30_Installation-and-user-guide.pdf)
3. Add support for TextSensors struggling with Scheme and registering in to_code method

Kudos:
A bit thanks to ssieb from the ESPHome Discord community for assisting i making this happen! https://github.com/ssieb
