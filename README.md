IoT-Predict: Real-Time Thermal Intelligence Pipeline
Overview
An end-to-end IoT & Machine Learning ecosystem that captures environmental telemetry and performs real-time predictive modeling. The system transforms raw sensor data from the edge into a supervised learning model for time-series forecasting.

High-Impact Features
Edge-to-Cloud Integration: Real-time data acquisition using an ESP32 and DHT11 sensor.

NoSQL Backend: Seamless data orchestration via Firebase Realtime Database.

Predictive Analytics: Implements Scikit-Learn Linear Regression to forecast temperatures based on temporal patterns.

Temporal Vectorization: Custom logic to convert standard time (HH:MM:SS) into numerical scalars for ML processing.

System Architecture
Perception Layer: ESP32 samples humidity/temperature and pushes JSON packets to the cloud.

Storage Layer: Firebase maintains a chronological history of environmental states.

Intelligence Layer: A Python-based engine fetches the JSON data, trains a regression model, and provides an Inference Interface for user-defined time predictions.

Technical Stack
Hardware: ESP32, DHT11 Sensor.

Cloud: Firebase (Realtime Database).

ML/Data: Python, Pandas, Scikit-Learn, NumPy.

Format: JSON (JavaScript Object Notation).

Future Roadmap
Live Data Streaming: Real-time model retraining using the Firebase Admin SDK.

Polynomial Regression: Capturing non-linear diurnal temperature curves for higher accuracy.
