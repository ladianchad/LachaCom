## COMMOM
| name | type | default value | description |
| ------ | ---------------| ----------------- | ----------------- |
|  USE_SYS_POLLING | `bool` | `false` | if true, create thread for syspolling and can set polling callback. |
| AUTO_START | `bool` | `true` | open file descriptor after init. |
| AUTO_RECONNECT | `bool` | `true` | reconnect when file descriptor closed by error. |

## UART

| name | type | default value | description |
| ------ | ---------------| ----------------- | ----------------- |
| BAUDRATE | `unsigned long` | `115200` | serial baudrate. |
| PROT | `string` | `/dev/ttyUSB0` | serial port. |