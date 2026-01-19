#ifndef PASSWORD_GENERATOR_H
#define PASSWORD_GENERATOR_H

#include <QString>

namespace cryptonotes {
    class PasswordGenerator {
    private:
        static const QString _specialCharacters;
        static unsigned int getRandomNumber(unsigned int inclusiveFrom, unsigned int exclusiveTo);
        static unsigned int getRandomNumber(unsigned int exclusiveTo);

    public:
        static QString generate(int length);
        static QString generate();
    };
}

#endif