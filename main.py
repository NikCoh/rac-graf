import sys


def call_action(action):
    if action == "on":
        print("Action recognized 'on'")
        return 1
    elif action == "off":
        print("Action recognized 'off'")
        return 2
    elif action == "up":
        print("Action recognized 'up'")
        return 3
    elif action == "down":
        print("Action recognized 'down'")
        return 4
    elif action == "switch":
        print("Action recognized 'switch'")
        return 5
    else:
        print("-Error reading device from:", action)
        return 0


def call_device(device_text_1, device_text_2):
    if device_text_1 == "radio":
        print("Device recognized 'radio'")
        return 0
    elif device_text_1 == "air" and device_text_2 == "conditioning":
        print("Device recognized 'air conditioning'")
        return 1
    else:
        print("-Error reading device from:", device_text_1, device_text_2)
        return 2


def call_transcription(text):
    device = 2
    action = 0
    text = text.lower().split()
    if len(text) < 4:
        print("-Error insufficient arguments")
        return 0

    if text[0] == "voice" and text[1] == "command":  # Razpoznava ključne besede
        print("Recognized 'Voice command'")

        device = call_device(text[2], text[3])  # Razpoznava naprave

        if device == 0:
            action = call_action(text[3])  # Razpoznava akcije
        elif device == 1:
            action = call_action(text[4])
            if action == 5:
                print("-Cannot change air conditioning")
                return 0
    else:
        print("-Error finding 'voice command' from:", text)
        return 0

    signal = 6 * device + action
    print("Sending signal", signal, "to MQTT")
    return signal
    # 0 & 6 error
    # 1 radio on    2 radio off     3 radio up      4 radio down       5 radio change
    # 7 ac on       8 ac off        9 ac up        10 ac down


if __name__ == '__main__':
    # input_text = "voice command radio"
    # input_text = "voice command air conditioning down"
    try:
        input_text = sys.argv[1]
        output_signal = call_transcription(input_text)
        sys.exit(output_signal)
    except Exception as e:
        print(f"-Unexpected error:", e)
        sys.exit(0)
