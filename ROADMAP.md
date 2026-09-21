# KT Roadmap

## M0 — Architecture foundation
Status: **DONE**

- product identity and design principles
- 8-bit CPU, 64 KiB logical address space and banked RAM
- four-port RS-232 subsystem
- Ethernet, SSD/SD, video, keyboard and mouse
- interrupts, DMA and watchdog direction
- preemptive K8T-OS
- shared BBS users/messages/files
- central ACL/publication policy
- Telnet, Gopher, HTTP, IRC, RSS, Atom and FidoNet/BinkP
- concurrency acceptance target

## M1 — Executable machine model
Status: **DONE**

- freeze M1 register map
- CPU programmer-visible state and reset/vector behaviour
- bank and interrupt controller registers
- UART and timer/RTC contracts
- host-side executable machine skeleton
- deterministic register/memory-map tests
- CI checks

## M1.5 — CPU market survey
Status: **DONE — W65C265S selected after M1.6 analytical qualification**

- prefer a currently produced physical CPU over a custom FPGA CPU
- minimize FPGA use
- preserve educational and maker-friendly architecture
- pure 8-bit operation is not a requirement; total system cost matters
- W65C265S selected as baseline: 8/16-bit core, 24-bit external addressing, four UARTs, timers, interrupts and watchdog integrated
- W65C02S retained as survey reference, not baseline

See [docs/M1_5_CPU_SURVEY.md](docs/M1_5_CPU_SURVEY.md).

## M1.6 — W65C265S feasibility
Status: **ANALYTICAL PASS**

Cycle-budget qualification covers four 115200-bit/s UART streams, interrupt/preemption load, Ethernet, storage, ANSI rendering and BBS activity. Reference model uses 36.5% of an 8 MHz CPU budget, leaving 63.5% analytical headroom. Physical hardware qualification remains mandatory.

See [docs/M1_6_W65C265S_FEASIBILITY.md](docs/M1_6_W65C265S_FEASIBILITY.md).

## M2 — KT CPU and memory subsystem
Status: **IN PROGRESS — M2.0–M2.32 DONE**

