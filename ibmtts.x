"Initialize the engine."
bool initializeEngine()

"Close the TTS Engine."
bool closeEngine()

"Find the sample rate used by the engine."
int32 getSampleRate()

"Return an array of char pointers representing names of supported voices."
[string] getVoiceIdentifiers()

"Cancel speaking of current message."
bool cancel()

"Set the language."
bool setLanguage(string language)

"Select a voice."
bool selectVoice(string voice)

"Set the speech rate."
bool setRate(float rate)

"Set the pitch."
bool setPitch(float pitch)

"Synthesize the text."
bool speakText(string text)

"Get audio data.  Returns 0 length array if none left."
[int16] getAudio()
