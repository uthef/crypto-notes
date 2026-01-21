#include "utils/passwordgenerator.h"
#include "openssl/rand.h"

using namespace cryptonotes;

const QString PasswordGenerator::_specialCharacters ="!@#$%^*()_-+=\";%:?~`/\\|<>,.";

QString PasswordGenerator::generate(int length) {
    QString password;
    
    int numberCount = getRandomNumber(1, length / 2 + 1);
    int specialCharCount = getRandomNumber(1, (length - numberCount) / 3 + 1);

    for (int i = 0; i < length - specialCharCount - numberCount; i++) {
        auto isNumber = getRandomNumber(2);

        QChar c = QChar(getRandomNumber('a', 'z' + 1));
        if (getRandomNumber(2)) c = c.toUpper();

        password.append(c);
    }

    for (int i = 0; i < numberCount; i++) {
        QChar c = QChar(getRandomNumber('0', '9' + 1));
        password.insert(getRandomNumber(password.length() + 1), c);
    }

    for (int i = 0; i < specialCharCount; i++) {
        QChar c = _specialCharacters.at(getRandomNumber(_specialCharacters.length()));
        password.insert(getRandomNumber(password.length() + 1), c);
    }

    return password;
}

QString PasswordGenerator::generate() {
    return generate(getRandomNumber(8, 33));
}

unsigned int PasswordGenerator::getRandomNumber(unsigned int inclusiveFrom, unsigned int exclusiveTo) {
    if (inclusiveFrom == exclusiveTo) exclusiveTo += 1;
    unsigned char randBytes[] = { 0, 0, 0, 0 };
    RAND_bytes(randBytes, 4);
    unsigned int randValue = *(unsigned int*)randBytes % (exclusiveTo - inclusiveFrom) + inclusiveFrom;

    return randValue;
}

unsigned int PasswordGenerator::getRandomNumber(unsigned int exclusiveTo) {
    return getRandomNumber(0, exclusiveTo);
}