\mainpage VGA Display Terminal Manual

\tableofcontents

\section intro_sec Introduction
Welcome to the VGA Display Terminal documentation. This project utilizes an **STM32F4** to drive a VGA screen.
The system operates as a serial terminal: it receives ASCII string commands via UART2 and renders graphics accordingly.

\section arch_sec Architecture
The software is divided into three distinct layers:

1. **Front Layer**: Parses incoming comma-separated strings (e.g., `lijn,0,0,100,100,rood`).
2. **Logic Layer**: Mathematical implementations of drawing algorithms (Bresenham lines, Midpoint circles).
3. **I/O Layer**: Hardware interface for UART interrupts and VGA pixel pushing.

![Software Architecture Diagram](architecture.jpeg)

\section usage_sec Hardware & Software Setup
To use the VGA Display Terminal, you need to set up the motherboard connections and configure a serial terminal on your PC.

\subsection hw_req Hardware Requirements
- **STM32F407G Board** mounted on the VGA Motherboard.
- **VGA Monitor** and VGA Cable.
- **USB Cables** (One for flashing/debug, one for UART communication).

\subsection connect_sec Connections
The motherboard handles the physical signal routing. Follow these steps:

1. **VGA**: Connect the VGA Monitor to the VGA port on the motherboard.
2. **UART**: Connect the motherboard's **UART USB port** to your PC.
3. **Flashing**: Connect the board's **ST-Link/Programming USB port** to your PC.

\subsection term_setup Terminal Configuration
1. Open a terminal program (e.g., **Tera Term**, **PuTTY**, or **RealTerm**).
2. Select the COM port associated with the **UART connection** (not the ST-Link).
3. Configure the serial port with the following settings:
   - **Baud Rate:** 115200
   - **Data Bits:** 8
   - **Parity:** None
   - **Stop Bits:** 1
   - **New Line:** CR+LF (Ensure sending `\n` on Enter)

\subsection send_cmd Sending Commands
Once connected, the screen may appear black or white depending on initialization.
1. Click into the terminal window.
2. Type a command string (e.g., `clearscherm,wit`).
3. Press **Enter**.
4. The parser expects a newline character (`\n`) to execute the command.

\section file_sec File Structure
The project files are organized as follows:

| File | Location | Description |
| :--- | :--- | :--- |
| **`main.c`** | Core/Src | Main entry point, system initialization, and infinite loop. |
| **`LogicLayer.c`** | Core/Src | **Logic Layer:** Parses incoming ASCII commands from the UART buffer. |
| **`API_LIB.c`** | Core/Src | **API Layer:** Contains the API functionality of the program. All functions are visible in the table under UART Command Protocol. |
| **`UART.c`** | Core/Src | **Driver Layer:** Handles UART interrupts and circular buffer management. |

\section cmd_sec UART Command Protocol
Commands must be sent via UART (**115200 baud, 8N1**).

**Format:** `command,param1,param2,...\n`

| Command | Function | Parameters | Description |
| :--- | :--- | :--- | :--- |
| **`pixel`** | UB_VGA_SetPixel() | `x,y,color` | Draws a single pixel. |
| **`lijn`** | API_draw_line() | `x1,y1,x2,y2,color,weight` | Draws a line between two points. |
| **`rechthoek`** | API_draw_rectangle() | `x,y,w,h,color,fill` | Draws a rectangle (fill: 1=yes, 0=no). |
| **`cirkel`** | API_draw_circle() | `x,y,r,color` | Draws a circle with radius r. |
| **`bitmap`** | API_draw_bitmap() | `id,x,y` | Draws a pre-loaded bitmap (IDs 0-11). |
| **`tekst`** | API_draw_text() | `x,y,color,"text",font,size` | Draws text. Font: "arial", "consolas", "comicsans". |
| **`figuur`** | API_draw_figure() | `x1,y1,...,x5,y5,color` | Draws a polygon connecting 5 points. |
| **`clearscherm`** | API_clearscreen() | `color` | Fills the entire screen with a color. |

\subsection color_subsec Color Reference
The following Dutch color strings are accepted by the parser:

- **Monochrome**: `zwart`, `wit`, `grijs`
- **Primary**: `rood`, `lichtrood`, `groen`, `lichtgroen`, `blauw`, `lichtblauw`
- **Secondary**: `geel`, `magenta`, `cyaan`, `bruin`