# KT

KT is a retro 8/16-bit communications terminal and BBS computer.

Its primary purpose is to cultivate BBSing: a dedicated multitasking BBS computer combining classic serial communications, a first-class local terminal and modern Ethernet services. KT prioritizes BBS capability, 24/7 reliability, communications throughput and practical BOM cost; K8 and K16 cover the educational-computer role.

## M0 goals

M0 defines the machine and software contracts before implementation begins.

KT is specified around:

- a WDC W65C265S 8/16-bit processor, independent of the K8 computer
- an educational and maker-friendly external bus architecture
- an 8-bit external data bus and native 24-bit address space
- minimal FPGA use; dedicated ICs, ordinary logic or small CPLDs are preferred where practical
- four first-class RS-232 ports
- 100BASE-TX Ethernet
- internal SSD storage plus removable SD
- text-first video with strong ANSI support
- keyboard and mouse, including terminal copy/paste
- preemptive multitasking in KT-OS
- a shared BBS core for users, messages and files
- per-area publication and access-control policy
- FidoNet/BinkP, Telnet, Gopher, HTTP, IRC, RSS and Atom

The design rule is simple: KT may use modern components where useful, but it must remain a real, understandable retro computer built around the W65C265S. No hidden Linux/ARM system is allowed to implement the machine's core services.

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) and [ROADMAP.md](ROADMAP.md).

## Status

**M2 — W65C265S CPU and memory subsystem: in progress (M2.0–M2.19 complete)**

W65C265S is the selected baseline CPU. M2.0–M2.19 freeze the platform, OS/BBS contracts, terminal conformance foundation, portable C terminal core and deterministic screen-state model. Next is **M2.20 — CP437/PETSCII glyph decoding and renderer-facing codepoint boundary**.

## Manufacturing

For fabrication files, release-package conventions, manufacturer choices, and funding/affiliate disclosure, see [MANUFACTURING.md](MANUFACTURING.md). Released hardware remains vendor-neutral and may be manufactured by any suitable PCB manufacturer. For project-specific PCB ordering options, see [ORDERING.md](ORDERING.md).

## License

Hardware design materials — including schematics, PCB layouts, manufacturing files, and HDL/RTL that describes hardware — are licensed under the **CERN Open Hardware Licence Version 2 - Permissive (CERN-OHL-P-2.0)**. See [LICENSE-HARDWARE](LICENSE-HARDWARE).

Software — including firmware, drivers, host tools, emulators, assemblers, compilers, utilities, and other executable code unless explicitly stated otherwise — is licensed under the **MIT License**. See [LICENSE-SOFTWARE](LICENSE-SOFTWARE).

Files that incorporate third-party material remain subject to their respective licences and notices.
