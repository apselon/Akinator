import speech_recognition as sr

rec = sr.Recognizer();

with sr.Microphone() as micro:
    audio = rec.listen(micro)

print(rec.recognize_google_cloud(audio));
