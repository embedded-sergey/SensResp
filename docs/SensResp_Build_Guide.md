---
title: "SensResp Build Guide"
author: "Sergey Morozov"
date: "2 August 2026"
revision: "0.3"
toc: true
toc-depth: 3
---

# SensResp Build Guide
**Revision:** 0.3 
**Author:** Sergey Morozov  
**Date:** 2 August 2026

<!-- begin-md-image -->
![SensResp Device](docs/images/SensResp_photo.jpg)
<!-- end-md-image -->

# Table of Contents

1. [Introduction](#1-introduction)
2. [Safety Notes](#2-safety-notes)  
   2.1 [Environmental Safety](#21-environmental-safety)  
   2.2 [Electrical Safety](#22-electrical-safety)  
   2.3 [Mechanical and Tool Safety](#23-mechanical-and-tool-safety)
3. [Bill of Materials](#3-bill-of-materials)
4. [Tools and Consumables](#4-tools-and-consumables)

---

# 1. Introduction

SensResp is an open‑source device for acquiring dissolved‑oxygen and temperature data from multiple Atlas Scientific probes. All measurements are visualized on an LCD display and transmitted to a computer over USB for logging and real‑time plotting. The entire system is assembled on a solderless breadboard, allowing additional sensors or components to be added with minimal modification. SensResp integrates with the PumpResp controller by fetching pump‑phase information and enabling threshold‑based pump activation in addition to time‑based control.

This guide explains how to assemble the hardware step by step. It covers preparing the enclosure, wiring the electronics, installing the user‑interface components, and loading the software. The document focuses strictly on construction and verification; experimental applications and biological context are described separately in the accompanying manuscript.

**User skill level.** The build guide is intended for students, technicians, and researchers with basic technical skills. No engineering background is required. Users should be comfortable with simple soldering and basic multimeter measurements (continuity and DC voltage).

**Assembly environment.** SensResp can be assembled in any standard workspace equipped for basic electronics and mechanical tasks, including university or biological‑station workshops, applied‑sciences laboratories, or local FabLabs (a global directory is provided by the Fab Foundation). The enclosure can be purchased and modified or fabricated using a 3D printer.

**Estimated build time.** Assembly, software setup, and full functional verification typically require 1–2 days for a first‑time build depending on user skills. When assembling multiple identical units, the per‑device time decreases substantially as enclosure preparation and wiring become more routine.

## Versioning

This guide corresponds to SensResp hardware, software, and documentation release **v1.0.0**.

Source files and updates are available in the public repository:  
<https://github.com/embedded-sergey/SensResp>

## Licensing

- **Software:** GPL‑3.0  
- **Hardware designs:** CERN-OHL-S v2 
- **Documentation (including this guide):** CC‑BY‑4.0  

These licenses ensure open access, modifiability, and reproducibility across all components of the SensResp platform. If this guide contains hardware diagrams or code snippets, those components are licensed under their respective hardware or software licenses.

# 2. Safety Notes

## 2.1 Environmental Safety

SensResp is designed to be water-resistant, providing protection against accidental splashes and operation in high humidity environments. To reduce moisture accumulation in humid environment, place an absorbent pack inside the enclosure. SensResp is not waterproof: if the device is submerged or dropped into the water, immediately power it off, disconnect the USB cable and do not resume operation until the enclosure and internal components have been thoroughly inspected and dried.

## 2.2 Electrical Safety

SensResp operates at 5-9 VDC, which falls within the extra-low-voltage category recognized in electrical safety standards worldwide. Devices operating at ≤ 24 VDC are generally considered safe for personal, educational, and research use, and in most countries can be built and used without electrical certification, provided that a certified external power supply is used and no mains‑voltage wiring is modified. 

The power supply unit must include integrated overcurrent protection (fuse or electronic current limit) and should be selected according to the total power requirements of the connected devices. SensResp draws less than 300 mA at a 5 V operating voltage, so a 7–9 V, 0.5 A power supply provides sufficient overhead, operating at roughly 70% of its maximum rated output. 

During assembly and testing, follow basic electrical‑safety practices. Keep the device disconnected from any power source while you work. Check the polarity of every wire before connecting power, and avoid any contact between the positive and negative lines. Before turning the device on, inspect all solder joints, connectors, and insulation to make sure nothing is loose or exposed. Use a multimeter to check your wiring: confirm that each connection has continuity and make sure there are no accidental shorts. Switch the device on only when you’re sure all connections are correct.

## 2.3 Mechanical and Tool Safety

Use adhesive materials, mechanical and electrical tools listed in the section "Tools and Consumables” with care. If you are inexperienced with some of them, seek assistance from a trained technician. Wear personal protection equipment: protective glasses and fume mask, especially when soldering and use heat‑resistant mat to protect your workspace. During the assembly process run the tests referred in the assembly instructions to make the assembly process to be safe and verify the quality of SensResp.

# 3. Bill of Materials

Price estimates for generic components use Amazon.com  as a reference, based on pricing available in August 2026. Amazon is chosen because it provides broad international availability, relatively stable pricing, and consistent product listings, which makes it a practical baseline for reproducible cost estimates. Because many components on Amazon are sold only in bulk packs, the bill of materials lists the price of the entire pack even when only one piece is needed, which inflates the apparent cost of a single device and results in unnecessary surplus. In practice, individual components can often be purchased at lower cost from suppliers that support small‑quantity or single‑unit purchasing, including local electronics stores, online marketplaces, and global engineering distributors such as Mouser or Digi‑Key. Prices for specialized sensors and measurement devices are taken directly from manufacturers’ websites when these items are not consistently available through the above distributors.

Prices listed in this BOM exclude VAT, customs duties, and delivery costs.

| Part Name | Manufacturer | Quantity | Unit Price (USD)¹ | Total Price (USD) | Notes |
|-----------|--------------|----------|--------------------|--------------------|-------|
| Industrial ABS enclosure, IP65, 200×120×75 mm | Generic | 1 pc | 10 | 10 | – |
| Round rocker switch, mounting hole diameter 15 mm, 2-pin | Generic | 1 pc | 6 | 6 | Bulk pack (3–5 pcs) |
| DC power jack, steel, 5.5×2.1 mm | Generic | 1 pc | 7 | 7 | Bulk pack (3–5 pcs) |
| Arduino Nano–compatible board (ATmega328P) with Mini-B USB cable and screw‑terminal adapter | Generic | 1 kit | 9 | 9 | The board can be replaced by the Arduino Nano rev.3 or 3.3 V-board from Nano family² |
| Push button, mounting hole diameter 16 mm, 2-pin | Generic | 1 pc | 10 | 10 | Bulk pack (10-24 pcs) |
| LED, 5 mm | Generic | 1 pc | 6 | 6 | Bulk pack (50–200 pcs) |
| Resistor, 220 Ω, ¼ W | Generic | 1 pc | 6 | 6 | Bulk pack (20–200 pcs) |
| Resistor, 1.6 kΩ, ¼ W | Generic | 2 pcs | 6 | 6 | Bulk pack (20–200 pcs) |
| Screw terminal block connector, 2-pin | Generic | 3 pcs | 4 | 4 | Bulk pack (20–50 pcs) |
| 20x4 LCD with I²C interface, HD44780 driver, 5V | Generic | 1 pc | 10 | 10 | - |
| Female to male jumper wires, square jumpers, 10 cm | Generic | 1 kit | 5 | 5 | – |
| Solderless breadboard BB830, 165.1 x 54.6 x 8.5 mm | Generic | 1 pc | 10 | 10 | Check reviews because cheap clones often have loose contacts, bad rails, and high resistance that disrupts I²C |
| EZO™ RTD Temperature Circuit, #EZO-RTD | Atlas Scientific | 1 pc | 36 | 36 | - |
| SMA Breakout Board, #SMA-B | Atlas Scientific | 1 pc | 8 | 8 | Compatible with PT1000 probes via SMA connector³ |
| EZO™ Dissolved Oxygen Circuit, #EZO-DO | Atlas Scientific | 4 pcs | 54 | 216 | - |
| Electrically Isolated EZO™ Carrier Board, #ISCCB-2 | Atlas Scientific | 4 pcs | 33 | 132 | Compatible with galvanic DO probes via SMA connector³ |

**Grand total: 481 USD**

*¹ Unit prices reflect the smallest purchasable quantity on Amazon.com. When only bulk packs are available, the listed price corresponds to the full pack even if a single piece is required.*

*² The design is compatible with 3.3 V‑logic Arduino Nano–family boards (e.g., Nano Every, Nano 33 IoT, Nano 33 BLE) provided that the 5 V relay module is replaced with a 3 V/3.3 V logic‑compatible relay module powered from 3–3.3 V. All other digital I/O (status LED, push button, PWM control signals) operate correctly at 3.3 V logic levels.*

*³ This BOM does not include DO probes, temperature probes, cables, or calibration consumables.*

# 4. Tools and Consumables
These items are not included in the BOM because they are general workshop supplies rather than project‑specific components, but they are still required for assembly. Most of the tools and consumables can be found in FabLabs, electronic/robotics clubs and Universities of Applied Sciences around the world.

Mechanical tools:
* utility cutter
* long-nose pliers
* wire cutters
* screwdriver
* drill with step drill bit
* rotary tool with 1 mm drill bit

Electrical tools and materials:
* soldering iron
* solder wire
* flux
* heat-shrink tubing

Adhesives:
* hot-glue gun
* hot-glue sticks
* superglue

Measuring and marking tools:
* multimeter
* ruler
* pencil

Safety tools:
* protective glasses
* fume mask
* heat-resistant mat
