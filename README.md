## ESPHome custom components

My creation of a nilan component for ESPHome.

### Example of minimal configuration yaml
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: develop
    files: [components/nilan/basic.yaml]
    refresh: 0s

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
```


### Example of a Nilan light compatible configuration yaml
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: develop
    files: [components/nilan/light.yaml]
    refresh: 0s

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
```

### Example of a Nilan Comfort compatible configuration yaml
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: develop
    files: [components/nilan/comfort.yaml]
    refresh: 0s

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
```


### Example of a yaml configuration with sensors working for all models
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: develop
    files: [components/nilan/light.yaml]
    refresh: 0s

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
```

### Example of a Nilan Comfort compatible configuration yaml
```yaml
packages:
  remote_package:
    url: https://github.com/Jopand/esphome_components
    ref: develop
    files: [components/nilan/all.yaml]
    refresh: 0s

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
```
