# 🌌 cosmosEngine

**cosmosEngine** is a 3D rendering engine built from scratch using modern OpenGL. The goal is to serve as a flexible sandbox for learning and experimenting with real-time graphics, simulation, and interactive visual tools — from basic geometry to complex physical systems.

---

## 🚀 Vision

cosmosEngine is designed to grow from a minimal 3D engine into a modular simulation and visualization framework. it aims to support:

- Real-time graphics and UI
- Physics simulation
- Interactive camera control
- Procedural scenes
- Educational visualization tools (e.g., planetary systems, field visualizations)

as a start...

---

## 🌠 Current Capabilities

- 🎮 Real-time object rendering with camera control
- 🧱 Basic geometry (textured object (eg. pyramid) with rotation)
- 🖱️ Interactive user interfaace via ImGui
- 📐 Dynamic camera positioning and scaling
- 🪟 Responsive to window resizing with correct aspect ratio
- 🧩 Modular components (Shader, VAO/VBO/EBO, Texture system)

---

## ✨ Demo

<img src="./screenshots/rotating_pyramid.gif" width="500"/>

---

## 🧭 Current Controls

| Feature        | Control Description                          |
|----------------|-----------------------------------------------|
| **Camera X/Y/Z**   | Move the virtual camera through the scene |
| **Scale**      | Resize the pyramid geometry uniformly         |
| **Rotation**   | Pyramid auto-rotates on the Y-axis            |

UI appears in a floating ImGui window — no need for keyboard shortcuts.

---

## 🔍 Architecture Overview

- **Main Loop:** Handles rendering, UI updates, and camera logic.
- **Scene System:** Defined via transformation matrices (model/view/proj).
- **UI Layer:** Built using ImGui for real-time controls.
- **Resource Management:** Modular wrapper classes for VAOs, VBOs, shaders, and textures.

