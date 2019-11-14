import speech_recognition as sr

rec = sr.Recognizer();

with sr.Microphone() as micro:
    audio = rec.listen(micro)

print("Text:" + rec.recognize_google(audio));
