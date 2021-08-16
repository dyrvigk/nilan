## ESPHome custom components

My creation of a nilan component for ESPHome.

### Installation
Add this repository to your ESPHome config yaml:

```yaml
external_components:
  - source: github://Jopand/esphome_components
```

### Example of minimal configuration yaml
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
 
external_components:
  - source: github://Jopand/esphome_components
 
nilan:
  address: 30
  update_interval: 30s

sensor:
  - platform: nilan
    temp_t7:
      name: "Inlet temperature"
      id: nilan_inlet_temp_t7
    target_temp:
      name: "Target temperature"
      id: nilan_target_temperature
    ventilation_speed:
      name: "Ventilation speed"
      id: nilan_ventilation_speed

binary_sensor:

text_sensor:
  - platform: nilan
    operation_mode:
      name: "Operation mode"
      id: nilan_operation_mode

climate:
  - platform: nilan
    name: Nilan
    current_temp_sensor_id: nilan_inlet_temp_t7
    target_temp_sensor_id: nilan_target_temperature
    fan_speed_sensor_id: nilan_ventilation_speed
    mode_text_sensor_id: nilan_operation_mode
```

### Example of a yaml configuration with all available sensors
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

external_components:
  - source: github://Jopand/esphome_components
 
nilan:
  address: 30
  update_interval: 30s

sensor:
  - platform: nilan
    temp_t0:
      name: "Controller temperature"
      id: nilan_controller_temp_t0
    temp_t1:
      name: "Fresh air intake temperature"
      id: nilan_intake_temp_t1
    temp_t2:
      name: "Inlet (before heater) temperature"
      id: nilan_inlet_temp_t2
    temp_t3:
      name: "Exhaust temperature"
      id: nilan_exhaust_temp_t3
    temp_t4:
      name: "Outlet temperature"
      id: nilan_outlet_temp_t4
    temp_t5:
      name: "Condenser temperature"
      id: nilan_condenser_temp_t5
    temp_t6:
      name: "Evaporator temperature"
      id: nilan_evap_temp_t6
    temp_t7:
      name: "Inlet (after heater) temperature"
      id: nilan_inlet_temp_t7
    temp_t8:
      name: "Outdoor temperature"
      id: nilan_outdoor_temp_t8
    temp_t9:
      name: "Heating surface temperature"
      id: nilan_heater_temp_t9
    temp_t10:
      name: "External room temperature"
      id: nilan_extern_temp_t10
    temp_t11:
      name: "Hot weater top temperature"
      id: nilan_top_temp_t11
    temp_t12:
      name: "Hot water bottom temperature"
      id: nilan_bottom_temp_t12
    temp_t13:
      name: "EK return temperature"
      id: nilan_return_temp_t13
    temp_t14:
      name: "EK supply temperature"
      id: nilan_supply_temp_t14
    temp_t15:
      name: "Room temperature"
      id: nilan_room_temp_t15
    temp_t16:
      name: "AUX temperature"
      id: nilan_aux_temp_t16
    temp_t17:
      name: "Preheater temperature"
      id: nilan_preheat_temp_t17
    temp_t18:
      name: "Pressure pibe temperature"
      id: nilan_prepibe_temp_t18
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
      name: "Bypass on"
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
    mode_text_sensor_id: nilan_operation_mode
```
