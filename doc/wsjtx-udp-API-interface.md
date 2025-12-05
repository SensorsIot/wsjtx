# WSJT-X Extended UDP Interface - Functional Specification

**Version:** 1.0

## 1. Introduction

This document specifies a set of new commands for the WSJT-X UDP network interface. The purpose of these extensions is to provide enhanced remote control capabilities beyond the scope of the original protocol, allowing for deeper integration with third-party applications.

## 2. Command Summary

### Standard Commands (Incoming)

| Command | Purpose |
|---------|---------|
| **Reply** | Simulate a double-click on a decode to initiate a QSO reply |
| **Clear** | Clear one or both decode windows (Band Activity / Rx Frequency) |
| **Close** | Gracefully shutdown WSJT-X, saving settings |
| **Replay** | Re-broadcast all current decodes to the external application |
| **HaltTx** | Stop transmitting immediately or disable auto-transmit mode |
| **FreeText** | Set the free text message field and optionally transmit it |
| **Location** | Update the station's Maidenhead grid locator |
| **HighlightCallsign** | Highlight a specific callsign with custom colors in decode windows |
| **SwitchConfiguration** | Switch to a different saved configuration profile |
| **Configure** | Dynamically change operating parameters (mode, frequency, DX call, etc.) |
| **AnnotationInfo** | Provide external sorting/priority information for callsigns |

### Extended Commands (Custom)

| Command | Purpose |
|---------|---------|
| **SetEnableTx** | Explicitly enable or disable the "Enable Tx" (Auto) button |

## 3. General Principles for Extending the Interface

Adding a new command to the UDP interface follows a consistent 4-step pattern that involves modifications to the C++ source code. This pattern ensures that new commands are integrated cleanly into the existing application architecture.

1.  **Define Message Type:** A new message `Type` is added to the `NetworkMessage::Type` enum in `Network/NetworkMessage.hpp`. This uniquely identifies the new command.

2.  **Add a C++ Signal:** A corresponding C++ signal is added to the `MessageClient` class in `Network/MessageClient.hpp`. This signal will be emitted when a valid message of the new type is received. The signal's signature must match the data payload of the new command.

3.  **Handle Incoming Message:** A `case` block is added to the `switch` statement in the `MessageClient::impl::parse_message` method (in `Network/MessageClient.cpp`). This code is responsible for reading the message payload from the `QDataStream` and emitting the corresponding signal.

4.  **Implement Action:** The new signal from the `MessageClient` is connected to a slot or lambda function within `widgets/mainwindow.cpp`. This connection, made in the `MainWindow` constructor, links the UDP command to the application logic that performs the requested action.

## 4. Message Format

All messages adhere to the existing WSJT-X UDP protocol format:

*   **Transport:** UDP
*   **Serialization:** Qt `QDataStream`
*   **Header:** A 32-bit magic number (`0xadbccbda`) followed by a 32-bit schema number.
*   **Payload:** A `quint32` message type identifier (from the `NetworkMessage::Type` enum) followed by the command-specific data fields.

## 5. Standard UDP Commands (Incoming Messages)

These commands are part of the standard WSJT-X UDP protocol and allow external applications to control WSJT-X remotely.

### 5.1. Reply

*   **Type Value:** `NetworkMessage::Reply`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Simulate a double-click on a specific decode message
*   **Payload:**
    *   `time` (QTime): Time of the decode
    *   `snr` (qint32): Signal-to-noise ratio
    *   `delta_time` (float): Time offset in seconds
    *   `delta_frequency` (quint32): Frequency offset in Hz
    *   `mode` (QString): Operating mode
    *   `message` (QString): The decoded message text
    *   `low_confidence` (bool): Low confidence decode flag
    *   `modifiers` (quint8): Keyboard modifiers (Shift, Ctrl, Alt)
*   **Action:** Triggers the same action as double-clicking on a decode in the Band Activity window. Typically used to initiate a QSO reply.

### 5.2. Clear

*   **Type Value:** `NetworkMessage::Clear`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Clear one or both decode windows
*   **Payload:**
    *   `window` (quint8): Window to clear (0=both, 1=Band Activity, 2=Rx Frequency)
*   **Action:** Clears the specified decode window(s).

### 5.3. Close

*   **Type Value:** `NetworkMessage::Close`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Request graceful shutdown of WSJT-X
*   **Payload:** None
*   **Action:** Closes WSJT-X application, allowing it to save settings and clean up resources.

### 5.4. Replay

*   **Type Value:** `NetworkMessage::Replay`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Request re-broadcast of all current decodes
*   **Payload:** None
*   **Action:** WSJT-X re-sends all decode messages from the current period to the external application.

### 5.5. HaltTx

*   **Type Value:** `NetworkMessage::HaltTx`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Stop transmitting immediately or disable auto-transmit
*   **Payload:**
    *   `auto_only` (bool): If `true`, only disables auto-TX; if `false`, halts immediately
*   **Action:** Stops transmission. Use `auto_only=false` for emergency halt, `auto_only=true` to prevent automatic transmissions while allowing manual operation.

### 5.6. FreeText (CQ)

*   **Type Value:** `NetworkMessage::FreeText`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Set free text message and optionally transmit it
*   **Payload:**
    *   `message` (QString): The free text message to set
    *   `send` (bool): If `true`, transmit the message immediately
*   **Action:** Sets the free text field in WSJT-X. If `send=true`, initiates transmission of that message.

### 5.7. Location

*   **Type Value:** `NetworkMessage::Location`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Update station grid locator
*   **Payload:**
    *   `location` (QString): Maidenhead grid locator (e.g., "FN42")
*   **Action:** Updates the "My Grid" field in WSJT-X settings.

