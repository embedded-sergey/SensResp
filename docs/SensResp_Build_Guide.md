---
title: "SensResp Build Guide"
author: "Sergey Morozov"
date: "31 July 2026"
revision: "0.2"
toc: true
toc-depth: 3
---

# SensResp Build Guide
**Revision:** 0.2 
**Author:** Sergey Morozov  
**Date:** 31 July 2026

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
| Resistor, 1.6 kΩ, ¼ W | Generic | 2 pc | 6 | 6 | Bulk pack (20–200 pcs) |
| Screw terminal block connector, 2-pin | Generic | 1 pc | 4 | 4 | Bulk pack (20–50 pcs) |
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