M2.0 freezes the W65C265S platform baseline, native 24-bit memory direction, real privilege model and minimum-FPGA hardware policy. M2.1 freezes the native-mode task context, private stacks, 1 kHz tick, 10 ms default quantum and priority/round-robin scheduler ABI. M2.2 freezes COP-based syscalls, error/register conventions and the bounded interrupt-service ABI. M2.3 freezes the BBS-first external bus direction: 2 MiB standard SRAM, 4 MiB onboard path, 512 KiB recovery flash, buffered controller apertures and cost/reliability-driven decode logic. M2.4 freezes four physical DTE DE-9 BBS/modem ports, one per integrated UART, with RTS/CTS, modem-control semantics and 1 KiB RX/TX rings. M2.5 freezes packet-level 10/100 Ethernet, an OS-owned TCP/IP stack and a 32-connection design floor so BBS networking is not constrained by hardware socket count. M2.6 freezes SSD/SD/flash roles, asynchronous block I/O, BBS durability classes, atomic intent/data/commit persistence and online snapshot semantics. M2.7 freezes one canonical BBS core across serial/Telnet, dynamic sessions, built-in groups, default-deny ACLs and canonical message/file objects with per-area publication surfaces. M2.8 freezes authentication/login, new-user/sysop management, mandatory native SSH, shared Telnet/SSH anti-bot admission control and KScript simple doors. M2.9 freezes data-driven ACL menus, ANSI/text bulletins and the budgeted capability-scoped KScript VM/API for automation and simple doors. M2.10 freezes 32-bit scalable message bases, paged disk-backed indexing, threads/last-read state and a bounded ANSI/plain editor with FidoNet-ready metadata. M2.11 freezes 32-bit scalable file areas, paged indexes, transactional staged uploads, bounded transfer jobs and X/Y/ZMODEM requirements. M2.12 freezes native FidoNet netmail/echomail, BinkP, toss/scan, duplicate suppression, nodelists and routing. M2.13 freezes the hardware-independent terminal pipeline, configurable screen model, incremental ANSI/VT parser and distinct PETSCII profile with shared cross-platform conformance. M2.14 expands VT/CP437/PETSCII/UTF-8/input compatibility and makes portable XMODEM, XMODEM-1K, YMODEM, ZMODEM and Kermit transfer engines first-class terminal requirements. M2.15 freezes the daily-use terminal UX: phonebook/profiles, bounded reconnect, Hayes modem profiles, scrollback/search, macros, capture and transfer integration. M2.16 freezes the portable core/platform API plus native K8T, AmigaOS 2.04+/68000 and Atari ST/TOS frontend contracts with shared conformance vectors. M2.17 adds the versioned platform-neutral terminal conformance corpus, golden ANSI screen/cursor/attribute vectors, CP437 BBS box-drawing references and PETSCII baseline vectors. M2.18 freezes the portable C terminal-core ABI and host-qualified reference implementation, including incremental ANSI CSI parsing, explicit ANSI/CP437/PETSCII profiles and malformed-sequence recovery. M2.19 freezes the portable caller-owned screen-state/reference renderer boundary, including M2.17 golden ANSI semantics, erase modes, cursor clamping, wrapping and scrolling. M2.20 freezes CP437/PETSCII glyph decoding, PETSCII charset state, renderer-facing Unicode/source-byte identity and pixel-faithful rendering metadata. M2.21 freezes the portable full-frame and incremental renderer/frontend contract, including caller-owned caching, dirty-cell updates, cursor-only updates and deterministic invalidation. M2.22 freezes the portable event/input ABI, normalized key identities, ANSI/VT key encoding and transport-neutral output/backpressure contract. M2.23 freezes the portable frontend/platform adapter boundary for native KT, AmigaOS and Atari ST/TOS. M2.24 freezes the terminal font/glyph provider, legacy-first CP437/PETSCII identity, Unicode/replacement fallback, validated bitmap contract and optional pixel-faithful bitmap renderer path. M2.25 freezes the caller-owned reference bitmap rasterizer/framebuffer contract, clipping, stride safety and deterministic inverse/underline/blink/cursor semantics. M2.26 freezes ANSI16/C64 reference palettes, terminal-profile mapping and deterministic indexed-framebuffer to RGB24 conversion. M2.27 freezes the framebuffer/display presentation boundary, INDEXED8/RGB24 validation, full-frame and bounded partial-row updates, and backend error propagation. M2.28 freezes incremental renderer damage reporting, conversion of terminal-row/cursor changes to clipped framebuffer scanlines, no-change suppression, and E2E partial display presentation. M2.29 freezes allocation-free presentation scheduling, immediate/coalesced policies, retry-safe display submission, and selective blink damage/presentation. M2.30 freezes the terminal viewport/resize contract, including FIT and fixed logical grids, deterministic placement, transactional resize rejection and resize-event integration. M2.31 freezes terminal geometry negotiation and session-size policy, including FIXED/VIEWPORT/REMOTE selection, M2.30 viewport integration, Telnet NAWS/SSH PTY source abstraction, exclusive remote-source ownership, deterministic disconnect fallback and bounded untrusted geometry. M2.32 freezes caller-owned terminal session/screen resize integration, including in-place content preservation, capacity-safe rejection, cursor clamping, renderer-cache invalidation, full resize damage and presentation scheduling. Next: M2.33 terminal resize transaction/orchestration boundary.

Integrate and freeze the W65C265S-based CPU/memory architecture. K8 compatibility is explicitly not a goal and K8 is not changed.

- educational and maker-friendly W65C265S architecture
- 8/16-bit W65C816-compatible CPU with 8-bit external data bus and 24-bit addressing
- readable 65C816 assembler/toolchain documentation
- minimize FPGA; use ordinary logic or small CPLD only where justified
- expose the external bus and memory/peripheral architecture clearly
- banking, privilege/traps and efficient context switching
- vectored/prioritised interrupt support suitable for four UARTs plus Ethernet/storage
- timer-driven preemption
- architecture documentation sufficient to explain and eventually build the CPU

## M3 — Console and ANSI terminal
Text video, keyboard/mouse, virtual consoles, ANSI renderer, scrollback/copy-paste and serial terminal sessions.

## M4 — Storage
Block protocol, SD, SSD, 64-bit LBA, K8TFS prototype, recovery and snapshots.

## M5 — Networking
Ethernet, ARP/IPv4/ICMP, UDP/TCP/DNS, sockets, Telnet and stress tests.

## M6 — BBS core
Users/authentication, security groups, message/file areas, ACL/publication policy, multi-node sessions and sysop console.

## M7 — Internet/classic services
Gopher, HTTP, RSS, Atom, IRC client/gateway and configurable per-service exposure.

## M8 — FidoNet
Addresses, netmail, echomail, packets/bundles, TIC/file echoes, nodelists, BinkP and dial-up RS-232 transport.

## M9 — Hardware prototype
Physical RS-232 x4, Ethernet, SSD/SD, video, keyboard/mouse, watchdog/RTC and concurrency qualification.

