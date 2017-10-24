#ifndef BACKLIGHT_H
#define BACKLIGHT_H

class Backlight
{
public:
    static Backlight & Instance();

    int getValue() const;
    bool setValue(const int &value);

private:
    Backlight();
    ~Backlight();
};

#endif // BACKLIGHT_H
