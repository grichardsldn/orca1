#pragma once

class Protector {
    public:

    Protector() {
    }
    double Tick(double input) {
        const double diff = input - lastInput;
        lastInput = input;
        if (energy < 10.0) {
            energy +=abs(diff);
        }
       
        if (energy > 0.0 ) {
            energy -= 0.01;
        }

        if (choked == true) {
            if (energy < 0.1) {
                choked = false;
            }
            return Buzz();
        } else {
            if (energy > 10.0) {
                choked = true;
                return Buzz();
            }
            
        }
        return input;
    }
    private:
    double Buzz() {
        through += 1;
        if (through > 200) {
            through = 0;
        }
        return (through > 20) ? 0.1: -0.1;
    }

    bool choked = false;
    int through = 0;
    double energy = 0.0;
    double lastInput = 0.0;
};
