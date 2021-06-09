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
  update_interval: 25s
 
sensor:
  - platform: nilan
    temp_t0:
      name: "Controller temperature"
      id: nilan_controller_temp_t0
    temp_t3:
      name: "Exhaust temperature"
      id: nilan_exhaust_temp_t3
    temp_t4:
      name: "Outlet temperature"
      id: nilan_outlet_temp_t4
    temp_t7:
      name: "Inlet temperature"
      id: nilan_inlet_temp_t7
    temp_t8:
      name: "Outdoor temperature"
      id: nilan_outdoor_temp_t8
    temp_t15:
      name: "Room temperature"
      id: nilan_room_temp_t15
    measured_humidity:
      name: "Humidity"
      id: nilan_humidity
    active_alarms:
      name: "Active alarms"
      id: nilan_active_alarms
    cool_target_temp:
      name: "Cooling set temperature"
      id: nilan_cooling_set_temp
    min_summer_temp:
      name: "Min summer temperature"
      id: nilan_min_summer_temp
    max_summer_temp:
      name: "Max summer temperature"
      id: nilan_max_summer_temp
    min_winter_temp:
      name: "Min winter temperature"
      id: nilan_min_winter_temp
    max_winter_temp:
      name: "Max winter temperature"
      id: nilan_max_winter_temp
    heat_exchange_efficiency:
      name: "Heat exchange efficiency"
      id: nilan_heat_exchange_efficiency
    inlet_fan:
      name: "Inlet fan speed"
      id: nilan_inlet_fan_speed
    exhaust_fan:
      name: "Exhaust fan speed"
      id: nilan_exhaust_fan_speed
    co2_ppm_level:
      name: "CO2"
      id: nilan_co2_level
    target_temp:
      name: "Target temperature"
      id: nilan_target_temperature
    ventilation_speed:
      name: "Ventilation speed"
      id: nilan_ventilation_speed
      
binary_sensor:
  - platform: nilan
    is_summer:
      name: "Sommerdrift"
      id: nilan_is_summer
    filter_ok:
      name: "Filter OK"
      id: nilan_filter_ok
    on_off_state:
      name: "On/Off tilstand"
      id: nilan_on_off_state
    bypass_on_off:
      name: "Bypass On"
      id: nilan_bypass_on
    door_open:
      name: "Door open"
      id: nilan_door_open

text_sensor:
  - platform: nilan
    operation_mode:
      name: "Operation mode"
      id: nilan_operation_mode
    control_state:
      name: "Control mode"
      id: nilan_control_state
    version_info:
      name: "Software version"
      id: nilan_version_info
    
climate:
  - platform: nilan
    name: Nilan
    current_temp_sensor_id: nilan_inlet_temp_t7
    target_temp_sensor_id: nilan_target_temperature
    fan_speed_sensor_id: nilan_ventilation_speed
```

TO-DO:
1. Add support for more sensors according to the description (https://fr.nilan.dk/Files//Filer/Download/French/Documentation/Guide%20de%20montage/Modbus%20CTS%20602/MODBUS_CTS-602_2.30_Installation-and-user-guide.pdf)

Kudos:
A bit thanks to ssieb from the ESPHome Discord community for assisting i making this happen! https://github.com/ssieb
