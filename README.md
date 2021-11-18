## ESPHome custom components

My creation of a nilan component for ESPHome.

### Installation
Add this repository to your ESPHome config yaml:

```yaml
external_components:
  - source: github://Jopand/esphome_components@use_modbus_controller
    refresh: 0s
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
  id: modbus_id
  uart_id: uart_modbus
  
modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
 
external_components:
  - source: github://Jopand/esphome_components@use_modbus_controller
    refresh: 0s
 
nilan:

sensor:
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Inlet (after heater) temperature"
    id: nilan_inlet_temp_t7
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 207
    filters:
      - multiply: 0.01

number:
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Ventilation speed set"
    id: nilan_ventilation_speed_set
    min_value: 0
    max_value: 4
    step: 1.0
    address: 1003
    force_new_range: true

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Temperature set"
    id: nilan_target_temperature_set
    min_value: 15
    max_value: 30
    step: 1.0
    address: 1004
    force_new_range: true
    write_lambda: "payload = modbus_controller::float_to_payload(x*100, modbus_controller::SensorValueType::U_WORD); return x;"
    lambda: "return x*0.01;"

text_sensor:
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Operation mode"
    id: nilan_operation_mode
    register_type: read
    address: 1001
    lambda: !lambda |-
      uint16_t int_mode = (data[item->offset] << 8) + data[item->offset+1];
      ESP_LOGD("main","Parsed operation mode int : %d", int_mode);
      std::string mode_str;
      switch (int_mode) {
        case 0: mode_str = "Off"; break;
        case 1: mode_str = "Heat"; break;
        case 2: mode_str = "Cool"; break;
        case 3: mode_str = "Auto"; break;
        case 4: mode_str = "Service"; break;
        default: mode_str = "Unknown"; break;
      }
      return mode_str;

climate:
  - platform: nilan
    name: Nilan
    current_temp_sensor_id: nilan_inlet_temp_t7
    target_temp_sensor_id: nilan_target_temperature_set
    fan_speed_sensor_id: nilan_ventilation_speed_set
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
  id: modbus_id
  uart_id: uart_modbus
  
modbus_controller:
  id: nilan_modbus_controller
  address: 30
  modbus_id: modbus_id
  update_interval: 30s
 
external_components:
  - source: github://Jopand/esphome_components@use_modbus_controller
    refresh: 0s
  - source: github://martgras/esphome@test-multiplecoil
    components: ["modbus_controller"]
 
nilan:
 
sensor:
#########################
#### INPUT REGISTERS ####
#########################
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Bus version"
    id: nilan_bus_version
    accuracy_decimals: 0
    register_type: read
    address: 0

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Controller temperature"
    id: nilan_controller_temp_t0
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 200
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Fresh air intake temperature"
    id: nilan_intake_temp_t1
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 201
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Inlet (before heater) temperature"
    id: nilan_inlet_temp_t2
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 202
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Exhaust temperature"
    id: nilan_exhaust_temp_t3
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 203
    filters:
      - multiply: 0.01
      
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Outlet temperature"
    id: nilan_outlet_temp_t4
    unit_of_measurement: °C
    accuracy_decimals: 1
    register_type: read
    address: 204
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Condenser temperature"
    id: nilan_condenser_temp_t5
    unit_of_measurement: °C
    accuracy_decimals: 1
    register_type: read
    address: 205
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Evaporator temperature"
    id: nilan_evap_temp_t6
    unit_of_measurement: °C
    accuracy_decimals: 1
    register_type: read
    address: 206
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Inlet (after heater) temperature"
    id: nilan_inlet_temp_t7
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 207
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Outdoor temperature"
    id: nilan_outdoor_temp_t8
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 208
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Heating surface temperature"
    id: nilan_heater_temp_t9
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 209
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "External room temperature"
    id: nilan_extern_temp_t10
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 210
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Hot weater top temperature"
    id: nilan_top_temp_t11
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 211
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Hot water bottom temperature"
    id: nilan_bottom_temp_t12
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 212
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "EK return temperature"
    id: nilan_return_temp_t13
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 213
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "EK supply temperature"
    id: nilan_supply_temp_t14
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 214
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Room temperature"
    id: nilan_room_temp_t15
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 215
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "AUX temperature"
    id: nilan_aux_temp_t16
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 216
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Preheater temperature"
    id: nilan_preheat_temp_t17
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 217
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Pressure pibe temperature"
    id: nilan_prepibe_temp_t18
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: read
    address: 218
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Humidity"
    id: nilan_humidity
    unit_of_measurement: '%'
    accuracy_decimals: 1
    device_class: humidity
    register_type: read
    address: 221
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "CO2"
    id: nilan_co2_level
    unit_of_measurement: "ppm"
    icon: "mdi:molecule-co2"
    accuracy_decimals: 0
    register_type: read
    address: 222
    filters:
      - multiply: 0.01  

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Active alarms"
    id: nilan_active_alarms
    accuracy_decimals: 0
    register_type: read
    address: 400

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Heat exchange efficiency"
    id: nilan_heat_exchange_efficiency
    unit_of_measurement: '%'
    accuracy_decimals: 1
    register_type: read
    address: 1204
    filters:
      - multiply: 0.01
      
###########################
#### HOLDING REGISTERS ####
###########################
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Exhaust fan speed"
    id: nilan_exhaust_fan_speed
    unit_of_measurement: '%'
    icon: "mdi:percent"
    accuracy_decimals: 0
    register_type: holding
    address: 200
    filters:
      - multiply: 0.01
    
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Inlet fan speed"
    id: nilan_inlet_fan_speed
    unit_of_measurement: '%'
    icon: "mdi:percent"
    accuracy_decimals: 0
    register_type: holding
    address: 201
    filters:
      - multiply: 0.01

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Cooling set temperature"
    id: nilan_cooling_set_temp
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: holding
    address: 1200
    filters:
      - multiply: 0.01
      
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Min summer temperature"
    id: nilan_min_summer_temp
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: holding
    address: 1201
    filters:
      - multiply: 0.01 
  
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Min winter temperature"
    id: nilan_min_winter_temp
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: holding
    address: 1202
    filters:
      - multiply: 0.01 
  
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Max summer temperature"
    id: nilan_max_summer_temp
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: holding
    address: 1203
    filters:
      - multiply: 0.01 
  
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Max winter temperature"
    id: nilan_max_winter_temp
    unit_of_measurement: °C
    accuracy_decimals: 1
    device_class: temperature
    register_type: holding
    address: 1204
    filters:
      - multiply: 0.01 

text_sensor:
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Software version"
    id: nilan_version_info
    register_type: read
    register_count: 3
    response_size: 6
    address: 1
    lambda: !lambda |-
      float bus_version = nilan_bus_version->get_state();
      std::string output;
      
      if (bus_version == 8) {
        output = data[item->offset+1];
        output += data[item->offset+0];
        output += data[item->offset+3];
        output += data[item->offset+2];
        output += data[item->offset+5]; 
        output += data[item->offset+4];
      } else {  
        output = data[item->offset+0];
        output += data[item->offset+1];
        output += '.';
        output += data[item->offset+2];
        output += data[item->offset+3];
        output += '.';
        output += data[item->offset+4];
        output += data[item->offset+5];
      }
 
      ESP_LOGD("main","Parsed SW version: %s", output.c_str()); 
      return output;

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Actual user function"
    id: nilan_actual_user_function
    register_type: holding
    address: 600
    lambda: !lambda |-
      uint16_t int_user_function = (data[item->offset] << 8) + data[item->offset+1];
      ESP_LOGD("main","Parsed user function int : %d", int_user_function);
      std::string actual_user_function;
      switch(int_user_function) {
        case 0: { actual_user_function = "0 : None"; break; }
        case 1: { actual_user_function = "1 : Extend"; break; }
        case 2: { actual_user_function = "2 : Inlet"; break; }
        case 3: { actual_user_function = "3 : Exhaust"; break; }
        case 4: { actual_user_function = "4 : External heater offset"; break; }
        case 5: { actual_user_function = "5 : Ventilate"; break; }
        case 6: { actual_user_function = "6 : Cooker Hood"; break; }
      }
      return actual_user_function;

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Operation mode"
    id: nilan_operation_mode
    register_type: read
    address: 1001
    lambda: !lambda |-
      uint16_t int_mode = (data[item->offset] << 8) + data[item->offset+1];
      ESP_LOGD("main","Parsed operation mode int : %d", int_mode);
      std::string mode_str;
      switch (int_mode) {
        case 0: mode_str = "Off"; break;
        case 1: mode_str = "Heat"; break;
        case 2: mode_str = "Cool"; break;
        case 3: mode_str = "Auto"; break;
        case 4: mode_str = "Service"; break;
        default: mode_str = "Unknown"; break;
      }
      return mode_str;
      
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Control mode"
    id: nilan_control_state
    register_type: read
    address: 1002
    lambda: !lambda |-
      uint16_t int_mode = (data[item->offset] << 8) + data[item->offset+1];
      ESP_LOGD("main","Parsed operation mode int : %d", int_mode);
      std::string mode_str;
      switch (int_mode) {
        case 0:  mode_str = "Off"; break;
        case 1:  mode_str = "Shift"; break;
        case 2:  mode_str = "Stop"; break;
        case 3:  mode_str = "Start"; break;
        case 4:  mode_str = "Standby"; break;
        case 5:  mode_str = "Ventilation stop"; break;
        case 6:  mode_str = "Ventilation"; break;
        case 7:  mode_str = "Heating"; break;
        case 8:  mode_str = "Cooling"; break;
        case 9:  mode_str = "Hot water"; break;
        case 10: mode_str = "Legionella"; break;
        case 11: mode_str = "Cooling + hot water"; break;
        case 12: mode_str = "Central heating"; break;
        case 13: mode_str = "Defrost"; break;
        default: mode_str = "Unknown"; break;
      }
      return mode_str;
      
#######################################################################      
#### WARNING - IF THERE ARE ANY SPECIAL CHARACTERS ON THE DISPLAY, ####
#### ENABLING THESE SENSORS WILL CRASH THE PROGRAM WHEN PUBLISHING ####
#######################################################################
#  - platform: modbus_controller
#    modbus_controller_id: nilan_modbus_controller
#    name: "Display upper line"
#    id: nilan_display_upper_line
#    force_new_range: true
#    register_type: read
#    register_count: 4
#    response_size: 8
#    address: 2002
    
#  - platform: modbus_controller
#    modbus_controller_id: nilan_modbus_controller
#    name: "Display lower line"
#    id: nilan_display_lower_line
#    force_new_range: true
#    register_type: read
#    register_count: 4
#    response_size: 8
#    address: 2007
      
number:
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "User time set (minutes)"
    id: nilan_user_time_set
    value_type: U_WORD
    min_value: 1.0
    max_value: 480.0
    lambda: !lambda |-
      uint16_t hours = x/100; // 100 is 1 hour
      uint16_t minutes = ((uint16_t)x) % 100;
      return (hours*60) + minutes;
    write_lambda: !lambda |-
      uint16_t intVal = x;
      uint16_t firstDigit = (intVal / 60); // 1 hour is written as '100' and not '60'
      uint16_t lastDigits = (intVal % 60); // 1:30 hours is written as '130' and not '90'
      uint16_t result = (firstDigit*100) + lastDigits;
      payload = modbus_controller::float_to_payload(result, modbus_controller::SensorValueType::U_WORD);
      return x;
    step: 1.0
    address: 602
    force_new_range: true
    
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "User ventilation speed set"
    id: nilan_user_ventilation_speed_set
    min_value: 0.0
    max_value: 4.0
    step: 1.0
    address: 603
    force_new_range: true
  
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "User temperature set"
    id: nilan_user_temperature_set
    min_value: 15.0
    max_value: 30.0
    step: 1.0
    address: 604
    force_new_range: true

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Ventilation speed set"
    id: nilan_ventilation_speed_set
    min_value: 0
    max_value: 4
    step: 1.0
    address: 1003
    force_new_range: true

  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "Temperature set"
    id: nilan_target_temperature_set
    min_value: 15
    max_value: 30
    step: 1.0
    address: 1004
    force_new_range: true
    write_lambda: "payload = modbus_controller::float_to_payload(x*100, modbus_controller::SensorValueType::U_WORD); return x;"
    lambda: "return x*0.01;"

switch:
  - platform: modbus_controller
    modbus_controller_id: nilan_modbus_controller
    name: "On/Off state"
    id: nilan_on_off_state
    register_type: holding
    bitmask: 1
    force_new_range: true
    use_write_multiple: true
    address: 100
    
climate:
  - platform: nilan
    name: Nilan
    current_temp_sensor_id: nilan_inlet_temp_t7
    target_temp_sensor_id: nilan_target_temperature_set
    fan_speed_sensor_id: nilan_ventilation_speed_set
    mode_text_sensor_id: nilan_operation_mode
```
