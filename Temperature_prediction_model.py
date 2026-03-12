import pandas as pd
import numpy as np
import json
from sklearn.linear_model import LinearRegression

# 1. LOAD DATA (Assuming the structure from your Firebase JSON)
with open("E:/PROJECTS/Robotics/temp_sensor/tempdata.json", "r") as f:
    raw_data = json.load(f)

history_data = raw_data["sensor"]["history"]
records = [{"time": k, "temperature": v["temperature"]} for k, v in history_data.items()]
df = pd.DataFrame(records)

# 2. FLEXIBLE TIME CONVERSION
def time_to_seconds(t):
    parts = list(map(int, t.split(":")))
    if len(parts) == 3:    # HH:MM:SS
        return parts[0]*3600 + parts[1]*60 + parts[2]
    elif len(parts) == 2:  # HH:MM
        return parts[0]*3600 + parts[1]*60
    else:
        raise ValueError("Invalid time format. Use HH:MM or HH:MM:SS")

# 3. TRAIN MODEL
df["time_sec"] = df["time"].apply(time_to_seconds)
X = df[["time_sec"]]
y = df["temperature"]

model = LinearRegression()
model.fit(X, y)

# 4. USER INPUT & PREDICTION
print("-" * 30)
user_input = input("Enter time to predict (e.g., 01:30 or 13:45): ")

try:
    time_sec = time_to_seconds(user_input)
    # Create a DataFrame for prediction to match training feature names
    pred_df = pd.DataFrame([[time_sec]], columns=["time_sec"])
    predicted_temp = model.predict(pred_df)

    print(f"--- Prediction Results ---")
    print(f"Input Time: {user_input}")
    print(f"Predicted Temperature: {predicted_temp[0]:.2f}°C")
except Exception as e:
    print(f"Error: {e}")
    
    
    
    
    
    
    
    