### 5.8. HighlightCallsign

*   **Type Value:** `NetworkMessage::HighlightCallsign`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Highlight specific callsign in decode windows with custom colors
*   **Payload:**
    *   `callsign` (QString): Callsign to highlight
    *   `bg` (QColor): Background color (invalid color = default)
    *   `fg` (QColor): Foreground/text color (invalid color = default)
    *   `last_only` (bool): If `true`, highlight only the most recent occurrence
*   **Action:** Applies custom highlighting to the specified callsign in the decode windows.

### 5.9. SwitchConfiguration

*   **Type Value:** `NetworkMessage::SwitchConfiguration`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Switch to a different saved configuration
*   **Payload:**
    *   `configuration_name` (QString): Name of the configuration to activate
*   **Action:** Switches WSJT-X to the specified configuration profile.

### 5.10. Configure

*   **Type Value:** `NetworkMessage::Configure`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Dynamically change operating parameters
*   **Payload:**
    *   `mode` (QString): Operating mode (e.g., "FT8", "FT4")
    *   `frequency_tolerance` (quint32): Frequency tolerance in Hz
    *   `submode` (QString): Submode designation
    *   `fast_mode` (bool): Enable fast mode
    *   `tr_period` (quint32): T/R period in seconds
    *   `rx_df` (quint32): Rx audio frequency in Hz
    *   `dx_call` (QString): DX callsign to work
    *   `dx_grid` (QString): DX grid locator
    *   `generate_messages` (bool): Auto-generate standard messages
*   **Action:** Reconfigures WSJT-X operating parameters without requiring manual GUI interaction.

### 5.11. AnnotationInfo

*   **Type Value:** `NetworkMessage::AnnotationInfo`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Provide external annotation/sorting information for callsigns
*   **Payload:**
    *   `dx_call` (QString): Callsign to annotate
    *   `sort_order_provided` (bool): Whether sort order is specified
    *   `sort_order` (quint32): Sort priority (0-50000, lower = higher priority)
*   **Action:** Allows external applications to influence callsign sorting/priority in decode windows.

## 6. Extended UDP Commands (Custom Additions)

These commands extend the standard WSJT-X UDP protocol with additional remote control capabilities.

### 6.1. SetEnableTx

*   **Type Value:** `NetworkMessage::SetEnableTx`
*   **Direction:** In (Client → WSJT-X)
*   **Purpose:** Explicitly control the state of the "Enable Tx" button
*   **Payload:**
    *   `enable` (bool): The desired state. `true` to enable (turn on), `false` to disable (turn off).
*   **Action:**
    *   Upon receiving this message, the application checks the current state of the "Enable Tx" button (`ui->autoButton`).
    *   If the desired state (`enable`) is different from the current state, the application will programmatically trigger a `click()` on the button to change its state.
    *   If the button is already in the desired state, no action is taken. This makes the command idempotent and robust.
*   **Implementation:** See Section 2 for the 4-step implementation pattern.

## 7. Discovering the WSJT-X Listening Port

### 7.1. The Ephemeral Port Challenge

WSJT-X binds to an ephemeral (dynamically assigned) UDP port for receiving incoming commands. This port is assigned by the operating system when WSJT-X starts and changes with each restart. External programs sending commands to WSJT-X must discover this port dynamically.

### 7.2. Port Discovery via UDP Source Port (Recommended)

The most elegant solution is to listen for any outgoing UDP message from WSJT-X on the configured "UDP Server" port (default 2237). The **UDP source port** of these messages is the listening port.

**How it works:**

1. WSJT-X sends Status, Heartbeat, Decode, and other messages to the configured UDP Server port (2237)
2. These UDP packets have a source port (e.g., 65443)
3. This source port IS the port where WSJT-X listens for incoming commands

**Example (Python):**

```python
import socket

# Listen on the port where WSJT-X sends messages
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('127.0.0.1', 2237))

# Receive any message
data, (sender_addr, sender_port) = sock.recvfrom(65536)

# sender_port is the listening port
print(f"WSJT-X listening port: {sender_port}")

# Now send commands to sender_port
```

**Advantages:**
- Platform-independent (works on Windows, Linux, macOS)
- No external tools required (no netstat)
- Automatic discovery on every WSJT-X restart
- Can track multiple WSJT-X instances via instance ID

### 7.3. Alternative: Operating System Tools

Port discovery via OS-specific tools like `netstat` (Windows) or `lsof` (Unix) is possible but less elegant:

```bash
# Windows: Find port by process ID
netstat -ano | grep <PID> | grep "UDP"

# Linux/macOS: Find port by process name
lsof -i UDP -a -p $(pgrep wsjtx)
```

## 8. Future Considerations: Configuration Commands

It is possible to extend the interface to allow remote configuration of application settings.

### 8.1. UDP Server Configuration

A command could be implemented to change the "UDP Server" settings found in the "Settings -> Reporting" tab.

*   **Proposed Command:** `SetUDPServer`
*   **Proposed Payload:**
    *   `hostname` (QString): A string containing the new hostname or IP address (e.g., "127.0.0.1").
    *   `port` (quint16): An unsigned integer for the new port number (e.g., `2237`).
*   **Feasibility:** This is feasible and would allow for dynamically redirecting WSJT-X's outgoing UDP status messages.

### 8.2. "Accept UDP Requests" Configuration

Controlling the "Accept UDP Requests" checkbox via a UDP command is technically possible but **not recommended**.

*   **Logical Pitfall:** A remote command could be sent to disable this feature. Once disabled, the application would stop listening for all UDP commands, making it impossible to send a subsequent command to re-enable it. This would result in a remote lockout, requiring manual intervention through the GUI. Due to this risk, this command should not be implemented.
