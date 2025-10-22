"""
voice_to_bluetooth.py
AI Voice Control for Smart Wheelchair
Author: Ayush rana
GitHub: https://github.com/Ayushrana27 
"""

import serial
import time
import speech_recognition as sr
import pyttsx3

# Initialize Bluetooth serial connection
bt = serial.Serial('COM6', 9600)  # Replace 'COM6' with your Bluetooth port
time.sleep(2)  # Wait for connection to establish

# Initialize text-to-speech engine
engine = pyttsx3.init()
recognizer = sr.Recognizer()

def speak(message):
    """Speak the given message using TTS engine"""
    engine.say(message)
    engine.runAndWait()

speak("Voice control initialized. Say a direction.")

while True:
    try:
        with sr.Microphone() as source:
            print("Listening...")
            recognizer.adjust_for_ambient_noise(source)
            audio = recognizer.listen(source)

            command = recognizer.recognize_google(audio).lower()
            print("Command:", command)

            if 'forward' in command:
                bt.write(b'f')
                speak("Moving forward")
            elif 'backward' in command or 'back' in command:
                bt.write(b'b')
                speak("Moving backward")
            elif 'left' in command:
                bt.write(b'l')
                speak("Turning left")
            elif 'right' in command:
                bt.write(b'r')
                speak("Turning right")
            elif 'stop' in command:
                bt.write(b's')
                speak("Stopped")
            else:
                speak("Command not recognized, please repeat")

    except sr.UnknownValueError:
        print("Could not understand audio, please repeat")
        speak("Could not understand, please repeat")
    except sr.RequestError as e:
        print("Speech recognition service error:", e)
        speak("Speech recognition service error")
