#pragma once

/**
 * Await for Serial to be available
 * @param timeout
 * @return
 */
bool awaitForSerial(size_t timeout = 8000) {
    // endless waiting
    if (!timeout) {
        while (!Serial.available())
            delay(5);

        return true;
    }

    timeout += millis();

    while (millis() < timeout && !Serial.available())
        delay(5);

    return Serial.available();
}

/**
 * Syntactic sugar to read int from Serial
 * @param defaultValue
 * @return
 */
int readInt(int defaultValue = -1) {
    if (!awaitForSerial())
        return defaultValue;

    return Serial.readStringUntil('\n').toInt();
}

/**
 * Print delimiter of given length
 * @param count
 * @param c
 */
void printDelimiter(uint8_t length, char c = '=') {
    for (uint8_t i = 0; i < length; i++)
        Serial.print(c);

    Serial.println();
}


/**
 * Ask user to enter a string
 * @param String
 * @return
 */
String promptString(const String& prompt) {
    Serial.print(prompt);
    Serial.print(": ");

    // loop until something is provided
    if (!awaitForSerial()) {
        Serial.println();
        return promptString(prompt);
    }

    // echo answer
    String answer = Serial.readStringUntil('\n');
    answer.trim();
    Serial.println(answer);

    return answer;
}


uint8_t promptChoice(const String& prompt, const String *choices, uint8_t count) {
    Serial.println(prompt);
    printDelimiter(prompt.length());

    // list of choices
    for (uint8_t i = 0; i < count; i++) {
        Serial.printf("[%d] %s\n", i + 1, choices[i]);
    }

    Serial.printf("Enter your choice [1-%d]: ", count);

    // loop until an answer is provided
    if (!awaitForSerial()) {
        Serial.println();

        return promptChoice(prompt, choices, count);
    }

    int choice = readInt();

    // validate answer
    if (choice < 1 || choice > count) {
        Serial.printf("\nInvalid choice %d. Allowed range is 1-%d", choice, count);

        return promptChoice(prompt, choices, count);
    }

    Serial.println(choice);

    return choice - 1;
}