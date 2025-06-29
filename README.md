# 💪 Fitness Tracker Console App (C++)

## 📌 Description
A console-based fitness tracker that helps users manage and monitor weight, BMI, water intake, sleep, calories, and protein consumption using C++ and file handling. It supports goal setting and real-time updates with a clean menu-driven interface.

---

## 🛠 Features
- Track weight and BMI with category classification
- Set desired weight and receive fat loss/gain guidance
- Monitor water and sleep intake
- Log calories and protein from daily food items
- Dietary preference (veg/non-veg) logging
- Color-coded, symbol-based feedback in the console
- Persistent data using file handling

---

## ⚙️ Working of the Application

### 🔁 Program Flow
1. Load existing user data or register a new user
2. Show a menu with options:
   - Update weight (recalculates BMI, shows trend, goal guidance)
   - Log water/sleep intake
   - Input calorie/protein intake
   - Display all stats in a tabular summary
   - Save and exit

### 🧠 Smart Features
- BMI suggestions (underweight, normal, overweight)
- Weight trend arrows (↑ or ↓) based on last value
- Shows how much weight to gain/lose for goal
- Alerts if BMI is outside the healthy range

### 📁 File Handling
- Saves user data and updates to `fitness_data.txt`
- Loads existing data when the app is restarted
- Maintains historical and latest values

### 🎨 Console UX
- Uses color-coded text and symbols for better readability
- Encouraging or warning messages based on user inpu