## M10 — Release qualification
Four active serial links plus multiple TCP sessions, BBS, Gopher/HTTP, FidoNet/BinkP, local terminal/IRC and storage activity without lost serial data.


## BBS scale, doors and bulletins

These are architectural requirements, not optional future polish.

### Scale

Canonical durable object IDs use 32-bit identifiers for users, messages, files, message areas and file areas. There is no intentionally small user-visible limit such as 255 areas, 65,535 messages per area or a fixed files-per-area ceiling.

Large BBS installations are disk-backed and indexed/paged. RAM use must scale with the active working set rather than the total number of messages/files. Practical capacity is therefore governed by storage, index format and performance rather than legacy BBS-era table limits.

### ANSI bulletins

Bulletins are first-class BBS objects/assets. A bulletin may have ANSI and plain-text representations and may be:

- shown during login or logout;
- selected from a bulletin menu;
- targeted by ACL/group;
- enabled for a date/time interval;
- exposed only on selected node/transport classes.

ANSI rendering includes color and cursor/control sequences appropriate to classic terminal presentation. Plain text is the required fallback.

### Doors

K8T has a native Door API. Doors receive a controlled session context rather than owning UART/TCP hardware directly. The context includes at least:

- user identity and groups;
- node/session identity;
- transport and terminal capabilities;
- remaining session time/policy;
- terminal input/output;
- permitted BBS message/file operations;
- door-local storage/configuration.

The same native door must work for serial and Telnet callers.

Door execution is subject to ACL, time/resource policy and logging. A failed door must not terminate other BBS nodes.

The compatibility roadmap includes classic drop-file adapters, beginning with investigation/qualification of DOOR.SYS and DORINFOx.DEF-style formats. Compatibility adapters map into the canonical K8T session model; they do not become a second user database.

Door chaining, sysop-only doors, per-door ACLs and usage statistics are required capabilities.


## Observability and home automation integration

K8T remains fully standalone: monitoring and home-automation systems are optional integrations, never runtime dependencies.

### Prometheus

Provide a lightweight Prometheus-compatible `/metrics` endpoint. Candidate metrics include uptime/reset cause, active nodes/sessions by transport, calls, message/file activity, transfer counters, FidoNet queue depth, free storage, memory pressure, network errors, UART overruns, watchdog events and available board health sensors.

Metrics collection must be bounded and must not materially disturb BBS callers.

### MQTT and Home Assistant

Provide an optional MQTT client for telemetry and carefully scoped control. Support Home Assistant MQTT Discovery so a K8T can appear automatically as a device with useful entities.

Candidate entities include online callers, per-node state, calls today, FidoNet queue, disk free, uptime, health/alarm state and available environmental/board sensors.

Read-only telemetry is the default. Remote actions such as maintenance mode, backup, reboot or shutdown require explicit configuration, authentication/authorization and audit logging. No remote integration may bypass the BBS ACL or management policy.

### Later integration candidates

The management API should be transport-neutral enough to support SNMP-style monitoring, webhooks and external dashboards later without coupling the BBS core to any one ecosystem.


## BBS completeness requirements

The following are explicit product requirements and must be represented by implementation milestones before a production release.

### Events and scheduler
- recurring and one-shot BBS events;
- FidoNet mail events and maintenance windows;
- scheduled backup, log rotation, bulletin rotation and script/door launch;
- missed-event policy after reboot or downtime.

### Sysop and inter-node communication
- caller-to-sysop page/chat;
- local sysop console attachment to a caller;
- inter-node chat between serial and network callers;
- presence/node listing subject to privacy policy.

### Terminal profiles
- ANSI and plain ASCII baseline;
- VT100/VT220-compatible behavior where practical;
- terminal width/height and capability negotiation/profile;
- UTF-8 on capable network terminals without making it mandatory internally;
- PETSCII compatibility is a planned extension for Commodore callers.

### File-transfer protocols
- XMODEM;
- YMODEM;
- ZMODEM as a required first-class protocol;
- transfer accounting, resume where protocol permits, ACL/quota integration and clean abort.

### Full FidoNet subsystem
- BinkP;
- netmail and echomail;
- packet/bundle handling;
- toss/scan;
- nodelist support;
- routing;
- scheduled mail events;
- canonical mapping into K8T message areas and ACL/publication policy.

### Call history and statistics
- Last Callers;
- calls today/total;
- per-node and per-transport statistics;
- uploads/downloads and transfer volume;
- message activity;
- door usage;
- selected counters exported through Prometheus.

### Audit
- sysop/admin actions;
- failed authentication and lockout/ban events;
- ACL and configuration changes;
- door administration;
- remote management actions including MQTT/Home Assistant;
- durable timestamped audit records with bounded retention policy.

