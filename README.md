# Calibração – Sensor de Pressão MPX10GP com Amplificador HW-685

## 🔧 Configuração Física

- **Sensor:** MPX10GP (diferencial, não compensado)
- **Amplificador:** HW-685 (baseado em LM358)
- **Microcontrolador:** ESP32
- **Leitura de sinal:** ADC de 12 bits (0–4095), 3.3V de referência

## 🔌 Esquema de Conexão

| MPX10GP | HW-685 (LM358) | ESP32 |
| --- | --- | --- |
| Pin 3 (+Vs) | VCC (5V) | — |
| Pin 1 (GND) | GND | GND |
| Pin 2 (+Vo) | IN+ | — |
| Pin 4 (−Vo) | IN− | — |
| — | OUT | GPIO34 (ADC1) |

> Obs.: OUT do HW-685 fornece tensão analógica amplificada proporcional à pressão diferencial.
> 

---

## ⚙️ Ajustes do Amplificador (HW-685)

O HW-685 possui dois trimpots ajustáveis:

- **Zero (Offset):** define o nível de saída quando a pressão diferencial é 0.
    - **Valor calibrado:** `3.6 kΩ` (ajustado até a leitura estabilizar em ~0.01866V na pressão atmosférica)
- **Span (Ganho):** define quanto a tensão de saída varia conforme a pressão.
    - **Valor calibrado:** `9.62 kΩ` (ajustado para que a saída chegue até ~0.26676V em 8 BAR)

### Resultado da Calibração

| Pressão (BAR) | Tensão Média (V) |
| --- | --- |
| 0 | 0.01866 |
| 1 | 0.09296 |
| 2 | 0.13836 |
| 3 | 0.16890 |
| 4 | 0.19740 |
| 5 | 0.21561 |
| 6 | 0.23354 |
| 6.5 | 0.24 |
| 7 | 0.24952 |
| 7.5 | 0.2550 |
| 8 | 0.26676 |