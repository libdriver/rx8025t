### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

GPIO Pin: INT GPIO17.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(rx8025t REQUIRED)
```

### 3. RX8025T

#### 3.1 Command Instruction

1. Show rx8025t chip and driver information.

   ```shell
   rx8025t (-i | --information)
   ```

2. Show rx8025t help.

   ```shell
   rx8025t (-h | --help)
   ```

3. Show rx8025t pin connections of the current board.

   ```shell
   rx8025t (-p | --port)
   ```

4. Run rx8025t register test.

   ```shell
   rx8025t (-t reg | --test=reg)
   ```

5. Run rx8025t timer test.

   ```shell
   rx8025t (-t timer | --test=timer)
   ```

6. Run rx8025t alarm test.

   ```shell
   rx8025t (-t alarm | --test=alarm)
   ```

7. Run rx8025t basic set time function, time is the unix timestamp.

   ```shell
   rx8025t (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
   ```

8. Run rx8025t basic get time function.

   ```shell
   rx8025t (-e basic-get-time | --example=basic-get-time)
   ```

9. Run rx8025t basic set ram function, hex is the set data and it is hexadecimal.

    ```shell
    rx8025t (-e basic-set-ram | --example=basic-set-ram) --data=<hex>
    ```

10. Run rx8025t basic get ram function.

    ```shell
    rx8025t (-e basic-get-ram | --example=basic-get-ram)
    ```

11. Run rx8025t timer set time function, time is the unix timestamp.

       ```shell
       rx8025t (-e timer-set-time | --example=timer-set-time) --timestamp=<time>
       ```

12. Run rx8025t timer get time function.

       ```shell
       rx8025t (-e timer-get-time | --example=timer-get-time)
       ```

13. Run rx8025t timer set ram function, hex is the set data and it is hexadecimal.

     ```shell
     rx8025t (-e timer-set-ram | --example=timer-set-ram) --data=<hex>
     ```

14. Run rx8025t timer get ram function.

     ```shell
     rx8025t (-e timer-get-ram | --example=timer-get-ram)
     ```

15. Run rx8025t timer config function, num is the counter number.

     ```shell
     rx8025t (-e timer-config | --example=timer-config) --clock=<4096HZ | 64HZ | 1HZ | MINUTE> --counter=<num>
     ```

16. Run rx8025t alarm set time function, time is the unix timestamp.

     ```shell
     rx8025t (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>
     ```

17. Run rx8025t alarm get time function.

     ```shell
     rx8025t (-e alarm-get-time | --example=alarm-get-time)
     ```

18. Run rx8025t alarm set ram function, hex is the set data and it is hexadecimal.

     ```shell
     rx8025t (-e alarm-set-ram | --example=alarm-set-ram) --data=<hex>
     ```
    
19. Run rx8025t alarm get ram function.

     ```shell
     rx8025t (-e alarm-get-ram | --example=alarm-get-ram)
     ```
    
20. Run rx8025t alarm config function, num is the minute, hour, date or weekday number, second is the timeout in second.

     ```shell
     rx8025t (-e alarm-config | --example=alarm-config) --minute=<num> --hour=<num> --date=<num> --weekday=<num> --date-week=<DATE | WEEK> --minute-mask=<true | false> --hour-mask=<true | false> --date-weekday-mask=<true | false> --timeout=<second>
     ```

#### 3.2 Command Example

```shell
./rx8025t -i

rx8025t: chip is EPSON RX8025T.
rx8025t: manufacturer is EPSON.
rx8025t: interface is IIC.
rx8025t: driver version is 1.0.
rx8025t: min supply voltage is 1.6V.
rx8025t: max supply voltage is 5.5V.
rx8025t: max current is 0.90mA.
rx8025t: max temperature is 85.0C.
rx8025t: min temperature is -40.0C.
```

```shell
./rx8025t -p

rx8025t: SCL connected to GPIO3(BCM).
rx8025t: SDA connected to GPIO2(BCM).
rx8025t: INT connected to GPIO17(BCM).
```

```shell
./rx8025t -t reg

rx8025t: chip is EPSON RX8025T.
rx8025t: manufacturer is EPSON.
rx8025t: interface is IIC.
rx8025t: driver version is 1.0.
rx8025t: min supply voltage is 1.6V.
rx8025t: max supply voltage is 5.5V.
rx8025t: max current is 0.90mA.
rx8025t: max temperature is 85.0C.
rx8025t: min temperature is -40.0C.
rx8025t: start register test.
rx8025t: rx8025t_start/rx8025t_stop test.
rx8025t: stop.
rx8025t: check stop ok.
rx8025t: start.
rx8025t: check start ok.
rx8025t: rx8025t_set_time/rx8025t_get_time test.
rx8025t: set time 2083-11-18 17:55:46 6.
rx8025t: check time ok.
rx8025t: rx8025t_set_temperature_compensation_interval/rx8025t_get_temperature_compensation_interval test.
rx8025t: set temperature compensation interval 0.5s.
rx8025t: check temperature compensation interval ok.
rx8025t: set temperature compensation interval 2.0s.
rx8025t: check temperature compensation interval ok.
rx8025t: set temperature compensation interval 10s.
rx8025t: check temperature compensation interval ok.
rx8025t: set temperature compensation interval 30s.
rx8025t: check temperature compensation interval ok.
rx8025t: rx8025t_set_update_interrupt/rx8025t_get_update_interrupt test.
rx8025t: enable update interrupt.
rx8025t: check update interrupt ok.
rx8025t: disable update interrupt.
rx8025t: check update interrupt ok.
rx8025t: rx8025t_set_timer_interrupt/rx8025t_get_timer_interrupt test.
rx8025t: enable timer interrupt.
rx8025t: check timer interrupt ok.
rx8025t: disable timer interrupt.
rx8025t: check timer interrupt ok.
rx8025t: rx8025t_set_alarm_interrupt/rx8025t_get_alarm_interrupt test.
rx8025t: enable alarm interrupt.
rx8025t: check alarm interrupt ok.
rx8025t: disable alarm interrupt.
rx8025t: check alarm interrupt ok.
rx8025t: rx8025t_set_test/rx8025t_get_test test.
rx8025t: enable test.
rx8025t: check test ok.
rx8025t: disable test.
rx8025t: check test ok.
rx8025t: rx8025t_set_test/rx8025t_get_test test.
rx8025t: set alarm week.
rx8025t: check alarm day week ok.
rx8025t: set alarm day.
rx8025t: check alarm day week ok.
rx8025t: rx8025t_set_update_select/rx8025t_get_update_select test.
rx8025t: set update select second.
rx8025t: check update select ok.
rx8025t: set update select minute.
rx8025t: check update select ok.
rx8025t: rx8025t_set_timer/rx8025t_get_timer test.
rx8025t: enable timer.
rx8025t: check timer ok.
rx8025t: disable timer.
rx8025t: check timer ok.
rx8025t: rx8025t_set_square_wave_frequency/rx8025t_get_square_wave_frequency test.
rx8025t: set square wave frequency 1hz.
rx8025t: check square wave frequency ok.
rx8025t: set square wave frequency 1024hz.
rx8025t: check square wave frequency ok.
rx8025t: set square wave frequency 32768Hz.
rx8025t: check square wave frequency ok.
rx8025t: rx8025t_set_timer_clock/rx8025t_get_timer_clock test.
rx8025t: set timer clock 4096hz.
rx8025t: check timer clock ok.
rx8025t: set timer clock 64hz.
rx8025t: check timer clock ok.
rx8025t: set timer clock 1hz.
rx8025t: check timer clock ok.
rx8025t: set timer clock 1 minute.
rx8025t: check timer clock ok.
rx8025t: rx8025t_set_timer_counter/rx8025t_get_timer_counter test.
rx8025t: set timer counter 3747.
rx8025t: check timer counter ok.
rx8025t: rx8025t_set_ram/rx8025t_get_ram test.
rx8025t: set ram 249.
rx8025t: check ram ok.
rx8025t: rx8025t_set_minute_alarm/rx8025t_get_minute_alarm test.
rx8025t: set minute alarm 1.
rx8025t: check minute alarm ok.
rx8025t: rx8025t_set_hour_alarm/rx8025t_get_hour_alarm test.
rx8025t: set hour alarm 2.
rx8025t: check hour alarm ok.
rx8025t: rx8025t_set_date_alarm/rx8025t_get_date_alarm test.
rx8025t: set date alarm 8.
rx8025t: check date alarm ok.
rx8025t: rx8025t_set_week_alarm/rx8025t_get_week_alarm test.
rx8025t: set week alarm 7.
rx8025t: check week alarm ok.
rx8025t: rx8025t_clear_flag test.
rx8025t: check clear flag ok.
rx8025t: rx8025t_get_flag test.
rx8025t: flag is 0x00.
rx8025t: rx8025t_set_minute_alarm_mask/rx8025t_get_minute_alarm_mask test.
rx8025t: enable minute alarm mask.
rx8025t: check minute alarm mask ok.
rx8025t: disable minute alarm mask.
rx8025t: check minute alarm mask ok.
rx8025t: rx8025t_set_hour_alarm_mask/rx8025t_get_hour_alarm_mask test.
rx8025t: enable hour alarm mask.
rx8025t: check hour alarm mask ok.
rx8025t: disable hour alarm mask.
rx8025t: check hour alarm mask ok.
rx8025t: rx8025t_set_week_day_alarm_mask/rx8025t_get_week_day_alarm_mask test.
rx8025t: enable week day alarm mask.
rx8025t: check week day alarm mask ok.
rx8025t: disable week day alarm mask.
rx8025t: check week day alarm mask ok.
rx8025t: finish register test.
```

```shell
./rx8025t -t timer

rx8025t: chip is EPSON RX8025T.
rx8025t: manufacturer is EPSON.
rx8025t: interface is IIC.
rx8025t: driver version is 1.0.
rx8025t: min supply voltage is 1.6V.
rx8025t: max supply voltage is 5.5V.
rx8025t: max current is 0.90mA.
rx8025t: max temperature is 85.0C.
rx8025t: min temperature is -40.0C.
rx8025t: start timer test.
rx8025t: 10s start.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq timer.
rx8025t: end.
rx8025t: finish timer test.
```

```shell
./rx8025t -t alarm

rx8025t: chip is EPSON RX8025T.
rx8025t: manufacturer is EPSON.
rx8025t: interface is IIC.
rx8025t: driver version is 1.0.
rx8025t: min supply voltage is 1.6V.
rx8025t: max supply voltage is 5.5V.
rx8025t: max current is 0.90mA.
rx8025t: max temperature is 85.0C.
rx8025t: min temperature is -40.0C.
rx8025t: start alarm test.
rx8025t: irq update.
rx8025t: set time 2073-10-14 20:00:00 2.
rx8025t: 1 min alarm start.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq update.
rx8025t: irq alarm.
rx8025t: end.
rx8025t: finish alarm test.
```

```shell
./rx8025t -e basic-set-time --timestamp=1777305000

rx8025t: set timestamp 1777305000.
```

```shell
./rx8025t -e basic-get-time

rx8025t: 2026-04-27 23:50:17 1.
```

```shell
./rx8025t -e basic-set-ram --data=0x08

rx8025t: ram write 0x08.
```

```shell
./rx8025t -e basic-get-ram

rx8025t: ram read 0x08.
```

```shell
./rx8025t -e timer-set-time --timestamp=1777305000

rx8025t: set timestamp 1777305000.
```

```shell
./rx8025t -e timer-get-time

rx8025t: 2026-04-27 23:50:15 1.
```

```shell
./rx8025t -e timer-set-ram --data=0x21

rx8025t: ram write 0x21.
```

```shell
./rx8025t -e timer-get-ram

rx8025t: ram read 0x21.
```

```shell
./rx8025t -e timer-config --clock=1HZ --counter=60

rx8025t: set clock 1hz, counter 60.
rx8025t: start timer.
rx8025t: irq timer.
rx8025t: end timer.
```

```shell
./rx8025t -e alarm-set-time --timestamp=1777305000

rx8025t: set timestamp 1777305000.
```

```shell
./rx8025t -e alarm-get-time

rx8025t: 2026-04-27 23:50:12 1.
```

```shell
./rx8025t -e alarm-set-ram --data=0x56

rx8025t: ram write 0x56.
```

```shell
./rx8025t -e alarm-get-ram

rx8025t: ram read 0x56.
```

```shell
./rx8025t -e alarm-config --minute=0 --hour=1 --date=2 --weekday=3 --date-week=DATE --minute-mask=true --hour-mask=true --date-weekday-mask=true --timeout=120

rx8025t: alarm minute is 0.
rx8025t: alarm hour is 1.
rx8025t: alarm date is 2.
rx8025t: alarm weekday is 3.
rx8025t: alarm date mode.
rx8025t: alarm minute mask is true.
rx8025t: alarm hour mask is true.
rx8025t: alarm date weekday mask is true.
rx8025t: start alarm.
rx8025t: irq alarm.
rx8025t: end alarm.
```

```shell
./rx8025t -h

Usage:
  rx8025t (-i | --information)
  rx8025t (-h | --help)
  rx8025t (-p | --port)
  rx8025t (-t reg | --test=reg)
  rx8025t (-t timer | --test=timer)
  rx8025t (-t alarm | --test=alarm)
  rx8025t (-e basic-set-time | --example=basic-set-time) --timestamp=<time>
  rx8025t (-e basic-get-time | --example=basic-get-time)
  rx8025t (-e basic-set-ram | --example=basic-set-ram) --data=<hex>
  rx8025t (-e basic-get-ram | --example=basic-get-ram)
  rx8025t (-e timer-set-time | --example=timer-set-time) --timestamp=<time>
  rx8025t (-e timer-get-time | --example=timer-get-time)
  rx8025t (-e timer-set-ram | --example=timer-set-ram) --data=<hex>
  rx8025t (-e timer-get-ram | --example=timer-get-ram)
  rx8025t (-e timer-config | --example=timer-config) --clock=<4096HZ | 64HZ | 1HZ | MINUTE> --counter=<num>
  rx8025t (-e alarm-set-time | --example=alarm-set-time) --timestamp=<time>
  rx8025t (-e alarm-get-time | --example=alarm-get-time)
  rx8025t (-e alarm-set-ram | --example=alarm-set-ram) --data=<hex>
  rx8025t (-e alarm-get-ram | --example=alarm-get-ram)
  rx8025t (-e alarm-config | --example=alarm-config) --minute=<num> --hour=<num> --date=<num> --weekday=<num>
          --date-week=<DATE | WEEK> --minute-mask=<true | false> --hour-mask=<true | false> --date-weekday-mask=<true | false>
          --timeout=<second>

Options:
      --clock=<4096HZ | 64HZ | 1HZ | MINUTE>
                                  Set the timer clock.([default: 4096HZ])
      --counter=<num>             Set the timer counter.([default: 100])
      --data=<hex>                Set ram data and it is hexadecimal.([default: 0x00])
      --date=<num>                Set the alarm date.([default: 0])
      --date-week=<DATE | WEEK>   Set the alarm date or weekday.([default: DATE])
      --date-weekday-mask=<true | false>
                                  Set the alarm date or weekday mask.([default: true])
  -e <basic-set-time | basic-get-time | basic-set-ram | basic-get-ram | timer-set-time | timer-get-time |
      timer-set-ram | timer-get-ram | timer-config | alarm-set-time | alarm-get-time |
      alarm-set-ram | alarm-get-ram | alarm-config>, --example=<basic-set-time | basic-get-time | basic-set-ram |
      basic-get-ram | timer-set-time | timer-get-time | timer-set-ram | timer-get-ram | timer-config | alarm-set-time |
      alarm-get-time | alarm-set-ram | alarm-get-ram | alarm-config>
                                  Run the driver example.
  -h, --help                      Show the help.
      --hour=<num>                Set the alarm hour.([default: 0])
      --hour-mask=<true | false>  Set the alarm hour mask.([default: true])
  -i, --information               Show the chip information.
      --minute=<num>              Set the alarm minute.([default: 0])
      --minute-mask=<true | false>
                                  Set the alarm minute mask.([default: true])
  -p, --port                      Display the pin connections of the current board.
  -t <reg | timer | alarm>, --test=<reg | timer | alarm>
                                  Run the driver test.
      --timestamp=<time>          Set the the unix timestamp.
      --weekday=<num>             Set the alarm weekday.([default: 1])
      --timeout=<second>          Set the timeout in second.([default: 60])
```
