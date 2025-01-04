After spending 2 hours fucking around with the Rust bindings/NixOs/Wayland the only sensible
choice was to switch to C after it started working.

## Brainstorming
- TCP for Session management and UDP for game info?

## SGCP
_Some Games Communication Protocol_
### Information that should somehow exist somewhere and be used in some way
- SessionID
- PlayerID
### Message Segments
**Headers** X bit - X Byte
- Start indication?
- Version
- MessageType
- SessionId (if?)
- ReceiverId (Not necessary I think?)

- Header End/Body start indication?

**Body** X bit - X Byte

**Checksum**
### Headers

#### Versioning
1 byte? reasonably less is probably enough? 5/6 at max?
maybe use more when "padding" of headers overall is needed
#### Message Type
Type determines length?

- Player Init chain
- Session Init chain
- Request/Response?
- Player Position
- Player Action?
- Session Termination/Leaving chain
- Status Request

### Body
#### Player Position
- x, y, z - What should be max?
- SessionId
- Who? PlayerID

### Checksum
CRC?


### Flows
#### Player Init
**TCP**
1. Player greets server
2. Server tells Player their ID
3. Player confirms with ID

Maybe player could also greet with his own ID if he has one to allow
for some kind of data persistance?

#### Session Initialization
**TCP**
1. Player wants to init session with server
2. Server responds with session id
3. Player confirms with session id
#### Join Session
**TCP**
1. Player sends init message with session id
2. Server confirms with session id

#### Movement
**UDP**
(Numbering of threads is arbitrary)
Thread 1:
- Server sends position of other player(s) 
- Position saved in some mutex global var whatever (idk how in C yet)
Thread 2:
- Renders generally everything
- When rendering players, read positions from global var
- At the end of iteration send current position to server (sending itself another thread?)

#### Leaving Session
**TCP**
1. Player sends leaving message to server
2. a (if not last player) Server informs other Players
2. b (if last player) Server terminates Session

#### Timeout
***TCP***
If no message for long time from Player?
1. Status request
2. If on received "disconnect" player/terminate session





## _For copying_
###############################################################################
