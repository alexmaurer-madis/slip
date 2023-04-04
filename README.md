# Serial Line Internet Procol (SLIP)

Very useful when your project must send **raw binary data** between two microcontrollers with serial line.

The SLIP protocol is a way to encapsulate binary data.
It can be seen as a packet framing protocol. See [rfc1055 from june 1988](https://datatracker.ietf.org/doc/html/rfc1055) and [wikipedia](https://en.wikipedia.org/wiki/Serial_Line_Internet_Protocol).

It does not provide :
* Addressing
* Packet type identification
* Error detection/correction or compression mechanisms.
  
Because the protocol does so little, though, it is usually very easy to implement and lightweight.


##  Description of control bytes

| Hex value | Abbreviation | Description             |
| :-------: | :----------- | :---------------------- |
|   0xC0    | FEND         | Frame End               |
|   0xDB    | FESC         | Frame Escape            |
|   0xDC    | TFEND        | Transposed Frame End    |
|   0xDD    | TFESC        | Transposed Frame Escape |

## How it work

A _Frame End_ byte is added at the beginning and at the end of the data to be sent.  
This delimits the data for ease of reception.

| FEND | data | FEND |
| ---- | ---- | ---- |
| 0xC0 | data | 0xC0 |

If the _FEND_ byte (0xC0) occurs in the data, the two byte sequence _FESC_, _TFEND_ is sent instead. (0xDB, 0xDC)  
If the _FESC_ byte (0xDB) occurs in the data, the two byte sequence _FESC_, _TFESC_ is sent instead. (0xDB, 0xDD)

## Example

```
Data to send
0x00, 0x30, 0x31, 0x32, 0xFF

Transposed with SLIP
0xC0, 0x00, 0x30, 0x31, 0x32, 0xFF, 0xC0
```
## Example with data containing control bytes to be escaped
```
Data to send
0xAA, 0xDB, 0xC0, 0x55, 0xDD, 0x11, 0xDC

Transposed with SLIP
0xC0, 0xAA, 0xDB, 0xDD, 0xDB, 0xDC, 0x55, 0xDD, 0x11, 0xDC, 0xC0
```



## Arduino Serial

Arduino serial object has a default rx buffer of 256 bytes.

- call ::setRxBufferSize(size_t size) before calling ::begin to change rxBuffer if you want it bigger.
- Serial.available() return the number of bytes in the buffer

## Library enhancement for maintainer
- [ ] Add a function to calculate the len of data after encoding (for dynamic buffer allocation)