### Backup, restore and migration
- consistent online backup;
- full restore;
- configuration export/import;
- SD-based recovery/migration;
- ability to move a BBS installation to replacement K8T hardware without rebuilding it manually.

### Time
- battery-backed RTC;
- NTP synchronization when networking is available;
- monotonic runtime clock for timers;
- explicit handling of invalid RTC/time jumps;
- timestamps suitable for BBS, FidoNet, audit and scheduled events.

### Power-fail and UPS
- optional power-fail/UPS input;
- early warning path to storage/kernel;
- stop or bound new durable commits during imminent power loss;
- flush critical persistence state when available hold-up time permits;
- reset/power-loss cause visible to sysop and monitoring.

### Modems
- Hayes AT-compatible modem operation;
- init strings and modem profiles;
- auto-answer;
- DCD/carrier handling;
- DTR hangup/reset;
- configurable baud/framing/flow control;
- dial-out support for classic networking/maintenance where configured.

### Secure remote access
Native SSH is a product requirement for encrypted terminal/sysop access and must be benchmarked/optimized for W65C265S CPU/RAM limits. Telnet and serial remain independent. An external gateway is a valid deployment option but does not replace the native SSH requirement. Telnet and SSH share pre-auth admission/rate/failure controls to resist automated bot banging and resource exhaustion.

## Scripting architecture

K8T will have a **small native BBS scripting language/VM** rather than embedding a large general-purpose language as a core dependency.

Working name: **KScript** (name is provisional).

Design goals:

- text source files that are easy for a sysop to edit;
- compact bytecode/intermediate representation for execution;
- deterministic bounded memory usage;
- cooperative execution with instruction/time budgets;
- no raw memory, UART, disk-sector or network-controller access;
- capability-based access to the canonical BBS API;
- event-driven hooks and scheduled scripts;
- scripts can be used by menus, login/logout, bulletins, moderation, automation and doors;
- the same API model is available regardless of serial/Telnet caller transport;
- script failure terminates only that script invocation.

Initial language facilities:

- integer, boolean and string values;
- variables and constants;
- if/else;
- loops with execution budgets;
- functions/subroutines;
- lists/maps only if they fit the measured memory budget;
- BBS API calls;
- event handlers;
- explicit return/error handling.

Initial event hooks should include login, logout, new user, message posted, file uploaded/downloaded, door start/exit, node connect/disconnect, scheduled event and sysop-triggered execution.

KScript can implement simple interactive doors as well as automation/customization. Native compiled doors remain the path for games and applications needing maximum performance or low-level capabilities. Both use the controlled canonical Door API/session context.

A later compatibility study may add a second scripting frontend or translator if a historically popular BBS language can map cleanly onto the KScript VM without compromising the core architecture.


## Sysop ANSI customization

K8T BBS is explicitly designed for SysOps who want to give their board a distinct visual identity rather than accept a fixed UI.

- ANSI-first customizable login/welcome/goodbye screens, menus, prompts, headers, footers, bulletins, help, error/status screens and door launch/return screens;
- plain-text fallback for non-ANSI callers, with PETSCII/profile-specific presentation possible through the same presentation layer;
- editable ANSI assets stored as ordinary BBS content with stable logical names, not compiled into the BBS executable;
- per-menu and per-command ANSI artwork, including hotkeys and cursor-positioned interfaces;
- variables/tokens for user, node, time, statistics, message/file counts and other safe BBS state;
- conditional presentation by user group, terminal profile, geometry, node/transport, date/time and KScript policy;
- KScript can select screens, compose dynamic sections and implement custom menu flows without bypassing ACL/security;
- themes/packs can replace a complete visual identity and can be exported/imported/backed up;
- preview/test mode for SysOps before publishing changed screens;
- versioned/atomic publication so a caller never sees a half-written ANSI screen;
- ANSI art remains SysOp-owned content and is included in backup/migration.

The default distribution should be usable immediately, but almost every caller-facing BBS presentation surface should be replaceable without recompiling K8T-OS.

## Terminal subsystem as a product pillar

The terminal is equal in importance to the BBS and must be beyond first-class: excellent ANSI/VT and PETSCII support, CP437/BBS rendering, scrollback/search, copy/paste, macros, capture, transfer protocols, modem profiles, phonebook and serial/Telnet/SSH transports. The emulation/state-machine core must be portable and hardware-independent so it can later underpin native terminal applications for Amiga and Atari ST (and potentially additional retro platforms). See `docs/TERMINAL_SUBSYSTEM.md`.
