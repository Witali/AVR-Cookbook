# Bootloaders and protocols

[Русская версия](bootloaders_ru.md)

## 1. Share identical tails and combine passes

Commands with the same end-of-packet check and success response can share that tail. A response builder can update the CRC while copying each payload byte, then publish the buffer once the data and CRC are ready. Every path into a shared tail must agree on the entry state, when it can publish, the response format, and the state changes. Test retries and resets, where differences can first appear.

## 2. Account for placement and Flash geometry

Track the code size, the reserved region, and the Flash available to the application separately. The smallest hardware boot section on ATmega328P is 256 words, or 512 bytes, so an image that shrinks within that reservation leaves the application boundary at the same address. In a software-managed layout, try whole erase groups where the device and entry mechanism allow it. Relink for each candidate start address.

A 256-byte-aligned boundary can simplify the limit check if the high-byte comparison covers the whole write range. Use the full address for unaligned limits. Test the last allowed block, a write starting at the boundary, a write crossing it, arithmetic overflow, and the space reserved for vectors.

## 3. Preserve entry conditions and hardware timing windows

Use peripheral reset defaults only when every entry path guarantees them. For example, a direct jump from an application can leave the UART configured differently from its reset state. Document entry through hardware reset, watchdog reset, and any application jump. Check the linked listing against the device datasheet to ensure that WDT, EEPROM, and SPM control sequences stay within their required timing windows.

## 4. Check worst-case latency before removing polls

Removing a poll can increase the longest delay before the receiver notices the first edge. Exercise every arrival phase around a polling gap. When replacing a CRC table with a smaller table or arithmetic, count the worst-case cycles available between samples and measure the replacement inside the complete receiver. Table size, alignment, lookup latency, and call overhead all affect the result.

## 5. Use explicit ownership to eliminate repeated checks

In a request-response protocol, grant ownership only after accepting a complete request. The response path can use a snapshot tied to that request and omit checks already enforced at acceptance. Invalidate the snapshot when ownership changes. Test retries, cancellation, malformed packets, resets, minimum gaps between packets, and early reads.

## 6. Keep safety checks during optimization

If the protocol requires a status ACK before a Flash write, verify that the ACK finishes before starting SPM. Test timeouts, malformed packets, retries of the last command, writes suppressed after an error, and the first transactions after reset. Verify that the application region keeps its contents and that the application starts. On a board, test the exact image and configuration you intend to use.

[Source files and official references](sources.md)
