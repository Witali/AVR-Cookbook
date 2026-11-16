# Bootloaders and protocols

[Русская версия](bootloaders_ru.md)

## 1. Share identical tails and combine passes

A historical ATmega328P optiboot build shared the end-of-packet check (`verify_eop`) and success response across several commands. This saved 4 bytes as part of the change from 368 to 358 bytes. The strict ATtiny88 MicroNucleus bootloader combined the response payload copy with the CRC calculation and saved 24 bytes at that stage. It publishes the buffer after the data and CRC are ready. Every path into a shared tail must agree on the entry state, when it can publish, the response format, and the state changes. Test retries and resets, where differences can first appear.

## 2. Account for placement and Flash geometry

Track the code size, the reserved region, and the Flash available to the application separately. The smallest hardware boot section on ATmega328P is 256 words, or 512 bytes. The application boundary stays at the same address when the boot code goes from 368 to 358 bytes within that section. On ATtiny with Virtual Boot, optiboot tries whole erase groups and relinks for each start address. The integration report gives code size and reservation as 549/576 bytes on ATtiny84/85, 505/512 bytes on ATtiny841, and 501/512 bytes on ATtiny1634. These figures come from builds and models.

MicroNucleus aligned the boundary so its low byte was zero. That simplified the Flash limit check and saved 4 bytes in the corresponding profile. Comparing only the high byte requires an aligned boundary and proof that the check covers the entire write range. Optiboot checks both bytes where the boundary is not aligned to 256 bytes. Test the last allowed block, a write starting at the boundary, a write crossing it, arithmetic overflow, and the space reserved for vectors.

## 3. Preserve entry conditions and hardware timing windows

A historical ATmega328P optiboot build fit in 350 bytes after removing a stub for the old SPM API and relying on UCSR0C resetting to 8N1. The build that fully initialized the UART occupied 358 bytes. The model reproduces a failure when an application changes the UART settings and then jumps directly to the smaller build. Document whether entry occurs through hardware reset, the watchdog, or an application jump. Check the linked listing against the device datasheet to ensure that writes to WDT, EEPROM, and SPM stay within their required timing windows.

## 4. Check worst-case latency before removing polls

MicroNucleus kept the USB polling points because removing them pushed receiver entry latency beyond the tested bound of 38 cycles. Some test packets still decoded. In a separate CRC16 experiment, the receiver used 11 cycles per byte with a 512-byte table. Outside the receiver, a 32-byte table needed 35 cycles per byte, a 16-byte table needed 19, and the table-free variant needed 23, excluding page setup and call overhead. The receive loop had 14 regular cycle slots available for CRC work, so it kept the larger table. The alternatives were measured as prototypes without building and testing a complete new USB image.

## 5. Use explicit ownership to eliminate repeated checks

In the Flash-only MicroNucleus experiment, only an accepted, complete SETUP packet establishes ownership. The IN path then uses the prepared response snapshot and can omit the repeated checks of old state. The patch saved 26 bytes in that experiment. Receive qualification was still open. Test ownership through retries, cancellation, malformed packets, resets, minimum inter-packet gaps, and early reads.

## 6. Keep safety checks during optimization

An archived 1,024-byte MicroNucleus image could start SPM before the status ACK finished. The project rejected the image even though it met the size target. When changing this code, test timeouts, malformed packets, retries of the last command, writes suppressed after an error, and the first transactions after reset. Check separately that the application region keeps its contents and that the application starts.

Board verification applies to specific images. The optiboot reports cover Middle and Full on an ATmega328PB Nano. ATtiny Virtual Boot has build and model results. In the strict EEPROM-compatible MicroNucleus line, the earlier 1,274-byte image was verified over USB on a board and the 1,262-byte image was verified in a model. The Flash-only experiments are a separate development line.

[Source files and official references](sources.md)
